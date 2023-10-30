//
// Created by Administrator on 2023/10/25.
//

#include "utils.h"

int Logger::loglevel = 0;

Logger &Logger::GetInstance() {
    static Logger instance;
    return instance;
}

void Logger::SetLogLevel(int level) {
#ifdef ANDROID
#include <sys/system_properties.h>
    char prop[PROP_VALUE_MAX];
    memset(prop,'\0',PROP_VALUE_MAX);
    __system_property_get("imagelib.propty.log",prop);
    loglevel = prop - 0x31;
#else
    loglevel = 3;
#endif
}

int Logger::GetLogLevel(int level) {
    return loglevel;
}

void Logger::I(const char *fmt, ...) {

}

void Logger::E(const char *fmt, ...) {

}


double TimeStatic::start_time_ = 0.0;
double TimeStatic::end_time_ = 0.0;


double TimeStatic::getCurrentTime() {
#ifdef _WIN32
    LARGE_INTEGER freq;
    LARGE_INTEGER pc;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&pc);
    return pc.QuadPart * 1000.0 / freq.QuadPart;
#else
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
#endif
}



TimeStatic &TimeStatic::GetInstance() {
    static TimeStatic demo;
    return demo;
}

void TimeStatic::Start() {
    GetInstance();
    start_time_ = getCurrentTime();
    end_time_ = start_time_;
}

void TimeStatic::Stop() {
    GetInstance();
    end_time_ = getCurrentTime();
}

float TimeStatic::TimeCost() {
    GetInstance();
    if (end_time_ <= start_time_) {
        Stop();
    }
    return (float) (end_time_ - start_time_);
}
