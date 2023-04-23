#include <chrono>
#if defined(WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <map>
#include <vector>
#include <stack>
#include <string>
#include <string.h>
using namespace std;
class TimerBase
{
public:
    virtual void start() =0;
    virtual void stop() =0;
    double microseconds()
    {
        return mMs * 1000.0;
    }
    double milliseconds()
    {
        return mMs;
    }
    double seconds()
    {
        return mMs / 1000.0;
    }
    void reset()
    {
        mMs = 0;
    }
    virtual double getCurrentTime()
    {
        return 0;
    }

protected:
    double mMs = 0;
};

class CpuTimer : public TimerBase
{
public:
    void start()
    {
        mStart = getCurrentTime();
    }
    void stop()
    {
        mStop = getCurrentTime();
        mMs = (mStop - mStart);
    }
    double getCurrentTime()
    {
        double tCurrent = 0.0;
#ifndef WIN32
        struct timeval start;
        gettimeofday(&start, NULL);
        tCurrent = start.tv_sec * 1000.0 + start.tv_usec / 1000.0;
#else
        SYSTEMTIME systm;
        GetLocalTime(&systm);
        tCurrent = systm.wSecond * 1000.0 + systm.wMilliseconds;
#endif
        return tCurrent;
    }

private:
    double mStart, mStop;
};

class VipTime
{
public:
    void Start()
    {
        if (!bForbid)
        {
            if(bFirstStart)
            {
                initTimeStamp = timeCpu.getCurrentTime();
                bFirstStart = false;
            }
            _time_stack.push(timeCpu.getCurrentTime());
           
        }
    }

    void Stop(string tag)
    {
        if (!bForbid)
        {
            double during = timeCpu.getCurrentTime() - _time_stack.top();
            std::map<string, double>::iterator it;
            it = mapDuring.find(tag);
            if (it == mapDuring.end())
            {
                mapDuring[tag] = during;
            }
            else
            {
                mapDuring[tag] += during;
            }
            if (!_time_stack.empty())
                _time_stack.pop();
            stopTimeDuring = timeCpu.getCurrentTime() - initTimeStamp;
        }
    }
    void Print()
    {
        if (!bForbid)
        {
            cnt++;
            timeTotal = stopTimeDuring;
            printf(" \n");
            LOGI(" ");
            printf("\t==== total [level %d] %8.3lf ms profile count:%4ld     \t\t  ms           %%\n", Timelevel,timeTotal, cnt);
            LOGI("\t==== total [level %d] %8.3lf ms profile count:%4ld     \t\t  ms           %%\n", Timelevel,timeTotal, cnt);
            std::map<string, double>::iterator it;

            if (bHaveChild)
            {
                for(int i=0;i<vecVipTime.size();i++)
                {
                    vecVipTime[i]->Timelevel= Timelevel+1;
                    vecVipTime[i]->toTag = "[child "+std::to_string(i)+"]"+" level "+std::to_string(vecVipTime[i]->Timelevel);
                    mapDuring[vecVipTime[i]->toTag] = vecVipTime[i]->stopTimeDuring;
                    Timelevel++;
                }
            }

            for (it = mapDuring.begin(); it != mapDuring.end(); it++)
            {
                string tag = it->first;
                double time = it->second;
                printf("\t%-32s\t%8.3lf \t%6.2f %% \t\n", tag.c_str(), time, ((time * 100.0f) / (timeTotal)));
                LOGI("\t%-32s\t%8.3lf \t%6.2f %% \t", tag.c_str(), time, ((time * 100.0f) / (timeTotal)));
            }
            if (bHaveChild)
            {
                for(int i=0;i<vecVipTime.size();i++)
                {
                    vecVipTime[i]->Print();   
                }
            }
            clean();
        }
    }
    void AddChild(VipTime *_childVip)
    {
        vecVipTime.push_back(_childVip);
        bHaveChild = true;
    }
    void SetForbid(bool flg)
    {
        bForbid = flg;
    }
    void setTotalTag(std::string tag)
    {
        toTag = tag;
    }
    void clean()
    {
        std::map<std::string, double> empty_map1;
        std::stack<double> _emptr_stack;
        mapDuring.swap(empty_map1);
        mapDuring.clear();
        _time_stack.swap(_emptr_stack);
        _time_stack.empty();
        bFirstStart = true;
        Timelevel = 0;
        vecVipTime.clear();
        bHaveChild = false;
    }
    VipTime()
    {
    }
    VipTime(char* str, bool flag)
    {
    }
    ~VipTime()
    {
        std::map<std::string, double> empty_map1;
        std::stack<double> _emptr_stack;
        mapDuring.swap(empty_map1);
        mapDuring.clear();
        _time_stack.swap(_emptr_stack);
        _time_stack.empty();
        bFirstStart = true;
        vecVipTime.clear();
    }

protected:
    std::map<string, double> mapDuring;
    CpuTimer timeCpu;
    double timeTotal = 1.0;
    long cnt = 0;
    bool bForbid = true;
    std::stack<double> _time_stack;
    std::string toTag;
    bool bHaveChild = false;
    bool bFirstStart = true;
    int Timelevel = 0;
    double initTimeStamp=0;
    double stopTimeDuring = 0;
    std::vector<VipTime *> vecVipTime;
};
