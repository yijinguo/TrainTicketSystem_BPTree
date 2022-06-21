#ifndef TicketSystem_CommandHandler
#define TicketSystem_CommandHandler

// #define debug
// #define rollbackSwitch

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>
// #include <vector>
// #include <algorithm>

#include "main.h"
// #include "Bpt_and_database.h"
// #include "B+Tree.hpp"
#include "BPlusTree.h"
#include "data.hpp"
#include "string.h"
#include "vector.h"

#include "user.hpp"
#include "train.hpp"
#include "time.hpp"
#include "order.hpp"

#include "log.hpp"

class CommandHandler {
private:
    std::string op ;
    char argument[30][10010] ;
    int key_cnt = 1, timeStamp = 0 ;
    bool isRollback = false ;

public:
    CommandHandler (const std::string _op) {
        // std::cout << _op << std::endl ;
        op = _op ;
    }

    void analyze () {
        bool isTimeStamp = 1 ;
        key_cnt = 1; int cur_len = 0 ;
        for (int i = 0; i < op.length(); i ++) {
            if (op[i] == ' ') {
                if (isTimeStamp) {
                    timeStamp = 0 ;
                    for (int i = 1; i < cur_len - 1; i ++)
                        timeStamp = timeStamp * 10 + argument[key_cnt][i] - '0' ;
                    cur_len = 0; isTimeStamp = 0 ;
                } else {
                    argument[key_cnt][cur_len ++] = 0; key_cnt ++; cur_len = 0 ;
                }
            } else {
                argument[key_cnt][cur_len ++] = op[i] ;
            }
        }
        argument[key_cnt][cur_len ++] = 0 ;
    }

