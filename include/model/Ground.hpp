#ifndef _CGA_T2_GROUND_HPP_
#define _CGA_T2_GROUND_HPP_

#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"

namespace model
{
    class Ground
    {
    public:
        Ground(float size, char *vert_shader, char *frag_shader) 
            : m_pShader(new Glsl(vert_shader, frag_shader)), m_fSize(size)
        {
            glGenVertexArrays(1, &m_iVAOID);
            glBindVertexArray(m_iVAOID);

            glGenBuffers(1, &m_iVBOID);
            glBindBuffer(GL_ARRAY_BUFFER, m_iVBOID);

            float s = size/2.f;
            struct vertex_data d[4] = {
                    {-s ,0.f,-s, 1.f,   1.f,0.f,0.f,1.f},
                    { s ,0.f,-s, 1.f,   0.f,1.f,0.f,1.f},
                    { s ,0.f, s, 1.f,   0.f,0.f,1.f,1.f},
                    {-s ,0.f, s, 1.f,   1.f,0.f,0.f,1.f}
            };

            data = &d[0];

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data)*4, data, GL_STATIC_DRAW);
            
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (char*)0 + offsetof(vertex_data, pos));
		    glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,  sizeof(vertex_data), (char*)0 + offsetof(vertex_data, color));
		    glEnableVertexAttribArray(1);
        }
        ~Ground()
        {
            delete m_pShader;
            glDeleteBuffers(1, &m_iVBOID);
            glDeleteBuffers(1, &m_iVAOID);
        }

        void onRender()
        {
            GLubyte indices[] = { 0,1,2,
                                  1,2,3 };
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
        }
    private:
        struct vertex_data
        {
            float pos[4]; //x,y,z,w
            float color[4]; //r,g,b,a
        };

    private:
        Glsl *m_pShader;
        GLuint m_iVAOID;
        GLuint m_iVBOID;

        float m_fSize;
        struct vertex_data *data;
    }; //end of class Ground.
} //end of namespace model.

#endif
