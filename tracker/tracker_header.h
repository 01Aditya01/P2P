#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <utility>
#include <mutex>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <fcntl.h> 
#include <stdio.h> 
#include <string.h>
#include <unordered_map>
#include <unordered_set>
using namespace std;


extern int PORT;
extern const char* SERVER_IP;
extern const char* tracker_info_path;
extern int tracker_no;
extern const int buffer_size;

extern unordered_map<string, vector<string>> user_connection_info; //key= clientsocket fd, val -> vector[0] = user_socket fd, vector[1] = IP, vector[2] = PORT

extern unordered_map<string, string> login; // user -id, password
extern unordered_map<string, pair<string, string>>  user_ip_port;// user - ip,port
extern unordered_map<string, unordered_set<string>> group_join; //key=group_id, val = set of users who have requested to join a group
extern unordered_map<string, pair<string, unordered_set<string>>> group_members; // key = group_id, val-> pair{owner->user_id, set of all members of group}
extern unordered_map<string, unordered_map<string, pair<string, unordered_set<string>>>> file_peers; // key ->group id, value-> (map -> key= file_name, val =pair(SHA, set of peers))
extern unordered_map<string, unordered_map<string, unordered_set<string>>> user_upload; // key=user_name, val= (map-> key=group_id, val = set of files uploaded by a user in a group)



//utility func
// void tokenise(char buffer[],const char* delimiter, vector<string> & out);
void startServer();
void getTrackerInfo();
void handleClient(int clientSocket);
string processCommands(vector<string>& , int, string& );
void tokenise(char buffer[],char delimiter, vector<string> & out);