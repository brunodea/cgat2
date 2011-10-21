#ifndef _CGA_T2_TEXTURED_MODEL_HPP_
#define _CGA_T2_TEXTURED_MODEL_HPP_

#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"

#include <iostream>
#include <cstdlib>

#include "Bmp.h"
#include "model/ShaderModel.hpp"
#include "model/VertexStructs.hpp"
#include "gl/GLObjects.hpp"

namespace model
{
    class TexturedModel : public ShaderModel
    {
    public:
        TexturedModel(char *vert_shader, char *frag_shader, char *texture_filename, GLuint texture,gl::TEXO::Filtering f,GLuint unit)
            : ShaderModel(vert_shader,frag_shader), m_pBmp(new Bmp(texture_filename)), m_TEXO(gl::TEXO(texture,f,unit))
        {
            m_pBmp->convertBGRtoRGB();
            if(m_pBmp->getImage() == NULL)
            {
                delete m_pBmp;
                m_pBmp = NULL;
                std::cout << "Couldn't load the texture " << texture_filename << ".\n";
                exit(1);
            }

            m_TEXO.bind(GL_TEXTURE_2D);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pBmp->getWidth(), 
                         m_pBmp->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_pBmp->getImage());
            glGenerateMipmap(GL_TEXTURE_2D);
            
            m_TEXO.unbind();
        }

        ~TexturedModel()
        {
            delete m_pBmp;
        }

        void renderTexture()
        {
            m_TEXO.bind(GL_TEXTURE_2D);
        }

        void afterRenderTexture()
        {
            m_TEXO.unbind();
        }

        virtual void render() = 0;
        virtual void setBufferData() = 0;

    protected:
        Bmp *m_pBmp;
        gl::TEXO m_TEXO;
    }; //end of class TexturedModel.
} //end of namespace model.

#endif
