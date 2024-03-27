 #include <bits/stdc++.h>
using namespace std;

struct PCB{
    int job_id;
    int TTL;                //total time limit
    int TLL;                //total line limit
    int TTC ;               //total time counter
    int TLC ;               //total line counter
};

struct PCB proc;

int ptr;                        //page table register
int visited[30];                //virtual group of 10
char M[300][4];                 //main memory
char IR[4];                     //instruction reg
char R[4];                      // reg
int IC;                         // instruction counter
bool C;                         //toggle reg
int SI;                         //sys intrpt
int VA;                         //
int RA;                         //
int PI;                         //programm intrpt
int TI;                         //time intrpt
int EM;                         //error messg
fstream inFile;
fstream outFile;

unordered_map<int, string> errors = {{0, "No Error"},{1, "Out of Data"},{2, "Line Limit Exceeded"},{3, "Time Limit Exceeded"},{4, "Operation Code Error"},{5, "Operand Error"},{6, "Invalid Page Fault"}};

void init(){
    for (int i = 0; i < 300; i++){                  //clearing memory
        for (int j = 0; j < 4; j++){
            M[i][j] = ' ';
        }
    }

    for(int i=0;i<30;i++){                          //clearing visited flags
        visited[i]=0;
    }

    IR[4] = {'-'};                                  //clearing ir and r ,, em,ti,ot,va,ptr,ic to 0 ,,c false
    R[4] = {'-'};
    IC = 0;
    C = false;
    ptr = 0;
    VA = 0;
    PI = 0;
    TI = 0;
    EM = 0;
}

int ALLOCATE(){                     //returning a random value lower than 30
    return (rand() % 30);
}

int ADDRESSMAP(int va){                 ///error 6
    int pte = ptr*10 + va / 10; // page table entry ,register  virtual address
    string temp = "";
    if (M[pte][0] == '*'){
        cout << "Page Fault" << endl;               //page  fault 66
        return -1;
    }else{
        for (int i = 0; i < 4; i++){
            if(M[pte][i]!=' ')
                temp += M[pte][i];
        }
        return ((stoi(temp) * 10) + (va % 10));
    }
}

int terminate(int Code){                    // print in file cause of termination in case of error
    cout<<"\n"<< errors[Code]<<endl;
    outFile<<"\nProgram Terminated abnormally"<<endl;
    outFile<<errors[Code]<<"\n\n";
}

void MOS(){                                         //errors 1,2
    if (SI == 1){
        string line;
        getline(inFile, line);
        if(line[0]=='$' && line[1]=='E' && line[2]=='N' && line[3]=='D'){       //$end
            EM=1;
            terminate(1);                                   //11 error due to end when  data is asked
            return;
        }
        int frame = ALLOCATE();
        while (visited[frame] != 0){
            frame = ALLOCATE();
        }
        visited[frame] = 1;
        int i = ptr;
        i = i*10;
        cout<<"\n\nPTR = "<<ptr<<endl;
        while (M[i][0] != '*'){
            i++;
        }
        int temp = frame / 10;
        M[i][0] = ' ';
        M[i][1] = ' ';
        M[i][2] = temp + 48;
        M[i][3] = frame % 10 + 48;
        int l = 0;
        frame = frame * 10;
        for (int i = 0; i < line.length() && line.length() < 40; i++){
            M[frame][l++] = line[i];
            if (l == 4){
                l = 0;
                frame += 1;
            }
        }
    }else if (SI == 2){
        proc.TLC+=1;               //increase line counter
        if(proc.TLC > proc.TLL){
            EM = 2;
            terminate(2);                   //line limit 22 as counter greater than limit
            return;
        }
        int add = IR[2] - 48;
        add = (add * 10);
        int ra = ADDRESSMAP(add);
        if (ra != -1){
            string out;
            for (int i = 0; i < 10; i++){
                for (int j = 0; j < 4; j++){
                    out += M[ra][j];
                }
                ra += 1;
            }
            outFile << out << "\n";
        }else{
            EM = 6;
            terminate(6);                   //invalid catche found
            PI=3;
        }
    }else if (SI == 3){
        outFile<<"\nProgram terminated successfully"<<"\n";
        outFile<<"IC = "<<IC<<"\tToggle: "<<C<<"\tTLC: "<<proc.TLC<<"\tTTC: "<<proc.TTC<<"\tTTL"<<proc.TTL<<"\tTLL"<<proc.TLL<<"\tJobId: "<<proc.job_id<<"\n";
        for(int i=0;i<3;i++){
                outFile<<"\t"<<IR[i];
            }
    }
}

