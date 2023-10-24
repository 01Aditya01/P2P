#include"peer_header.h"

void startClient(){
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
    while (true) {
        std::string message;
        std::cout << "Enter a command: ";
        std::getline(std::cin, message);
        // if(message == "quit") break;
        try{
            vector<string> args = allWords(message);
            string finalCommand = processCommands(message);
            finalCommand.push_back('\0');
            cout<<"final command: "<<finalCommand<< endl;
            // string firstArg = firstWord(finalCommand);

            cout<<"args= "<<args[0]<<endl;
            send(clientSocket, finalCommand.c_str(), finalCommand.length(), 0);
            recv(clientSocket, buffer, tracker_response_buffer_size, 0);
            cout<<"tracker response: "<< buffer<<"\n";
            if(args[0] == "download_file"){
                cout<<"inside peer.cpp if download_file"<<endl;
                vector<string> tracker_response;
                tokenise(buffer, '\n', tracker_response);
                // args[1]= group_id, [2] = file_name, [3] = destination_path
                handleDownload(tracker_response, args[1], args[2], args[3]);
                
            }
        }catch (const std::exception& e){
            cerr << "Error: " <<e.what() <<"\n";
        }        
    }

    close(clientSocket);

}