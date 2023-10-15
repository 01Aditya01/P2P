#include"peer_header.h"

char* MY_IP;
int MY_PORT;

const char* TRACKER_IP;
int TRACKER_PORT;
const int chunk_size = 512 * 1024; //Bytes

unordered_map<string, pair<string, unordered_set<int>>> files_uploaded;

int main(int argc, char** argv) {
    if(argc < 3){
        cerr << "insufficient arguements provided, 3 arguements needed" <<endl;
        exit(1);
    }
    cout<<"befor ip"<<endl;
    getIpPort(argv[1]);
    cout<<"MY_IP: "<<MY_IP<<endl;
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
    char buffer[1024];
    while (true) {
        std::string message;
        std::cout << "Enter a command (or 'quit' to exit): ";
        std::getline(std::cin, message);

        try{
            string finalCommand = processCommands(message);
            cout<<"final command: "<<finalCommand<< endl;
            send(clientSocket, finalCommand.c_str(), sizeof(finalCommand), 0);
            recv(clientSocket, buffer, sizeof(buffer), 0);
            cout<<"tracker response: "<< buffer<<"\n";
        }catch (const std::exception& e){
            cerr << "Error: " <<e.what() <<"\n";
        }        
    }

    close(clientSocket);
    return 0;
}
