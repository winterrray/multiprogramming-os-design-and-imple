#include <bits/stdc++.h>

using namespace std;

    char IR[4];
    char R[4];
    int  IC,SI;
    bool Toggle;
    char Memory[100][4];
    fstream inFile;
    fstream outFile;
void init(){
    for (int i = 0; i < 100; i++){
        for (int j = 0; j < 4; j++){
            Memory[i][j] = ' ';
        }
    }
    IR[4] = {'-'};  //instruction reg
    R[4] = {'-'};   // reg
    IC = 0;         //instruction counter
    Toggle = false;
}

void mos(){
    if (SI == 1){                       //gd
        string line;
        getline(inFile, line);
        int add = IR[2] - 48;
        add = (add * 10) + (IR[3] - 48);
        int l=0;
        for (int i = 0; i < line.length() && line.length() < 40; i++){
            Memory[add][l++] = line[i];
            if(l==4){
                l=0;
                add+=1;
            }
        }
    }
    else if (SI == 2){                              //pd
        int add = IR[2] - 48;
        add = (add * 10);
        string out;
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 4; j++){
                out += Memory[add][j];
            }
            add+=1;
        }
        
        outFile << out <<"\n";
    }

    else if (SI == 3){                              //halt
        outFile<<"\n";
    }
}

void ExecuteUserProgram(){
    while (true){
        for (int i = 0; i < 4; i++){
            IR[i] = Memory[IC][i];
        }
        IC++;

        if (IR[0] == 'L' && IR[1] == 'R'){
            int add = IR[2] - 48;
            add = (add * 10) + (IR[3] - 48);

            for (int i = 0; i < 4; i++)
                R[i] = Memory[add][i];
        }

        else if (IR[0] == 'S' && IR[1] == 'R'){
            int add = IR[2] - 48;
            add = (add * 10) + (IR[3] - 48);

            for (int i = 0; i < 4; i++)
                Memory[add][i] = R[i];
        }

        else if (IR[0] == 'C' && IR[1] == 'R'){
            int flag = 0;
            int add = IR[2] - 48;
            add = (add * 10) + (IR[3] - 48);

            for (int i = 0; i < 4; i++){
                if (R[i] != Memory[add][i])
                    flag = 1;
            }

            if (flag == 1)
                Toggle = false;
            else
                Toggle = true;
        }

        else if (IR[0] == 'B' && IR[1] == 'T'){
            int add = IR[2] - 48;
            add = (add * 10) + (IR[3] - 48);
            if (Toggle == true)
                IC = add;
        }

        else if (IR[0] == 'G' && IR[1] == 'D'){
            SI = 1;
            mos();
        }

        else if (IR[0] == 'P' && IR[1] == 'D'){
            SI = 2;
            mos();
        }

        else if (IR[0] == 'H'){
            SI = 3;
            mos();
            cout << "Halt" << endl;
            break;
        }
    }
}

void Load(){
    int block = 0;
    string line;
    while (getline(inFile, line)){
        string str = "";
        for (int i = 0; i < 4; i++){
            str += line[i];
        }
        if (str == "$AMJ"){
            block=0;
            init();
        }
        else if (str == "$DTA"){
            ExecuteUserProgram();
        }
        else if (str == "$END"){
            for(int i = 0; i<50; i++){
                cout<<"M["<<i<<"]\t";
                for(int j = 0; j<4; j++ ){
                    cout<<Memory[i][j];
                }
                cout<<"\tM["<<i+50<<"]\t";
                for(int j = 0; j<4; j++ ){
                    cout<<Memory[i+50][j];
                }cout<<"\n";
            }
            cout<<"\n\n";
        }
        else{
            if (block >= 100){
                cout << "Memory Exceeded";
                break;
            }
            else{
                int k = 0;
                for (int i = block; i < block+10 && k < line.length(); i++){
                    for (int j = 0; j < 4 && k < line.length(); j++){
                        if(line[k]=='H'){
                            Memory[i][j] = line[k++];
                            break;
                        }
                        else{
                            Memory[i][j] = line[k++];
                        }
                    }
                }
                block += 10;
                line.clear();
            }
        }
    }
}

int main(){
    inFile.open("inputt.txt", ios::in);
    outFile.open("Output.txt", ios::out);
    if (!inFile){
        cout << "File Doesn't Exist '" << endl;
    }
    else{
        cout << "File Exist" << endl;
    }
    Load();
    return 0;
}
