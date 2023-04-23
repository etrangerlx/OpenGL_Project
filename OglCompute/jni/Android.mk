# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)
SOLUTION_PATH := $(LOCAL_PATH)/../../

include $(CLEAR_VARS)
LOCAL_MODULE     := OglCompute
EXCLUDE_FILES    := 
MY_PREFIX        := $(LOCAL_PATH)
ALL_SOURCES      := $(wildcard $(MY_PREFIX)/../*.cpp)
EXCLUDE_SRCS     := $(foreach filex, $(EXCLUDE_FILES), $(MY_PREFIX)/../$(filex))
MY_SOURCES       := $(filter-out $(EXCLUDE_SRCS), $(ALL_SOURCES))  
LOCAL_SRC_FILES  := $(MY_SOURCES:$(MY_PREFIX)/%=%)
LOCAL_CFLAGS     := -Wno-write-strings -fopenmp -DOpenGLES
LOCAL_CXXFLAGS   := -fopenmp
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS    += -D__ARM__ -mfloat-abi=softfp -mfpu=neon 
LOCAL_CFLAGS    += -march=armv7-a -mtune=cortex-a8
LOCAL_ARM_MODE := arm  
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_CFLAGS    += -D__ARM__ -march=armv8-a
else ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS    += -D__X86__
else ifeq ($(TARGET_ARCH_ABI),x86_64)
LOCAL_CFLAGS    += -D__X86__
endif
LOCAL_C_INCLUDES := $(SOLUTION_PATH)/OglCompute
LOCAL_C_INCLUDES += $(SOLUTION_PATH)/3rdparty 
LOCAL_LDLIBS += -llog -fopenmp -static-openmp -lEGL -lGLESv3 -lnativewindow
LOCAL_CFLAGS +=  -DOpenGLES -DANDROID -fno-bounds-check -funroll-loops -funswitch-loops -fvisibility=hidden
include $(BUILD_STATIC_LIBRARY)