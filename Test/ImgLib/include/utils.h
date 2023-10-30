//
// Created by Administrator on 2023/10/25.
//

#ifndef IMGLIB_UTILS_H
#define IMGLIB_UTILS_H
#define  LOG_TAG "ImageBasic"

#ifdef ANDROID
#include<android/log.h>

#define  LOGE_(format,...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,"[ERROR][%s][%s]" format , ##__VA_ARGS__)
#define  LOGI_(format,...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG,"[INFO][%s][%s]" format ,  ##__VA_ARGS__)
#else
#define  LOGE_(format, ...)  printf("[ERROR][%s][%s]" format,LOG_TAG,##__VA_ARGS__)
#define  LOGI_(format, ...)  printf("[INFO][%s][%s]" format, LOG_TAG,##__VA_ARGS__)
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

class Logger {
private:
    static int loglevel;

    Logger() {

    }

    Logger(const Logger &obj);

    Logger &operator=(const Logger &obj);

public:
    static Logger &GetInstance();

    ~Logger() {

    }

    static void SetLogLevel(int level);

    static int GetLogLevel(int level);

    static void I(const char *fmt, ...);

    static void E(const char *fmt, ...);

};

#ifdef ANDROID
#include<android/log.h>

#define  LOGE(format,...)  { \
        if(Logger::GetLogLevel()>=1) { \
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,"[ERROR][%s][%s]" format , ##__VA_ARGS__) \
        }\
    }

#define  LOGI(format,...) { \
        if(Logger::GetLogLevel()>=2) { \
            __android_log_print(ANDROID_LOG_INFO,  LOG_TAG,"[INFO][%s][%s]" format ,  ##__VA_ARGS__) \
        } \
    }

#else

#define  LOGE(format, ...)  { \
        if(Logger::GetLogLevel()>=1) { \
            printf("[ERROR][%s][%s]" format,LOG_TAG,##__VA_ARGS__); printf("\n"); \
        } \
    }

#define  LOGI(format, ...) { \
        if(Logger::GetLogLevel()>=2) { \
            printf("[INFO][%s][%s]" format, LOG_TAG,##__VA_ARGS__); printf("\n");\
        } \
    }
#endif


class TimeStatic {

private:
    TimeStatic() {
        start_time_ = 0;
        end_time_ = 0;
    }

    static double start_time_, end_time_;

    static double getCurrentTime();

public:
    static TimeStatic &GetInstance();

    static void Start();

    static void Stop();

    static float TimeCost();
};

#endif //IMGLIB_UTILS_H
