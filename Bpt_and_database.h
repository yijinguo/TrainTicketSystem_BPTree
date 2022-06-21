//bububu
// Created by 罗皓天 on 2021/5/10.
//

#ifndef TICKETSYSTEM_BPT_AND_DATABASE_H
#define TICKETSYSTEM_BPT_AND_DATABASE_H

#include <cstring>
#include <iostream>
#include <fstream>
#include "vector.h"

// #include "vector.h"
#include "string.h"
#include "data.hpp"//size 即m为B+树阶数：最多有几个儿子
//#include "cache.h"
const int size = 100;
const int maxkey = size - 1;//单个节点最多能存多少key
class Database {
private:
    struct node{
        bool is_leaf;
        int keycnt;
        int fa;
        int lbro;
        int rbro;
        int nxt;
        int son[size + 3];
        data key[size + 1];

        node(){
            is_leaf = true;
            keycnt = 0;//0-based
            fa = -1;
            lbro = -1;
            rbro = -1;
            for (int i = 0 ; i < size + 3 ; ++i) {
                son[i] = -1;
            }

        }
        void print();
    };
    std::fstream io;
    char file_name[100];
    int root = -1;
    int nodenum = 0;//0-based
    const int node_size = sizeof (node);
    const int init_offset = 2 * sizeof (int);//跳过存root与nodenum的地址

public:
    Database(const char* file){
        strcpy(file_name , file);
        std::fstream in (file , std::ios :: in | std::ios :: binary);
        if (!in.is_open()){
            std::fstream out (file , std::ios :: out | std::ios :: binary);
            out.write(reinterpret_cast<char *>(&root) , sizeof root);
            out.write(reinterpret_cast<char *>(&nodenum), sizeof(nodenum));
            out.close();
        } else{
            in.read(reinterpret_cast<char *>(&root), sizeof(root));
            in.read(reinterpret_cast<char *>(&nodenum), sizeof(nodenum));
        }
        in.close();
        io.open(file , std::ios::in | std::ios :: out | std::ios :: binary);
    }
    ~Database(){
        io.close();
    }
    void print(int nod);
    void print() ;
    bool empty() ;
    void clear () ;
    void clear(data &tmp);
    node disk_read(int pos);
    void disk_write(int pos , node &x);
    void update_root();
    std::pair<int , int> find(int nod , const data &x);
    void find(int nod , const data &x , sjtu::vector<int> &cap);
    std::pair<int , int> find(const data &x);
    int findKey(const data &x);
    void find (const data &x , sjtu::vector<int>&cap);
    int search(int nod , const data &x);//return where can we insert x
    void update_son_fa(int son , int pa);
    void insert(int &pa , int lchild , int rchild , const data &x);
    void checkpapa(int pa);
    void insert(const data &x);
    //void erase_par(int nod);
    void erase(const data &x);
    data findmin(int nod);
    void add_tmp(int pos);
    int have_tmp();
};



#endif //TICKETSYSTEM_BPT_AND_DATABASE_H
