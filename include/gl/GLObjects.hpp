/*
    Classes com funcionalidades básicas para objetos do OpenGL (como VAO,VBO,FBO,etc).
    Foram criadas para dar um nível um pouco maior de abstração e para deixar outros lugares do código mais limpos
    e diminuir a redundância de código.
*/
#ifndef _CGA_T2_GL_OBJECTS_HPP_
#define _CGA_T2_GL_OBJECTS_HPP_

#include "GL/gl3w.hpp"
#include "GL/glfw.h"

namespace gl
{
    class GLObj
    {
    public:
        GLObj()  {}

        GLuint& id() { return m_iID; }
    private:
        GLuint m_iID;
    };//end of class GLObj.

    class VAO : public GLObj
    {
    public:
        VAO() : GLObj() { glGenVertexArrays(1,&id()); }
        ~VAO() { glDeleteVertexArrays(1,&id()); }

        void bind() { glBindVertexArray(id()); }
        void unbind() { glBindVertexArray(0); }
    }; //end of class VAO.

    class VBO : public GLObj
    {
    public:
        VBO() : GLObj() { glGenBuffers(1,&id()); }
        ~VBO() { glDeleteBuffers(1,&id()); }

        void bind(GLenum target) 
        { 
            m_BindTarget = target;
            glBindBuffer(target,id()); 
        }
        void unbind() { glBindBuffer(m_BindTarget,0); }

    private:
        GLenum m_BindTarget;
    }; //end of class VBO.
    
    class TEXO : public GLObj
    {
    public:
        enum Filtering
        {
            GOOD,
            BAD
        }; //end of enum Filtering.
    public:
        TEXO(GLuint curr_tex, Filtering f, GLuint sampler_unit) 
            : GLObj(), m_CurrentTexture(curr_tex), m_iSamplerUnit(sampler_unit)
        { 
            glActiveTexture(m_CurrentTexture);
            glGenTextures(1,&id());
            genSampler(f);
        }

        ~TEXO()
        {
            glDeleteTextures(1,&id());
            glDeleteSamplers(1,&m_iSamplerID);
        }

        void bind(GLenum target)
        {
            m_Target = target;
            glBindSampler(m_iSamplerUnit,m_iSamplerID);
            glActiveTexture(m_CurrentTexture);
            glBindTexture(m_Target,id());
        }

        void unbind()
        {
            glBindTexture(m_Target,0);
            glBindSampler(m_iSamplerUnit,0);
        }

    private:
        void genSampler(Filtering f)
        {
            glGenSamplers(1,&m_iSamplerID);
            switch(f)
            {
            case GOOD:
                glSamplerParameteri(m_iSamplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glSamplerParameteri(m_iSamplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glSamplerParameteri(m_iSamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glSamplerParameteri(m_iSamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                break;
            case BAD:
                glSamplerParameteri(m_iSamplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glSamplerParameteri(m_iSamplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glSamplerParameteri(m_iSamplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glSamplerParameteri(m_iSamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                break;
            default:
                break;
            }
        }

    private:
        GLuint m_CurrentTexture; //GL_TEXTURE0,GL_TEXTURE1,etc.
        GLenum m_Target;
        GLuint m_iSamplerID;
        GLuint m_iSamplerUnit;
    }; //end of class TEXO.

    class RBO : public GLObj
    {
    public:
        RBO() 
            : GLObj()
        {
            glGenRenderbuffers(1,&id());
        }

        ~RBO()
        {
            glDeleteRenderbuffers(1,&id());
        }

        void bind() { glBindRenderbuffer(GL_RENDERBUFFER,id()); }
        void unbind() { glBindRenderbuffer(GL_RENDERBUFFER,0); }

        void setStorage(GLenum internalformat, int width, int height)
        {
            glRenderbufferStorage(GL_RENDERBUFFER,internalformat,width,height);
        }

    }; //end of class RBO. //render buffer object.

    class FBO : public GLObj
    {
    public:
        enum Filtering
        {
            GOOD,
            BAD
        }; //end of enum Filtering.
    public:
        FBO(GLenum target_framebuffer, GLuint active_texture, Filtering f, int width, int height)
            : GLObj(), m_TargetFramebuffer(target_framebuffer), m_CurrentTexture(active_texture), m_RBO()
        {
            glGenFramebuffers(1,&id());

            glActiveTexture(m_CurrentTexture);
            glGenTextures(1,&m_iRenderedTexture);
            
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,0);
            adjustTexParameters(f);

            m_RBO.bind();
            m_RBO.setStorage(GL_DEPTH_COMPONENT,width,height);
            glFramebufferRenderbuffer(m_TargetFramebuffer,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_RBO.id());

            glFramebufferTexture(m_TargetFramebuffer, GL_COLOR_ATTACHMENT0,m_iRenderedTexture,0);
            
            GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT};
            glDrawBuffers(2, DrawBuffers);

            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Problem when setting up FBO." << std::endl;
        }
        ~FBO()
        {
            glDeleteFramebuffers(1,&id());
            glDeleteTextures(1,&m_iRenderedTexture);
        }

        void bind() 
        {
            glBindFramebuffer(m_TargetFramebuffer,id());
            glActiveTexture(m_CurrentTexture);
            glBindTexture(GL_TEXTURE_2D,m_iRenderedTexture);
            m_RBO.bind();
        }

        void unbind()
        {
            glBindFramebuffer(m_TargetFramebuffer,0);
            glBindTexture(GL_TEXTURE_2D,0);
            m_RBO.unbind();
        }

        void adjustTexParameters(Filtering f)
        {
            if(f == GOOD)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else if(f == BAD)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
        }

    private:
        GLuint m_iRenderedTexture;
        GLuint m_CurrentTexture; //GL_TEXTURE0,GL_TEXTURE1,etc.
        GLenum m_TargetTexture;
        GLenum m_TargetFramebuffer;
        RBO m_RBO;
    }; //end of class FBO.
}// end of namespace gl.

#endif
