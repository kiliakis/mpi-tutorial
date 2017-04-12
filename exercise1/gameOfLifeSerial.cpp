#include <vector>
#include <iostream>
#include "utilities.h"

using namespace std;


int main(int argc, char * argv[]){
    int N = 10;
    int turns = 100;
    float P = 0.5;
    
    if(argc > 1){
        N = stoi(argv[1]); 
    }
    
    if(argc > 2){
        turns = stoi(argv[2]);    
    }
    
    if(argc > 2){
        P = stof(argv[3]);    
    }
    

    cout << "Dimension: " << N << "\n";
    cout << "Turns: " << turns << "\n";
    cout << "Possibility: " << P << "\n";
    

    auto map = generate_map(N, P);
    print_map(map);

    return 0;
}