    void user_read (user &cur, int pos) {
        userio.seekg (pos, std::ios::beg) ;
        userio.read (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    int user_write (user &cur) {
        userio.seekg (0, std::ios::end) ;
        int pos = userio.tellg() ;
        // int pos = userio.tellg(), file_pos ;
        // pos -= sizeof (int) ;
        // userio.read (reinterpret_cast<char *>(&file_pos), sizeof (file_pos)) ;
        userio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return pos ;
    }

    void user_write (int pos, user &cur) {
        userio.seekp (pos, std::ios::beg) ;
        userio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    void train_read (train &cur, int pos) {
        trainio.seekg (pos, std::ios::beg) ;
        trainio.read (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    int train_write (train &cur) {
        trainio.seekp (0, std::ios::end) ;
        int pos = trainio.tellp(), file_pos, nxt_file_pos, res ;
        pos -= sizeof (int) ;
        trainio.seekg (pos, std::ios::beg) ;
        trainio.read (reinterpret_cast<char *>(&file_pos), sizeof (file_pos)) ;
        if (file_pos == -1) {
            if (pos == 0) trainio.seekp (0, std::ios::beg) ;
            else trainio.seekp (0, std::ios::end) ;
            res = trainio.tellp() ;
            trainio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
            trainio.seekp (0, std::ios::end) ;
            trainio.write (reinterpret_cast<char *>(&file_pos), sizeof (file_pos)) ;
            return res ;
        } else {
            trainio.seekp (file_pos, std::ios::beg) ;
            trainio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
            res = file_pos ;
            trainio.seekg (file_pos + sizeof (cur), std::ios::beg) ;
            trainio.read (reinterpret_cast<char *>(&nxt_file_pos), sizeof (nxt_file_pos)) ;
            trainio.seekp (0, std::ios::end) ;
            pos = trainio.tellp(); pos -= sizeof (int) ;
            trainio.seekp (pos, std::ios::beg) ;
            trainio.write (reinterpret_cast<char *>(&nxt_file_pos), sizeof (nxt_file_pos)) ;
            return res ;
        }
    }

    void train_write (int pos, train &cur) {
        trainio.seekp (pos, std::ios::beg) ;
        trainio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    void train_delete (int pos) {
        trainio.seekp (0, std::ios::end) ;
        int tmp = trainio.tellp(); tmp -= sizeof (int) ;
        int file_pos ;
        trainio.seekg (tmp, std::ios::beg) ;
        trainio.read (reinterpret_cast<char *>(&file_pos), sizeof (file_pos)) ;
        trainio.seekp (tmp, std::ios::beg) ;
        trainio.write (reinterpret_cast<char *>(&pos), sizeof (pos)) ;
        tmp = pos + sizeof (train) ;
        trainio.seekp (tmp, std::ios::beg) ;
        trainio.write (reinterpret_cast<char *>(&file_pos), sizeof (file_pos)) ;
    }

    void order_read (order &cur, int pos) {
        orderio.seekg (pos, std::ios::beg) ;
        orderio.read (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    int order_write (order &cur) {
        orderio.seekp (0, std::ios::end) ;
        int pos = orderio.tellp() ;
        orderio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return pos ;
    }

    void order_write (int pos, order &cur) {
        orderio.seekp (pos, std::ios::beg) ;
        orderio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    void log_read (log &cur, long long pos) {
        logio.seekg (pos, std::ios::beg) ;
        logio.read (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    void update_last_log_pos (long long pos) {
        logio.seekp (0, std::ios::beg) ;
        logio.write (reinterpret_cast<char *>(&pos), sizeof pos) ;
    }

    long long get_last_log_pos () {
        long long pos ;
        logio.seekg (0, std::ios::beg) ;
        logio.read (reinterpret_cast<char *>(&pos), sizeof pos) ;
        return pos ;
    }

    void log_write (log &cur) {
        logio.seekp (0, std::ios::end) ;
        long long curPos = logio.tellp() ;
        logio.write (reinterpret_cast<char *>(&cur), sizeof cur) ;
        logio.seekp (0, std::ios::beg) ;
        logio.write (reinterpret_cast<char *>(&curPos), sizeof curPos) ;
    }

    void run () {
        try {
            analyze () ;
            // printf("%d ", timeStamp) ;
            // for (int i = 1; i <= key_cnt; i ++) printf("%s ", argument[i]) ;
            // printf("\n") ;
            //if (!isRollback) printf("[%d] ", timeStamp) ;
            if (strcmp (argument[1], "add_user") == 0) {
                add_user () ;
            } else if (strcmp (argument[1], "delete_user") == 0) {
                delete_user () ;
            } else if (strcmp (argument[1], "login") == 0) {
                login () ;
            } else if (strcmp (argument[1], "logout") == 0) {
                logout () ;
            } else if (strcmp (argument[1], "query_profile") == 0) {
                query_profile () ;
            } else if (strcmp (argument[1], "modify_profile") == 0) {
                modify_profile () ;
            } else if (strcmp (argument[1], "add_train") == 0) {
                add_train () ;
            } else if (strcmp (argument[1], "release_train") == 0) {
                release_train() ;
            } else if (strcmp (argument[1], "unrelease_train") == 0) {
                unrelease_train () ;
            } else if (strcmp (argument[1], "query_train") == 0) {
                query_train () ;
            } else if (strcmp (argument[1], "delete_train") == 0) {
                delete_train () ;
            } else if (strcmp (argument[1], "query_ticket") == 0) {
                query_ticket () ;
            } else if (strcmp (argument[1], "query_transfer") == 0) {
                query_transfer () ;
            } else if (strcmp (argument[1], "buy_ticket") == 0) {
                buy_ticket () ;
            } else if (strcmp (argument[1], "query_order") == 0) {
                query_order () ;
            } else if (strcmp (argument[1], "refund_ticket") == 0) {
                refund_ticket () ;
            } else if (strcmp (argument[1], "add_train_seat") == 0) {
                add_train_seat() ;
            } else if (strcmp (argument[1], "delete_order") == 0) {
                delete_order() ;
            } else if (strcmp (argument[1], "delete_pending_order") == 0) {
                delete_pending_order() ;
            } else if (strcmp (argument[1], "add_pending_order") == 0) {
                add_pending_order () ;
            } else if (strcmp (argument[1], "sell_train_seat") == 0) {
                sell_train_seat () ;
            } else if (strcmp (argument[1], "change_order_toSuccess") == 0) {
                change_order_toSuccess () ;
            } else if (strcmp (argument[1], "change_order_toPending") == 0) {
                change_order_toPending () ;
            } else if (strcmp (argument[1], "rollback") == 0) {
                rollback () ;
            } else if (strcmp (argument[1], "clean") == 0) {
                clean() ;
            } else if (strcmp (argument[1], "exit") == 0) {
                printf("bye\n") ;
                isExit = 1 ;
            }
        } catch (const char* s) {
            if (!isRollback) printf("-1\n") ;
            #ifdef debug
                printf("exception: %s\n", s) ;
            #endif
        }
    }

    void add_user () {
        String cur_username, username, password, name, mailAddr ;
        int privilege ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'c') cur_username = argument[i + 1] ;
            else if (argument[i][1] == 'u') username = argument[i + 1] ;
            else if (argument[i][1] == 'p') password = argument[i + 1] ;
            else if (argument[i][1] == 'n') name = argument[i + 1] ;
            else if (argument[i][1] == 'm') mailAddr = argument[i + 1] ;
            else if (argument[i][1] == 'g') privilege = String (argument[i + 1]).toInt() ;
        }

        if (users.empty()) {
            privilege = 10 ;
        } else {
            sjtu::vector<int> pos ;
            curUsers.find (data (cur_username, 0), pos) ;
            if (pos.empty()) throw "cur user not logged in" ;

            user cur_user; user_read (cur_user, pos[0]) ;
            if (cur_user.getPrivilege() <= privilege) throw "invalid privilege" ;
        }

        sjtu::vector<int> pos ;
        users.find (data (username, 0), pos) ;
        if (!pos.empty()) throw "user already exists" ;
        user new_user = user (username, password, name, mailAddr, privilege) ;
        int write_pos = user_write (new_user) ;
        users.Insert (data (username, write_pos), write_pos, write_pos) ;

        if (!isRollback) {
            printf ("0\n") ;

            #ifdef rollbackSwitch
                std::string logStr = "["; logStr += int_to_str (timeStamp).str; logStr += "] " ;
                logStr += "delete_user -u " ;
                for (int i = 0; i < username.len; i ++)
                    logStr = logStr + username.str[i] ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                    curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        }
    }

    void delete_user () {
        String username ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = argument[i + 1] ;
        }
        sjtu::vector<int> pos ;
        users.find (data (username, 0), pos) ;
        users.Remove (data (username, pos[0]), pos[0]) ;
    }

    void login () {
        String username, password ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = argument[i + 1] ;
            else if (argument[i][1] == 'p') password = argument[i + 1] ;
        }

        sjtu::vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (!pos.empty()) throw "already logged in" ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "username not exists" ;

        int user_file_pos = pos[0] ;
        user cur_user; user_read (cur_user, user_file_pos) ;
        cur_user.login (password) ;
        curUsers.Insert (data (username, user_file_pos), user_file_pos, user_file_pos) ;

        if (!isRollback) {
            printf ("0\n") ;

            #ifdef rollbackSwitch
                std::string logStr = "["; logStr += int_to_str (timeStamp).str; logStr += "] " ; 
                logStr += "logout -u " ;
                logStr = logStr + username.str ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        }
    }

    void logout () {
        String username ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = argument[i + 1] ;
        }
        sjtu::vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "cur user not logged in" ;
        int user_file_pos = pos[0] ;
        curUsers.Remove (data (username, user_file_pos), user_file_pos) ;

        pos.clear() ;
        users.find (data (username, 0), pos) ;
        user curUser; user_read (curUser, pos[0]) ;
        if (!isRollback) {
            printf ("0\n") ;

            #ifdef rollbackSwitch
                std::string logStr = "["; logStr += int_to_str (timeStamp).str; logStr += "] " ;
                logStr += "login -u " ;
                logStr = logStr + username.str ;
                logStr = logStr + " -p " + curUser.getPassword().str ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        }
    }

    void query_profile () {
        if (isRollback) return ;
        String cur_username, username ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'c') cur_username = argument[i + 1] ;
            else if (argument[i][1] == 'u') username = argument[i + 1] ;
        }

        sjtu::vector<int> pos ;
        curUsers.find (data (cur_username, 0), pos) ;
        if (pos.empty()) throw "cur user not logged in" ;
        int cur_user_file_pos = pos[0] ;
        user cur_user; user_read (cur_user, cur_user_file_pos) ;

        pos.clear() ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not found" ;
        int query_user_file_pos = pos[0] ;
        user query_user; user_read (query_user, query_user_file_pos) ;
        if (!(cur_user.getPrivilege() > query_user.getPrivilege() || cur_username == username)) throw "invalid privilege" ;
        
        std::cout << query_user << std::endl ;
    }

    void modify_profile () {
        String cur_username, username, password, name, mailAddr ;
        int privilege = -1 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'c') cur_username = argument[i + 1] ;
            else if (argument[i][1] == 'u') username = argument[i + 1] ;
            else if (argument[i][1] == 'p') password = argument[i + 1] ;
            else if (argument[i][1] == 'n') name = argument[i + 1] ;
            else if (argument[i][1] == 'm') mailAddr = argument[i + 1] ;
            else if (argument[i][1] == 'g') privilege = String (argument[i + 1]).toInt() ;
        }

        sjtu::vector<int> pos ;
        curUsers.find (data (cur_username, 0), pos) ;
        if (pos.empty()) throw "user enot logged in" ;
        int cur_user_file_pos = pos[0] ;
        user cur_user; user_read (cur_user, cur_user_file_pos) ;
        
        pos.clear() ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not found" ;
        int modify_user_file_pos = pos[0] ;
        user modify_user; user_read (modify_user, modify_user_file_pos) ;

        if (!(cur_user.getPrivilege() > modify_user.getPrivilege() || cur_username == username)) throw "invalid privilege" ;
        if (privilege != -1 && privilege >= cur_user.getPrivilege()) throw "invalid privilege" ;

        if (!isRollback) {
            #ifdef rollbackSwitch
                std::string logStr = "["; logStr += int_to_str (timeStamp).str; logStr += "] " ;
                logStr += "modify_profile -c " ;
                logStr = logStr + username.str ;
                logStr = logStr + " -u " + username.str ;
                if (!password.empty()) logStr = logStr + " -p " + modify_user.getPassword().str ;
                if (!name.empty()) logStr = logStr + " -n " + modify_user.getName().str ;
                if (!mailAddr.empty()) logStr = logStr + " -m " + modify_user.getMailAddress().str ;
                if (privilege != -1) logStr = logStr + " -g " + int_to_str (modify_user.getPrivilege()).str ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        }

        if (!password.empty()) modify_user.modifyPassword (password) ;
        if (!name.empty()) modify_user.modifyName (name) ;
        if (!mailAddr.empty()) modify_user.modifyMailAddress (mailAddr) ;
        if (privilege != -1) modify_user.modifyPrivilege (privilege) ;
        user_write (modify_user_file_pos, modify_user) ;

        if (!isRollback) std::cout << modify_user << std::endl ;
    }

    String int_to_str (int x) {
        std::string str ;
        while (x) {
            str = str + (char)(x % 10 + '0') ;
            x /= 10 ;
        }
        std::reverse (str.begin(), str.end()) ;
        return String (str.c_str()) ;
    }

    void split_String (String *res, char *str) {
        int cur_id = 1, cur_len = 0, len = strlen (str) ;
        String tmp ;
        for (int i = 0; i < len; i ++) {
            if (str[i] == '|') {
                res[cur_id] = tmp ;
                tmp.clear() ;
                cur_id ++, cur_len = 0 ;
            } else {
                tmp += str[i] ;
            }
        }
        res[cur_id] = tmp ;
    }

    void split_int (int *res, char *str) {
        int cur_id = 1, len = strlen (str) ;
        res[cur_id] = 0 ;
        for (int i = 0; i < len; i ++) {
            if (str[i] == '|') {
                cur_id ++; res[cur_id] = 0 ;
            } else {
                res[cur_id] = res[cur_id] * 10 + str[i] - '0' ;
            }
        }
    }

    void add_train() {
        int stationNum, seatNum, prices[110], travelTimes[110], stopoverTimes[110] ;
        String trainID, stations[110] ;
        char type ;
        String startTimeStr, saleDateStr ;
        Time startTime, saleDate[3] ;

        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = argument[i + 1] ;
            else if (argument[i][1] == 'n') stationNum = String (argument[i + 1]).toInt() ;
            else if (argument[i][1] == 'm') seatNum = String (argument[i + 1]).toInt() ;
            else if (argument[i][1] == 's') split_String (stations, argument[i + 1]) ;
            else if (argument[i][1] == 'p') split_int (prices, argument[i + 1]) ;
            else if (argument[i][1] == 'x') startTime = Time (0, argument[i + 1]), startTimeStr = argument[i + 1] ;
            else if (argument[i][1] == 't') split_int (travelTimes, argument[i + 1]) ;
            else if (argument[i][1] == 'o') split_int (stopoverTimes, argument[i + 1]) ;
            else if (argument[i][1] == 'd') {
                String tmp = argument[i + 1] ;
                saleDateStr = tmp ;
                saleDate[1] = Time (tmp.substr (0, 4), 0); saleDate[2] = Time (tmp.substr (6, 10), 0) ;
            }
            else if (argument[i][1] == 'y') type = argument[i + 1][0] ;
        }


        sjtu::vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (!pos.empty()) throw "train already exists" ;
        
        train new_train = train (trainID, stationNum, stations, seatNum, prices, startTime, travelTimes, stopoverTimes, saleDate, type) ;
        new_train.startTimeStr = startTimeStr; new_train.saleDateStr = saleDateStr ;
        int train_file_pos = train_write (new_train) ;
        trains.Insert (data (trainID, train_file_pos), train_file_pos, train_file_pos) ;

        for (int i = 1; i <= stationNum; i ++) {
            trainStations.Insert (data (stations[i], train_file_pos), train_file_pos, train_file_pos) ;
        }

        if (!isRollback) {
            printf("0\n") ;

            #ifdef rollbackSwitch
                std::string logStr = "["; logStr += int_to_str (timeStamp).str; logStr += "] " ;
                logStr += "delete_train -i " ;
                logStr = logStr + trainID.str ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        }
    }

    void release_train () {
        String trainID ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = argument[i + 1] ;
        }

        sjtu::vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not found" ;
        int train_file_pos = pos[0] ;
        train cur_train; train_read (cur_train, train_file_pos) ;
        cur_train.release() ;
        train_write (train_file_pos, cur_train) ;

        if (!isRollback) {
            printf("0\n") ;

            #ifdef rollbackSwitch
                std::string logStr = "["; logStr += int_to_str (timeStamp).str; logStr += "] " ;
                logStr += "unrelease_train -i " ;
                logStr = logStr + trainID.str ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        }
    }

