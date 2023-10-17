#include"peer_header.h"

// const string DOWNLOAD_PATH = "~/IIIT-H/AOS/p2pDownloads";

std::function<bool()> getChunkInfo(string PEER_IP, string PEER_PORT, string fileName, 
    unordered_map<string, vector<pair<string, string>>>& chunkPeer, mutex& chunkPeerMutex, atomic<int> &tasksCompleted, int totalTasks ,condition_variable& cv, mutex& cv_mtx) {
    return [PEER_IP, PEER_PORT, fileName, &chunkPeer, &chunkPeerMutex, &tasksCompleted, totalTasks ,&cv, &cv_mtx]() -> bool {

        sockaddr_in peerAddr;
        int clientSocket;
        // Create socket
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Error creating socket" << std::endl;
            return ;
        }
        cout<<"inside connectPeer.cpp thread lambda"<<endl;

        peerAddr.sin_family = AF_INET;
        peerAddr.sin_port = htons(stoi(PEER_PORT));
        inet_pton(AF_INET, PEER_IP.c_str(), &peerAddr.sin_addr);

        // Connect to the tracker
        if (connect(clientSocket, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) == -1) {
            perror("Error connecting to the peer");
            return ;
        }
        char buffer[chunk_info_buffer_size];
        string finalCommand = "chunkInfo " + fileName + " \0";
        send(clientSocket, finalCommand.c_str(), finalCommand.length(), 0);
        recv(clientSocket, buffer, chunk_info_buffer_size, 0);
        cout<<"response from peer server, Chunk_no: "<<buffer<<endl;

        vector<string> chunk_no;
        tokenise(buffer,'\n', chunk_no);

        {
            std::lock_guard<std::mutex> lock(chunkPeerMutex);
            for( string& chunk: chunk_no){
                chunkPeer[chunk].push_back({PEER_IP, PEER_PORT});
            }
        }
        tasksCompleted++;

        if(tasksCompleted >= totalTasks){
            std::unique_lock<std::mutex> lock(cv_mtx);
            cv.notify_all();
        }
        return true;
    };
}

///////////////////////////////////////////////

std::function<bool()> downloadChunk(string PEER_IP, string PEER_PORT, string fileName, 
    string chunk_no, int fd) {
    return [PEER_IP, PEER_PORT, fileName, chunk_no, fd]() -> bool {

        sockaddr_in peerAddr;
        int clientSocket;
        // Create socket
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Error creating socket" << std::endl;
            return ;
        }
        cout<<"inside connectPeer.cpp thread lambda"<<endl;

        peerAddr.sin_family = AF_INET;
        peerAddr.sin_port = htons(stoi(PEER_PORT));
        inet_pton(AF_INET, PEER_IP.c_str(), &peerAddr.sin_addr);

        // Connect to the tracker
        if (connect(clientSocket, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) == -1) {
            perror("Error connecting to the peer");
            return ;
        }
        
        char shaBuffer[41];
        char fileBuffer[chunk_size];
        string finalCommand = "download " + fileName + " " + chunk_no + " \0";
        send(clientSocket, finalCommand.c_str(), finalCommand.length(), 0);
        recv(clientSocket, shaBuffer, 41, 0);
        cout<<"response from peer server, sha: "<<shaBuffer<<endl;

        int offset = stoi(chunk_no)*chunk_size;
        off_t file_offset = lseek(fd, offset, SEEK_SET);

        string chunkSHA = "";
        while (true) {
            int bytesReceived = recv(clientSocket, fileBuffer, chunk_size, 0);
            if (bytesReceived <= 0) {
                // End of file or error
                break;
            }
            std::string str(fileBuffer, bytesReceived);
            chunkSHA += str;
            write(fd, fileBuffer, bytesReceived);
        }

        if (chunkSHA == shaBuffer){
            return true;
        }
        return false;
    };
}





/////////////////////////////////////////////////
void p2pClient(vector<string>& tracker_response, string group_id, string fileName, string DOWNLOAD_PATH){
    cout<<"inside connectPeer.cpp p2pClient"<<endl;

    // to store which chunk is available with how many peers
    unordered_map<string, vector<pair<string, string>>> chunkPeer; // key = chunk-No, val= vector<pair<IP, PORT>>

    std::atomic<int> tasksCompleted(0);
    std::condition_variable cv;
    std::mutex cv_mtx;

    ThreadPool pool(10);
    std::mutex chunkPeerMutex;
    int n = tracker_response.size();
    int totalTasks = (n-1)/2;

    for(int i=1; i<n; i+=2){
        auto getChunkInfoTask = getChunkInfo(tracker_response[i], tracker_response[i+1], fileName, chunkPeer, chunkPeerMutex,
         tasksCompleted, totalTasks, cv, cv_mtx);
        pool.Submit(getChunkInfoTask);
    }

    // wait till all threads finish their task
    std::unique_lock<std::mutex> lock(cv_mtx);
    cv.wait(lock, [&tasksCompleted, &totalTasks]{
        return tasksCompleted >= totalTasks;
    });
    
    // open the file where chunks downloaded will be written
    string downloadFilePath = DOWNLOAD_PATH + "/" + fileName;
    int fd = open(downloadFilePath.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd == -1){
        perror("open");
        exit(1);
    }

    ThreadPool downloadChunkPool(10);
    string fileSHA = tracker_response[0];

    for(auto& chunk_no: chunkPeer){
        bool result =false;
        int i=0;
        while(!result){
            cout<<"SHA false number= "<<i<<endl;
            int peerCount = chunk_no.second.size();
            int random_number = (std::rand())%peerCount;
            auto downloadChunkTask = downloadChunk(chunk_no.second[random_number].first,
             chunk_no.second[random_number].second, fileName, chunk_no.first, fd);
            bool Result = downloadChunkPool.Submit(downloadChunkTask);
            // result = futureResult.get();
        }
        if(files_for_upload.find(fileName) == files_for_upload.end()){
            files_for_upload[fileName] = {downloadFilePath, {stoi(chunk_no.first)}};
            std::thread(informTracker, fileName, group_id, fileSHA).detach();
        }
        else{
            files_for_upload[fileName].second.insert(stoi(chunk_no.first));
        }
    }

}

void handleDownload(vector<string>& tracker_response, string group_id, string fileName, string DOWNLOAD_PATH){
    cout<<"inside connectPeer.cpp handleDownload"<<endl;

    p2pClient(tracker_response, group_id, fileName, DOWNLOAD_PATH);
    // unordered_map<string

}
