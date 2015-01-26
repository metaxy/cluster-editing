cmake_minimum_required(VERSION 2.6)
project(cluster4)
find_path(
    GUROBI_PATH include/gurobi_c++.h
    PATHS ~/bin/gurobi600/linux64/ /afs/akt.tu-berlin.de/user/pwalger/gruppe1/gurobi /home/users/w/walger/irb-ubuntu/cluster/gruppe1/gurobi
)
message("GUROBI PATH " . ${GUROBI_PATH})
#/afs/akt.tu-berlin.de/user/pwalger/gruppe1/aufgabe4/gurobi600/)
#set(GUROBI_PATH /home/paul/bin/gurobi600/linux64/)
set( cluster_SRCS
    graph.cpp
    mgraph.cpp
    state.cpp
    common.cpp
    randomize.cpp
    anyoption.cpp
)
ADD_DEFINITIONS(
    -std=c++11
    #    -Ofast
)
include_directories(
   ${GUROBI_PATH}/include
)
add_executable(ce-blp ${cluster_SRCS} gurobilp.cpp  state_blp.cpp main_blp.cpp )
add_executable(ce-blp-rel ${cluster_SRCS} gurobilp.cpp  state_blp_rel.cpp main_blp_rel.cpp)
add_executable(ce-lp-round ${cluster_SRCS} gurobilp.cpp state_lp_round.cpp main_lp_round.cpp)
add_executable(ce-approx3 ${cluster_SRCS} state_approx3.cpp main_approx3.cpp)
add_executable(ce-approx2k ${cluster_SRCS} state_approx2k.cpp main_approx2k.cpp)
add_executable(ce-branch ${cluster_SRCS} state_branch.cpp main_branch.cpp)
find_library(G1 libgurobi_g++4.2.a ${GUROBI_PATH}/lib)
find_library(G2 libgurobi.so.6.0.0 ${GUROBI_PATH}/lib)

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
