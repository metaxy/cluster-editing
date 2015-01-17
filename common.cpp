#include "common.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;
Common::Common()
{
}


Graph Common::graphFromStdin()
{
    Graph g;
    string line, node1, node2;
    while (getline(cin, line))
    {
        int i;
        if ((i = line.find_first_of("#")) != string::npos) {
            line = line.substr(0, i);
        }
        stringstream ss(line);
        ss >> node1 >> node2;
        if (node1 == "") continue;
        if (node2 == "")
        {
            cout << "Eingabefehler\n";
            return g;
        }
        g.insertEdge(node1, node2);
    }
    return g;
}
Graph Common::graphFromFile(string fileName)
{
    Graph g;
    string line, node1, node2;
    std::ifstream infile(fileName);
    while (std::getline(infile, line))
    {
        int i;
        if ((i = line.find_first_of("#")) != string::npos) {
            line = line.substr(0, i);
        }
        stringstream ss(line);
        ss >> node1 >> node2;
        if (node1 == "") continue;
        if (node2 == "")
        {
            cout << "Eingabefehler\n";
            return g;
        }
        g.insertEdge(node1, node2);
    }
    return g;
}

void Common::printMatrix(int **matrix, unsigned int size)
{
#ifdef _DEBUG
    for(int i = 0; i < size; i++) {
        for(int y = 0; y < size; y++) {
            clog << std::setw(2) << matrix[i][y] << " ";
        }
        clog << endl;
    }
#endif
}

void Common::printVector(int *vector, unsigned int size)
{
#ifdef _DEBUG
    for(int i = 0; i < size; i++) {
        clog << i <<"="<< vector[i] << " ";
    }
    clog << endl;
#endif
}


