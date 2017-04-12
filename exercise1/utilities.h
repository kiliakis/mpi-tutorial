#include <vector>
#include <iostream>
#include <random>

typedef std::vector<std::vector<int> > map_t;

void print_map(const map_t &map)
{
    std::cout << "\n";
    for(int i = 0; i < map.size()+2; i++)
        std::cout << "# ";
    std::cout << '\n';

    for(const auto &row: map){
        std::cout << "# ";
        for(const auto &c: row){
            std::cout << c << " ";
        }
        std::cout << "#\n";
    }
    
    for(int i = 0; i < map.size()+2; i++)
        std::cout << "# ";
    std::cout << "\n";

}


map_t generate_map(int N, float P)
{
    map_t map(N, std::vector<int>(N));
    std::default_random_engine gen;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    for(auto &row: map)
        for(auto &c: row)
            c = dist(gen) > P;
   return map; 
}