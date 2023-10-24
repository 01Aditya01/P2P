#include"peer_header.h"

char* MY_IP;
int MY_PORT;

const char* TRACKER_IP;
int TRACKER_PORT;
const int chunk_size = 512 * 1024; //Bytes
const int tracker_response_buffer_size = 1024;
const int chunk_info_buffer_size = 16'384;

unordered_map<string, pair<string, unordered_set<int>>> files_for_upload;
unordered_map<string, unordered_map<string, string>> downloads;

int main(int argc, char** argv) {
    if(argc < 3){
        cerr << "insufficient arguements provided, 3 arguements needed" <<endl;
        exit(1);
    }
    cout<<"befor ip"<<endl;
    getIpPort(argv[1]);
    cout<<"MY_IP: "<<MY_IP<<endl;
    cout<<"MY_PORT: "<<MY_PORT<<endl;
    getTrackerInfo(argv[2]);
    std::thread(startServer).detach(); // Start the server in a separate thread
    sleep(2);
    int clientSocket;
    sockaddr_in trackerAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    trackerAddr.sin_family = AF_INET;
    trackerAddr.sin_port = htons(TRACKER_PORT);
    inet_pton(AF_INET, TRACKER_IP, &trackerAddr.sin_addr);

    // Connect to the tracker
    if (connect(clientSocket, (struct sockaddr*)&trackerAddr, sizeof(trackerAddr)) == -1) {
        perror("Error connecting to the server");
        return 1;
    }
    char buffer[tracker_response_buffer_size];
    while (true) {
        std::string message;
        std::cout << "Enter a command (or 'quit' to exit): ";
        std::getline(std::cin, message);

        try{
            string finalCommand = processCommands(message);
            finalCommand.push_back('\0');
            cout<<"final command: "<<finalCommand<< endl;
            // string firstArg = firstWord(finalCommand);

            vector<string> args = allWords(finalCommand);
            cout<<"args= "<<args[0]<<endl;
            send(clientSocket, finalCommand.c_str(), finalCommand.length(), 0);
            recv(clientSocket, buffer, tracker_response_buffer_size, 0);
            cout<<"tracker response: "<< buffer<<"\n";
            if(args[0] == "download_file"){
                cout<<"inside peer.cpp if download_file"<<endl;
                vector<string> tracker_response;
                tokenise(buffer, '\n', tracker_response);
                handleDownload(tracker_response, args[2]);
                
            }
        }catch (const std::exception& e){
            cerr << "Error: " <<e.what() <<"\n";
        }        
    }

    close(clientSocket);
    return 0;
}
