#include"peer_header.h"

void getIpPort(char* str){
    // vector<string> info;
    // char *token = strtok(str, ":");
   
    // Keep printing tokens while one of the
    // delimiters present in str[].
    // tokenise(str, ':', info);
    // while (token != NULL)
    // {
    //     info.push_back(token);
    //     // printf("%s\n", token);
    //     token = strtok(NULL, ":");
    // }
    int i, last;
    for(i=0; str[i]!='\0'; i++){
        if(str[i] == ':') last = i;
    }
    char* ip = new char[last+1];
    char* port = new char[i-last];
    strncpy(ip, str, last);
    ip[last] = '\0';
    strcpy(port, str + last + 1);
    // port[i-last] = '\0';

    MY_IP = strdup(ip);
    MY_PORT = stoi(strdup(port));

    cout<<"ip=" <<MY_IP<<endl << "port= "<<MY_PORT<<endl;

    delete []ip;
    delete []port;
    // MY_IP = info[0].c_str();
    // MY_PORT = stoi(info[1]);
}

void getTrackerInfo(char* tracker_info_path){
    char buffer[1024];
    // const char
    int fd = open(tracker_info_path, O_RDONLY);
    if(read(fd, buffer, 1024)==-1){
        perror("read");
        exit(1);
    }

   
    // Keep printing tokens while one of the
    // delimiters present in str[].
    vector<string> info;
    tokenise(buffer, '\n', info);
    // char *token = strtok(buffer, "\n");
    // while (token != NULL)
    // {
    //     info.push_back(token);
    //     printf("%s\n", token);
    //     token = strtok(NULL, "\n");
    // }
    TRACKER_IP = info[0].c_str();
    TRACKER_PORT = stoi(info[1]);
}

void tokenise(char buffer[],char delimiter, vector<string> & out){

    std::istringstream iss(buffer);
    std::string token;

    while (getline(iss, token, delimiter)) {
        out.push_back(token);
        // std::cout << token << std::endl;
    }
}

vector<string> allWords(string s){
    int n= s.length();
    int i=0;
    vector<string> words;
    string cur;
    while(i<n){
        if(s[i]==' '){
            words.push_back(cur);
            cur="";
        }
        else cur.push_back(s[i]);
        i++;
    }
    words.push_back(cur);
    return words;
}