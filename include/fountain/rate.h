#ifndef __RATE_H_
#define __RATE_H_

#include <sys/time.h>
#include <thread>

namespace ftn {

class Rate
{
    std::uint64_t base_time;
    std::uint64_t counter;
    struct timeval tv_start, tv_end;
public:
    Rate(double hz) : counter(0)
    {
        base_time = 1000000000 / hz;
    }

    bool sleep()
    {
        bool rlt = false;
        
        if (counter > 0)
        {
            gettimeofday(&tv_end, NULL);
            std::uint64_t time_left =  base_time * (counter + 1) - ((tv_end.tv_sec - tv_start.tv_sec) * 1000000 + (tv_end.tv_usec - tv_start.tv_usec)) * 1000;
            if (time_left > 0)
            {
                std::this_thread::sleep_for(std::chrono::nanoseconds(time_left));
                rlt = true;
            }
            else
            {
                std::this_thread::yield();
            }

            if (counter > 1000 && time_left > 0)
            {
                auto at = base_time * counter;
                auto ts = at / 1000000000;
                auto tus = (at - ts * 1000000000) / 1000;
                tv_start.tv_sec += ts;
                tv_start.tv_usec += tus;
                counter = 1;
            }
            else
            {
                ++counter;
            }
        }
        else
        {
            gettimeofday(&tv_start, NULL);
            std::this_thread::sleep_for(std::chrono::nanoseconds(base_time));
            rlt = true;
            ++counter;
        }
        
        
        return rlt;
    }

};
}



#endif