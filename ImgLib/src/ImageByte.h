//
// Created by Administrator on 2023/7/5.
//

#ifndef OPENGL_TEST_IMAGEBYTE_H
#define OPENGL_TEST_IMAGEBYTE_H


#include "BasicImageArray.h"

typedef enum {
    IMAGE_TYPE_NONE,
    IMAGE_TYPE_GRAY,
    IMAGE_TYPE_NV12,
    IMAGE_TYPE_NV21,
    IMAGE_TYPE_YUV,
    IMAGE_TYPE_RGB,

} IMAGE_TYPE;

class IMAGEBYTE : public CBasicImageArray_BYTE {
protected:
    BYTE *planptr[4];
    int Stride[4];
    IMAGE_TYPE m_type;
public:
    IMAGEBYTE();
};

class CGrayImage : public IMAGEBYTE {
public:
    CGrayImage();

    bool Create(int Width, int Height);

    bool SaveFile(const char *filename);

    CGrayImage(const CGrayImage &obj);

    CGrayImage &operator=(const CGrayImage &obj);

};


#endif //OPENGL_TEST_IMAGEBYTE_H
