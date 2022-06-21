#include <iostream>
#include <fstream>
#include <cstring>

#include "main.h"
// #include "Bpt_and_database.h"
// #include "B+Tree.hpp"
#include "BPlusTree.h"
#include "CommandHandler.hpp"

Tools::BPlusTree<int, int> users ("usersBPT.dat", "usersBPTData.dat") ;
Tools::BPlusTree<int, int> curUsers ("curUsers.dat", "curUsersData.dat") ;
Tools::BPlusTree<int, int> trains ("trainsBPT.dat", "trainsBPTData.dat") ;
Tools::BPlusTree<int, int> trainStations ("trainStations.dat", "trainStationsData.dat") ;
Tools::BPlusTree<int, int> orders ("ordersBPT.dat", "ordersBPTData.dat") ;
Tools::BPlusTree<int, int> pendingOrders ("pendingOrders.dat", "pendingOrdersData.dat") ;

std::fstream userio, trainio, orderio, seatio, logio ;

void init () {
    std::fstream in ("users.dat", std::ios::in | std::ios::binary) ;
    if (!in.is_open()) {
        std::fstream out ("users.dat", std::ios::out | std::ios::binary) ;
        out.close() ;
        userio.open ("users.dat", std::ios::in | std::ios::out | std::ios::binary) ;
        userio.seekp (0, std::ios::end) ;
    }
    in.close() ;
    if (!userio.is_open()) userio.open ("users.dat", std::ios::in | std::ios::out | std::ios::binary) ;

    in.open ("trains.dat", std::ios::in | std::ios::binary) ;
    if (!in.is_open()) {
        std::fstream out ("trains.dat", std::ios::out | std::ios::binary) ;
        out.close() ;
        trainio.open ("trains.dat", std::ios::in | std::ios::out | std::ios::binary) ;
        trainio.seekp (0, std::ios::end) ;
        int tmp = -1 ;
        trainio.write (reinterpret_cast<char *>(&tmp), sizeof (tmp)) ;
    }
    in.close() ;
    if (!trainio.is_open()) trainio.open ("trains.dat", std::ios::in | std::ios::out | std::ios::binary) ;

    in.open ("orders.dat", std::ios::in | std::ios::binary) ;
    if (!in.is_open()) {
        std::fstream out ("orders.dat", std::ios::out | std::ios::binary) ;
        out.close() ;
        orderio.open ("orders.dat", std::ios::in | std::ios::out | std::ios::binary) ;
        orderio.seekp (0, std::ios::end) ;
    }
    in.close() ;
    if (!orderio.is_open()) orderio.open ("orders.dat", std::ios::in | std::ios::out | std::ios::binary) ;

    in.open ("seats.dat", std::ios::in | std::ios::binary) ;
    if (!in.is_open()) {
        std::fstream out ("seats.dat", std::ios::out | std::ios::binary) ;
        out.close() ;
        seatio.open ("seats.dat", std::ios::in | std::ios::out | std::ios::binary) ;
        seatio.seekp (0, std::ios::end) ;
    }
    in.close() ;
    if (!seatio.is_open()) seatio.open ("seats.dat", std::ios::in | std::ios::out | std::ios::binary) ;

    in.open ("log.dat", std::ios::in | std::ios::binary) ;
    if (!in.is_open()) {
        std::fstream out ("log.dat", std::ios::out | std::ios::binary) ;
        out.close() ;
        logio.open ("log.dat", std::ios::in | std::ios::out | std::ios::binary) ;
        logio.seekp (0, std::ios::end) ;
        long long tmp = -1 ;
        logio.write (reinterpret_cast<char *>(&tmp), sizeof (tmp)) ;
    }
    in.close() ;
    if (!logio.is_open()) logio.open ("log.dat", std::ios::in | std::ios::out | std::ios::binary) ;
}

bool isExit ;

int main() {
    //freopen("5.in" , "r" , stdin);
    //freopen("gyj.out", "w", stdout);
    // std::ios::sync_with_stdio(false) ;
    // std::cin.tie (0); std::cout.tie (0) ;

    init () ;
    std::string op ;
    while (getline (std::cin, op)) {
        CommandHandler cmd = CommandHandler (op) ;
        cmd.run () ;
        if (isExit) break ;
    }

    userio.close() ;
    trainio.close() ;
    orderio.close() ;
    seatio.close() ;
    logio.close() ;
    curUsers.clear() ;
    return 0 ;
}