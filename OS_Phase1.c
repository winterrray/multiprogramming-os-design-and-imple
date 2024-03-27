#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MM_count = 0;
int noIn = 0;

void init(char MM[][4]) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 4; j++) {
            MM[i][j] = '-';
        }
    }
}

int find(char ch) { // function for finding number after GD __ and PD __
    switch (ch) {
        case '0':
            return 0;
        case '1':
            return 10;
        case '2':
            return 20;
        case '4':
            return 30;
        default:
            return 10;
    }
}

void Write(char IR[], char MM[][4]) {
    int sadd = find(IR[2]);     //start address
    int eadd = sadd + 9;        // end addres

    FILE* outputFile = fopen("output.txt", "a");

    for (int i = sadd; i < eadd; i++) {
        for (int j = 0; j < 4; j++) {
            if (MM[i][j] != '-') {
                fprintf(outputFile, "%c", MM[i][j]);
            }
        }
    }
    fclose(outputFile);
}

void Read(char* data, char IR[], char MM[][4]) { // read data from input file and store into memory
    IR[3] = '0';            //in case user input is wrong.. error handling

    int row = find(IR[2]);

    int col = 0;
    for (int i = 0; i < strlen(data); i++) {
        if (col == 4) {
            row++;
            col = 0;
        }
        MM[row][col] = data[i];
        col++;
    }
    MM_count = MM_count + 10;
}

void Terminate() {      //indicatr end o output
    FILE* outputFile = fopen("output.txt", "a");
    fprintf(outputFile, "\n\n");
    fclose(outputFile);
}

void MOS(char* data, char MM[][4], char IR[], int si) {
    switch (si) {
        case 1:
            Read(data, IR, MM);
            break;
        case 2:
            Write(IR, MM);
            break;
        case 3:
            Terminate();
            break;
        default:
            return;
    }
}

void executeuserprogram(char* data, int Ic, char MM[][4]) {
    int si = 3;
    int c = 0;
    char IR[4];

    while (Ic < noIn) {
        si = 3;
        for (int j = 0; j < 4; j++) {
            IR[j] = MM[Ic][j];
        }
        Ic++;
        char R[40];
        char function[3] = { IR[0], IR[1], '\0' };
        int operandAdd = find(IR[2]);

        if (strcmp(function, "LR") == 0) {      //load
            si = 0;
            int r_count = 0;
            for (int i = operandAdd; i <= (operandAdd + 9); i++) {
                for (int j = 0; j < 4; j++) {
                    R[r_count++] = MM[i][j];
                }
            }
        } else if (strcmp(function, "SR") == 0) {   //store
            int r_count = 0;
            for (int i = operandAdd; i < (operandAdd + 9); i++) {
                for (int j = 0; j < 4; j++) {
                    MM[i][j] = R[r_count++];
                }
            }
            si = 0;
        } else if (strcmp(function, "CR") == 0) {       //compare reg content & mem
            int r_count = 0;
            for (int i = operandAdd; i < (operandAdd + 9); i++) {
                for (int j = 0; j < 9; j++) {
                    if (MM[i][j] != R[r_count++]) {
                        printf("Not Equal");
                    }
                }
            }
        } else if (strcmp(function, "BT") == 0) {
            if (c == 1) {
                Ic = operandAdd;
            }
        } else if (strcmp(function, "GD") == 0) {
            si = 1;
        } else if (strcmp(function, "PD") == 0) {
            si = 2;
        } else if (strcmp(function, "H-") == 0) {
            si = 3;
        } else {
            printf("Invalid Job");
            return;
        }

        MOS(data, MM, IR, si);
    }
}

void startexecution(char* data, char MM[][4]) {
    int Ic = 0;
    executeuserprogram(data, Ic, MM);
}

void Load(char MM[][4]) {
    FILE* fin;
    char line[1024];

    fin = fopen("input.txt", "r");
    if (fin == NULL) {
        printf("Unable to open input.txt\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), fin) != NULL) {
        char* opcode = line;
        char op[5];
        strncpy(op, opcode, 4);
        op[4] = '\0';

        if (strcmp(op, "$AMJ") == 0) {
            continue;
        } else if (strcmp(op, "$DTA") == 0) {
            if (fgets(line, sizeof(line), fin) != NULL) {
                char data[1024];
                strcpy(data, line);
                startexecution(data, MM);
            } else {
                printf("\n**Data Missing");
                break;
            }
        } else if (strcmp(op, "$END") == 0) {
            MM_count = 0;
            continue;
        } else { // program card
            char inst[1024];
            strcpy(inst, line);
            int row = MM_count;
            int col = 0;
            for (int i = 0; i < strlen(inst); i++) {
                if (col == 4) {
                    row++;
                    col = 0;
                }
                MM[row][col] = inst[i];
                col++;
            }
            noIn = row + 1;
        }
    }

    fclose(fin);
}

int main() {
    char MM[100][4];
    init(MM);
    Load(MM);

    printf("\n Main Memory Simulation\n\n");

    for (int i = 0; i < 30; i++) {
        printf("%2d     ", i);
        for (int j = 0; j < 4; j++) {
            printf("%5c ", MM[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}
