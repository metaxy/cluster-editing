#include "common.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "anyoption.h"
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
            cout << "Eingabefehler" << endl;
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
            cout << "Eingabefehler" << endl;
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

map<string, string> Common::parseConfig(int argc, char* argv[])
{
    map<string,string> config;
    AnyOption *opt = new AnyOption();
    opt->noPOSIX();
    opt->addUsage( "" );
    opt->addUsage( "Usage: " );
    opt->addUsage( "" );
    opt->addUsage( "" );
    opt->setFlag( "help", 'h' );
    opt->setOption( "seed" );
    opt->setOption( "reduceAll" );
    opt->setOption( "reduceZero" );
    opt->setOption( "reduce_factor_clique" );
    opt->setOption( "reduce_summand_clique" );
    opt->setOption( "reduce_exponent_clique" );
    opt->setOption( "reduce_factor_cutting" );
    opt->setOption( "reduce_summand_cutting" );
    opt->setOption( "reduce_exponent_cutting" );
    opt->setOption( "reduce_factor_size" );
    opt->setOption( "reduce_summand_size" );

    /* go through the command line and get the options  */
    opt->processCommandArgs( argc, argv );
    if( opt->getValue( "seed" ) != NULL) {
        config["seed"] = opt->getValue( "seed" );
    }

    if( opt->getValue( "reduceAll" ) != NULL) {
        config["reduceAll"] = opt->getValue( "reduceAll" ) ;
    }

    if( opt->getValue( "reduceZero" ) != NULL) {
        config["reduceZero"] = opt->getValue( "reduceZero" );
    }
    if( opt->getValue( "reduce2K" ) != NULL) {
        config["reduce2K"] = opt->getValue( "reduce2K" );
    }
    if( opt->getValue( "reduce2K" ) != NULL) {
        config["reduce2K"] = opt->getValue( "reduce2K" );
    }

    // StateApprox2K
    if( opt->getValue( "reduce_factor_clique" ) != NULL) {
        config["reduce_factor_clique"] = opt->getValue( "reduce_factor_clique" );
    }
    if( opt->getValue( "reduce_summand_clique" ) != NULL) {
        config["reduce_summand_clique"] = opt->getValue( "reduce_summand_clique" );
    }
    if( opt->getValue( "reduce_exponent_clique" ) != NULL) {
        config["reduce_exponent_clique"] = opt->getValue( "reduce_exponent_clique" );
    }
    if( opt->getValue( "reduce_factor_cutting" ) != NULL) {
        config["reduce_factor_cutting"] = opt->getValue( "reduce_factor_cutting" );
    }
    if( opt->getValue( "reduce_summand_cutting" ) != NULL) {
        config["reduce_summand_cutting"] = opt->getValue( "reduce_summand_cutting" );
    }
    if( opt->getValue( "reduce_exponent_cutting" ) != NULL) {
        config["reduce_exponent_cutting"] = opt->getValue( "reduce_exponent_cutting");
    }
    if( opt->getValue( "reduce_factor_size" ) != NULL) {
        config["reduce_factor_size"] = opt->getValue( "reduce_factor_size" );
    }
    if( opt->getValue( "reduce_summand_size" ) != NULL) {
        config["reduce_summand_size"] = opt->getValue( "reduce_summand_size" );
    }

    /* 8. DONE */
    delete opt;
    return config;
}

