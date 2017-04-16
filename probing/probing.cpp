#include <iostream>
#include <algorithm>
#include <mpi.h>

using namespace std;

int main(int argc, char * argv[])
{
    int rank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    printf("[%d/%d] Hello World!\n", rank, nprocs);

    if(rank == 0){
        int num;
        cout << "Enter the number of students\n";
        cin >> num;
        int *id = new int[num];
        cout << "Enter the student ids\n";
        for(int i = 0; i < num; i++){
            cin >> id[i];
        }
        printf("[%d/%d] Student ids:\n", rank, nprocs);
        for(int i = 0; i < num; i++){
            cout << id[i] << "\n";
        }
        MPI_Send(id, num, MPI_INT, 1, 0, MPI_COMM_WORLD);
        delete[] id;
    }else{
        MPI_Status status;
        int num;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  
        MPI_Get_count(&status, MPI_INT, &num);
        int *id = new int[num];
        MPI_Recv(id, num, MPI_INT, status.MPI_SOURCE, status.MPI_TAG,
                MPI_COMM_WORLD, &status);
        printf("[%d/%d] Student ids:\n", rank, nprocs);
        for(int i = 0; i < num; i++){
            cout << id[i] << "\n";
        }
        delete[] id;
    }

    printf("[%d/%d] Goodbye!\n", rank, nprocs);
    // define time struct
    MPI_Finalize();
    return 0;
}

