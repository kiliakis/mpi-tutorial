#include <vector>
#include <iostream>
#include <algorithm>
#include "utilities.h"

using namespace std;

int next_state(const map_t & map_now, int i, int j, int N);

int main(int argc, char * argv[]){
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
    

    auto map_now = generate_map(N, P);
    print_map(map_now);
    
    auto map_next = map_now;

    for(int turn = 0; turn < Turns; turn++){
        for(int i = 0; i < N; i++){
            for(int j = 0; j< N; j++){
                map_next[i][j] = next_state(map_now, i, j, N);       
            }
        }
        swap(map_now, map_next);
    }
    
    print_map(map_now);

    return 0;
}

int next_state(const map_t & map_now, int i, int j, int N){
    return !map_now[i][j];
}
