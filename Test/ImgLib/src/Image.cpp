//
// Created by Administrator on 2023/7/5.
//
#include "Image.h"
#include<iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

GrayImage::GrayImage() : Mat_<BYTE>() {
}

GrayImage::GrayImage(size_t Width, size_t Height) : Mat_<BYTE>(Width, Width, Height, 1) {
}

GrayImage::GrayImage(size_t Width, size_t Stride, size_t Height) : Mat_(Width, Stride, Height, 1) {

}

GrayImage::GrayImage(const GrayImage &obj) {
    ClearBuffer();
    int res = CreateBuffer(obj.GetWidth(), obj.GetStride(), obj.GetHeight(), obj.GetDim());
    memcpy(GetPtr(), obj.GetPtr(), obj.GetBufferSizeByte());
}

GrayImage &GrayImage::operator=(const GrayImage &obj) {
    if (this == &obj) {
        return *this;
    }
    GrayImage m(obj);
    memcpy(GetPtr(), m.GetPtr(), m.GetBufferSizeByte());
    return *this;
}

YUV420Image::YUV420Image() : Mat_<BYTE>() {

}

YUV420Image::YUV420Image(size_t Width, size_t Height) : Mat_<BYTE>(Width, Height, 2) {

}

YUV420Image::YUV420Image(size_t Width, size_t Stride, size_t Height) : Mat_(Width, Stride, Height, 3) {

}

YUV420Image::YUV420Image(const YUV420Image &obj) {
    ClearBuffer();
    int res = CreateBuffer(obj.GetWidth(), obj.GetStride(), obj.GetHeight(), obj.GetDim());
    memcpy(GetPtr(), obj.GetPtr(), obj.GetBufferSizeByte());
}

YUV420Image &YUV420Image::operator=(const YUV420Image &obj) {
    if (this == &obj) {
        return *this;
    }
    YUV420Image m(obj);
    memcpy(GetPtr(), m.GetPtr(), m.GetBufferSizeByte());
    return *this;
}

BYTE *YUV420Image::GetUVPtr() {
    return GetPtr() + GetStride() * GetHeight();
}


RGBImage::RGBImage() : Mat_<BYTE>() {

}

RGBImage::RGBImage(size_t Width, size_t Height) : Mat_<BYTE>(Width, Height, 3) {

}

RGBImage::RGBImage(const RGBImage &obj) {
    ClearBuffer();
    int res = CreateBuffer(obj.GetWidth(), obj.GetStride(), obj.GetHeight(), obj.GetDim());
    memcpy(GetPtr(), obj.GetPtr(), obj.GetBufferSizeByte());
}

RGBImage &RGBImage::operator=(const RGBImage &obj) {
    if (this == &obj) {
        return *this;
    }
    int res = CreateBuffer(obj.GetWidth(), obj.GetStride(), obj.GetHeight(), obj.GetDim());
    if (res) {
        memcpy(GetPtr(), obj.GetPtr(), obj.GetBufferSizeByte());
    }
    return *this;
}

RGBImage::RGBImage(size_t Width, size_t Stride, size_t Height) : Mat_(Width, Stride, Height, 3) {

}

void RGBImage::LoadImage(const char *filename) {
    int iw, ih, n;
// 加载图片获取宽、高、颜色通道信息
    unsigned char *idata = stbi_load(filename, &iw, &ih, &n, 0);
    CreateBuffer(iw, iw, ih, n);
    memcpy(GetPtr(), idata, GetBufferSizeByte());
    stbi_image_free(idata);
    idata = nullptr;
}

void RGBImage::SaveImage(const char *filename) {
    std::string str_file(filename);
    char path[1024];
    size_t pos = str_file.find(".");
    std::string sufix = std::string(str_file, 0, pos);
    std::string postfix = std::string(str_file, pos, str_file.size() - pos);
    sprintf(path, "%s_%dx%dx%d_%d.%s", str_file.c_str(),
            GetWidth(), GetStride(), GetHeight(), GetDim(),
            postfix.c_str());
    stbi_write_bmp(path, GetWidth(), GetHeight(), GetDim(), GetPtr());
}

RGBImageA::RGBImageA() : Mat_<BYTE>() {

}

RGBImageA::RGBImageA(size_t Width, size_t Height) : Mat_<BYTE>(Width, Width, Height, 4) {

}

RGBImageA::RGBImageA(size_t Width, size_t Stride, size_t Height) : Mat_(Width, Stride, Height, 4) {

}

RGBImageA::RGBImageA(const RGBImageA &obj) {
    ClearBuffer();
    int res = CreateBuffer(obj.GetWidth(), obj.GetStride(), obj.GetHeight(), obj.GetDim());
    memcpy(GetPtr(), obj.GetPtr(), obj.GetStride() * obj.GetHeight() * obj.GetDim() * sizeof(BYTE));
}

RGBImageA &RGBImageA::operator=(const RGBImageA &obj) {
    if (this == &obj) {
        return *this;
    }
    int res = CreateBuffer(obj.GetWidth(), obj.GetStride(), obj.GetHeight(), obj.GetDim());
    if (res) {
        memcpy(GetPtr(), obj.GetPtr(), obj.GetBufferSizeByte());
    }
    return *this;
}

void RGBImageA::SaveImage(const char *filename) {
    int iw, ih, n;
// 加载图片获取宽、高、颜色通道信息
    unsigned char *idata = stbi_load(filename, &iw, &ih, &n, 0);
    CreateBuffer(iw, iw, ih, n);
    memcpy(GetPtr(), idata, GetBufferSizeByte());
    stbi_image_free(idata);
    idata = nullptr;
}

void RGBImageA::LoadImage(const char *filename) {
    std::string str_file(filename);
    char path[1024];
    size_t pos = str_file.find(".");
    std::string sufix = std::string(str_file, 0, pos);
    std::string postfix = std::string(str_file, pos, str_file.size() - pos);
    sprintf(path, "%s_%dx%dx%d_%d.%s", str_file.c_str(),
            GetWidth(), GetStride(), GetHeight(), GetDim(),
            postfix.c_str());
    stbi_write_bmp(path, GetWidth(), GetHeight(), GetDim(), GetPtr());
}


