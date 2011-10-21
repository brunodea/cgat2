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

#include "util/CommonShader.hpp"

namespace model
{
    class Ground : public TexturedModel
    {
    public:
        Ground(float size, char *vert_shader, char *frag_shader, char *texture_filename)
            : m_fSize(size),m_fSunlightAngle(10.f),
            TexturedModel(vert_shader, frag_shader,texture_filename,GL_TEXTURE0,gl::TEXO::GOOD,0)
        {
            adjustVertexAttribs(TEXVERT);
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
            m_fSunlightAngle = (m_fSunlightAngle > 10000.f) ? 0.f : m_fSunlightAngle + 0.001f;
            util::setMatrices(m_pShader);
            util::setSunlight(m_pShader,m_fSunlightAngle);
            GLubyte indices[] = { 0,1,2,
                                  2,3,0 };
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
        }

        void setBufferData()
        {
            float r = 100.f; //repeat the texture.
            float s = m_fSize/2.f;
            float x = pos()[0];
            float y = pos()[1];
            float z = pos()[2];

            struct TexVert d[4] = {
                    { -s+x ,  y, -s+z, 1.f,  0.f,r  },
                    {  s+x ,  y, -s+z, 1.f,  r,r  },
                    {  s+x , y,  s+z, 1.f,  r,0.f  },
                    { -s+x , y,  s+z, 1.f,  0.f,0.f  }
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(TexVert)*4, &d[0], GL_STATIC_DRAW);
        }

        void onUpdate() {}
        void onKeyEvent(int key, int state) {}
        
    private:
        float m_fSize;
        float m_fSunlightAngle;
    }; //end of class Ground.
} //end of namespace model.

#endif
