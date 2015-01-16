cmake_minimum_required(VERSION 2.6)
project(cluster4)
#set(GUROBI_PATH ${PROJECT_SOURCE_DIR}/../gurobi)
set(GUROBI_PATH ~/bin/gurobi600/linux64/)
#/afs/akt.tu-berlin.de/user/pwalger/gruppe1/aufgabe4/gurobi600/)
#set(GUROBI_PATH /home/paul/bin/gurobi600/linux64/)
set( cluster_SRCS
    graph.cpp
    state.cpp
    gurobilp.cpp
    common.cpp
    randomize.cpp

    state_blp.cpp
    state_blp_rel.cpp
    state_lp_round.cpp
)
ADD_DEFINITIONS(
    -std=c++11
    -Ofast
)
include_directories(
   ${GUROBI_PATH}/include
)
add_executable(ce-blp ${cluster_SRCS} main_blp.cpp)
add_executable(ce-blp-rel ${cluster_SRCS} main_blp_rel.cpp)
add_executable(ce-lp-round ${cluster_SRCS} main_lp_round.cpp)
find_library(G1 libgurobi_g++4.2.a ${GUROBI_PATH}/lib)
find_library(G2 libgurobi.so.6.0.0 ${PROJECT_BINARY_DIR} ${GUROBI_PATH}/lib)

target_link_libraries(
    ce-blp
    ${G2}
    ${G1}
)
target_link_libraries(
    ce-blp-rel
    ${G2}
    ${G1}
)
target_link_libraries(
    ce-lp-round
    ${G2}
    ${G1}
)
