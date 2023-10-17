#include"peer_header.h"

string processClientCommands(vector<string>& command){
    if(command[0] == "chunkInfo"){
        cout<<"inside chunkinfo"<<endl;
        string response;
        for(auto chunk_no: files_for_upload[command[1]].second){
            response.append(to_string(chunk_no));
        }
        response.push_back('\n');
        return response;
    }
}

const int receive_buffer_size = 1024;
const int send_buffer_size = 16'384;

void handleClient(int clientSocket) {
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
        string response =  processClientCommands(command);
        cout<<"action: "<<response<<endl;
        // Echo the message back to the client
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}

