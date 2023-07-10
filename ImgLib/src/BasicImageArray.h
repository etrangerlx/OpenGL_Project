#ifndef __BASIC_IMAGE_ARRAY_H_
#define __BASIC_IMAGE_ARRAY_H_

#include <memory>

template<class T>class CBasicImageArray {
public:

protected:
    int m_nWidth;
    int m_nHeight;
    int m_nDim;
    T *m_pData;
    bool m_bShadowBuffer = false;

public:
    CBasicImageArray() {
        m_bShadowBuffer = false;
        m_nWidth = 0;
        m_nHeight = 0;
        m_nDim = 0;
        m_pData = nullptr;
    }

    ~CBasicImageArray() {
        if (m_pData != nullptr && !m_bShadowBuffer) {
            delete[] m_pData;
            m_pData = nullptr;
        }
    }

    void ClearBuffer() {
        if (m_pData != nullptr && !m_bShadowBuffer) {
            delete[] m_pData;
            m_pData = nullptr;
        }
        m_nWidth = 0;
        m_nHeight = 0;
        m_nDim = 0;
        m_pData = nullptr;
        m_bShadowBuffer = false;
    }

    bool SetImageSize(int W, int H, int D) {
        if (m_bShadowBuffer) {
            return false;
        }
        ClearBuffer();
        m_pData = new T[W * H * D];
        if(m_pData == nullptr) {
            return false;
        }
        m_nWidth = W;
        m_nHeight = H;
        m_nDim = D;
        return true;
    }

    __inline T *GetImageData() {
        return m_pData;
    };
    __inline int GetWidth() {
        return m_nWidth;
    };
    __inline int GetHeight() {
        return m_nHeight;
    };
    __inline int GetDim() {
        return m_nDim;
    }
};
typedef unsigned char BYTE;
typedef CBasicImageArray<BYTE> CBasicImageArray_BYTE;

#endif

