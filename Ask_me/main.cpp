#include<bits/stdc++.h>
using namespace std;

struct user{
    string name{""} , password{""} , email{""};
    int id{0};
};

class user_manager{
private:
    string path{""};//the path of the file
    int id{0};//to keep the last used id
    map<string,bool>used_names,used_emails;
    map<int,user>users;

    //split every line into its data like (name,id,email,password)
    void split_lines(vector<string> lines){
        user new_user;
        for(int i = 0;i<(int)lines.size();i++){
            istringstream iss(lines[i]);
            iss>>new_user.id>>new_user.name>>new_user.email>>new_user.password;
            used_emails[new_user.email] = true , used_names[new_user.name] = true; //assign them as used
            id = max(id,new_user.id);//keep updating the id
            users[new_user.id] = new_user;
        }
        id++;
    }

    //split the whole file into lines
    vector<string> split_file(){
        vector<string>lines;
        string line;
        ifstream file_handle(path);
        if(file_handle.fail()){
            cout<<"sorry,something went wrong\n";
            exit(0);
        }
        while(getline(file_handle,line)){
            if(line!=""){
                lines.push_back(line);
            }
        }
        file_handle.close();
        return lines;
    }

    //overwrite into the users file to update the users data
    void update_users_file_data(){
        auto status = ios::in | ios::out | ios::trunc;
        ofstream file_handle(path,status);
        for(auto& pair : users){
            ostringstream oss;
            int u_id = pair.second.id;
            string name = pair.second.name , email = pair.second.email , password = pair.second.password;
            oss<<u_id<<" "<<name<<" "<<email<<" "<<password;
            file_handle<<oss.str()<<"\n";
        }
        file_handle.close();
        return;
    }

public:
    //constructor which takes the path of the users file
    user_manager(string path):path(path){}

    void load_users_file_data(){
        vector<string>lines = split_file();
        split_lines(lines);
        return;
    }

    bool check_email(string& email){
        if(used_emails.count(email) or email.size()<13){
            return false;
        }

        string base{"@gmail.com"};
        for(int i = 0,j = (int)email.size()-10;i<11;i++,j++){
            if(email[j]!=base[i]){
                return false;
            }
        }

        for(int i = 0;i<(int)email.size()-10;i++){
            if(!isalnum(email[i])){
                return false;
            }
        }

        return true;
    }

    bool check_name(string& name){
        return !used_names.count(name);
    }

    user sign_up(){
        string name{""} , email{""} ,password{""};
        int u_id{0};
        bool anonymous{false};
        user new_u;

        cout<<"Enter your name: ";
        cin>>name;

        while(!check_name(name)){
            cout<<"invalid name,please try again\n";
            cout<<"Enter your name: ";
            cin>>name;
        }

        cout<<"Enter your email: ";
        cin>>email;

        while(!check_email(email)){
            cout<<"invalid email,please try again\n";
            cout<<"Enter your email: ";
            cin>>email;
        }

        cout<<"Enter your password: ";
        cin>>password;

        u_id = id++;
        new_u.id = u_id,new_u.name = name,new_u.email = email,new_u.password = password,new_u.name = name;
        used_emails[email] = true , used_names[name] = true;
        users[u_id] = new_u;

        cout<<"successfully signed up\n";
        return new_u;
    }

    user sign_in(){
        int u_id;
        string password{""};
        cout<<"Enter your id: ";
        cin>>u_id;

        while(users.count(u_id)==0){
            cout<<"invalid id,please try again\n";
            cout<<"Enter your id: ";
            cin>>u_id;
        }

        cout<<"Enter your password: ";
        cin>>password;

        while(users[u_id].password!=password){
            cout<<"wrong password,please try again\n";
            cout<<"Enter your password: ";
            cin>>password;
        }

        user new_u = users[u_id];

        cout<<"successfully signed in\n";
        return new_u;
    }

    void add_new_users(){
        update_users_file_data();
        return;
    }
};


struct question{
    string q{""};
    int from_id{0} , to_id{0} , q_id{0};
    bool anonymous{false};
};

class question_manager{
private:
    int id{0};
    string path1{""} , path2{""};
    map<int,vector<question>>from_to;
    map<int,string>from_to_answer;

