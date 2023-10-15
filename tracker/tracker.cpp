#include"tracker_header.h"

int PORT;
const char* SERVER_IP;
const char* tracker_info_path;
int tracker_no;
unordered_map<string, vector<string>> user_connection_info;
unordered_map<string, string> login;
unordered_map<string, pair<string, string>> user_ip_port;
unordered_map<string, unordered_set<string>> group_join;
unordered_map<string, pair<string, unordered_set<string>>> group_members;
unordered_map<string, unordered_map<string, pair<string, unordered_set<string>>>> file_peers;
unordered_map<string, unordered_map<string, unordered_set<string>>> user_upload;

int main(int argc, char** argv) {
    if(argc < 3){
        cerr << "insufficient arguements provided, 3 arguements needed" <<endl;
        exit(1);
    }
    tracker_info_path = argv[1];
    tracker_no = stoi(argv[2]);
    getTrackerInfo();

    std::thread(startServer).detach(); // Start the server in a separate thread
    sleep(2);
    
    string input_cmd;
    cin>>input_cmd;

    if(input_cmd == "quit"){
        exit(1);
    }
    return 0;
}
