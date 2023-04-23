#include "ImageMatrixRenderer.hpp"

class MatrixShader : public Shader
{
public:
    MatrixShader()
    {
        m_fs = 
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                vec3 rgb = texture(tex0,texcoord).bgr;\n\
				color = vec4(rgb,1.0);\n\
            }";
    }
};

ImageMatrixRenderer::ImageMatrixRenderer() 
                : m_MatrixShader(nullptr)
{
    
}

bool ImageMatrixRenderer::init()
{
    initVAO(256, 256);
	checkGLError();

    m_MatrixShader = new MatrixShader();
    m_MatrixShader->compile();
	checkGLError();
    printf("m_ScaleShader->compile\n");
    return true;
}

void ImageMatrixRenderer::updataMesh(int mesh_w, int mesh_h, int img_w, int img_h,int nDim,float *pH)
{
	m_pos.clear();
    /*
    int i, j, k, x1, y1, x2, y2;
	int nInPitch = img_w * nDim;
	float fx, fy, fY[3], fX[3];
    float H[3][3];
    for(int kh=0;kh<3;kh++)
    {
        for(int kw=0;kw<3;kw++)
        {
            H[kh][kw] = pH[kh*3+kw];
        }
    }
	fY[0] = H[0][2] + 0.5 * (H[0][0] + H[0][1]);
	fY[1] = H[1][2] + 0.5 * (H[1][0] + H[1][1]);
	fY[2] = H[2][2] + 0.5 * (H[2][0] + H[2][1]);

	for (y1 = 0; y1 < mesh_h; y1++)
	{
		fX[0] = fY[0];
		fX[1] = fY[1];
		fX[2] = fY[2];
		for (x1 = 0; x1 < mesh_w; x1++)
		{

            float pos_x = x1*1.0f / mesh_w * 2 - 1;
			float pos_y = y1*1.0f / mesh_h * 2 - 1;
			float pos_z = 0;
		
			fx = fX[0] / fX[2];
			fy = fX[1] / fX[2];

			fX[0] += H[0][0];
			fX[1] += H[1][0];
			fX[2] += H[2][0];
            int x2 = (int)fx;
            int y2 = (int)fy;
           
            float tex_x = x1*1.0f / mesh_w;//x2*1.0f / img_w;
			float tex_y = y1*1.0f / mesh_h;//y2*1.0f / img_h;

            m_pos.push_back(pos_x);
			m_pos.push_back(pos_y);
			m_pos.push_back(pos_z);
			m_pos.push_back(tex_x);
			m_pos.push_back(tex_y);
		}
		fY[0] += H[0][1];
		fY[1] += H[1][1];
		fY[2] += H[2][1];
	}
	*/
	CHECK;
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);CHECK;
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_pos.size(),
	// 	&m_pos[0], GL_STATIC_DRAW);CHECK;
	// checkGLError();
	// glEnableVertexAttribArray(0);CHECK;
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);CHECK;
	// checkGLError();
	// glEnableVertexAttribArray(1);CHECK;
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));CHECK;
	glBindBuffer(GL_ARRAY_BUFFER, 0);CHECK;
	checkGLError();
}

void ImageMatrixRenderer::setInputTexture(OglTexture *tex)
{
    m_input = tex;
}

void ImageMatrixRenderer::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void ImageMatrixRenderer::initVAO(int mesh_w, int mesh_h)
{
//	m_pos.clear();
	m_indice.clear();

	for (int h = 0; h < mesh_h; h++)
	{
		for (int w = 0; w < mesh_w; w++)
		{
			if (h < mesh_h - 1 && w < mesh_w - 1)
			{
				m_indice.push_back(h*mesh_w + w);
				m_indice.push_back(h*mesh_w + w + 1);
				m_indice.push_back((h + 1)*mesh_w + w + 1);
				m_indice.push_back(h*mesh_w + w);
				m_indice.push_back((h + 1)*mesh_w + w);
				m_indice.push_back((h + 1)*mesh_w + w + 1);
			}
		}
	}
	
	glGenBuffers(1, &m_vbo);CHECK;
	glGenBuffers(1, &m_ebo);CHECK;
	//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);CHECK;
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);CHECK;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indice.size() * sizeof(unsigned int), &m_indice[0],
		GL_STATIC_DRAW);CHECK;
	//glBindVertexArray(0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ImageMatrixRenderer::render()
{
	m_fbo.attachTexture(m_output);CHECK;
	m_fbo.bind();CHECK;
	glViewport(0, 0, m_output->getWidth(), m_output->getHeight());CHECK;
	glClearColor(0.0, 0.0, 0.0, 1.0);
	m_MatrixShader->use();CHECK;
//	glBindVertexArray(m_vao);
	glActiveTexture(GL_TEXTURE0);
	m_MatrixShader->setInt("tex0", 0);CHECK;
	m_input->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);CHECK;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indice.size() * sizeof(unsigned int), &m_indice[0],
		GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, m_indice.size(), GL_UNSIGNED_INT, (GLvoid *)0);CHECK;
//	glBindVertexArray(0);
	m_fbo.unbind();CHECK;
	m_input->unbind();CHECK;
	checkGLError();

}

void ImageMatrixRenderer::deinit()
{
    if (m_MatrixShader)
    {
        delete m_MatrixShader;
        m_MatrixShader = NULL;
    }
    deinitVao();
}
