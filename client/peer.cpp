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
    // std::thread(startServer).detach(); // Start the server in a separate thread
    // sleep(2);
    // std::thread(startClient).detach();

    // // serverThread.join();
    // // clientThread.join();

    std::thread serverThread(startServer);
    sleep(2);
    std::thread clientThread(startClient);

    serverThread.join();
    clientThread.join();
    
    return 0;
}
