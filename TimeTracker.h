 
#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include "Globals.h"
#include <string>
class Tracker
{
    private:
    // msb  map<string, bool>
        msb status; // key is the string and the value is whether the timer is on
        map<string,vector<long long> > times; // vector of start and end times
        long long speed;
    public:
        void start(string param);
        void end(string param);
        void print(string param);
        void print_all();
        unsigned long long rdtsc(void);
        Tracker(long long sp);
};
#endif
