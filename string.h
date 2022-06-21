#ifndef CODE_ST_RING_H
#define CODE_ST_RING_H

#include <iostream>
#include <cstring>
// #include <assert.h>

class String {

    friend std::ostream& operator << (std::ostream &out, const String &str) ;

public:
    char str[110];
    int len = 0;//记录字母个数，有一个记一个
    int hash_val = 0 ;
public:
    String(const char *ch) {
        // assert (strlen (ch) < 35) ;
        strcpy (str, ch) ;
        len = strlen (ch);
        str[len] = '\0' ;
        hash_val = 0 ;
        for (int i = 0; i < len; ++i) {
            hash_val = (1ll * hash_val * 13331 + str[i]) % 998244353 ;
        }
        hash_val = (hash_val + 998244353) % 998244353 ;
    }
    String():hash_val(0),len(0){
        memset(str , 0 , sizeof str);
    }
    String(const String &ot) {
        strcpy (str, ot.str) ;
        len = ot.len; str[len] = '\0' ;
        hash_val = 0 ;
        for (int i = 0; i < len; ++i) {
            hash_val = (1ll * hash_val * 13331 + str[i]) % 998244353 ;
        }
        hash_val = (hash_val + 998244353) % 998244353 ;
    }
    ~String() {}
    String& operator=(const String &ot){
        if (this == &ot){
            return *this;
        }
        len = ot.len;
        for (int i = 0; i < ot.len; ++i) {
            str[i] = ot.str[i];
        }
        //strcpy (str, ot.str);
        str[len] = '\0' ;
        hash_val = ot.hash_val ;
        return *this;
    }
//    String& operator=(String ot){
//        if (this == &ot){
//            return *this;
//        }
//        len = ot.len;
//        for (int i = 0; i < ot.len; ++i) {
//            str[i] = ot.str[i];
//        }
//        //strcpy (str, ot.str);
//        str[len] = '\0' ;
//        hash_val = ot.hash_val ;
//        return *this;
//    }
    String& operator = (const char *_str) {
        strcpy (str, _str) ;
        len = strlen (_str) ;
        hash_val = 0 ;
        for (int i = 0; i < len; ++i) {
            hash_val = (1ll * hash_val * 13331 + str[i]) % 998244353 ;
        }
        hash_val = (hash_val + 998244353) % 998244353 ;
        return *this ;
    }
    String& operator += (const char _c) {
        str[len ++] = _c; str[len] = 0 ;
        hash_val = (1ll * hash_val * 13331 + _c) % 998244353 ;
        hash_val = (hash_val + 998244353) % 998244353 ;
        return *this ;
    }
    void clear() {
        len = hash_val = 0; str[len] = '\0' ;
    }
    bool operator<(const String &ot) const {
        return strcmp(this->str , ot.str) < 0;
    }
    bool operator<=(const String &ot) const {
        return strcmp(this->str , ot.str) <= 0;
    }
    bool operator==(const String &ot) const {
        return strcmp(this->str , ot.str) == 0;
    }
    bool operator != (const String &ot) const {
        return strcmp (this -> str, ot.str) != 0 ;
    }
    bool operator>(const String &ot) const {
        return strcmp(this->str , ot.str) > 0;
    }
    bool operator>=(const String &ot) const {
        return strcmp(this->str , ot.str) >= 0;
    }

    char operator [] (const int id) const {
        return str[id] ;
    }

    char& operator [] (const int id) {
        return str[id] ;
    }

    bool empty () const {
        return len == 0 ;
    }

    int toInt () const {
        int res = 0 ;
        for (int i = 0; i < len; i ++) {
            res = res * 10 + str[i] - '0' ;
        }
        return res ;
    }

    String substr (int l, int r) const {
        String res ;
        for (int i = l; i <= r; i ++) res.str[i - l] = str[i] ;
        res.len = r - l + 1 ;
        return res ;
    }

    operator int () {
        return hash_val ;
    }
};

#endif //CODE_ST_RING_H