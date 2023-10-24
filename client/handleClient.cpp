#include"peer_header.h"

string processClientCommands(int clientSocket, vector<string>& command, std::mutex& readMutex){
    if(command[0] == "chunkInfo"){
        cout<<"inside chunkinfo"<<endl;
        string response;
        for(auto chunk_no: files_for_upload[command[1]].second){
            response.append(to_string(chunk_no));
            response.push_back('\n');
        }
        response.push_back('\n');
        send(clientSocket, response.c_str(), response.length(), 0);

        return response;
    }

    else if(command[0] == "download"){
        string filePath = files_for_upload[command[1]].first;
        int chunk_no = stoi(command[2]);
        int fd = open(filePath.c_str(), O_RDONLY);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        int bytesRead;
        char readBuffer[chunk_size];
        {
            std::lock_guard<std::mutex> lock(readMutex);
            int offset = lseek(fd, chunk_size*chunk_no, SEEK_SET);
            bytesRead = read(fd, readBuffer, chunk_size);
        }
        string chunkSHA = calculateChunkSHA1(readBuffer, bytesRead);
        send(clientSocket, chunkSHA.c_str(), chunkSHA.length(), 0);
        send(clientSocket, readBuffer, bytesRead, 0);
        return chunkSHA;
    }
    return "";
}

const int receive_buffer_size = 1024;
const int send_buffer_size = 16'384;

void handleClient(int clientSocket, std::mutex& readMutex) {
    char receive_buffer[receive_buffer_size];
    char send_buffer[send_buffer_size];
    int bytesRead;

    while (true) {
        bytesRead = recv(clientSocket, receive_buffer, receive_buffer_size, 0);
        if (bytesRead <= 0) {
            close(clientSocket);
            // std::lock_guard<std::mutex> lock(mtx);
            // clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
            break;
        }
        // buffer[bytesRead] = '\0';
        cout<<"command received: "<<receive_buffer<<endl;
        vector<string> command;
        tokenise(receive_buffer, ' ', command);
        string response =  processClientCommands(clientSocket, command, readMutex);
        cout<<"action: "<<response<<endl;
        // Echo the message back to the client
        // send(clientSocket, response.c_str(), response.length(), 0);
    }
}

