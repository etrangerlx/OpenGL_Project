//
// Created by Administrator on 2023/8/17.
//

#ifndef IMGLIBTEST_TYPEDEF_H
#define IMGLIBTEST_TYPEDEF_H


#include "Optimation.h"

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT32;
typedef unsigned long UINT64;

typedef char CHAR;
typedef short SHORT;
typedef int INT32;
typedef long INT64;

typedef float FLOAT32;
typedef double FLOAT64;

template<typename _Tp>
struct Point_ {
    Point_()
            : x(0), y(0) {
    }

    Point_(_Tp _x, _Tp _y)
            : x(_x), y(_y) {
    }

    _Tp x;
    _Tp y;
};

template<typename _Tp>
struct Size_ {
    Size_()
            : width(0), height(0) {
    }

    Size_(_Tp _w, _Tp _h)
            : width(_w), height(_h) {
    }

    _Tp width;
    _Tp height;
};

template<typename _Tp>
struct Rect_ {
    Rect_()
            : x(0), y(0), width(0), height(0) {
    }

    Rect_(_Tp _x, _Tp _y, _Tp _w, _Tp _h)
            : x(_x), y(_y), width(_w), height(_h) {
    }

    Rect_(Point_<_Tp> _p, Size_<_Tp> _size)
            : x(_p.x), y(_p.y), width(_size.width), height(_size.height) {
    }

    _Tp x;
    _Tp y;
    _Tp width;
    _Tp height;

    // area
    _Tp area() const {
        return width * height;
    }
};

template<typename _Tp>
static inline Rect_<_Tp> &operator&=(Rect_<_Tp> &a, const Rect_<_Tp> &b) {
    _Tp x1 = std::max(a.x, b.x), y1 = std::max(a.y, b.y);
    a.width = std::min(a.x + a.width, b.x + b.width) - x1;
    a.height = std::min(a.y + a.height, b.y + b.height) - y1;
    a.x = x1;
    a.y = y1;
    if (a.width <= 0 || a.height <= 0)
        a = Rect_<_Tp>();
    return a;
}

template<typename _Tp>
static inline Rect_<_Tp> &operator|=(Rect_<_Tp> &a, const Rect_<_Tp> &b) {
    _Tp x1 = std::min(a.x, b.x), y1 = std::min(a.y, b.y);
    a.width = std::max(a.x + a.width, b.x + b.width) - x1;
    a.height = std::max(a.y + a.height, b.y + b.height) - y1;
    a.x = x1;
    a.y = y1;
    return a;
}

template<typename _Tp>
static inline Rect_<_Tp> operator&(const Rect_<_Tp> &a, const Rect_<_Tp> &b) {
    Rect_<_Tp> c = a;
    return c &= b;
}

template<typename _Tp>
static inline Rect_<_Tp> operator|(const Rect_<_Tp> &a, const Rect_<_Tp> &b) {
    Rect_<_Tp> c = a;
    return c |= b;
}


template<class _Tp>
class Mat_ {
private:
    _Tp *m_pBuffer;
    size_t m_BufferSizeByte;
    bool m_bShadowBuffer;
protected:
    bool GetShadowBuffer() {
        return m_bShadowBuffer;
    }

    void SetShadowBuffer(bool flag) {
        m_bShadowBuffer = flag;
    }

    size_t GetBufferSizeByte() const {
        return m_BufferSizeByte;
    }

public:
    size_t m_nWidth;
    size_t m_nStride;
    size_t m_nHeight;
    size_t m_nDim;
    size_t m_MatSizeByte;

    void SetMatProperty(size_t _W, size_t _S, size_t _H, size_t _D, size_t _MatSizeByte) {
        m_nWidth = _W;
        m_nStride = _S;
        m_nHeight = _H;
        m_nDim = _D;
        m_MatSizeByte = _MatSizeByte;
    }

    void ClearBuffer() {
        if (GetBufferSizeByte()) {
            return;
        }
        if (m_pBuffer != nullptr && !GetShadowBuffer()) {
            delete[] m_pBuffer;
            m_pBuffer = nullptr;
        }
        SetMatProperty(0, 0, 0, 0, 0);
    }

    bool CreateBuffer(size_t W, size_t S, size_t H, size_t D) {
        if (GetShadowBuffer()) {
            return false;
        }
        size_t sz = W * H * D;
        if (S > W) {
            if (D == 2) {
                sz = S * H * 3 / 2;
            } else {
                sz = S * H * D;
            }
        } else {
            if (D == 2) {
                sz = W * H * 3 / 2;
            } else {
                sz = W * H * D;
            }
        }
        if (sz > m_BufferSizeByte) {
            ClearBuffer();
            m_pBuffer = new _Tp[sz];
            if (m_pBuffer == nullptr) {
                return false;
            }
            m_BufferSizeByte = sz;
        }
        SetMatProperty(W, S, H, D, sz);
        return true;
    }

    Mat_() : m_pBuffer(nullptr),
             m_BufferSizeByte(0),
             m_bShadowBuffer(false),

             m_nWidth(0),
             m_nStride(0),
             m_nHeight(0),
             m_nDim(0),
             m_MatSizeByte(0) {
    }

    Mat_(size_t W, size_t H, size_t D) : m_pBuffer(nullptr),
                                         m_BufferSizeByte(0),
                                         m_bShadowBuffer(false),