    void unrelease_train() {
        String trainID ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = argument[i + 1] ;
        }

        sjtu::vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not found" ;
        int train_file_pos = pos[0] ;
        train cur_train; train_read (cur_train, train_file_pos) ;
        cur_train.unrelease() ;
        train_write (train_file_pos, cur_train) ;
    }

    void query_train () {
        if (isRollback) return ;
        String trainID ;
        Time date ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = argument[i + 1] ;
            else if (argument[i][1] == 'd') date = Time (argument[i + 1], 0) ;
        }

        sjtu::vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not found" ;
        int train_file_pos = pos[0] ;
        train cur_train; train_read (cur_train, train_file_pos) ;
        if (!cur_train.runningOnDate (date)) throw "train doesn't run on this date" ;
        cur_train.print (date) ;
    }

    void delete_train () {
        String trainID ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = argument[i + 1] ;
        }

        sjtu::vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not found" ;
        int train_file_pos = pos[0] ;
        train cur_train; train_read (cur_train, train_file_pos) ;
        if (cur_train.isReleased()) throw "already released" ;

        for (int i = 1; i <= cur_train.getStationNum(); i ++) {
            trainStations.Remove (data (cur_train.getStation(i), train_file_pos), train_file_pos) ;
        }
        trains.Remove (data (trainID, train_file_pos), train_file_pos) ;
        train_delete (train_file_pos) ;

        if (!isRollback) {
            printf("0\n") ; 

            #ifdef rollbackSwitch
                std::string logStr = "["; logStr += int_to_str (timeStamp).str; logStr += "] " ;
                logStr += "add_train -i " ;
                logStr = logStr + trainID.str ;
                logStr = logStr + " -n " + int_to_str (cur_train.getStationNum()).str ;
                logStr = logStr + " -m " + int_to_str (cur_train.getSeatNum()).str ;
                logStr = logStr + " -s " ;
                for (int i = 1; i <= cur_train.getStationNum(); i ++)
                    logStr = logStr + cur_train.getStation(i).str + (i == cur_train.getStationNum() ? "" : "|") ;
                logStr = logStr + " -p " ;
                for (int i = 1; i < cur_train.getStationNum(); i ++)
                    logStr = logStr + int_to_str (cur_train.getPrice(i)).str + (i == cur_train.getStationNum() - 1 ? "" : "|") ;
                logStr = logStr + " -x " + cur_train.startTimeStr.str ;
                logStr = logStr + " -t " ;
                for (int i = 1; i < cur_train.getStationNum(); i ++)
                    logStr = logStr + int_to_str (cur_train.getTravellingTime(i)).str + (i == cur_train.getStationNum() - 1 ? "" : "|") ;
                logStr = logStr + " -o " ;
                for (int i = 2; i < cur_train.getStationNum(); i ++)
                    logStr = logStr + int_to_str (cur_train.getStopoverTime(i)).str + (i == cur_train.getStationNum() - 1 ? "" : "|") ;
                logStr = logStr + " -d " + cur_train.saleDateStr.str ;
                logStr = logStr + " -y " + cur_train.getType() ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        }
    }

    // class cmp_time {
    // public:
    //     cmp_time () {}
    //     bool operator () (const order &a, const order &b) const {
    //         if (a.getTravellingTime() == b.getTravellingTime()) return a.getTrainID() < b.getTrainID() ;
    //         return a.getTravellingTime() < b.getTravellingTime() ;
    //     }
    // } ;

    // class cmp_cost {
    // public:
    //     cmp_cost () {}
    //     bool operator () (const order &a, const order &b) const {
    //         if (a.getPrice() == b.getPrice()) return a.getTrainID() < b.getTrainID() ;
    //         return a.getPrice() < b.getPrice() ;
    //     }
    // } ;

    static bool cmp_time (order &a, order &b) {
        if (a.getTravellingTime() == b.getTravellingTime()) return a.getTrainID() < b.getTrainID() ;
        return a.getTravellingTime() < b.getTravellingTime() ;
    }

    static bool cmp_cost (order &a, order &b) {
        if (a.getPrice() == b.getPrice()) return a.getTrainID() < b.getTrainID() ;
        return a.getPrice() < b.getPrice() ;
    }

    void query_ticket () {
        if (isRollback) return ;
        String fromStation, toStation ;
        Time date ;
        int priority = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 's') fromStation = argument[i + 1] ;
            else if (argument[i][1] == 't') toStation = argument[i + 1] ;
            else if (argument[i][1] == 'd') date = Time (String (argument[i + 1]), 0) ;
            else if (argument[i][1] == 'p') priority = strcmp (argument[i + 1], "time") == 0 ? 0 : 1 ;
        }

        sjtu::vector<int> train1, train2 ;
        sjtu::vector<int> pos ;
        trainStations.find (data (fromStation, 0), train1) ;
        trainStations.find (data (toStation, 0), train2) ;

        // std::sort (train1.begin(), train1.end()) ;
        // std::sort (train2.begin(), train2.end()) ;
        // train1.sort1 () ;
        // train2.sort1 () ;

        sjtu::vector<int> possible_trains ;
        int train1_id = 0, train2_id = 0 ;
        while (train1_id < train1.size() && train2_id < train2.size()) {
            if (train1[train1_id] < train2[train2_id]) train1_id ++ ;
            else if (train1[train1_id] > train2[train2_id]) train2_id ++ ;
            else {
                possible_trains.push_back (train1[train1_id]) ;
                train1_id ++; train2_id ++ ;
            }
        }

        if (possible_trains.empty()) {
            printf("0\n"); return ;
        }

        sjtu::vector<order> orders ;
        // sjtu::vector<order, std::less<int>, cmp_time, cmp_cost> orders ;
        int order_cnt = 0 ;
        for (int i = 0; i < possible_trains.size(); i ++) {
            train cur_train; train_read (cur_train, possible_trains[i]) ;
            if (!cur_train.isReleased()) continue ;
            if (!cur_train.direction (fromStation, toStation)) continue ;
            if (!cur_train.canDepartFromStationOnDate (date, fromStation)) continue ;
            Time trainStartTime = cur_train.getStartTime (date, fromStation) ;
            orders.push_back (order (cur_train.getTrainID(), fromStation, toStation, 
            cur_train.getLeavingTime (trainStartTime, fromStation), 
            cur_train.getArrivingTime (trainStartTime, toStation), 
            cur_train.calPrice (fromStation, toStation), 
            cur_train.calSeats (trainStartTime, fromStation, toStation), 
            cur_train.calTravellingTime (fromStation, toStation))) ;
        }
        // if (priority == 0) std::sort (orders.begin(), orders.end(), cmp_time) ;
        // else std::sort (orders.begin(), orders.end(), cmp_cost) ;
        if (priority == 0) orders.sort (cmp_time) ;
        else orders.sort (cmp_cost) ;
        std::cout << orders.size() << std::endl ;
        for (int i = 0; i < orders.size(); i ++)
            orders[i].print() ;
    }

    bool compareTransferOrder (int priority, int travellingTime, int totalPrice, String train1ID, String train2ID, int optTravellingTime, int optPrice, String optTrain1ID, String optTrain2ID) {
        if (priority == 0) {
            if (travellingTime < optTravellingTime) return 1 ;
            else if (travellingTime > optTravellingTime) return 0 ;
            if (totalPrice < optPrice) return 1 ;
            else if (totalPrice > optPrice) return 0 ;
            if (train1ID < optTrain1ID) return 1 ;
            else if (train1ID > optTrain1ID) return 0 ;
            if (train2ID < optTrain2ID) return 1 ;
            else return 0 ;
        } else {
            if (totalPrice < optPrice) return 1 ;
            else if (totalPrice > optPrice) return 0 ;
            if (travellingTime < optTravellingTime) return 1 ;
            else if (travellingTime > optTravellingTime) return 0 ;
            if (train1ID < optTrain1ID) return 1 ;
            else if (train1ID > optTrain1ID) return 0 ;
            if (train2ID < optTrain2ID) return 1 ;
            else return 0 ;
        }
    }

    void query_transfer () {
        if (isRollback) return ;
        String fromStation, toStation ;
        Time date ;
        bool priority = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 's') fromStation = argument[i + 1] ;
            else if (argument[i][1] == 't') toStation = argument[i + 1] ;
            else if (argument[i][1] == 'd') date = Time (argument[i + 1], 0) ;
            else if (argument[i][1] == 'p') priority = strcmp (argument[i + 1], "time") == 0 ? 0 : 1 ;
        }

        sjtu::vector<int> train1_pos, train2_pos ;
        trainStations.find (data (fromStation, 0), train1_pos) ;
        trainStations.find (data (toStation, 0), train2_pos) ;

        int optTravellingTime = 1e9, optPrice = 1e9 ;
        String optTrain1ID, optTrain2ID ;
        order order_1, order_2 ;

        for (int i = 0; i < train1_pos.size(); i ++) {
            train train1; train_read (train1, train1_pos[i]) ;
            if (!train1.isReleased()) continue ;
            if (!train1.canDepartFromStationOnDate (date, fromStation)) continue ;
            Time train1_startTime = train1.getStartTime (date, fromStation) ;
            int train1_stationNum = train1.getStationNum() ;
            for (int stationID = train1.getStationID (fromStation) + 1; stationID <= train1_stationNum; stationID ++) {
                String midStation = train1.getStation (stationID) ;
                if (midStation == toStation) continue ;
                Time train1_leavingTime = train1.getLeavingTime (train1_startTime, fromStation) ;
                Time train1_arrivingTime = train1.getArrivingTime (train1_startTime, midStation) ;
                int train1_travellingTime = train1.calTravellingTime (fromStation, midStation) ;
                int train1_price = train1.calPrice (fromStation, midStation) ;
                int train1_seat = train1.calSeats (train1_startTime, fromStation, midStation) ;

                for (int j = 0; j < train2_pos.size(); j ++) {
                    if (train1_pos[i] == train2_pos[j]) continue ;
                    train train2; train_read (train2, train2_pos[j]) ;
                    if (!train2.isReleased()) continue ;
                    if (!train2.direction (midStation, toStation)) continue ;
                    if (!train2.canDepartFromStationAferTime (train1_arrivingTime, midStation)) continue ;

                    Time train2_startTime = train2.getStartTimeAfterTime (train1_arrivingTime, midStation) ;
                    Time train2_leavingTime = train2.getLeavingTime (train2_startTime, midStation) ;
                    Time train2_arrivingTime = train2.getArrivingTime (train2_startTime, toStation) ;
                    int train2_travellingTime = train2.calTravellingTime (midStation, toStation) ;
                    int train2_price = train2.calPrice (midStation, toStation) ;
                    int train2_seat = train2.calSeats (train2_startTime, midStation, toStation) ;

                    int travellingTime = train2_arrivingTime - train1_leavingTime ;
                    int totalPrice = train1_price + train2_price ;
                    // if (priority == 0 && (travellingTime < cost || (travellingTime == cost && train1_travellingTime < order_1.getTravellingTime()))) {
                    //     cost = travellingTime ;
                    //     order_1 = order (train1.getTrainID(), fromStation, midStation, train1_leavingTime, train1_arrivingTime, train1_price, train1_seat, train1_travellingTime) ;
                    //     order_2 = order (train2.getTrainID(), midStation, toStation, train2_leavingTime, train2_arrivingTime, train2_price, train2_seat, train2_travellingTime) ;
                    // }
                    // if (priority == 1 && (train1_price + train2_price < cost || (train1_price + train2_price == cost && train1_travellingTime < order_1.getTravellingTime()))) {
                    //     cost = train1_price + train2_price ;
                    //     order_1 = order (train1.getTrainID(), fromStation, midStation, train1_leavingTime, train1_arrivingTime, train1_price, train1_seat, train1_travellingTime) ;
                    //     order_2 = order (train2.getTrainID(), midStation, toStation, train2_leavingTime, train2_arrivingTime, train2_price, train2_seat, train2_travellingTime) ;
                    // }
                    if (compareTransferOrder (priority, travellingTime, totalPrice, train1.getTrainID(), train2.getTrainID(), optTravellingTime, optPrice, optTrain1ID, optTrain2ID)) {
                        optTravellingTime = travellingTime ;
                        optPrice = totalPrice ;
                        optTrain1ID = train1.getTrainID(); optTrain2ID = train2.getTrainID() ;
                        order_1 = order (train1.getTrainID(), fromStation, midStation, train1_leavingTime, train1_arrivingTime, train1_price, train1_seat, train1_travellingTime) ;
                        order_2 = order (train2.getTrainID(), midStation, toStation, train2_leavingTime, train2_arrivingTime, train2_price, train2_seat, train2_travellingTime) ;
                    }
                }
            }
        }
        if (optTravellingTime == 1e9) printf("0\n") ;
        else {
            order_1.print(); order_2.print() ;
        }
    }

    void buy_ticket () {
        String username, trainID, fromStation, toStation ;
        Time date ;
        int ticketNum ;
        bool queue = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = argument[i + 1] ;
            else if (argument[i][1] == 'i') trainID = argument[i + 1] ;
            else if (argument[i][1] == 'd') date = Time (argument[i + 1], 0) ;
            else if (argument[i][1] == 'n') ticketNum = String (argument[i + 1]).toInt() ;
            else if (argument[i][1] == 'f') fromStation = argument[i + 1] ;
            else if (argument[i][1] == 't') toStation = argument[i + 1] ;
            else if (argument[i][1] == 'q') queue = strcmp (argument[i + 1], "false") == 0 ? 0 : 1 ;
        }

        sjtu::vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;
        int user_file_pos = pos[0] ;
        user cur_user; user_read (cur_user, user_file_pos) ;

        pos.clear() ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not found" ;
        int train_file_pos = pos[0] ;
        train cur_train; train_read (cur_train, train_file_pos) ;

        if (!cur_train.isReleased()) throw "train not released" ;
        if (!cur_train.direction (fromStation, toStation)) throw "cannot depart" ;
        if (!cur_train.canDepartFromStationOnDate (date, fromStation)) throw "cannot depart" ;
        if (ticketNum > cur_train.getSeatNum()) throw "no enough tickets" ;

        Time trainStartTime = cur_train.getStartTime (date, fromStation) ;
        int remainingSeats = cur_train.calSeats (trainStartTime, fromStation, toStation) ;
        
        if (!queue && remainingSeats < ticketNum) throw "no enough tickets" ;

        order cur_order = order (trainID, fromStation, toStation, 
        cur_train.getLeavingTime (trainStartTime, fromStation), 
        cur_train.getArrivingTime (trainStartTime, toStation), 
        cur_train.calPrice (fromStation, toStation), ticketNum, 
        cur_train.calTravellingTime (fromStation, toStation)) ;

        if (remainingSeats >= ticketNum) {
            cur_order.setStatus (success) ;
            cur_train.sellSeats (trainStartTime, fromStation, toStation, ticketNum) ;
            int order_file_pos = order_write (cur_order) ;
            orders.Insert (data (username, order_file_pos), order_file_pos, order_file_pos) ;
            printf("%lld\n", 1ll * ticketNum * cur_train.calPrice (fromStation, toStation)) ;

            #ifdef rollbackSwitch
                std::stringstream s ;
                s << "[" << timeStamp << "] " ;
                s << "add_train_seat -i " << trainID << " -T " << trainStartTime.getDateStr() << " -s " << fromStation << " -t " << toStation << " -n " << ticketNum ;
                std::string logStr ;
                getline (s, logStr) ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;

                s.clear(); logStr.clear() ;
                s << "[" << timeStamp << "] " ;
                s << "delete_order -u " << username << " -p " << order_file_pos ;
                getline (s, logStr) ;
                preFilePos = get_last_log_pos() ;
                curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        } else {
            cur_order.setStatus (pending) ;
            int order_file_pos = order_write (cur_order) ;
            orders.Insert (data (username, order_file_pos), order_file_pos, order_file_pos) ;
            pendingOrders.Insert (data (trainID, order_file_pos), order_file_pos, order_file_pos) ;
            printf("queue\n") ;

            #ifdef rollbackSwitch
                std::stringstream s ;
                std::string logStr ;
                s << "[" << timeStamp << "] " ;
                s << "delete_order -u " << username << " -p " << order_file_pos ;
                getline (s, logStr) ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;

                s.clear(); logStr.clear() ;
                s << "[" << timeStamp << "] " ;
                s << "delete_pending_order -i " << trainID << " -p " << order_file_pos ;
                getline (s, logStr) ;
                preFilePos = get_last_log_pos() ;
                curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        }

        // if (trainID == String ("LeavesofGrass")) cur_train.print(date) ;

        // train_write (train_file_pos, cur_train) ;
    }

    void query_order () {
        if (isRollback) return ;
        String username ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = argument[i + 1] ;
        }

        sjtu::vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;

        pos.clear() ;
        orders.find (data (username, 0), pos) ;
        for (int i = 0; i < pos.size() / 2; i ++)
            std::swap (pos[i], pos[pos.size() - 1 - i]) ;
        std::cout << pos.size() << std::endl ;
        for (int i = 0; i < pos.size(); i ++) {
            order cur_order; order_read (cur_order, pos[i]) ;
            std::cout << cur_order << std::endl ;
        }
    }

    void refund_ticket () {
        String username ;
        int num = 1 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = argument[i + 1] ;
            else if (argument[i][1] == 'n') num = String (argument[i + 1]).toInt() ;
        }

        sjtu::vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;

        pos.clear() ;
        orders.find (data (username, 0), pos) ;
        if (pos.size() < num) throw "no nth order" ;
        // std::reverse (pos.begin(), pos.end()) ;
        // pos.reverse() ;
        for (int i = 0; i < pos.size() / 2; i ++)
            std::swap (pos[i], pos[pos.size() - 1 - i]) ;
        int order_file_pos = pos[num - 1] ;
        order cur_order; order_read (cur_order, pos[num - 1]) ;
        if (cur_order.getStatus() == refunded) throw "cannot refund" ;

        String trainID = cur_order.getTrainID() ;
        if (cur_order.getStatus() == pending) {
            pendingOrders.Remove (data (trainID, order_file_pos), order_file_pos) ;

            #ifdef rollbackSwitch
                std::stringstream s ;
                std::string logStr ;
                s << "[" << timeStamp << "] " ;
                s << "add_pending_order -i " << trainID << " -p " << order_file_pos ;
                getline (s, logStr) ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;

                s.clear(); logStr.clear() ;
                s << "[" << timeStamp << "] " ;
                s << "change_order_toPending -p " << order_file_pos ;
                getline (s, logStr) ;
                preFilePos = get_last_log_pos() ;
                curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif
        } else {
            sjtu::vector<int> tmp ;
            trains.find (data (trainID, 0), tmp) ;
            int train_file_pos = tmp[0] ;
            train cur_train; train_read (cur_train, train_file_pos) ;
            Time trainStartTime = cur_train.getStartTimeFromLeavingTime (cur_order.getLeavingTime(), cur_order.getFromStation()) ;
            cur_train.addSeats (trainStartTime, cur_order.getFromStation(), cur_order.getToStation(), cur_order.getSeatNum()) ;

            #ifdef rollbackSwitch
                std::stringstream s ;
                s << "[" << timeStamp << "] " ;
                s << "sell_train_seat -i " << trainID << " -T " << trainStartTime.getDateStr() << " -s " << cur_order.getFromStation() << " -t " << cur_order.getToStation() << " -n " << cur_order.getSeatNum() ;
                std::string logStr ;
                getline (s, logStr) ;
                long long preFilePos = get_last_log_pos() ;
                log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;

                s.clear(); logStr.clear() ;
                s << "[" << timeStamp << "] " ;
                s << "change_order_toSuccess -p " << order_file_pos ;
                getline (s, logStr) ;
                preFilePos = get_last_log_pos() ;
                curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                #ifdef debug
                curLog.print() ;
                #endif
                log_write (curLog) ;
            #endif

            tmp.clear() ;
            pendingOrders.find (data (trainID, 0), tmp) ;
            for (int i = 0; i < tmp.size(); i ++) {
                order pending_order; order_read (pending_order, tmp[i]) ;
                Time pending_startTime = cur_train.getStartTimeFromLeavingTime (pending_order.getLeavingTime(), pending_order.getFromStation()) ;
                if (pending_order.getSeatNum() <= cur_train.calSeats (pending_startTime, pending_order.getFromStation(), pending_order.getToStation())) {
                    cur_train.sellSeats (pending_startTime, pending_order.getFromStation(), pending_order.getToStation(), pending_order.getSeatNum()) ;
                    
                    #ifdef rollbackSwitch
                        s.clear(); logStr.clear() ;
                        s << "[" << timeStamp << "] " ;
                        s << "add_train_seat -i " << trainID << " -T " << pending_startTime.getDateStr() << " -s " << pending_order.getFromStation() << " -t " << pending_order.getToStation() << " -n " << pending_order.getSeatNum() ;
                        getline (s, logStr) ;
                        long long preFilePos = get_last_log_pos() ;
                        log curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                        #ifdef debug
                            curLog.print() ;
                        #endif
                        log_write (curLog) ;
                    #endif

                    pending_order.setStatus (success) ;
                    order_write (tmp[i], pending_order) ;
                    pendingOrders.Remove (data (trainID, tmp[i]), tmp[i]) ;

                    #ifdef rollbackSwitch
                        s.clear(); logStr.clear() ;
                        s << "[" << timeStamp << "] " ;
                        s << "change_order_toPending -i " << trainID << " -p " << tmp[i] ;
                        getline (s, logStr) ;
                        preFilePos = get_last_log_pos() ;
                        curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                        #ifdef debug
                            curLog.print() ;
                        #endif
                        log_write (curLog) ;

                        s.clear(); logStr.clear() ;
                        s << "[" << timeStamp << "] " ;
                        s << "add_pending_order -i " << trainID << " -p " << tmp[i] ;
                        getline (s, logStr) ;
                        preFilePos = get_last_log_pos() ;
                        curLog = log (logStr.c_str(), timeStamp, preFilePos) ;
                        #ifdef debug
                            curLog.print() ;
                        #endif
                        log_write (curLog) ;
                    #endif
                }
            }
            // train_write (train_file_pos, cur_train) ;
        }
        cur_order.setStatus (refunded) ;
        order_write (order_file_pos, cur_order) ;

        printf("0\n") ;
    }

    void add_train_seat() {
        String trainID, fromStation, toStation ;
        Time trainStartTime ;
        int num = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = argument[i + 1] ;
            else if (argument[i][1] == 'T') trainStartTime = Time (argument[i + 1], 0) ;
            else if (argument[i][1] == 's') fromStation = argument[i + 1] ;
            else if (argument[i][1] == 't') toStation = argument[i + 1] ;
            else if (argument[i][1]) num = String (argument[i + 1]).toInt() ;
        }
        sjtu::vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        train curTrain; train_read (curTrain, pos[0]) ;
        curTrain.addSeats (trainStartTime, fromStation, toStation, num) ;
        // train_write (pos[0], curTrain) ;
    }

    void delete_order () {
        String username ;
        int filePos = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = String (argument[i + 1]) ;
            else if (argument[i][1] == 'p') filePos = String (argument[i + 1]).toInt() ;
        }
        orders.Remove (data (username, filePos), filePos) ;
    }

    void delete_pending_order () {
        String trainID ;
        int filePos ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = String (argument[i + 1]) ;
            else if (argument[i][1] == 'p') filePos = String (argument[i + 1]).toInt() ;
        }
        pendingOrders.Remove (data (trainID, filePos), filePos) ;
    }

    void add_pending_order () {
        String trainID ;
        int filePos ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = String (argument[i + 1]) ;
            else if (argument[i][1] == 'p') filePos = String (argument[i + 1]).toInt() ;
        }
        pendingOrders.Insert (data (trainID, filePos), filePos, filePos) ;
    }

    void sell_train_seat() {
        // printf("sell_train_seat\n") ;
        String trainID, fromStation, toStation ;
        Time trainStartTime ;
        int num = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'i') trainID = argument[i + 1] ;
            else if (argument[i][1] == 'T') {
                String tmp = argument[i + 1] ;
                trainStartTime = Time (tmp.substr (0, 4), 0) ;
            } 
            else if (argument[i][1] == 's') fromStation = argument[i + 1] ;
            else if (argument[i][1] == 't') toStation = argument[i + 1] ;
            else if (argument[i][1]) num = String (argument[i + 1]).toInt() ;
        }
        sjtu::vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        train curTrain; train_read (curTrain, pos[0]) ;
        curTrain.sellSeats (trainStartTime, fromStation, toStation, num) ;
        // train_write (pos[0], curTrain) ;
    }

    void change_order_toSuccess() {
        int filePos = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'p') filePos = String (argument[i + 1]).toInt() ;
        }
        order curOrder; order_read (curOrder, filePos) ;
        curOrder.setStatus (success) ;
        order_write (filePos, curOrder) ;
    }

    void change_order_toPending() {
        int filePos = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'p') filePos = String (argument[i + 1]).toInt() ;
        }
        order curOrder; order_read (curOrder, filePos) ;
        curOrder.setStatus (pending) ;
        order_write (filePos, curOrder) ;
    }

    void rollback () {
        String username ;
        int targTimeStamp = 0 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 't') targTimeStamp = String (argument[i + 1]).toInt() ;
        }
        // printf("timeStamp: %d\n", timeStamp) ;
        // printf("targTimeStamp: %d\n", targTimeStamp) ;
        if (targTimeStamp > timeStamp) throw "time travel!" ;
        long long curFilePos = get_last_log_pos() ;
        while (curFilePos != -1) {
            log curLog; log_read (curLog, curFilePos) ;
            // printf("curLog: timestamp: %d %s\n", curLog.timeStamp, curLog.str) ;
            if (curLog.timeStamp <= targTimeStamp) break ;
            op.clear(); op = std::string (curLog.str) ;
            // if (targTimeStamp == 5900765) std::cout << op << "\n" ;
            isRollback = true; run () ;
            curFilePos = curLog.preFilePos ;
        }
        update_last_log_pos (curFilePos) ;
        curUsers.clear() ;
        isRollback = false ;

        printf("0\n") ;
    }

    void clean () {
        users.clear() ;
        curUsers.clear() ;
        trains.clear() ;
        trainStations.clear() ;
        orders.clear() ;
        pendingOrders.clear() ;

        printf("0\n") ;
    }
} ;

#endif