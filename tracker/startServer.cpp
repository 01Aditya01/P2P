#include"tracker_header.h"

void startServer() {
    int serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // cout << "dsf" << htons(PORT) << endl;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    // cout<<"Port is "<< serverAddr.sin_port<<endl;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);


    // Bind the socket to an address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error bind");
        std::cerr << "Error binding the socket" << std::endl;
        return;
    }
    // if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {


    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on the socket" << std::endl;
        return;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        // Accept incoming connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting the client connection" << std::endl;
            continue;
        }

        // std::lock_guard<std::mutex> lock(mtx);
        // clientSockets.push_back(clientSocket);

        // Create a thread to handle the client
        std::thread(handleClient, clientSocket).detach();
    }

    close(serverSocket);
}