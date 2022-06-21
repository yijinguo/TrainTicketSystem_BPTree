#include <fstream>

// #include "B+Tree.hpp"
// #include "Bpt_and_database.h"
#include "BPlusTree.h"
#include "data.hpp"

extern bool isExit ;

extern Tools::BPlusTree<int, int> users ;
extern Tools::BPlusTree<int, int> curUsers ;
extern Tools::BPlusTree<int, int> trains ;
extern Tools::BPlusTree<int, int> trainStations ;
extern Tools::BPlusTree<int, int> orders ;
extern Tools::BPlusTree<int, int> pendingOrders ;

extern std::fstream userio ;
extern std::fstream trainio ;
extern std::fstream orderio ;
extern std::fstream seatio ;
extern std::fstream logio ;