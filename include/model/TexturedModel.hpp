#ifndef _CGA_T2_TEXTURED_MODEL_HPP_
#define _CGA_T2_TEXTURED_MODEL_HPP_

#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"

#include <iostream>
#include <cstdlib>

#include "Bmp.h"

namespace model
{
    class TexturedModel
    {
    public:
        TexturedModel(char *texture_filename)
            : m_pBmp(new Bmp(texture_filename))
        {
            m_pBmp->convertBGRtoRGB();
            if(m_pBmp->getImage() == NULL)
            {
                delete m_pBmp;
                m_pBmp = NULL;
                std::cout << "Couldn't load the texture " << texture_filename << ".\n";
                exit(1);
            }

            glActiveTexture(GL_TEXTURE0);
            glGenTextures(1,&m_iTextureID);
            glBindTexture(GL_TEXTURE_2D,m_iTextureID);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pBmp->getWidth(), 
                         m_pBmp->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_pBmp->getImage());
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,0);

            glGenSamplers(1,&m_iSamplerID);
            glSamplerParameteri(m_iSamplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glSamplerParameteri(m_iSamplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glSamplerParameteri(m_iSamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glSamplerParameteri(m_iSamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }

        ~TexturedModel()
        {
            delete m_pBmp;

            glDeleteBuffers(1, &m_iTOBID);
            glDeleteTextures(1, &m_iTextureID);
            glDeleteSamplers(1, &m_iSamplerID);
        }

        virtual void initTOB(int repeat)
        {
            float tex[4][2] = { {0.f,(float)repeat}, {(float)repeat,(float)repeat}, {(float)repeat,0.f}, {0.f,0.f} };
            glGenBuffers(1, &m_iTOBID);
            glBindBuffer(GL_ARRAY_BUFFER, m_iTOBID);

            glBufferData(GL_ARRAY_BUFFER, sizeof(tex)*4, &tex[0], GL_STATIC_DRAW);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0,2,GL_FLOAT,GL_TRUE,sizeof(GL_FLOAT)*2,0);
        }

        void renderTexture()
        {
            glBindSampler(0, m_iSamplerID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_iTextureID);
        }

        void afterRenderTexture()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindSampler(0,0);
        }

    protected:
        Bmp *m_pBmp;
        GLuint m_iTextureID;
        GLuint m_iSamplerID;
        GLuint m_iTOBID;
    }; //end of class TexturedModel.
} //end of namespace model.

#endif