void EXECUTE(){                                     //errors 5,3,4
    while (true){
        if(PI!=0 || TI!=0 || EM!=0){
            outFile<<"IC = "<<IC<<"\tToggle: "<<C<<"\tTLC: "<<proc.TLC<<"\tTTC: "<<proc.TTC<<"\tTTL"<<proc.TTL<<"\tTLL"<<proc.TLL<<"\tJobId: "<<proc.job_id<<"\n";
            for(int i=0;i<3;i++){
                outFile<<"\t"<<IR[i];
            }
            break;
        }

        RA = ADDRESSMAP(IC);

        if(M[RA][0]!='H' && (!isdigit(M[RA][2]) || !isdigit(M[RA][3]))){
            EM = 5;
            terminate(5);                   //if h is not present  and the next two char are not numbers then it is terminated  for Operand error 55
            outFile<<"IC = "<<IC<<"\tToggle: "<<C<<"\tTLC: "<<proc.TLC<<"\tTTC: "<<proc.TTC<<"\tTTL: "<<proc.TTL<<"\tTLL: "<<proc.TLL<<"\tJobId: "<<proc.job_id<<"\n";
            for(int i=0;i<3;i++){
                outFile<<"\t"<<IR[i];
            }
        }
        for (int i = 0; i < 4; i++){
            IR[i] = M[RA][i];
        }
        IC++;
        int add = IR[2] - 48;
        add = (add * 10) + (IR[3] - 48);

        if((IR[0]=='G' && IR[1]=='D') || (IR[0]=='S' && IR[1]=='R'))            //increase time counter 2 for gd and sr
            proc.TTC+=2;
        else
            proc.TTC+=1;

        if(proc.TTC > proc.TTL){
            EM = 3;
            TI = 2;
            terminate(3);                                                   //time limit Exceeded 33
            outFile<<"IC = "<<IC<<"\tToggle: "<<C<<"\tTLC: "<<proc.TLC<<"\tTTC: "<<proc.TTC<<"\tTTL: "<<proc.TTL<<"\tTLL: "<<proc.TLL<<"\tJobId: "<<proc.job_id<<"\n";
            for(int i=0;i<3;i++){
                outFile<<"\t"<<IR[i];
            }
            break;
        }

        if (IR[0] == 'L' && IR[1] == 'R'){              //lr  load register
            int ra = ADDRESSMAP(add);
            if(ra == -1){
                EM=6;
                terminate(6);                                       //invalid catche found
            }else{
                for (int i = 0; i < 4; i++)
                    R[i] = M[ra][i];
            }
        }else if (IR[0] == 'S' && IR[1] == 'R'){            //sr sys reg
            int ra = ADDRESSMAP(add);
            if(ra!=-1){
                for (int i = 0; i < 4; i++)
                    M[ra][i] = R[i];
            }else{
                int frame = ALLOCATE();
                while (visited[frame] != 0){
                    frame = ALLOCATE();
                }
                visited[frame] = 1;
                int i = ptr;
                i = i*10;
                while (M[i][0] != '*'){
                    i++;
                }
                int temp = frame / 10;
                M[i][0] = ' ';
                M[i][1] = ' ';
                M[i][2] = temp + 48;
                M[i][3] = frame % 10 + 48;
                frame = frame * 10;
                for (int i = 0; i < 4; i++)
                    M[frame][i] = R[i];
            }
        }else if (IR[0] == 'C' && IR[1] == 'R'){                    //cr compare register with memory location
            int flag = 0;
            int ra = ADDRESSMAP(add);
            if(ra = -1){
                EM=6;
                terminate(6);                                       //invalid catche found
            }else{
                for (int i = 0; i < 4; i++){
                    if (R[i] != M[ra][i])
                        flag = 1;
                }
                if (flag == 1)
                    C = false;
                else
                    C = true;
            }
        }else if (IR[0] == 'B' && IR[1] == 'T'){                    //bt
            if (C == true)
                IC = add;
        }
        else if (IR[0] == 'G' && IR[1] == 'D'){                     //gd get from data
            SI = 1;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D'){                     //pd put data to file
            SI = 2;
            MOS();
        }else if (IR[0] == 'H'){                                    // h halt
            SI = 3;
            MOS();
            break;
        }else{
            EM = 4;
            terminate(4);                                           //Operation code error  if command is not included  44
            outFile<<"IC = "<<IC<<"\tToggle: "<<C<<"\tTLC: "<<proc.TLC<<"\tTTC: "<<proc.TTC<<"\tTTL: "<<proc.TTL<<"\tTLL: "<<proc.TLL<<"\tJobId: "<<proc.job_id<<"\n";
            for(int i=0;i<3;i++){
                outFile<<"\t"<<IR[i]<<"\n\n\n";
            }
            break;
        }
    }
}

