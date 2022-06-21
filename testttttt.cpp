//
// Created by 罗皓天 on 2021/6/4.
//
#include <iostream>
#include "vector.h"
int main(){
    sjtu::vector<Num> res;
    res.push_back(Num(1 , 2));
    res.push_back(Num(2 , 3));
    for (int i = 99; i >=3 ; --i) {
        res.push_back(Num(i , i + 1));
    }
    res.sort(compare);
    for (auto i : res){
        std::cout << i.a << ' ' << i.b <<std::endl;
    }
}