#include <vector>
#include <iostream>
#include <algorithm>
#include "utilities.h"

using namespace std;


int main(int argc, char *argv[])
{
    char *inFileName = NULL;

    if(argc > 1){
        inFileName = argv[1];
    }
    
    vector<int> inDataVector;
    
    FILE* inFile = fopen(inFileName, "r");
    int target, number;

    fscanf(inFile, "%d", &target);

    while (fscanf(inFile, "%d", &number) == 1){
        inDataVector.push_back(number);
        // printf("%d\n", number);
    }

    int *inData = inDataVector.data();
    int inSize = inDataVector.size();
    printf("Target element %d\n", target);
    printf("Input Size %d\n", inSize);
     
    const char *outFileName = "output.data";
    FILE *outFile = fopen(outFileName, "w");
    
    int occurrences = 0;
    for(int i = 0; i < inSize; i++){
        if(inData[i] == target){
            fprintf(outFile, "%d\n", i);
            occurrences++;
        }
    }
    
    printf("The target was found %d times\n", occurrences);
    return 0;
}

