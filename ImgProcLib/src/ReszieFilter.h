//
// Created by Administrator on 2023/7/8.
//

#ifndef IMGLIB_RESZIEFILTER_H
#define IMGLIB_RESZIEFILTER_H

#include "../../ImgLib/src/ImageByte.h"

class ReszieFilter {
private:
    bool HUpScaleImage_4x(BYTE *pInLine, BYTE *pOutLine, int nInWidth, int nOutWidth);

    bool VUpScaleImage_4x(BYTE *pInLine[], BYTE *pOutLine1, BYTE *pOutLine2, BYTE *pOutLine3, BYTE *pOutLine4, int Width);

protected:

public:
    ReszieFilter();

    ~ReszieFilter();

    bool DownScaleImage_4x(CGrayImage *pInImge, CGrayImage *pOutImage, int nInWidth, int nInHeight, int nInStride = 0);

    bool UpScaleImage_4x(CGrayImage *pInImge, CGrayImage *pOutImage, int nInWidth, int nInHeight, int nInStride = 0);

    bool Process(CGrayImage *pInImge, CGrayImage *pOutImage);

};


#endif //IMGLIB_RESZIEFILTER_H
