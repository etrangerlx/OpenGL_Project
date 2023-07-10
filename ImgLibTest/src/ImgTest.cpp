//
// Created by Administrator on 2023/7/9.
//
#include "ImageByte.h"
#include "ReszieFilter.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("argc < 3 !");
    }
    CGrayImage nInImgae, nDownImage, nUpImage;
    int Width = atoi(argv[2]);
    int Height = atoi(argv[3]);
    nInImgae.Create(Width, Height);

    FILE *fp = nullptr;
    printf("%s\n", argv[1]);
    fp = fopen(argv[1], "r");
    fread(nInImgae.GetImageData(), Width * Height, 1, fp);
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
    }
    nInImgae.SaveFile("Origne.yuv");
    ReszieFilter resizeobj;
    nDownImage.Create(Width / 4, Height / 4);
    resizeobj.DownScaleImage_4x(&nInImgae, &nDownImage, Width, Height);
    nDownImage.SaveFile("Down.yuv");
    nUpImage.Create(Width, Height);
    resizeobj.UpScaleImage_4x(&nDownImage, &nUpImage, Width / 4, Height / 4);
    nUpImage.SaveFile("Up.yuv");
    return 0;
}

