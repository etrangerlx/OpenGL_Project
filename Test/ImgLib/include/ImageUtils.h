//
// Created by Administrator on 2023/10/26.
//

#ifndef IMGLIB_IMAGEUTILS_H
#define IMGLIB_IMAGEUTILS_H

#include "Image.h"
#include <libyuv/convert.h>

class ImageUtils {
public:
    bool nv2gray(YUV420Image *pInImage, GrayImage *pOutImage);

    //// NV to rgb &yuv
    bool nv12torgb(YUV420Image *pInImage, RGBImage *pOutImage);

    bool nv21torgb(YUV420Image *pInImage, RGBImage *pOutImage);

    bool rgb2nv12(RGBImage *pInImage, YUV420Image *pOutImage);

    bool rgb2nv21(RGBImage *pInImage, YUV420Image *pOutImage);


    bool nv12toyuv(YUV420Image *pInImage, YUVImage *pOutImage);

    bool nv21toyuv(YUV420Image *pInImage, YUVImage *pOutImage);

    //// ResizeFilter
    bool ReSize(GrayImage *pInImage, GrayImage *pOutImage);

    bool ReSize(YUV420Image *pInImage, YUV420Image *pOutImage);

    bool ReSize(RGBImage *pInImage, RGBImage *pOutImage);

    bool ReSize(RGBImageA *pInImage, RGBImageA *pOutImage);

    //// Crop
    bool Crop(GrayImage *pInImage, Rect inRect, GrayImage *pOutImage);

    bool Crop(GrayImage *pInImage, Rect inRect, GrayImage *pOutImage, Rect outRect);

    bool Crop(YUV420Image *pInImage, Rect inRect, YUV420Image *pOutImage);

    bool Crop(YUV420Image *pInImage, Rect inRect, YUV420Image *pOutImage, Rect outRect);

private:
};


#endif //IMGLIB_IMAGEUTILS_H
