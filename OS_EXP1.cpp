#include <iostream>
#include <fstream>
#include<string.h>
#include <string>

using namespace std;

int MM_count =0;
int noIn=0;

void init(char MM[][4]){

    for(int i =0;i<100;i++){
        for(int j=0;j<4;j++){
            MM[i][j] = '-';
        }
    }
}

int find(char ch) {      //function for finding number after GD __ and PD __

    switch(ch){

        case '0':
            return 0;
            break;
        case '1':
            return 10;
            break;
        case '2':
            return 20;
            break;
        case '3':
            return 30;
            break;
        case '4':
            return 40;
            break;
        case '5':
            return 50;
            break;
        
        default :
            return 0;
            break;
    }
}

void Write(char IR[],char MM[][4]) {

    int sadd = find(IR[2]);

    ofstream outputFile("output.txt",ios::app); 

    for(int i =sadd;i<sadd+10;i++){
        for(int j=0;j<4;j++){
            if(MM[i][j] != '-') {
            outputFile<<MM[i][j];
            }
        }
    }
    outputFile.close();
}


void Read(string data ,char IR[],char MM[][4]){  //read data from input file and store into memory

    IR[3] = '0';  //error handling
    
    int row = find(IR[2]);
  
    int col =0;
    for(int i =0;i<data.length();i++) {
        if(col == 4){
            row++;
            col = 0;
        }
        MM[row][col] = data[i];
        col++;
    }
    MM_count = MM_count+10;
}

void Terminate(){

    ofstream outputFile("output.txt",ios::app);
    outputFile<<"\n\n"<<endl;
    outputFile.close();
}

void MOS(string data,char MM[][4],char IR[],int si) {
 
    switch(si) {

        case 1:
            Read(data,IR,MM);
            break;
        case 2:
            Write(IR,MM);
            break;
        case 3:
            Terminate();
            break;
        default:
            return;
            break;
    }
}

void executeuserprogram(string data,int Ic,char MM[][4]){
  
    int si = 3;

    int c;

    char IR[4];

    while(Ic < noIn){

    si = 3;

    for (int j = 0; j < 4; j++) {
            IR[j] = MM[Ic][j];
    }
    
    Ic++;

    char R[4];         //register
    
    string function = "";
    function = function + IR[0];
    function = function + IR[1];

    int operandAdd = find(IR[2]);

    if(function == "LR"){
        si = 0;
        //for(int i =operandAdd;i<(operandAdd+10);i++){
            for(int j = 0;j<4;j++){
                R[j] = MM[operandAdd][j];
            }
        //}
    }

    else if(function == "SR") {

        for(int i = operandAdd;i<(operandAdd+10);i++){
            for(int j=0;j<4;j++){
                MM[operandAdd][j] = R[j];
            }
        }  
        si =0;
    }

    else if(function == "CR"){

            int r_count = 0;
            bool equal = true;  // Assume equality by default

            //for (int i = operandAdd; i < (operandAdd + 10); i++) {
                for (int j = 0; j < 4; j++) {
                    if (MM[operandAdd][j] != R[j]) {
                        equal = false;  // If any character doesn't match, set equal to false
                        break;          // Exit inner loop early, no need to continue comparing
                    }
                }
                if (!equal) {
                    break;  // Exit outer loop if equal is false
                }
            //}

            if (equal)
                c=1;
            else
                c=0;
        }

    else if(function == "BT"){
        if(c == 1){
            Ic = operandAdd;
        }
    }
    else if(function == "GD"){
        si = 1;
    }
    else if(function == "PD"){
        si = 2;
    }
    else if(function == "H-"){
        si = 3;
    }
    else{
        printf("Invalid Job"); 
        return;
    }

    MOS(data,MM,IR,si);

    }
}

void startexecution(string data,char MM[][4]){
    int Ic = 0;
    executeuserprogram(data,Ic,MM);
}


void Load(char MM[][4]) {
 
    string line;

    ifstream fin;
    
    fin.open("input.txt");
 
    while (getline(fin, line)) {

        string opcode = line;
        string op = opcode.substr(0,4);
        
        if(op == "$AMJ"){                   // start
            continue;
        }
        else if(op == "$DTA"){                 // data card
            
            if (getline(fin, line)){
                string data = line;
                startexecution(data,MM);
            }
            else{
                printf("\n**Data Missing");
                break;
            }
        }
        else if(op == "$END"){              //terminate job
            MM_count = 0;
            continue;
        }
        else{                               //program card
            string inst = line;
            int row = MM_count;
            int col =0;
            for(int i =0;i<inst.length();i++){
                if(col == 4){
                    row++;
                    col = 0;
                }
                MM[row][col] = inst[i];
                col++;
            }
            noIn = row+1;
        }
    }

    fin.close();
}

int main(){

    char MM[100][4];

    init(MM);

    printf("\n Main Memory (before)\n\n");

    for (int i=0 ; i<100 ; i++ ){
        printf("%2d     ",i);
        for(int j=0;j<4;j++){
            printf("%5c ",MM[i][j]);
        }
        printf("\n");
    }
    printf("\n");
   
    Load(MM);

    printf("\n Main Memory (after)\n\n");
    
    for(int i =0;i<100;i++){
        printf("%2d     ",i);
        for(int j=0;j<4;j++){
            printf("%5c ",MM[i][j]);
        }
        printf("\n");
    }
    printf("\n");
   
    return 0;
}