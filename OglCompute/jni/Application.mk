# Build both ARMv5TE and ARMv7-A machine code.
NDK_TOOLCHAIN_VERSION := 4.9
APP_PLATFORM := android-24
APP_ABI    := armeabi-v7a
APP_ABI    += arm64-v8a
APP_MODULES:= OglCompute
APP_OPTIM  := release  
APP_STL    := c++_static
#APP_STL    := c++_shared
OPT_CFLAGS := -O2 -fno-exceptions -fno-bounds-check -funroll-loops -funswitch-loops -fvisibility=hidden \
              -finline-functions -funsafe-loop-optimizations
              
OPT_CPPFLAGS := $(OPT_CLFAGS)  
  
APP_CFLAGS  := $(APP_CFLAGS) $(OPT_CFLAGS) -frtti -std=c++11  
APP_CPPFLAGS := $(APP_CPPFLAGS) $(OPT_CPPFLAGS) 
