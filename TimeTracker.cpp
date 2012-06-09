#include "TimeTracker.h"

Tracker::Tracker(long long sp)
{
    speed = sp;
}
void Tracker::start(string param)
{
    if(present(status,param))
    {
        if(!status[param]) // if the status is in end state, which is false itself
        {
            status[param] = true;
        }
    }
    else
    {
        status[param] = true; // means that the timer is running
    }
    times[param].push_back(rdtsc()); // push the current timer to the end
}
void Tracker::end(string param)
{
    if(present(status,param))
    {
        if(status[param]) // only if the timer is running
        {
            times[param].push_back(rdtsc()); // push the current timer to the end
            status[param] = false;
        }
    }
    else
    {
        std::cerr << " Timer not started "<<std::endl;
    }
}
void Tracker::print(string param)
{
    // if the timer is not in end state then end the timer and continue
    if(!status[param])
        end(param);
    // now add the successive times
    long long cumulative = 0;
    long long start,end;
    if(present(times,param))
    {
        tr(times[param],it)
        {
            start = *it;
            it++;
            end = *it;
            cumulative += (end-start);
        }
        std::cout<<param<<":"<<cumulative/(1.0*speed)<<std::endl;
    }
    else
    {
        std::cerr << " parameter not found "<<std::endl;
    }
}
void Tracker::print_all()
{
    tr(times,it)
    {
        print(it->first);
    }
}
inline unsigned long long Tracker::rdtsc(void)
{
	unsigned long long result=0;
	unsigned a, d;

	do {
		__asm__ __volatile__("rdtsc" : "=a" (a), "=d" (d));
		result = ((unsigned long long)a) | (((unsigned long long)d) << 32);
	} while (__builtin_expect ((int) result == -1, 0));
	return result;
}
