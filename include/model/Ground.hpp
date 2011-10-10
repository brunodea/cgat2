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
#include "model/ShaderModel.hpp"

namespace model
{
    class Ground : public TexturedModel, public ShaderModel
    {
    public:
        Ground(float size, char *vert_shader, char *frag_shader, char *texture_filename)
            : m_fSize(size), TexturedModel(texture_filename), ShaderModel(vert_shader, frag_shader)
        {
            initVBO();
        }

        void beforeRender()
        {
            renderTexture();
        }

        void afterRender()
        {
            afterRenderTexture();
        }

        void render()
        {
            GLubyte indices[] = { 0,1,2,
                                  2,3,0 };
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
        }

        void initVBO()
        {
            glGenVertexArrays(1, &m_iVAOID);
            glBindVertexArray(m_iVAOID);

            float r = 10.f;
            float s = m_fSize/2.f;
            struct vertex_data d[4] = {
                    { -s ,  0.f, -s, 1.f  },
                    {  s ,  0.f, -s, 1.f  },
                    {  s , 0.f,  s, 1.f  },
                    { -s , 0.f,  s, 1.f  }
            };
            
            m_Data = &d[0];
            
            initTOB(100);

            glGenBuffers(1, &m_iVBOID);
            glBindBuffer(GL_ARRAY_BUFFER, m_iVBOID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data)*4, m_Data, GL_STATIC_DRAW);
            
		    glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), 0);
            //(char*)0 + offsetof(vertex_data, pos));
        }

    private:
        struct vertex_data
        {
            float pos[4]; //x,y,z,w
        };

    private:
        struct vertex_data *m_Data;
        float m_fSize;
    }; //end of class Ground.
} //end of namespace model.

#endif
