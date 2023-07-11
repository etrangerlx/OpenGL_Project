//
// Created by Administrator on 2023/7/9.
//
#include "ImageByte.h"
#include "ReszieFilter.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("argc < 3 !");
    }
    CGrayImage nDownImage[4], nUpImage[4];
    int Width = atoi(argv[2]);
    int Height = atoi(argv[3]);
    nDownImage[0].Create(Width, Height);

    FILE *fp = nullptr;
    printf("%s\n", argv[1]);
    fp = fopen(argv[1], "r");
    fread(nDownImage[0].GetImageData(), Width * Height, 1, fp);
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
    }
    ReszieFilter resizeobj;
    for (int i = 0; i < 2; i++) {
        nDownImage[i + 1].Create(Width / pow(4, i + 1), Height / pow(4, i + 1));
        resizeobj.DownScaleImage_4x(&nDownImage[i], &nDownImage[i + 1], nDownImage[i].GetWidth(), nDownImage[i].GetHeight());
        char name[256];
        sprintf(name, "Down[%d].yuv", i + 1);
        nDownImage[i + 1].SaveFile(name);

    }
//    nDownImage[0].Create(Width / 4, Height / 4);
//    resizeobj.DownScaleImage_4x(&nInImgae, &nDownImage[1], Width, Height);
//    nDownImage[0].SaveFile("Down[0].yuv");
//    nUpImage[0].Create(Width, Height);
//    resizeobj.UpScaleImage_4x(&nDownImage[0], &nUpImage[0], Width / 4, Height / 4);
//    nUpImage[0].SaveFile("Up[0].yuv");
    return 0;
}