                                         m_nWidth(0),
                                         m_nStride(0),
                                         m_nHeight(0),
                                         m_nDim(0),
                                         m_MatSizeByte(0) {
        size_t sz = W * H * D;
        if (D == 2) {
            sz = W * H * 3 / 2;
        }
        if (sz > m_BufferSizeByte) {
            ClearBuffer();
            m_pBuffer = new _Tp[sz];
            if (m_pBuffer == nullptr) {
                return;
            }
            m_BufferSizeByte = sz;
        }
        SetMatProperty(W, W, H, D, sz);
    }

    Mat_(size_t W, size_t S, size_t H, size_t D): m_pBuffer(nullptr),
                                                  m_BufferSizeByte(0),
                                                  m_bShadowBuffer(false),

                                                  m_nWidth(0),
                                                  m_nStride(0),
                                                  m_nHeight(0),
                                                  m_nDim(0),
                                                  m_MatSizeByte(0) {

        size_t sz = W * H * D;
        if (S > W) {
            if (D == 2) {
                sz = S * H * 3 / 2;
            } else {
                sz = S * H * D;
            }
        } else {
            if (D == 2) {
                sz = W * H * 3 / 2;
            } else {
                sz = W * H * D;
            }
        }
        if (sz > m_BufferSizeByte) {
            ClearBuffer();
            m_pBuffer = new _Tp[sz];
            if (m_pBuffer == nullptr) {
                return;
            }
            m_BufferSizeByte = sz;
        }
        SetMatProperty(W, S, H, D, sz);
    }

// copy
    Mat_(const Mat_ &m) {
        m_BufferSizeByte = m.m_BufferSizeByte;
        m_nWidth = m.m_nWidth;
        m_nStride = m.m_nStride;
        m_nHeight = m.m_nHeight;
        m_nDim = m.m_nDim;
        m_MatSizeByte = m.m_MatSizeByte;
        m_pBuffer = new _Tp[m_BufferSizeByte];
        m_bShadowBuffer = false;
        memcpy(m_pBuffer, m.m_pBuffer, m_BufferSizeByte);
    }

    Mat_ &operator=(const Mat_ &m) {
        if (this == &m) {
            return *this;
        }
        this->ClearBuffer();
        this->CreateBuffer(m.m_nWidth, m.m_nStride, m.m_nHeight, m.m_nDim);
        memcpy(m_pBuffer, m.m_pBuffer, m_BufferSizeByte);
        return *this;
    }

    Mat_ Clone(const Mat_ &m) {
        if (GetBufferSizeByte() == 0) {
            return Mat_();
        }
        Mat_ mm(m.m_nWidth, m.m_nStride, m.m_nHeight, m.m_nDim);
        memcpy(mm.m_pBuffer, m.m_pBuffer, m_BufferSizeByte);
        return mm;
    }

    void SetValue(_Tp value) {
        memset(GetPtr(), value, m_MatSizeByte);
    }

    virtual ~Mat_() {
        ClearBuffer();
    }


    __inline _Tp *GetPtr() const {
        return m_pBuffer;
    };

    __inline int GetWidth() const {
        return m_nWidth;
    };

    __inline _Tp *GetLine(size_t y) const {
        if (y > m_nHeight) {
            y = m_nHeight;
        }
        if (m_nDim == 2) {
            return m_pBuffer + y * m_nStride * 1;
        } else {
            return m_pBuffer + y * m_nStride * m_nDim;
        }
    };

    __inline int GetStride() const {
        return m_nStride;
    };

    __inline int GetHeight() const {
        return m_nHeight;
    };

    __inline int GetDim() const {
        return m_nDim;
    }

    bool LoadBuffer(const char *filename, size_t W, size_t S, size_t H, size_t D) {
        std::string f(filename);
        this->CreateBuffer(W, S, H, D);
        FILE *fp = fopen(filename, "rb");
        fseek(fp, 0, SEEK_END);
        size_t len = ftell(fp);
        if (len > GetBufferSizeByte()) {
            printf("The buffer is smaller the file%s!", filename);
            return false;
        }
        fseek(fp, 0, SEEK_SET);
        fread(this->GetPtr(), len, 1, fp);
        if (fp != nullptr) {
            fclose(fp);
            fp = nullptr;
            return false;
        }
        return true;
    }

    bool SaveBuffer(const char *filename) {
        std::string f(filename);
        std::size_t pos = f.find_last_of(".");
        std::string prefix = f.substr(0, pos);
        std::string postfix = f.substr(pos, f.size() - 1);
        char FileName[256];
        sprintf(FileName, "%s_%dx%dx%dx%d%s", prefix.c_str(), GetWidth(), GetStride(), GetHeight(), GetDim(),
                postfix.c_str());
        FILE *fp = fopen(FileName, "w+");
        fwrite(GetPtr(), GetBufferSizeByte(), 1, fp);
        if (fp != nullptr) {
            fclose(fp);
            fp = nullptr;
            return false;
        }
        return true;
    }
};


typedef Point_<int> Point;
typedef Point_<float> Point2f;

typedef Size_<int> Size;
typedef Size_<float> Size2f;

typedef Rect_<int> Rect;
typedef Rect_<float> Rect2f;
#endif //IMGLIBTEST_TYPEDEF_H
