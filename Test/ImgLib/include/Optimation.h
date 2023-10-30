//
// Created by Administrator on 2023/8/17.
//

#ifndef IMGLIBTEST_OPTIMATION_H
#define IMGLIBTEST_OPTIMATION_H
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility("default")))
#define DLL_LOCAL  __attribute__ ((visibility("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif

#include <iostream>
#include <memory>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <omp.h>

#define USE_SIMD

#ifdef USE_SIMD

#ifdef __x86_64__

#include <NEON_2_SSE.h>

#elif __aarch64__

#include <neon_arm.h>

#endif

#endif

#endif //IMGLIBTEST_OPTIMATION_H
