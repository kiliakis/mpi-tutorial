#include <vector>
#include <iostream>
#include <algorithm>
#include "utilities.h"
#include <mpi.h>

using namespace std;

int next_state(const map_t &map, int i, int j, int N);
int count_neighb(const map_t &map, int i, int j);
void fill_bounds(const map_t &map, int N);

void exchange_bounds(MPI_Comm MPI_COMM, map_t map,
        int N, MPI_Datatype &ROW, MPI_Datatype &COL);

int main(int argc, char * argv[])
{
    int N = 10;
    int Turns = 100;
    float P = 0.5;

    int rank, nprocs;

    MPI_Init(&argc, &argv);


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
    MPI_Comm_size(COMM_2D, &nprocs);


    printf("[%d/%d] Hello World!\n", rank, nprocs);
    printf("[%d/%d] My coordinates are (%d,%d) \n", rank,
            nprocs, coords[0], coords[1]);

    int west, east, north, south;
    MPI_Cart_shift(COMM_2D, 0, 1, &south, &north);
    MPI_Cart_shift(COMM_2D, 1, 1, &west, &east);

    // printf("[%d/%d] West is (%d) \n", rank, nprocs, west);
    // printf("[%d/%d] East is (%d) \n", rank, nprocs, east);
    // printf("[%d/%d] North is (%d) \n", rank, nprocs, north);
    // printf("[%d/%d] South is (%d) \n", rank, nprocs, south);

    if(argc > 1){
        N = atoi(argv[1]);
    }

    if(argc > 2){
        Turns = atoi(argv[2]);
    }

    if(argc > 3){
        P = atof(argv[3]);
    }

    // printf("[%d/%d] Configuration N: %d, Turns: %d, P: %.1f\n",
    //         rank, nprocs, N, Turns, P); 

    // TODO: Adjust accordingly to support any combination
    int localN = N / dims[0];

    map_t old_map = (map_t) allocate2D(localN+2);
    map_t new_map = (map_t) allocate2D(localN+2);
    // Create column and row data types
    MPI_Datatype MPI_ROW, MPI_COLUMN;
    MPI_Type_vector(localN, 1, localN+2, MPI_INT, &MPI_COLUMN);
    MPI_Type_commit(&MPI_COLUMN);

    MPI_Type_vector(1, localN, 1, MPI_INT, &MPI_ROW);
    MPI_Type_commit(&MPI_ROW);


    // Populate the map
    populate_map(old_map, localN, P, rank);
    old_map[0][0] = old_map[localN+1][localN+1] = -1;
    old_map[0][localN+1] = old_map[localN+1][0] = -1;
    
    for(int i = 0; i < nprocs; i++){
        MPI_Barrier(COMM_2D);
        if(i == rank){
            printf("[%d/%d] Ready to print!\n", rank, nprocs);
            print_map(old_map, localN);
        }
    }

    // fill the boundary conditions
    exchange_bounds(COMM_2D, old_map, localN, MPI_ROW, MPI_COLUMN);
    // copy data to both arrays
    copy2D(old_map, new_map, localN+2);
    for(int turn = 0; turn < Turns; turn++){
        for(int i = 0; i < localN + 2; i++){
            for(int j = 0; j < localN + 2; j++){
                new_map[i][j] = next_state(old_map, i, j, localN);
            }
        }
        swap(new_map, old_map);
        exchange_bounds(COMM_2D, old_map, localN, MPI_ROW, MPI_COLUMN);
    }

    int aliveCells = cum_sum2(old_map, localN);
    printf("[%d/%d] Number of alive cells %d\n", rank, nprocs, aliveCells);

    int totalAliveCells = 0;
    MPI_Reduce(&aliveCells, &totalAliveCells, 1, 
            MPI_INT, MPI_SUM, 0, COMM_2D);
    
    if(rank == 0)
        printf("[%d/%d] Total alive cells %d\n", rank,
            nprocs, totalAliveCells);

    deallocate2D(old_map, localN+2);
    deallocate2D(new_map, localN+2);
    MPI_Finalize();
    return 0;
}

void exchange_bounds(MPI_Comm MPI_COMM, map_t map,
        int N, MPI_Datatype &MPI_ROW, MPI_Datatype &MPI_COLUMN)
{
    int west, east, north, south;
    MPI_Cart_shift(MPI_COMM, 0, 1, &south, &north);
    MPI_Cart_shift(MPI_COMM, 1, 1, &west, &east);

    // Exchange borders
    MPI_Request request_array[2*4];
    // First the sends
    // North
    MPI_Isend(&map[1][1], 1, MPI_ROW, north, 0,
            MPI_COMM_WORLD, &request_array[0]);
    // South
    MPI_Isend(&map[N][1], 1, MPI_ROW, south, 1,
            MPI_COMM_WORLD, &request_array[1]);
    // West 
    MPI_Isend(&map[1][1], 1, MPI_COLUMN, west, 2,
            MPI_COMM_WORLD, &request_array[2]);
    // East 
    MPI_Isend(&map[1][N], 1, MPI_COLUMN, east, 3,
            MPI_COMM_WORLD, &request_array[3]);

    // Then the receives 
    // North 
    MPI_Irecv(&map[0][1], 1, MPI_ROW, north, 1,
            MPI_COMM_WORLD, &request_array[4 + 0]);
    // South
    MPI_Irecv(&map[N+1][1], 1, MPI_ROW, south, 0,
            MPI_COMM_WORLD, &request_array[4 + 1]);
    // West 
    MPI_Irecv(&map[1][0], 1, MPI_COLUMN, west, 3,
            MPI_COMM_WORLD, &request_array[4 + 2]);
    // East 
    MPI_Irecv(&map[1][N+1], 1, MPI_COLUMN, east, 2,
            MPI_COMM_WORLD, &request_array[4 + 3]);

    MPI_Status status_array[2*4];
    MPI_Waitall(8, request_array, status_array);



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

