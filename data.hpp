#ifndef data_type
#define data_type

#include <cstdio>
#include <cstring>

#include "string.h"

struct data {
    int hash_val, pos ;
    data () {
        pos = -1 ;
    }
    data (const int _hash_val, int p) {
        hash_val = _hash_val ;
        pos = p ;
    }
    data (String &str, int p) {
        hash_val = str.hash_val; pos = p ;
    }
    data& operator=(const data &ot){
        if (this == &ot) return *this;
        hash_val = ot.hash_val; pos = ot.pos ;
        return *this;
    }
    bool operator < (const data &a) const {
        if (hash_val == a.hash_val) return pos < a.pos ;
        return hash_val < a.hash_val ;
    }
    bool operator <= (const data &a) const {
        if (hash_val == a.hash_val) return pos <= a.pos ;
        return hash_val < a.hash_val ;
    }
    bool operator == (const data &a) const {
        return hash_val == a.hash_val && pos == a.pos ;
    }
    operator int () {
        return hash_val ;
    }
} ;

#endif