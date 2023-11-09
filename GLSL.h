//
// Created by Administrator on 2023/11/8.
//

const char Vertex[] = "#version 330 core\n\
                      layout (location = 0) in vec3 position;\
                      layout (location = 1) in vec3 color;\
                      layout (location = 2) in vec2 texCoord;\
                      out vec3 ourColor;\
                      out vec2 TexCoord;\
                      uniform mat4 transform;\
                      void main()\
                      {\
                            gl_Position = vec4(position, 1.0f);\
                            ourColor = color;\
                            TexCoord = vec2(texCoord.x,1.0f-texCoord.y);\
                      }";

//const char Vertex[] = "#version 330 core\n\
//layout (location = 0) in vec3 position;\
//layout (location = 1) in vec3 color;\
//layout (location = 2) in vec2 texCoord;\
//out vec3 ourColor;\
//out vec2 TexCoord;\
//void main()\
//{\
//gl_Position = vec4(position, 1.0f);\
//ourColor = color;\
//TexCoord = texCoord;\
//}";

const char Fragment[] = "#version 330 core\n\
                        in vec3 ourColor;\
                        in vec2 TexCoord;\
                        out vec4 color;\
                        uniform float mixValue;\
                        uniform sampler2D ourTexture1;\
                        uniform sampler2D ourTexture2;\
                        void main()\
                        {\
                            color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), mixValue);\
                        }";

//const char Fragment[] = "#version 330 core\n\
//                        in vec3 ourColor;\
//                        in vec2 TexCoord;\
//                        out vec4 color;\
//                        uniform sampler2D ourTexture;\
//                        void main()\
//                        {\
//                        color = texture(ourTexture, TexCoord);\
//                        }";