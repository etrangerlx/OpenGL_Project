#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#include <string>
#include <stdlib.h>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main() {
    std::cout << "Hello, STB_Image" << std::endl;
    std::string inputPath = "wall.jpg";
    int iw, ih, n;
    // 加载图片获取宽、高、颜色通道信息
    unsigned char *idata = stbi_load(inputPath.c_str(), &iw, &ih, &n, 0);
    int ow = iw / 2;
    int oh = ih / 2;
    auto *odata = (unsigned char *) malloc(ow * oh * n);
    // 改变图片尺寸
    stbir_resize(idata, iw, ih, 0, odata, ow, oh, 0, STBIR_TYPE_UINT8, n, STBIR_ALPHA_CHANNEL_NONE, 0,
                 STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
                 STBIR_FILTER_BOX, STBIR_FILTER_BOX,
                 STBIR_COLORSPACE_SRGB, nullptr
    );
    std::string outputPath = "output.png";
    // 写入图片
    stbi_write_png(outputPath.c_str(), ow, oh, n, odata, 0);
    stbi_image_free(idata);
    stbi_image_free(odata);
    return 0;
}