#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

void server() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, newAddr;
    socklen_t addrSize;
    char buffer[1024];

    // Create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the server socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == 0) {
        std::cout << "Server is listening..." << std::endl;
    } else {
        std::cout << "Error in listening" << std::endl;
    }

    // Accept a connection
    addrSize = sizeof(newAddr);
    newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addrSize);
    std::cout << "Client: " << buffer << std::endl;
    newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addrSize);
    std::cout << "Client: " << buffer << std::endl;

    // Receive data from the client
    recv(newSocket, buffer, 1024, 0);

    // Close sockets
    close(newSocket);
    close(serverSocket);
}

void client() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Create the client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in connection");
        exit(1);
    }

    // Send data to the server
    std::string message = "Hello from the client!";
    strcpy(buffer, message.c_str());
    send(clientSocket, buffer, strlen(buffer), 0);

    // Close the socket
    close(clientSocket);
}

int main() {
    std::thread serverThread(server);
    sleep(2);
    std::thread clientThread(client);

    serverThread.join();
    clientThread.join();

    return 0;
}