    vector<string> split_answer_file(){
        ifstream file_handle(path2);
        string line{""};
        vector<string>lines;

        if(file_handle.fail()){
            exit(0);
        }

        while(getline(file_handle , line)){
            if(line!=""){
                lines.push_back(line);
            }
        }

        file_handle.close();
        return lines;
    }
    void split_answer_lines(){
        vector<string>lines = split_answer_file();
        int cnt{0};
        string temp{""};

        for(auto& line : lines){
            int q_id = 0;
            string a {""};
            cnt = 0,temp = "",line+=',';

            for(auto& c : line){
                if(c==','){
                    cnt++;
                    if(cnt==1){
                        q_id = stoi(temp),temp = "";
                    }else{
                        a+=temp,temp = "";
                    }
                }else{
                    temp+=c;
                }
            }
            if(a!="")
                from_to_answer[q_id] = a;
        }
    }

    vector<string> split_question_file(){
        ifstream file_handle(path1);
        string line{""};
        vector<string>lines;

        if(file_handle.fail()){
            exit(0);
        }

        while(getline(file_handle,line)){
            if(line!=""){
                lines.push_back(line);
            }
        }

        file_handle.close();
        return lines;
    }

    void split_question_lines(){
        vector<string> lines = split_question_file();
        string temp{""};
        int cnt{0};

        for(int i = 0;i<(int)lines.size();i++){
            lines[i]+=',',cnt = 0;
            question new_q;
            for(int j = 0;j<(int)lines[i].size();j++){
                if(lines[i][j]==','){
                    cnt++;
                    if(cnt==1){
                        new_q.from_id = stoi(temp),temp = "";
                    }
                    else if(cnt==2){
                        new_q.to_id = stoi(temp),temp = "";
                    }
                    else if(cnt==3){
                        new_q.q_id = stoi(temp),temp = "";
                    }
                    else if(cnt==4){
                        new_q.anonymous = stoi(temp),temp="";
                    }
                    else{
                        new_q.q = temp,temp = "";
                    }
                }else{
                    temp+=lines[i][j];
                }
            }
            id = max(id,new_q.q_id);
            from_to[new_q.from_id].push_back(new_q);
        }
        id++;
    }

    void update_questions_answers_file_data(){
        auto status = ios::in | ios::out | ios::trunc;
        ofstream file_handle1(path1,status) , file_handle2(path2,status);

        for(auto &pair : from_to){
            for(auto &x : pair.second){
                string line1 = "" , line2 = "";
                line1+=to_string(x.from_id) + "," + to_string(x.to_id) + "," + to_string(x.q_id) + "," + to_string(x.anonymous) + "," + x.q;
                file_handle1<<line1<<"\n";

                if(from_to_answer.count(x.q_id)){
                    line2+=to_string(x.q_id) + "," + from_to_answer[x.q_id];
                    file_handle2<<line2<<"\n";
                }

            }
        }
    }
public:
    question_manager(string path1,string path2):path1(path1) , path2(path2){}
    //
    void load_question_answers_files_data(){
        split_answer_lines();
        split_question_lines();
        return;
    }

    void ask_question(user& dummy_user){
        question new_q;
        string line{""};

        new_q.from_id = dummy_user.id;
        cout<<"Enter other id :: ";
        cin>>new_q.to_id;
        cout<<"would you like to ask anonymously or not (1 or 0) :: ";
        cin>>new_q.anonymous;
        cin.ignore();
        cout<<"Enter the question :: ";
        getline(cin,new_q.q);

        new_q.q_id = id;
        from_to[new_q.from_id].push_back(new_q);
        id++;
    }

    void answer_question(user& dummy_user){
        int q_id = 0 , choice;
        string ans = "";
        bool is = false;

        cout<<"Enter question id :: ";
        cin>>q_id;

        for(auto& pair : from_to){
                for(auto& ques : pair.second){
                    if(ques.q_id == q_id and ques.to_id == dummy_user.id){
                        is = true;
                        break;
                    }
                }
                if(is)
                    break;
        }

        if(!is){
            cout<<"this question has not been asked to you\n";
            return;
        }

        if(from_to_answer.count(q_id)){
            cout<<"this question has been answered before\n";
            cout<<from_to_answer[q_id]<<"\n";
            cout<<"1) delete the answer\n2) update the answer\n";
            cin>>choice;
            if(choice == 1){
                from_to_answer.erase(q_id);
                return;
            }
        }
        cin.ignore();
        cout<<"Enter your answer :: ";
        getline(cin,ans);
        from_to_answer[q_id] = ans;
        return;
    }

