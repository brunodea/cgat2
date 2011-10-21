#ifndef _CGA_T2_SHADER_MODEL_HPP_
#define _CGA_T2_SHADER_MODEL_HPP_

#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "model/GameObject.hpp"
#include "gl/GLObjects.hpp"
#include "model/VertexStructs.hpp"

namespace model
{
    class ShaderModel : public GameObject
    {
    public:
        ShaderModel(char *vert_shader, char *frag_shader)
            : GameObject(), m_pShader(new Glsl(vert_shader, frag_shader)), m_fAngle(4.25f),
              m_VAO(), m_VBO()
        {
        }
        
        ~ShaderModel()
        {
            delete m_pShader;
        }

        virtual void onRender()
        {
            m_pShader->setActive(true);
                beforeRender();
                
                m_VAO.bind();
                render();
                m_VAO.unbind();

                afterRender();
            m_pShader->setActive(false);
        }

        virtual void beforeRender() {}
        virtual void afterRender() {}

        virtual void render() = 0; //renderizacao especifica do modelo.
       
        virtual void onUpdate() = 0;
        virtual void onKeyEvent(int key, int state) = 0;

        virtual void setBufferData() = 0;

    protected:
        void adjustVertexAttribs(VertexStruct vs)
        {
            m_VAO.bind();
            m_VBO.bind(GL_ARRAY_BUFFER);
                
            setBufferData(); 
            
            switch(vs)
            {
            case POSVERT:
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(PosVert),(void*)offsetof(PosVert, pos));

                break;
            case TEXVERT:
                
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(TexVert),(void*)offsetof(TexVert, pos));

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(TexVert),(void*)offsetof(TexVert, uv));

                break;
            case COLORVERT:
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(ColorVert),(void*)offsetof(ColorVert, pos));
                
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(ColorVert),(void*)offsetof(ColorVert, color));

                break;
            default:
                break;
            }
            m_VBO.unbind();
            m_VAO.unbind();
        }
    protected:
        Glsl *m_pShader;
        gl::VAO m_VAO;
        gl::VBO m_VBO;

    private:
        float m_fAngle;
    }; //end of class ShaderModel.
} //end of namespace model.

#endif
