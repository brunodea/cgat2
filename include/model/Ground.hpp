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

#include "model/TexturedModel.hpp"

namespace model
{
    class Ground : public TexturedModel
    {
    public:
        Ground(float size, char *vert_shader, char *frag_shader, char *texture_filename)
            : TexturedModel(texture_filename)
        {
            glGenVertexArrays(1, &m_iVAOID);
            glBindVertexArray(m_iVAOID);

            float r = 10.f;
            float s = size/2.f;
            struct vertex_data d[4] = {
                    { -s ,  0.f, -s, 1.f  },
                    {  s ,  0.f, -s, 1.f  },
                    {  s , size,  s, 1.f  },
                    { -s , size,  s, 1.f  }
            };
            
            data = &d[0];
            
            initTOB(10);

            glGenBuffers(1, &m_iVBOID);
            glBindBuffer(GL_ARRAY_BUFFER, m_iVBOID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data)*4, data, GL_STATIC_DRAW);
            
		    glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), 0);//(char*)0 + offsetof(vertex_data, pos));
            
            m_pShader = new Glsl(vert_shader, frag_shader);
            
            m_loc_u_projection = m_pShader->getUniformLoc("projection");
            m_loc_u_modelview = m_pShader->getUniformLoc("modelview");
            m_loc_u_textureMap = m_pShader->getUniformLoc("textureMap");
        }
        ~Ground()
        {
            delete m_pShader;
            glDeleteBuffers(1, &m_iVBOID);
            glDeleteVertexArrays(1, &m_iVAOID);
        }

        void onRender()
        {
            m_pShader->setActive(true);

            renderTexture();
            
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
        };

    private:
        Glsl *m_pShader;
        GLuint m_iVAOID;
        GLuint m_iVBOID;

        struct vertex_data *data;
        GLint m_loc_u_projection;
        GLint m_loc_u_modelview;
        GLint m_loc_u_textureMap;
    }; //end of class Ground.
} //end of namespace model.

#endif
