//
// Created by Administrator on 2023/7/8.
//

#include "ReszieFilter.h"

ReszieFilter::ReszieFilter() {

}

ReszieFilter::~ReszieFilter() {

}

bool ReszieFilter::DownScaleImage_4x(CGrayImage *pInImge, CGrayImage *pOutImage, int nInWidth, int nInHeight,
                                     int nInStride) {
    int nOutWidth = (nInWidth + 3) >> 2;
    int nOutHeight = (nInHeight + 3) >> 2;
    if (nInStride == 0) {
        nInStride = nInWidth;
    }
    if (!pOutImage->Create(nOutWidth, nOutHeight)) {
        return false;
    }
    BYTE *pInBuf = pInImge->GetImageData();
    int y = 0;
    for (; y < nOutHeight; y++) {
        BYTE *pIn[4];
        BYTE *pOut = (BYTE *) pOutImage->GetImageData() + y * nOutWidth;
        int LineIndex = ((y + 1) << 2) < nInHeight - 1 ? (y + 1) << 2 : nInHeight - 1;
        pIn[0] = pInBuf + (LineIndex - 4) * nInStride;
        pIn[1] = pInBuf + (LineIndex - 3) * nInStride;
        pIn[2] = pInBuf + (LineIndex - 2) * nInStride;
        pIn[3] = pInBuf + (LineIndex - 1) * nInStride;
        int x = 0;
        for (; x < nOutWidth; x++) {
            int Y = 0;
            int RowIndex = ((x + 1) << 2) < nInWidth - 1 ? (x + 1) << 2 : nInWidth - 1;
            for (int i = 0; i < 4; i++) {
                Y += pIn[0][RowIndex - 4];
                Y += pIn[1][RowIndex - 3];
                Y += pIn[2][RowIndex - 2];
                Y += pIn[3][RowIndex - 1];
            }
            Y >>= 4;
            *(pOut + x) = (BYTE) Y;
        }
    }

    return true;
}

bool
ReszieFilter::UpScaleImage_4x(CGrayImage *pInImge, CGrayImage *pOutImage, int nInWidth, int nInHeight, int nInStride) {
//    int nProcs = omp_get_num_procs();
    int nProcs = 1;
    int nOutWidth = pOutImage->GetWidth();
    BYTE *pBuffer = new BYTE[3 * nOutWidth * nProcs];
    if (pBuffer == nullptr) {
        return false;
    }
    int x = 0;
    BYTE *pInLine[3];
    BYTE *pOutLine[3];
//#pragma omp parallel for schedule(dynamic,8)
    for (; x < nInHeight; x++) {
//        int nThreadid = omp_get_thread_num();
        int nThreadid = 0;
        int start = (x == 0) ? 1 : 0;
        int end = (x == nInHeight - 1) ? -1 : 0;
        pInLine[0] = pInImge->GetImageData() + (x - 1 + start) * nInWidth;
        pInLine[1] = pInImge->GetImageData() + (x) * nInWidth;
        pInLine[2] = pInImge->GetImageData() + (x + 1 + end) * nInWidth;
        pOutLine[0] = pBuffer + (nThreadid + 0) * nOutWidth;
        pOutLine[1] = pBuffer + (nThreadid + 1) * nOutWidth;
        pOutLine[2] = pBuffer + (nThreadid + 2) * nOutWidth;
        HUpScaleImage_4x(pInLine[0], pOutLine[0], nInWidth, nOutWidth);
        HUpScaleImage_4x(pInLine[1], pOutLine[1], nInWidth, nOutWidth);
        HUpScaleImage_4x(pInLine[2], pOutLine[2], nInWidth, nOutWidth);
        BYTE *pOutLine1 = pOutImage->GetImageData() + (x * 4 + 0) * nOutWidth;
        BYTE *pOutLine2 = pOutImage->GetImageData() + (x * 4 + 1) * nOutWidth;
        BYTE *pOutLine3 = pOutImage->GetImageData() + (x * 4 + 2) * nOutWidth;
        BYTE *pOutLine4 = pOutImage->GetImageData() + (x * 4 + 3) * nOutWidth;
        VUpScaleImage_4x(pOutLine, pOutLine1, pOutLine2, pOutLine3, pOutLine4, nOutWidth);
    }
    return true;
}

bool ReszieFilter::HUpScaleImage_4x(BYTE *pInLine, BYTE *pOutLine, int nInWidth, int nOutWidth) {
    if (nOutWidth == 0) {
        nOutWidth = nInWidth << 2;
    } else {
        if (nOutWidth != nInWidth << 2) {
            return false;
        }
    }
    int Y0, Y1, Y2, Y3;
    BYTE *pIn[3];
    int x = 0;
    for (; x < nInWidth; x++) {
        int start = (x == 0) ? 1 : 0;
        int end = (x == nInWidth - 1) ? -1 : 0;
        pIn[0] = pInLine + x - 1 + start;
        pIn[1] = pInLine + x;
        pIn[2] = pInLine + x + 1 + end;
        Y0 = (*pIn[0] * 3 + *pIn[1] * 5) / 8;
        Y1 = (*pIn[0] * 1 + *pIn[1] * 7) / 8;
        Y2 = (*pIn[1] * 7 + *pIn[2] * 1) / 8;
        Y3 = (*pIn[1] * 5 + *pIn[2] * 3) / 8;
        *(pOutLine++) = (BYTE) Y0;
        *(pOutLine++) = (BYTE) Y1;
        *(pOutLine++) = (BYTE) Y2;
        *(pOutLine++) = (BYTE) Y3;
    }
    return true;
}

bool ReszieFilter::VUpScaleImage_4x(BYTE *pInLine[], BYTE *pOutLine1, BYTE *pOutLine2, BYTE *pOutLine3, BYTE *pOutLine4,
                                    int Width) {
    int x, Y0, Y1, Y2, Y3, Y4, Y5, Y6;
    x = 0;
    for (; x < Width; x++) {
        Y0 = pInLine[0][x];
        Y1 = pInLine[1][x];
        Y2 = pInLine[2][x];
        Y3 = (Y0 * 3 + Y1 * 5) / 8;
        Y4 = (Y0 * 1 + Y1 * 7) / 8;
        Y5 = (Y1 * 7 + Y2 * 1) / 8;
        Y6 = (Y1 * 5 + Y2 * 3) / 8;
        pOutLine1[x] = (BYTE) Y3;
        pOutLine2[x] = (BYTE) Y4;
        pOutLine3[x] = (BYTE) Y5;
        pOutLine4[x] = (BYTE) Y6;
    }
    return true;
}
