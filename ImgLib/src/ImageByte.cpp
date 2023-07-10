//
// Created by Administrator on 2023/7/5.
//


#include "ImageByte.h"


IMAGEBYTE::IMAGEBYTE() : CBasicImageArray() {
    for (int i = 0; i < 4; i++) {
        planptr[i] = nullptr;
        Stride[i] = 0;
    }
    m_type = IMAGE_TYPE_NONE;
}

CGrayImage::CGrayImage() : IMAGEBYTE() {
    m_type = IMAGE_TYPE_GRAY;
    m_pData = nullptr;
    m_nWidth = 0;
    m_nHeight = 0;
    m_nDim = 0;

}

bool CGrayImage::Create(int Width, int Height) {
    int res = SetImageSize(Width, Height, 1);
    m_type = IMAGE_TYPE_GRAY;
    planptr[0] = m_pData;
    Stride[0] = Width;
    return true;
}

CGrayImage::CGrayImage(const CGrayImage &obj) {
    if (m_bShadowBuffer) {
        return;
    } else {
        ClearBuffer();
        int res = SetImageSize(obj.m_nWidth, obj.m_nHeight, 1);
        memcpy(m_pData, obj.m_pData, obj.m_nWidth * obj.m_nHeight * sizeof(BYTE));
    }
}

CGrayImage &CGrayImage::operator=(const CGrayImage &obj) {
    if (this == &obj) {
        return *this;
    }
    int res = SetImageSize(obj.m_nWidth, obj.m_nHeight, 1);
    if (res) {
        memcpy(m_pData, obj.m_pData, obj.m_nWidth * obj.m_nHeight * sizeof(BYTE));
    }
    return *this;
}

#include <string>

bool CGrayImage::SaveFile(const char *filename) {
    std::string f(filename);
    std::size_t pos = f.find_last_of(".");
    std::string prefix = f.substr(0, pos);
    std::string postfix = f.substr(pos, f.size() - 1);
    char FileName[256];
    sprintf(FileName, "%s_%dx%dx%d%s", prefix.c_str(), m_nWidth, m_nHeight, m_nDim, postfix.c_str());
    FILE *fp = nullptr;
    fp = fopen(FileName, "w+");
    fwrite(m_pData, m_nWidth * m_nHeight, 1, fp);
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
    }
    return false;
}
