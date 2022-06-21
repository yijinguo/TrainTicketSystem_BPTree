#ifndef TicketSystem_User
#define TicketSystem_User

#include <iostream>
#include "string.h"

class user {

    friend std::ostream& operator << (std::ostream &out, const user &_user) ;

private:
    String username, password, name, mailAddr ;
    int privilege ;

public:
    user () {}
    user (const String &_username, const String &_password, const String &_name, const String &_mailAddr, const int _privilege) {
        username = _username; password = _password ;
        name = _name; mailAddr = _mailAddr ;
        privilege = _privilege ;
    }

    int getPrivilege () const {
        return privilege ;
    }

    void login (const String &_password) const {
        if (password != _password) throw "wrong password" ;
    }

    String getPassword () {
        return password ;
    }

    void modifyPassword (const String &_password) {
        password = _password ;
    }

    String getName () {
        return name ;
    }

    void modifyName (const String &_name) {
        name = _name ;
    }

    String getMailAddress () {
        return mailAddr ;
    }

    void modifyMailAddress (const String &_mailAddr) {
        mailAddr = _mailAddr ;
    }

    void modifyPrivilege (const int _privilege) {
        privilege = _privilege ;
    }
} ;

std::ostream& operator << (std::ostream &out, const user &_user) {
    out << _user.username << " " << _user.name << " " << _user.mailAddr << " " << _user.privilege ;
    return out ;
}

#endif