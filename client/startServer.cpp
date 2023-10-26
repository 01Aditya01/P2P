#include"peer_header.h"

void startServer() {
    int serverSocket, clientSocket, newSocket;
    sockaddr_in serverAddr, clientAddr, newAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    socklen_t addrSize;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(MY_PORT);
    // cout<<"Port is "<< serverAddr.sin_port<<endl;
    serverAddr.sin_addr.s_addr = inet_addr(MY_IP);
    cout<<"server ip: "<<MY_IP<<endl;
    cout<<"server port: "<<MY_PORT<<endl;
    // Bind the socket to an address

    // if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    //     perror("Error in binding");
    //     exit(1);
    // }

    // // Listen for incoming connections
    // if (listen(serverSocket, 10) == 0) {
    //     std::cout << "Server is listening..." << std::endl;
    // } else {
    //     std::cout << "Error in listening" << std::endl;
    // }

    // // Accept a connection
    // addrSize = sizeof(newAddr);
    // newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addrSize);
    // cout<<"request accepted"<<endl;

        // clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);


    //////////////////////////////////
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding the socket" << std::endl;
        return;
    }
    // if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {


    // Listen for incoming connections
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening on the socket" << std::endl;
        return;
    }

    std::cout << "Server is listening on port " << MY_PORT << std::endl;

    while (true) {
        // Accept incoming connection
        cout<<"server accept wait"<< endl;
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        cout<<"after server accept"<< endl;
        // clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            // std::cerr << "Error accepting the client connection" << std::endl;
            perror("Error accepting the client connection");
            continue;
        }


        // Create a thread to handle the client
        std::mutex readMutex;
        std::thread(handleClient, clientSocket,std::ref(readMutex)).detach();
    }

    close(serverSocket);
}