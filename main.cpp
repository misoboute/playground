#include <iostream>
#include "SMDebug.h"

#define M2(X, ...) X __VA_ARGS__
#define M1(X, ...) X __VA_ARGS__

#define SM_VM_NARG(...) SM_VM_NARG_(__VA_ARGS__, SM_VM_RSEQ_N)
#define SM_VM_NARG_(...) SM_VM_ARG_N(__VA_ARGS__)
#define SM_VM_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, _11, _12, _13, \
    _14, _15, _16, _17, _18, _19, N, ...) N
#define SM_VM_RSEQ_N 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, \
    4, 3, 2, 1, 0

#define SMDW2(...) std::cout SMDW_ARG_EXPAND10(__VA_ARGS__)
#define SMDW1(X1) SMDBG << SMWCH(X)
#define SMDW2(X1, X2) SMDBG << SMWCH(X1) << SMWCH(X2)
#define SMDW3(X1, X2, X3) SMDBG << SMWCH(X1) << SMWCH(X2) << SMWCH(X3)
#define SMDW(...) SMDW##SM_VM_NARG(__VA_ARGS__)(__VA_ARGS__)

void PrintEnv(const char* envVar)
{
    auto envVal = std::getenv(envVar);
    if (envVal == nullptr)
        std::cout << 
        "Envirnoment variable " << envVar << " not found!" << std::endl;
    else
        std::cout << envVar << " = " << envVal << std::endl;
}

int main(int argc, char** argv)
{

    std::cout << SM_VM_NARG(1); // << 1
    std::cout << SM_VM_NARG(1, 2); // << 1 << 2
    std::cout << SM_VM_NARG(1, 2, 3); // << 1 << 2 << 3
}
