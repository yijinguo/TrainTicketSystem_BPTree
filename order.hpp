#ifndef TrainSystem_order
#define TrainSystem_order

#include <iostream>

#include "string.h"

enum Status {success, pending, refunded} ;
static const char status_str[3][20] = {"[success]", "[pending]", "[refunded]"} ;

class order {

    friend std::ostream& operator << (std::ostream &out, const order &_order) ;

private:
    String trainID, fromStation, toStation ;
    Time leavingTime, arrivingTime ;
    int price, seat, travellingTime ;
    Status status ;

public:
    order () {}
    order (const String &_trainID, const String &_fromStation, const String &_toStation, const Time &_leavingTime, const Time &_arrivingTime, const int _price, const int _seat, const int _travellingTime) {
        trainID = _trainID ;
        fromStation = _fromStation; toStation = _toStation ;
        leavingTime = _leavingTime; arrivingTime = _arrivingTime ;
        price = _price; seat = _seat; travellingTime = _travellingTime ;
    }
    order (const order &_order) {
        trainID = _order.trainID; fromStation = _order.fromStation; toStation = _order.toStation ;
        leavingTime = _order.leavingTime; arrivingTime = _order.arrivingTime ;
        price = _order.price; seat = _order.seat; travellingTime = _order.travellingTime ;
        status = _order.status ;
    }

    order& operator = (const order &_order) {
        if (this == &_order) return *this ;
        trainID = _order.trainID; fromStation = _order.fromStation; toStation = _order.toStation ;
        leavingTime = _order.leavingTime; arrivingTime = _order.arrivingTime ;
        price = _order.price; seat = _order.seat; travellingTime = _order.travellingTime ;
        status = _order.status ;
        return *this ;
    }

    void print () {
        std::cout << trainID << " " << fromStation << " " << leavingTime << " -> " << toStation << " " << arrivingTime << " " << price << " " << seat << std::endl ;
    }

    String& getTrainID () {
        return trainID ;
    }

    Status getStatus () const {
        return status ;
    }

    Time getLeavingTime () const {
        return leavingTime ;
    }

    String& getFromStation () {
        return fromStation ;
    }

    String& getToStation () {
        return toStation ;
    }

    int getTravellingTime () const {
        return travellingTime ;
    }

    int getPrice () const {
        return price ;
    }

    int getSeatNum () const {
        return seat ;
    }

    void setStatus (Status _status) {
        status = _status ;
    }
} ;

std::ostream& operator << (std::ostream &out, const order &_order) {
    out << status_str[_order.status] << " " << _order.trainID << " " << _order.fromStation << " " << _order.leavingTime << " -> " << _order.toStation << " " << _order.arrivingTime << " " << _order.price << " " << _order.seat ;
    return out ;
}

#endif