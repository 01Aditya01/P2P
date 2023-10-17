#include"peer_header.h"

void informTracker(string fileName, string group_id, string fileSHA){
    int clientSocket;
    sockaddr_in trackerAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return ;
    }

    trackerAddr.sin_family = AF_INET;
    trackerAddr.sin_port = htons(TRACKER_PORT);
    inet_pton(AF_INET, TRACKER_IP, &trackerAddr.sin_addr);

    // Connect to the tracker
    if (connect(clientSocket, (struct sockaddr*)&trackerAddr, sizeof(trackerAddr)) == -1) {
        perror("Error connecting to the server");
        return ;
    }
    char buffer[tracker_response_buffer_size];

    string finalCommand = "upload_file " + fileName + " " + group_id + " " + fileSHA;
    try{
        send(clientSocket, finalCommand.c_str(), finalCommand.length(), 0);
        recv(clientSocket, buffer, tracker_response_buffer_size, 0);
        cout<<"tracker response: "<< buffer<<"\n";

    }catch(const std::exception& e){
            cerr << "Error: " <<e.what() <<"\n";
    }        
    

    close(clientSocket);

}