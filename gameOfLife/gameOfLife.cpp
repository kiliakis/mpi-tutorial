#include <vector>
#include <iostream>
#include <algorithm>
#include "utilities.h"
#include <mpi.h>

using namespace std;

int next_state(const map_t &map, int i, int j, int N);
int count_neighb(const map_t &map, int i, int j);
void fill_bounds(const map_t &map, int N);


int main(int argc, char * argv[])
{
    int N = 10;
    int Turns = 100;
    float P = 0.5;

    int rank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(argc > 1){
        N = stoi(argv[1]);
    }

    if(argc > 2){
        Turns = stoi(argv[2]);
    }

    if(argc > 2){
        P = stof(argv[3]);
    }
    printf("[%d/%d] Hello World!\n", rank, nprocs);

    // Create the communicator
    int ndim = 2;
    int periods[2] = {1, 1};
    int dims[2] = {2, 2};
    int coords[2];
    MPI_Comm COMM_2D;

    MPI_Cart_create(MPI_COMM_WORLD, ndim, dims, 
            periods, 0, &COMM_2D);
    MPI_Comm_rank(COMM_2D, &rank);
    MPI_Cart_coords(COMM_2D, rank, 2, coords);
    printf("[%d/%d] My coords are (%d,%d) \n", rank,
            nprocs, coords[0], coords[1]);
    int west, east, north, south;
    MPI_Cart_shift(COMM_2D, 0, 1, &south, &north);
    MPI_Cart_shift(COMM_2D, 1, 1, &west, &east);

    if(rank == 0){
        printf("[%d/%d] West is (%d) \n", rank, nprocs, west);
        printf("[%d/%d] East is (%d) \n", rank, nprocs, east);
        printf("[%d/%d] North is (%d) \n", rank, nprocs, north);
        printf("[%d/%d] South is (%d) \n", rank, nprocs, south);
    }
    

    // map_t map_now = (map_t) allocate2D(N+2);
    // map_t map_next = (map_t) allocate2D(N+2);
    // cout << "Dimension: " << N << "\n";
    // cout << "Turns: " << Turns << "\n";
    // cout << "Possibility: " << P << "\n";
    // 
    // map_t map_now = (map_t) allocate2D(N+2);
    // map_t map_next = (map_t) allocate2D(N+2);
    // 
    // populate_map(map_now, N, P);
    // print_map(map_now, N);
    // fill_bounds(map_now, N);
    // copy2D(map_now, map_next, N+2);
    //  
    // int changes = 1; 
    // for (int turn = 0; turn < Turns && changes > 0; turn++){
    //     for(int i = 0; i < N; i++){
    //         for(int j = 0; j < N; j++){
    //             map_next[i][j] = next_state(map_now, i, j, N);       
    //         }
    //     }
    //     fill_bounds(map_next, N);
    //     changes = diff(map_now, map_next, N+2);
    //     swap(map_now, map_next);
    // }
    //  
    // print_map(map_now, N);
    // cout << "Alive cells: " << cum_sum(map_now, N+2) << '\n';
    // 
    // deallocate2D(map_now, N+2);
    // deallocate2D(map_next, N+2);


    MPI_Finalize();
    return 0;
}


int next_state(const map_t & map, int i, int j, int N)
{
    // if not boundary
    if(i > 0 && i < N+1 && j > 0 && j < N+1){
        int neighb = count_neighb(map, i, j);
        if(neighb == 3) return 1;
        else if(neighb == 2) return map[i][j];
        else return 0;
    } else { // boundary
        return map[i][j];
    }

}


int count_neighb(const map_t &map, int i, int j)
{
    return map[i-1][j] + map[i][j-1] + map[i+1][j] + map[i][j+1];
}


void fill_bounds(const map_t &map, int N){
    map[0][0] = map[0][N+1] = 0;
    map[N+1][0] = map[N+1][N+1] = 0;

    for(int i = 0; i < N; i++){
        map[0][i+1] = map[N][i+1];
        map[N+1][i+1] = map[1][i+1];
        map[i+1][0] = map[i+1][N];
        map[i+1][N+1] = map[i+1][1];
    }

}

