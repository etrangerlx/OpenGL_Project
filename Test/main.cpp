//
// Created by Administrator on 2023/4/26.
//
#include <Image.h>
#include <ImageUtils.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("argc < 2");
        return -1;
    }
    size_t width = atoi(argv[2]);
    size_t stride = atoi(argv[3]);
    size_t height = atoi(argv[4]);
    char *filename = argv[1];
    YUV420Image nv;
    nv.LoadBuffer(filename, width, stride, height, 2);
    nv.SaveBuffer("nv12.yuv");
    YUVImage yuv(width, stride, height);
    ImageUtils filter;
    filter.nv12toyuv(&nv, &yuv);
    yuv.SaveBuffer("yuv444.yuv");
    return 0;
}