void LOAD(){
    cout<<"\nReading Data..."<<endl;
    int m = 0;
    string line;
    while (getline(inFile, line)){
        string str = "";
        for (int i = 0; i < 4; i++){
            str += line[i];
        }
        if (str == "$AMJ"){  // initializing all values
            init();
            ptr = ALLOCATE();
            for (int i = ptr * 10; i < ptr * 10 + 10; i++){         //memory to ****
                for (int j = 0; j < 4; j++){
                    M[i][j] = '*';
                }
            }
            visited[ptr] = 1;
            string jobid_str = "";
            string TTL_str = "";
            string TLL_str = "";
            for (int i = 0; i < 4; i++){              //adding first job p where from 4-7 jid ,8-11 ttl and 12-15 tll
                jobid_str += line[i + 4];
                TTL_str += line[i + 8];
                TLL_str += line[i + 12];
            }
            proc.job_id = stoi(jobid_str);
            proc.TTL = stoi(TTL_str);
            proc.TLL = stoi(TLL_str);
            proc.TLC = 0;
            proc.TTC = 0;
        }
        else if (str == "$DTA"){
            EXECUTE();
        }else if (str == "$END"){ //at end print whole memory block
            for(int i = 0; i<100; i++){
                cout<<"\tM["<<i<<"]\t";
                for(int j = 0; j<4; j++ ){
                    cout<<M[i][j];
                }
                cout<<"\t\tM["<<i+100<<"]\t";
                for(int j = 0; j<4; j++ ){
                    cout<<M[i+100][j];
                }
                cout<<"\t\tM["<<i+200<<"]\t";
                for(int j = 0; j<4; j++ ){
                    cout<<M[i+200][j];
                }
                cout<<endl;
            }
            cout << "\n\t\t\t\tHalt\t\t\t\t\n\n" <<endl;
        }else{
            int frameNo = ALLOCATE();
            while (visited[frameNo] != 0){
                frameNo = ALLOCATE();
            }
            visited[frameNo] = 1;
            int i = ptr;
            while (M[i][0] != '*'){
                i++;
            }
            int temp = frameNo / 10;
            M[i][0] = ' ';
            M[i][1] = ' ';
            M[i][2] = temp + 48;
            M[i][3] = frameNo % 10 + 48;
            int len = 0;
            for (int i = frameNo * 10; i < frameNo * 10 + 10 && len < line.length(); i++){
                for (int j = 0; j < 4 && len < line.length(); j++){
                    if (line[len] == 'H'){
                        M[i][j] = line[len++];
                        break;
                    }else{
                        M[i][j] = line[len++];
                    }
                }
            }
            line.clear();
        }
    }
}

int main(){
    inFile.open("input2.txt", ios::in);
    outFile.open("Output2.txt", ios::out);
    if (!inFile){
        cout << "The file is not present!" << endl;
    }else{
        cout << "File opened successfully!" << endl;
    }
    LOAD();
    return 0;
}
