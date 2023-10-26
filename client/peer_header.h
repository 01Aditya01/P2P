#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <sstream>
#include <atomic>
#include <thread>
#include <future>
#include <cstdlib>
#include <condition_variable>
#include <vector>
#include <utility>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <fcntl.h> 
#include <stdio.h> 
#include <string.h>
#include <unordered_map>
#include <unordered_set>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std;

extern char* MY_IP;
extern int MY_PORT;

extern const char* TRACKER_IP;
extern int TRACKER_PORT;
extern const int tracker_response_buffer_size;
extern const int chunk_info_buffer_size;
extern int tracker_clientSocket;
// extern const int tracker_response_buffer_size;


extern const int chunk_size ; //Bytes
extern unordered_map<string, pair<string, unordered_set<int>>> files_for_upload; // key = file_name, val =pair(file_path, set of chunk no present with me)
extern unordered_map<string, unordered_map<string, string>> downloads; //key= group_id, val = unordered_map->(key = file_name, val =download_status [D]/[C])

void getIpPort(char* str);
void getTrackerInfo(char* tracker_info_path);
void tokenise(char buffer[],char delimiter, vector<string> & out);
void startServer();
void handleClient(int clientSocket, std::mutex& readMutex);
string processCommands(string);
string fileSHA(string);
string sha1HashToString(const char*);
string calculateChunkSHA1(const char* buffer, size_t length);

string firstWord(string);
vector<string> allWords(string);
void handleDownload(vector<string>&, string, string, string);
void startClient();
void informTracker(string fileName, string group_id, string fileSHA);




class ThreadPool {
public:
    ThreadPool(int numThreads);
    // void Submit(std::function<void()> task);
    bool Submit(std::function<bool()> task);
    ~ThreadPool();

private:
    // Declare private members and helper functions in the header file
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool running;
    // Additional private members and helper functions can be declared here
};

#endif // HEADER_H
