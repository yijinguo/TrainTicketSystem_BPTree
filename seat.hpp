#ifndef TicketSystem_seat
#define TicketSystem_seat

#include <iostream>

class Seat {

private:
    int seat[101] ;
public:
    Seat () {}
    Seat (int n, int val) {
        for (int i = 1; i <= n; i ++) seat[i] = val ;
    }
    int operator [] (const int id) const {
        return seat[id] ;
    }
    int query (int l, int r) const {
        int res = 1e9 ;
        for (int i = l; i <= r; i ++)
            res = std::min (res, seat[i]) ;
        return res ;
    }
    void add (int val, int l, int r) {
        for (int i = l; i <= r; i ++)
            seat[i] += val ;
    }
} ;

#endif