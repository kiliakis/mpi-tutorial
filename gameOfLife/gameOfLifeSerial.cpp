#include <vector>
#include <iostream>
#include <algorithm>
#include "utilities.h"

using namespace std;

int next_state(const map_t &map, int i, int j, int N);
int count_neighb(const map_t &map, int i, int j);
void fill_bounds(const map_t &map, int N);


int main(int argc, char * argv[])
{
    int N = 10;
    int Turns = 100;
    float P = 0.5;
    
    if(argc > 1){
        N = stoi(argv[1]);
    }
    
    if(argc > 2){
        Turns = stoi(argv[2]);
    }
    
    if(argc > 2){
        P = stof(argv[3]);
    }

    cout << "Dimension: " << N << "\n";
    cout << "Turns: " << Turns << "\n";
    cout << "Possibility: " << P << "\n";
    
    map_t map_now = (map_t) allocate2D(N+2);
    map_t map_next = (map_t) allocate2D(N+2);
    
    populate_map(map_now, N, P);
    print_map(map_now, N);
    fill_bounds(map_now, N);
    copy2D(map_now, map_next, N+2);
     
    int changes = 1; 
    for (int turn = 0; turn < Turns && changes > 0; turn++){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                map_next[i][j] = next_state(map_now, i, j, N);       
            }
        }
        fill_bounds(map_next, N);
        changes = diff(map_now, map_next, N+2);
        swap(map_now, map_next);
    }
     
    print_map(map_now, N);
    cout << "Alive cells: " << cum_sum(map_now, N+2) << '\n';
    
    deallocate2D(map_now, N+2);
    deallocate2D(map_next, N+2);
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

