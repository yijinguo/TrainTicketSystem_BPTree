//
// Created by 罗皓天 on 2021/6/2.
//

#ifndef CODE_HASHMAP_H
#define CODE_HASHMAP_H
#include <functional>
#include <iostream>
#include "string.h"
// using namespace std ;
//template<class Key , class Data >//key是站名 ， data是车站内容，即顺序
class HashMap{
private:
    const int size = 101;
    const int mod = 291143;
    struct pair{
    public:
        int first;//站名
        short second;//序号
        pair(){
            first = 0;
            second = 0;
        }
        pair(int l , short r):first(l),second(r){};
    };
    pair order[101];
    int cur = 0;
    void sort(int l , int r){
        int left = l , right = r;
        int mid = order[(l + r) >> 1].first;
        do{
            while (order[left].first < mid) left++;
            while (order[right].first > mid) right--;
            if (left <= right){
                std::swap (order[left] , order[right]) ;
                left++;
                right--;
            }
        } while (left <= right);
        if (right > l)sort(l , right);
        if (left < r)sort(left , r);
    }
public:
    HashMap() = default;
    void insert(const String &key1 , int data1) {
        pair nod (key1.hash_val, data1);
        order[++cur] = nod;
    }
    void sort(){
        sort(1 , cur);
    }
    short find (const String &key1) const {
        int hash_val = key1.hash_val ;
        int l = 0;
        int r = cur + 1;
        while (l + 1 < r){
            int mid = (l + r) >> 1;
            if (order[mid].first >= hash_val) r = mid;
            else l = mid;
        }
        if (r == cur + 1 || order[r].first != hash_val) return -1;
        else return order[r].second;
    }
};
#endif //CODE_HASHMAP_H
