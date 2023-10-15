#include"peer_header.h"

void handleClient(int clientSocket) {
    char buffer[1024];
    int bytesRead;

    while (true) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            close(clientSocket);
            // std::lock_guard<std::mutex> lock(mtx);
            // clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
            break;
        }
        buffer[bytesRead] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        // Echo the message back to the client
        send(clientSocket, buffer, bytesRead, 0);
    }
}