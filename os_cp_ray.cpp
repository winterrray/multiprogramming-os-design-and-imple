#include <iostream>
#include <fstream>
//#include <string.h>
//#include <stdlib.h>
#include <conio.h>
//#include <string>

using namespace std;

int M[100][4];
int IR[4];

void MOS(string data) {
    int SI = 3;
    switch (SI)
    {
    case 1:
        read(data, IR);
        break;
    
    case 2:
        write(data);
        break;

    case 3:
        terminate();
        break;

    default:
        break;
    }
}

void read(string data, int IR[]) {
    IR[4]=0;    //error handling
    readFromFile();

}

void write(string data) {
    write in the output file after the instruction put data
}

void terminate() {
    ofstream opFile("outputray.txt", ios::app);
    opFile<<"\n\n"<<endl;
    opFile.close();
}

void load() {
    
}

  