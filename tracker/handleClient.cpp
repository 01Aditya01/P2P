#include"tracker_header.h"

void handleClient(int clientSocket) {
    char buffer[1024];
    int bytesRead;

    string user_id;

    while (true) {
        bzero(buffer, sizeof(buffer));
        bytesRead = recv(clientSocket, buffer, buffer_size, 0);
        if (bytesRead <= 0) {
            close(clientSocket);
            // std::lock_guard<std::mutex> lock(mtx);
            // clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
            break;
        }
        cout<<"command received: "<<buffer<<endl;
        vector<string> command;
        tokenise(buffer, ' ', command);
        string response =  processCommands(command, clientSocket, user_id);
        // buffer[bytesRead] = '\0';
        // std::cout << "Received: " << buffer << std::endl;

        // Echo the message back to the client
        response.push_back('\0');
        cout<<"action: " <<response<<endl;;
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}

string processCommands(vector<string>& command, int clientSocket, string& user_id){
    if(command[0] == "create_user"){

        if(login.find(command[1])!=login.end()){
            return "user already exists";
        }
        login[command[1]] = command[2]; 
        return "user created successfully";    
    }

    else if(command[0] == "login"){
        // command [1]= user_id, [2]=pass, [3]=user's server ip, [4]=user's server port
        if(user_connection_info.find(command[1])!=user_connection_info.end()){
            return "user already logged in";
        }
        if(login.find(command[1])==login.end()){
            return "no such user exists";
        }
        if(login[command[1]]!=command[2]){
            return "invalid id/password";
        }

        user_id = command[1];
        // save the socket, ip, port of the logged in user
        user_connection_info[command[1]] = {to_string(clientSocket), command[3], command[4]};
        return "user logged in successfully";    


    }

    else if(command[0] == "create_group"){
        if(user_id == ""){
            return "login required";
        }
        if(group_members.find(command[1])!=group_members.end()){
            return "group already exists";
        }
        string cur_user = user_id;
        group_members[command[1]] = {cur_user, {cur_user}};
        return "group created successfully";    

    }

    else if(command[0] == "join_group"){
        if(user_id == ""){
            return "login required";
        }
        if(group_members.find(command[1])==group_members.end()){
            return "no such group exists";
        }
        if(group_members[command[1]].second.find(user_id)!=group_members[command[1]].second.end()){
            return "user is already a member of the specified group";
        }
        string cur_user = user_id;
        group_join[command[1]].insert(user_id);
        return "group join request logged successfully";    

    }


    else if(command[0] == "leave_group"){
        if(user_id == ""){
            return "login required";
        }
        if(group_members.find(command[1])==group_members.end()){
            return "no such group exists";
        }
        if(group_members[command[1]].second.find(user_id) == group_members[command[1]].second.end()){
            return "you are not a member of the specified group";
        }
        group_members[command[1]].second.erase(user_id);
        if(group_members[command[1]].first == user_id){
            if(group_members[command[1]].second.empty()) group_members.erase(command[1]);
            else group_members[command[1]].first = *group_members[command[1]].second.begin();
        }
        return "user was removed from the specified group successfully";
        
    }


    else if(command[0] == "list_requests"){
        if(user_id == ""){
            return "login required";
        }
        if(group_members.find(command[1])==group_members.end()){
            return "no such group exists";
        }
        if(group_members[command[1]].first != user_id){
            return "you are not authorized to access this group's join requests";
        }
        // cout<<"pending join requests:"<<endl;
        // int n = group_join[command[1]].size();

        string list;
        for(auto& user: group_join[command[1]]){
            list.append(user);
            list.push_back('\n');
            // cout<<user<<"\n";
        }
        return list;
    }


    else if(command[0] == "accept_request"){
        if(user_id == ""){
            return "login required";
        }
        if(group_members.find(command[1])==group_members.end()){
            return "no such group exists";
        }
        if(group_members[command[1]].first != user_id){
            return "you are not authorized to accept this group's join request";
        }
        if(group_join[command[1]].find(command[2]) == group_join[command[1]].end()){
            return "no such pending join requested";
        }
        group_join[command[1]].erase(command[2]);
        group_members[command[1]].second.insert(command[2]);
        
        return "group join requested successfully";
    }

    else if(command[0] == "list_groups"){
        if(user_id == ""){
            return "login required";
        }
        string list;
        for(auto& user: group_members){
            list.append(user.first);
            list.push_back('\n');
            // cout<<user<<"\n";
        }
        return list;
    }

    else if(command[0] == "list_files"){
        if(user_id == ""){
            return "login required";
        }
        if(group_members.find(command[1])==group_members.end()){
            return "no such group exists";
        }
        if(group_members[command[1]].second.find(user_id)==group_members[command[1]].second.end()){
            return "you are not a member of this group";
        }
        string list;
        if(file_peers.find(command[1]) == file_peers.end()){
            return list;
        }

        for(auto& file: file_peers[command[1]]){
            list.append(file.first);
            list.push_back('\n');
            // cout<<user<<"\n";
        }
        return list;
    }

    else if(command[0] == "upload_file"){
        if(user_id == ""){
            return "login required";
        }
        if(group_members.find(command[2])==group_members.end()){
            return "no such group exists";
        }
        if(group_members[command[2]].second.find(user_id)==group_members[command[2]].second.end()){
            return "you are not a member of this group";
        }
        if(file_peers[command[2]].find(command[1])!=file_peers[command[2]].end()){
            if (file_peers[command[2]][command[1]].first != command[3]){
                return "a different file with same name already exists. Choose a different file name";
            }
            
        }
        else file_peers[command[2]][command[1]].first = command[3];
        file_peers[command[2]][command[1]].second.insert(user_id); 
        user_upload[user_id][command[2]].insert(command[1]);
        return "file uploaded successfully";

    }

    else if(command[0] == "logout"){
        if(user_id == ""){
            return "no user is currently logged in";
        }
        user_connection_info.erase(user_id);
        if(user_upload.find(user_id) != user_upload.end()){
            for(auto& group : user_upload[user_id]){
                for(auto& file: group.second){
                    file_peers[group.first][file].second.erase(user_id);
                    if(file_peers[group.first][file].second.empty()) file_peers[group.first].erase(file);
                }
                if(file_peers[group.first].empty()) file_peers.erase(group.first);
            }
            user_upload.erase(user_id);
            user_connection_info.erase(user_id);
        }
        user_id = "";
        return "user logged out successfully";
    }

    else if(command[0] == "download_file"){
        if(user_id == ""){
            return "Error: login required";
        }
        if(group_members.find(command[1])==group_members.end()){
            return "Error: no such group exists";
        }
        if(group_members[command[1]].second.find(user_id)==group_members[command[1]].second.end()){
            return "Error: you are not a member of this group";
        }
        if(file_peers.find(command[1]) == file_peers.end()){
            return "Error: this group doesn't contain the given file";
        }
        if(file_peers[command[1]].find(command[2]) == file_peers[command[1]].end()){
            return "Error: this group doesn't contain the given file";
        }

        string response = file_peers[command[1]][command[2]].first;
        for(auto& peer: file_peers[command[1]][command[2]].second){
            response.push_back('\n');
            response.append(user_connection_info[peer][1]);
            response.push_back('\n');
            response.append(user_connection_info[peer][2]);
        }
        return response;
    }

    return "invalid command";
}