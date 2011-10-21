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
        /*
        virtual void bind() {};
        virtual void bind(GLenum target) {};
        virtual void unbind() {};
*/
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
}// end of namespace gl.

#endif
