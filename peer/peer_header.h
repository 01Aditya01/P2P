#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <fcntl.h> 
#include <stdio.h> 
#include <string.h>
#include <unordered_map>
#include <unordered_set>
#include <sys/stat.h>
#include <openssl/sha.h>

using namespace std;

extern char* MY_IP;
extern int MY_PORT;

extern const char* TRACKER_IP;
extern int TRACKER_PORT;

extern unordered_map<string, pair<string, unordered_set<int>>> files_uploaded; // key = file_name, val =pair(file_path, set of chunk no present with me)
extern const int chunk_size ; //Bytes

void getIpPort(char* str);
void getTrackerInfo(char* tracker_info_path);
void tokenise(char buffer[],char delimiter, vector<string> & out);
void startServer();
void handleClient(int clientSocket);
string processCommands(string);
string fileSHA(string);
string sha1HashToString(const char*);