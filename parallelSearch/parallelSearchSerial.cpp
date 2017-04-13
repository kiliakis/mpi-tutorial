#include <vector>
#include <iostream>
#include <algorithm>
#include "utilities.h"

using namespace std;


int main(int argc, char * argv[])
{
    char *inFileName = NULL;

    if(argc > 1){
        inFileName = argv[1];
    }
    
    vector<int> inDataVector;
    
    FILE* inFile = fopen(inFileName, "r");
    int number;

    // while(feof(inFile) == 1){
    while (fscanf(inFile, "%d", &number) == 1){  
        inDataVector.push_back(number);
        printf("%d\n", number);
    }
    // }

    int *inData = inDataVector.data();
    int inSize = inDataVector.size();
    printf("Input Size %d\n", inSize);    
    return 0;
}