    void print_questions_to_me(user& dummy_user){
        bool is = false;

        for(auto& pair : from_to){
            for(auto& ques : pair.second){
                if(ques.to_id == dummy_user.id){
                    if(ques.anonymous){
                        cout<<"from : anonymous | ";
                    }else{
                        cout<<"from : "<<ques.from_id<<" | ";
                    }
                    cout<<"question : "<<ques.q<<"\n";
                    if(from_to_answer.count(ques.q_id)){
                        cout<<"\t\tAnswer :: "<<from_to_answer[ques.q_id]<<"\n";
                    }
                    is = true;
                }
            }
        }

        if(!is){
            cout<<"nobody has asked you yet\n";
        }

        return;
    }

    void print_questions_from_me(user& dummy_user){
        if(from_to[dummy_user.id].size() == 0){
            cout<<"you have not asked any question before\n";
            return;
        }

        for(auto& ques : from_to[dummy_user.id]){
            cout<<"to "<<ques.to_id<<" | question :: "<<ques.q<<"\n";
            if(from_to_answer.count(ques.q_id)){
                cout<<"\t\tAnswer :: "<<from_to_answer[ques.q_id]<<"\n";
            }
        }

        return;
    }

    void delete_question(user& dummy_user ){
        int q_id;
        bool is = false;
        vector<question>&v = from_to[dummy_user.id];

        cout<<"Enter the question id : ";
        cin>>q_id;

        if(!v.size()){
            cout<<"there are no questions from you \n";
            return;
        }

        for(auto it = v.begin() ; it!= v.end() ; it++){
            if(it->from_id == dummy_user.id and q_id == it->q_id){
                is = true;
                from_to_answer.erase(q_id);
                v.erase(it);
                return;
            }
        }

        if(!is){
            cout<<"there is no question with this id asked from you\n";
        }

        return;
    }

    void add_new_questions_answers(){
        update_questions_answers_file_data();
    }
};

class run_system{
private:
    question_manager q_m;
    user_manager u_m;

    //acts like home page
    void menu1(){
        cout<<"1) sign in\n";
        cout<<"2) sign up\n";
        cout<<"3) Exit\n";
    }

    void menu2(){
        cout<<"1)Ask a question\n";
        cout<<"2)Delete a question\n";
        cout<<"3)Answer a question\n";
        cout<<"4)Show questions to me\n";
        cout<<"5)Show questions from me\n";
        cout<<"6)Exit\n";
    }

public:
    //constructor which calls question manager and user manager constructors
    run_system(string path1,string path2,string path3):q_m(path1,path2) , u_m(path3){ //takes the paths of files
        q_m.load_question_answers_files_data();
        u_m.load_users_file_data();
    }
    //show menus and take responses
    void run(){
        int choice1,choice2;
        user current_user;

        menu1();
        cin>>choice1;

        if(choice1==1){
            current_user = u_m.sign_in();
        }else if(choice1==2){
            current_user = u_m.sign_up();
        }

        if(choice1==1 or choice1==2){
            menu2();
            cin>>choice2;
            if(choice2==1){
                q_m.ask_question(current_user);
            }else if(choice2==2){
                q_m.delete_question(current_user);
            }else if(choice2==3){
                q_m.answer_question(current_user);
            }else if(choice2==4){
                q_m.print_questions_to_me(current_user);
            }else if(choice2==5){
                q_m.print_questions_from_me(current_user);
            }
        }
        //update the files with new data
        u_m.add_new_users();
        q_m.add_new_questions_answers();
        cout<<"Thank u :)\n";
        exit(0);
    }
};

int main(){
    string path1{"questions.txt"};
    string path2{"answers.txt"};
    string path3{"users.txt"};

    run_system s(path1,path2,path3);
    s.run();
}
