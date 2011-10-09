#ifndef _CGA_T2_GROUND_HPP_
#define _CGA_T2_GROUND_HPP_

#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"

#include <iostream>
#include <cstdlib>

#include "util/MatrixStack.h"
#include "macros.h"
#include "Bmp.h"

namespace model
{
    class Ground
    {
    public:
        Ground(float size, char *vert_shader, char *frag_shader, char *texture) 
            : m_fSize(size), m_pImage(new Bmp(texture)), m_iTextureID(0)
        {
            m_pImage->convertBGRtoRGB();
            m_Data = m_pImage->getImage();
            if(m_Data == NULL)
            {
                delete m_pImage;
                std::cout << "Bumpmap for the ground couldn't be loaded." << std::endl;
                exit(1);
            }
            glActiveTexture(GL_TEXTURE0);
            glGenTextures(1,&m_iTextureID);
            glBindTexture(GL_TEXTURE_2D,m_iTextureID);

           /* glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
*/
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pImage->getWidth(), 
                m_pImage->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,0);

            glGenSamplers(1,&m_iSamplerID);
            /*glSamplerParameterf(m_iSamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glSamplerParameterf(m_iSamplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
            
            glSamplerParameteri(m_iSamplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glSamplerParameteri(m_iSamplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glSamplerParameteri(m_iSamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glSamplerParameteri(m_iSamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,m_iTextureID);*/


            glGenVertexArrays(1, &m_iVAOID);
            glBindVertexArray(m_iVAOID);

            float r = 1.f;
            float s = size/2.f;
            struct vertex_data d[4] = {
                    {-s ,0.f,-s, 1.f,   0.f,r},
                    { s ,0.f,-s, 1.f,   r,r},
                    { s ,size, s, 1.f,   r,0.f},
                    {-s ,size, s, 1.f,   0.f,0.f}
            };
            
            float tex[4][2] = { {0.f,r}, {r,r}, {r,0.f}, {0.f,0.f} };

            data = &d[0];
            
            glGenBuffers(1, &m_iTEXID);
            glBindBuffer(GL_ARRAY_BUFFER, m_iTEXID);

            glBufferData(GL_ARRAY_BUFFER, sizeof(tex), &tex[0], GL_STATIC_DRAW);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0,2,GL_FLOAT,GL_TRUE,sizeof(GL_FLOAT)*2,0);

            
            glGenBuffers(1, &m_iVBOID);
            glBindBuffer(GL_ARRAY_BUFFER, m_iVBOID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data)*4, data, GL_STATIC_DRAW);
            
		    glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (char*)0 + offsetof(vertex_data, pos));

            //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(vertex_data), (char*)0 + offsetof(vertex_data, tex));
		    //glEnableVertexAttribArray(2);
            
            m_pShader = new Glsl(vert_shader, frag_shader);
            
            m_loc_u_projection = m_pShader->getUniformLoc("projection");
            m_loc_u_modelview = m_pShader->getUniformLoc("modelview");
            m_loc_u_textureMap = m_pShader->getUniformLoc("textureMap");
        }
        ~Ground()
        {
            delete m_pImage;
            delete m_pShader;
            glDeleteBuffers(1, &m_iVBOID);
            glDeleteBuffers(1, &m_iTEXID);
            glDeleteVertexArrays(1, &m_iVAOID);
            
            glDeleteTextures(1, &m_iTextureID);
            glDeleteSamplers(1, &m_iSamplerID);
        }

        void onRender()
        {
            m_pShader->setActive(true);

            glBindSampler(0, m_iSamplerID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_iTextureID);
            
            glUniformMatrix4fv(m_loc_u_projection, 1, GL_TRUE, util::MATRIXSTACK->projection().elements());
            glUniformMatrix4fv(m_loc_u_modelview, 1, GL_TRUE, util::MATRIXSTACK->top().elements());
            GLubyte indices[] = { 0,1,2,
                                  2,3,0 };
            glBindVertexArray(m_iVAOID);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindSampler(0,0);

            m_pShader->setActive(false);
        }
    private:
        struct vertex_data
        {
            float pos[4]; //x,y,z,w
            float tex[2]; //r,g,b,a
        };

    private:
        Glsl *m_pShader;
        GLuint m_iVAOID;
        GLuint m_iVBOID;
        GLuint m_iTEXID;

        float m_fSize;
        struct vertex_data *data;
        GLint m_loc_u_projection;
        GLint m_loc_u_modelview;
        GLint m_loc_u_textureMap;
        GLuint m_iTextureID;
        GLuint m_iSamplerID;
        Bmp *m_pImage;
        unsigned char *m_Data;
    }; //end of class Ground.
} //end of namespace model.

#endif
