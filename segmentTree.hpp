#ifndef TicketSystem_segmentTree
#define TicketSystem_segmentTree

#define lson l, m, rt * 2
#define rson m + 1, r, rt * 2 + 1

#include <iostream>

class segmentTree {

private:
    int n ;
    int val[410], add[410] ;

public:
    segmentTree () {
        memset (val, 0, sizeof val) ;
        memset (add, 0, sizeof add) ;
    } 
    void pushup (int rt) {
        val[rt] = std::min (val[rt * 2], val[rt * 2 + 1]) ;
    }
    void pushdown (int l, int r, int rt) {
        if (add[rt] == 0) return ;
        int m = (l + r) >> 1 ;
        add[rt * 2] += add[rt] ;
        val[rt * 2] += add[rt] ;
        add[rt * 2 + 1] += add[rt] ;
        val[rt * 2 + 1] += add[rt] ;
        add[rt] = 0 ;
    }
    void build (int a, int l, int r, int rt) {
        if (l == r) {
            val[rt] = a; return ;
        }
        int m = (l + r) >> 1 ;
        build (a, lson); build (a, rson) ;
        pushup (rt) ;
    }
    void build (int size, int a) {
        n = size ;
        build (a, 1, n, 1) ;
    }
    void update (int L, int R, int add_val, int l, int r, int rt) {
        if (L <= l && R >= r) {
            val[rt] += add_val; add[rt] += add_val ;
            return ;
        }
        pushdown (l, r, rt) ;
        int m = (l + r) >> 1 ;
        if (L <= m) update (L, R, add_val, lson) ;
        if (R > m) update (L, R, add_val, rson) ;
        pushup (rt) ;
    }
    void update (int L, int R, int add_val) {
        update (L, R, add_val, 1, n, 1) ;
    }
    int query (int L, int R, int l, int r, int rt) {
        if (L <= l && R >= r) return val[rt] ;
        pushdown (l, r, rt) ;
        int m = (l + r) >> 1, res = 1e9 ;
        if (L <= m) res = std::min (res, query (L, R, lson)) ;
        if (R > m) res = std::min (res, query (L, R, rson)) ;
        return res ;
    }
    int query (int L, int R) {
        return query (L, R, 1, n, 1) ;
    }
} ;

#endif