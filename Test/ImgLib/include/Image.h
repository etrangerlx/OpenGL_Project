//
// Created by Administrator on 2023/7/5.
//

#ifndef OPENGL_TEST_IMAGEBYTE_H
#define OPENGL_TEST_IMAGEBYTE_H

#include "SimpleType.h"

class GrayImage : public Mat_<BYTE> {
public:
    GrayImage();

    GrayImage(size_t Width, size_t Height);

    GrayImage(size_t Width, size_t Stride, size_t Height);

    explicit GrayImage(const GrayImage &obj);

    GrayImage &operator=(const GrayImage &obj);
};

class YUV420Image : public Mat_<BYTE> {
public:
    YUV420Image();

    YUV420Image(size_t Width, size_t Height);

    YUV420Image(size_t Width, size_t Stride, size_t Height);

    explicit YUV420Image(const YUV420Image &obj);

    YUV420Image &operator=(const YUV420Image &obj);

    BYTE *GetUVPtr();
};

class RGBImage : public Mat_<BYTE> {
public:
    RGBImage();

    RGBImage(size_t Width, size_t Height);

    RGBImage(size_t Width, size_t Stride, size_t Height);

    explicit RGBImage(const RGBImage &obj);

    RGBImage &operator=(const RGBImage &obj);

    void SaveImage(const char *filename);

    void LoadImage(const char *filename);

};

typedef RGBImage YUVImage;

class RGBImageA : public Mat_<BYTE> {
public:
    RGBImageA();

    RGBImageA(size_t Width, size_t Height);

    RGBImageA(size_t Width, size_t Stride, size_t Height);

    explicit RGBImageA(const RGBImageA &obj);

    RGBImageA &operator=(const RGBImageA &obj);

    void SaveImage(const char *filename);

    void LoadImage(const char *filename);

};


#endif //OPENGL_TEST_IMAGEBYTE_H
