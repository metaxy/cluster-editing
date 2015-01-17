#ifndef COMMON_H
#define COMMON_H

#define _DEBUG
#include "graph.h"
#include <cstring>
#include <tuple>
#include "graph.h"
class Common
{
public:
    Common();
    Graph static graphFromStdin();
    Graph static graphFromFile(string fileName);

    static void printMatrix(int **matrix, unsigned int size);
    static void printVector(int *vector, unsigned int size);

};

template <typename T> int sgn(T val) {
    if(val < T(0)) return -1;
    return 1;
}

#ifdef _DEBUG
#define de(str) do { std::clog << str << std::endl; } while( false )
#else
#define de(str) do { } while ( false )
#endif

#endif // COMMON_H
