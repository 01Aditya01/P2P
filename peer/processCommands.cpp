#include"peer_header.h"

string processCommands(string message){
    vector<string> command;
    tokenise(message.data(), ' ', command);

    if(command[0]== "create_user"){
        if(command.size() < 3){
            throw runtime_error("too few arguements");
        }
        // cout<<"create_user message: "<< message<<endl;
        // const char* finalCommand = message.c_str();
        return message;
    }

    else if(command[0]== "login"){
        if(command.size() < 3){
            throw runtime_error("too few arguements");
        }
        return (message + " " + MY_IP + " " + to_string(MY_PORT));
    }

    else if(command[0]== "create_group"){
        if(command.size() < 2){
            throw runtime_error("too few arguements");
        }
        return message;
    }

    else if(command[0] == "join_group"){
        if(command.size() < 2){
            throw runtime_error("too few arguements");
        }
        return message;
    }
    else if(command[0] == "leave_group"){
        if(command.size() < 2){
            throw runtime_error("too few arguements");
        }
        return message;
    }
    else if(command[0] == "list_requests"){
        if(command.size() < 2){
            throw runtime_error("too few arguements");
        }
        return message;
    }
    else if(command[0] == "accept_request"){
        if(command.size() < 3){
            throw runtime_error("too few arguements");
        }
        return message;
    }
    else if(command[0] == "list_groups"){
        return message;
    }
    else if(command[0] == "list_files"){
        if(command.size() < 2){
            throw runtime_error("too few arguements");
        }
        return message;
    }
    else if(command[0] == "upload_file"){
        if(command.size() < 3){
            throw runtime_error("too few arguements");
        }
        string sha1;
        try{
            sha1 = fileSHA(command[1]);
        }catch(const std::exception& e){
            throw;
        }

        int i = command[1].length() - 1;
        while(i>=0 && command[1][i]!='/'){
            i--;
        }
        string finalCommand, fileName;
        fileName = command[1].substr(i+1);
        finalCommand += command[0] + " " + fileName + " " + command[2] + " " + sha1;
        // finalCommand = upload_file <file_name> <group_id> <file_SHA>

        struct stat fileStat;
        int fileSize=0;
        if (stat(command[1].c_str(), &fileStat) == 0) {
            fileSize = fileStat.st_size ;
        } else {
            // std::cerr << "Error getting file size." << std::endl;
            throw runtime_error("Error getting file size");
        }
        int total_chunks = fileSize/chunk_size;
        unordered_set<int> chunks;
        for(int i=0; i<total_chunks; i++){
            chunks.insert(i);
        }

        files_uploaded[fileName] = {command[1], chunks};
        return finalCommand;
    }

    else if(command[0] == "logout"){
        return message;
    }

    throw runtime_error("invalid command");
}