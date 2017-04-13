#include <vector>
#include <iostream>
#include <random>

typedef int** map_t;


map_t allocate2D(int N){
    map_t map = new int*[N];
    for(int i = 0; i < N; i++)
        map[i] = new int[N];
    return map;
}

void deallocate2D(map_t &map, int N){
    for(int i = 0; i < N; i++)
        delete[] map[i];
    delete[] map;
}


void copy2D(map_t source, map_t dest, int N){
    for(int i = 0; i < N; i++)
        for(int j = 0; j< N; j++)
            dest[i][j] = source[i][j];

}


void print_map(const map_t map, int N)
{
    std::cout << "\n";
    for(int i = 0; i < N+2; i++)
        std::cout << "# ";
    std::cout << '\n';

    for(int i = 1; i <= N; i++){
        std::cout << "# ";
        for(int j = 1; j<= N; j++){
            std::cout << map[i][j] << " ";
        }
        std::cout << "#\n";
    }
    
    for(int i = 0; i < N+2; i++)
        std::cout << "# ";
    std::cout << "\n";

}


void populate_map(map_t &map, int N, float P)
{
    std::default_random_engine gen;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    for(int i = 1; i <= N; i++)
        for(int j=1; j<=N; j++)
            map[i][j] = dist(gen) > P; 
}

int cum_sum(const map_t map, int N)
{
    int sum = 0;
    for(int i = 0; i < N; i++)
        sum += std::accumulate(&map[i][0], &map[i][N], 0);
    return sum;
}

int diff(map_t map_now, map_t map_next, int N)
{
    int res = 0;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            res += (map_now[i][j] - map_next[i][j]) != 0;
    return res;
    
}
