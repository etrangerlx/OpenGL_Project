//
// Created by Administrator on 2023/10/26.
//

#include "ImageUtils.h"

bool ImageUtils::nv12toyuv(YUV420Image *pInImage, YUVImage *pOutImage) {
//    libyuv::ScalePlane(pInImage->GetPtr(), pInImage->GetStride(), pInImage->GetWidth(), pInImage->GetHeight(),
//                       pOutImage->GetPtr(), pOutImage->GetStride(), pOutImage->GetStride(), pOutImage->GetHeight(),
//                       libyuv::kFilterLinear);
    YUV420Image tmp(*pInImage);
    libyuv::NV12ToI420(pInImage->GetPtr(), pInImage->GetStride(), pInImage->GetUVPtr(), pInImage->GetStride(),
                       tmp.GetPtr(), tmp.GetStride(),
                       tmp.GetUVPtr(), tmp.GetStride() >> 1,
                       tmp.GetUVPtr() + (tmp.GetStride() * tmp.GetHeight() >> 2), tmp.GetStride() >> 1,
                       tmp.GetWidth(), tmp.GetHeight());
    tmp.SaveBuffer("tmp.yuv");

//    libyuv::ScalePlane(tmp.GetPtr(), tmp.GetStride(), tmp.GetWidth(), tmp.GetHeight(),
//                       pOutImage->GetPtr(), pOutImage->GetStride(), pOutImage->GetWidth(), pOutImage->GetHeight(),
//                       libyuv::kFilterBilinear);
//    libyuv::ScalePlane(tmp.GetUVPtr(), tmp.GetStride()>>1, tmp.GetWidth()>>1, tmp.GetHeight() >> 1,
//                       pOutImage->GetPtr() +pOutImage->GetHeight() * pOutImage->GetStride(), pOutImage->GetStride(), pOutImage->GetWidth(),
//                       pOutImage->GetHeight(),
//                       libyuv::kFilterBilinear);
    libyuv::I420ToI444(tmp.GetPtr(), tmp.GetStride(),
                       tmp.GetUVPtr(), tmp.GetStride() >> 1,
                       tmp.GetUVPtr() + (tmp.GetStride() * tmp.GetHeight() >> 2), tmp.GetStride() >> 1,
                       pOutImage->GetPtr(), pOutImage->GetStride(),
                       pOutImage->GetPtr() +pOutImage->GetHeight() * pOutImage->GetStride(), pOutImage->GetStride(),
                       pOutImage->GetPtr() +pOutImage->GetHeight() * pOutImage->GetStride() * 2, pOutImage->GetStride(),
                       pOutImage->GetWidth(), pOutImage->GetHeight());
    return false;
}
