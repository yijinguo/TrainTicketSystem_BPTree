//
// Created by 罗皓天 on 2021/6/3.
//

#ifndef CODE_CACHE_H
#define CODE_CACHE_H
#include "string.h"
#include "Bpt_and_database.h"
#include "data.hpp"
//cache : 由一个哈希表和一个双向链表组成 哈希表将B+树node的地址哈希并且存进双向链表的指针 双向链表存进B+树的node数据
template <class Data>//通过站名返回地址
class HashTable{
private:
    class List{
    private:
        struct Node{
            Node* next;
            int key;
            Data data;
            Node() = delete;
            Node(Node *nxt ,const int &key1 ,const Data &data1):next(nxt),key(key1),data(data1){};
        };
        Node* head;
        int Listsize;
    public:
        List():Listsize(0),head(nullptr){};
        ~List(){
            while (head != nullptr){
                Node* tmp = head;
                head = head->next;
                delete tmp;
            }
        }
        void insert(const int &key1 , const Data &data1){
            head = new Node(head , key1 , data1);
            ++Listsize;
        }
        Node* find(const int &key1){
            if (Listsize == 0)return nullptr;
            Node *q = head;
            while (q != nullptr){
                if (q->key == key1)return q;
                q = q->next;
            }
            return nullptr;
        }
        void erase(const int &key1){
            if (Listsize == 0)return;
            if (head->key == key1){
                Node* tmp = head;
                head = head->next;
                delete tmp;
                --Listsize;
                return;
            }
            Node* p = nullptr;
            Node* q = head;
            while (q != nullptr){
                if (q->key == key1)break;
                p = q;
                q = q->next;
            }
            p->next = q->next;
            delete q;
            --Listsize;
        }
    };
    int capacity;
    List* dataset;
    int getIndex(const int &key1){
        //return key1.hashit() % capacity;
        return key1 % capacity;
    }

public:
    HashTable() = delete;
    explicit HashTable(const int capacity1){
        capacity = capacity1;
        dataset = new List[capacity1];
    }
    ~HashTable(){
        delete []dataset;
    }
    bool exist(const int &key1){
        int index = getIndex(key1);
        return (dataset[index].find(key1) != nullptr);
    }
    void erase(const int &key1){
        int index = getIndex(key1);
        dataset[index].erase(key1);
    }
    void insert(const int &key1 , const Data &data1){
        int index = getIndex(key1);
        dataset[index].insert(key1 , data1);
    }
    Data& find(const int &key1){
        int index = getIndex(key1);
        return dataset[index].find(key1)->data;
    }
    Data& operator[](const int &key1){
        return this->find(key1);
    }
    void clear(){
        delete [] dataset;
        dataset = new List [capacity];
    }
};
template <class leaf>
class doublelist{
private:
    struct node{
        leaf data;
        int position;
        node* pre;
        node* nxt;
        node(){
            pre = nullptr;
            nxt = nullptr;
            position = -1;
        }
        node(leaf data1 , int pos , node* pre1 , node* nxt1):data(data1),pre(pre1),nxt(nxt1),position(pos){};
    };
    node* head;
    node* tail;
    doublelist(){
        head->nxt = tail;
        tail->pre = head;
    }
    ~doublelist(){
        while (head->nxt != tail){
            node* tmp = head->nxt;
            head->nxt = tmp->nxt;
            tail->pre = tmp->pre;
            delete tmp;
        }
        head = nullptr;
        tail = nullptr;
    }
    void insert(const leaf &le , const int &pos){
        node* tmp = new node(le , pos , head , head->nxt);
        head->nxt->nxt->pre = tmp;
        head->nxt = tmp;
    }
    void pop(){
        node* tmp = tail->pre;
        tmp->pre->nxt = tail;
        tail->pre = tmp->pre;
        delete tmp;
    }
    void move_to_head(node* re){
        re->pre->nxt = re->nxt;
        re->nxt->pre = re->pre;
        head->nxt->pre = re;
        re->nxt = head->nxt;
        re->pre = head;
        head->nxt = re;
    }
};
template<int x = 100>
class cache{
//    friend Database;
//    friend doublelist<Database::node>
//    friend HashTable<doublelist<Database::node>::node>
private:
    const int size = x;
    doublelist<Database::node> twolist;
    HashTable<doublelist<Database::node>::node*> hashmap;
    int cnt = 0;
public:
    cache():hashmap(x),twolist(x){
        cnt = 0;
    }
    void insert(const int &pos , const Database::node &leav){//insert是加到最前面的
        if (hashmap.exist(pos))return;
        twolist.insert(leav , pos);
        doublelist<Database::node>::node* tmp = twolist.head->nxt;
        hashmap.insert(pos , tmp);
        cnt ++;
        if (cnt > size){
            int posi = twolist.tail->pre->position;
            twolist.pop();
            hashmap.erase(posi);
            cnt--;
        }
    }
    Database::node que
    ry(const int &pos){
        if (!hashmap.exist(pos))return Database::node();
        else {
            twolist.move_to_head(hashmap.find(pos));
            Database::node tmp = hashmap.find(pos)->data;
            return tmp;
        }
    }
};
#endif //CODE_CACHE_H
