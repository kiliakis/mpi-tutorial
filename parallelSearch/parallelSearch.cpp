#include <vector>
#include <iostream>
#include <algorithm>
#include "utilities.h"
#include <mpi.h>

using namespace std;


int main(int argc, char *argv[])
{
    int rank, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    printf("Processor %d of %d: Hello World\n", rank, nprocs);
    
    int target, block;
    int *rootData = NULL;
    int *rootBlocks = NULL;
    if(rank == 0){
        char *inFileName = NULL;
        if(argc > 1){
            inFileName = argv[1];
        }
        
        vector<int> inDataVector;
        
        FILE* inFile = fopen(inFileName, "r");
        int number;

        fscanf(inFile, "%d", &target);

        while (fscanf(inFile, "%d", &number) == 1){  
            inDataVector.push_back(number);
            // printf("%d\n", number);
        }
        
        int inSize = inDataVector.size();
        rootData = inDataVector.data();
        fclose(inFile);
        

        // Broadcast target and size
        MPI_Bcast(&target, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        rootBlocks = new int[nprocs];
        block = (inSize + nprocs - 1) / nprocs;
        for(int i = 0; i < nprocs; i++)
            if((i+1)*block <= inSize)
                rootBlocks[i] = block;
            else 
                rootBlocks[i] = inSize - (i+1) * block; 
        
    }else{
        MPI_Bcast(&target, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    

    // Send block for each proc
    MPI_Scatter(rootBlocks, 1, MPI_INT, &block, 1, 
                MPI_INT, 0, MPI_COMM_WORLD);

    printf("[%d/%d] My block has  %d elements\n", rank, nprocs, block);
    
    // Send data for each proc
    int *inData = new int[block];
    MPI_Scatter(rootData, block, MPI_INT, inData, block,
                MPI_INT, 0, MPI_COMM_WORLD);
    
    printf("[%d/%d] I received %d elements\n", rank, nprocs, block);
    
    // Find occurrences
    vector<int> indices;
    int count = 0;
    for(int i = 0; i < block; i++){
        if(inData[i] == target){
            indices.push_back(i);
            count++;
        }
    }
      
    printf("[%d/%d] I found %d elements that match the target\n",
          rank, nprocs, count);
    

    // Send to root num of occurrences of each proc
    int *rootOccurrences = NULL;
    if(rank == 0)
        rootOccurrences = new int[nprocs];
    int occurrences = indices.size();
    MPI_Gather(&occurrences, 1, MPI_INT, rootOccurrences, 1, 
               MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank == 0){
        occurrences = 0;
        for(int i = 0; i < nprocs; i++){
            printf("[%d/%d] Processor %d found %d occurrences\n", rank, 
                    nprocs, i, rootOccurrences[i]);
            occurrences += rootOccurrences[i];
        }
        


    }
    
    // Send the indices with gatherv
    int *rootDispls = NULL;
    int *rootIndices = NULL;
    if(rank == 0){
        rootIndices = new int[occurrences];
        rootDispls = new int[nprocs];
        rootDispls[0] = 0;
        for(int i = 1; i < nprocs; i++){
            rootDispls[i] = rootOccurrences[i-1] + rootDispls[i-1];
        }
        // for(int i = 0; i < nprocs; i++)
        //     printf("Displacement %d = %d\n", i, rootDispls[i]);
    }
    MPI_Gatherv(indices.data(), indices.size(), MPI_INT, rootIndices, 
               rootOccurrences, rootDispls, MPI_INT, 0, MPI_COMM_WORLD);

    
    // Write to file
    if(rank == 0){
        const char *outFileName = "output.data";
        FILE *outFile = fopen(outFileName, "w");
        int offset = 0;
        for(int i = 0; i < nprocs; i++){
            for(int j = 0; j < rootOccurrences[i]; j++)
                rootIndices[j + rootDispls[i]] += offset;
            offset += rootBlocks[i];
        }
        for(int i = 0; i < occurrences; i++){
            fprintf(outFile, "%d\n", rootIndices[i]);
        }
        fclose(outFile);
    }
    
    if(inData != NULL)
        delete[] inData;
    if(rootIndices != NULL)
        delete[] rootIndices;
    if(rootBlocks != NULL)
        delete[] rootBlocks;
    if(rootDispls != NULL)
        delete[] rootDispls;
    if(rootOccurrences != NULL)
        delete[] rootOccurrences;
    MPI_Finalize();
    return 0;
}

