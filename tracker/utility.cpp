#include"tracker_header.h"

// void tokenise(char buffer[], const char* delimiter, vector<string> & out){
//     char *token = strtok(buffer, delimiter);
   
//     // Keep printing tokens while one of the
//     // delimiters present in str[].
//     while (token != NULL)
//     {
//         out.push_back(token);
//         token = strtok(NULL, delimiter);
//     }
// }

void tokenise(char buffer[],char delimiter, vector<string> & out){
    // char *token = strtok(buffer, delimiter);
   
    // Keep printing tokens while one of the
    // delimiters present in str[].
    // while (token != NULL)
    // {
    //     out.push_back(token);
    //     token = strtok(NULL, delimiter);
    // }

    std::istringstream iss(buffer);
    std::string token;

    while (getline(iss, token, delimiter)) {
        out.push_back(token);
        // std::cout << token << std::endl;
    }
}

void getTrackerInfo(){
    char buffer[512];
    int fd = open(tracker_info_path, O_RDONLY);
    if(read(fd, buffer, sizeof(buffer))==-1){
        perror("read");
        exit(1);
    }

    vector<string> info;
    tokenise(buffer, '\n', info);
    SERVER_IP = info[0].c_str();
    PORT = stoi(info[1]);
}