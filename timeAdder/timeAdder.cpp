#include <iostream>
#include <algorithm>
#include <mpi.h>

using namespace std;

struct mytime_t {
    int h;
    int m;
    int s;

    mytime_t(){
        h = m = s = 0;    
    };

    mytime_t(int _h, int _m, int _s){
        h = _h;
        m = _m;
        s = _s;
        this->add(mytime_t());
    };


    void add(const mytime_t &b){
        int carry;
        s += b.s;
        carry = s / 60;
        s = s % 60;

        m += b.s + carry;
        carry = m / 60;
        m = m % 60;

        h += b.h + carry;
    };
};

mytime_t add(const mytime_t &a, const mytime_t &b){
    mytime_t ret;
    int carry;
    ret.s = a.s + b.s;
    carry = ret.s / 60;
    ret.s = ret.s % 60;

    ret.m = a.m + b.m + carry;
    carry = ret.m / 60;
    ret.m = ret.m % 60;

    ret.h = a.h + b.h + carry;

    return ret;
}

void addclock(void *in, void *inout, int *len, MPI_Datatype *type);

int main(int argc, char * argv[])
{
    int rank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    printf("[%d/%d] Hello World!\n", rank, nprocs);

    // define time struct
    const int nitems = 3;
    int blocklengths[3] = {1, 1, 1};
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype MPI_TIME;
    MPI_Aint offsets[3];
    offsets[0] = offsetof(mytime_t, h);
    offsets[1] = offsetof(mytime_t, m);
    offsets[2] = offsetof(mytime_t, s);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &MPI_TIME);
    MPI_Type_commit(&MPI_TIME);

    // Define custom operator
    MPI_Op MPI_ADDCLOCK;
    MPI_Op_create(addclock, 0, &MPI_ADDCLOCK);

    // Each process initializes its timer
    mytime_t timer(rank * 10, rank * 10, rank * 10);

    mytime_t totalTime;

    MPI_Reduce(&timer, &totalTime, 1, MPI_TIME, MPI_ADDCLOCK, 
            0, MPI_COMM_WORLD);
    
    if(rank == 0){
        printf("Total time is: %d:%d:%d\n", 
            totalTime.h, totalTime.m, totalTime.s);   
     }
    MPI_Finalize();
    return 0;
}

void addclock(void *in, void *inout, int *len, MPI_Datatype *type)
{
    mytime_t *time_in = static_cast<mytime_t *>(in);
    mytime_t *time_inout = static_cast<mytime_t *>(inout);
    for(int i = 0; i < *len; i++){
        time_inout[i].add(time_in[i]);
    }
}


