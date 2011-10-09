#ifndef _CGA_T2_SHADER_MODEL_HPP_
#define _CGA_T2_SHADER_MODEL_HPP_

#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"

#include <iostream>
#include <cstdlib>

namespace model
{
    class ShaderModel
    {
    public:
        ShaderModel(char *vert_shader, char *frag_shader)
            : m_pShader(new Glsl(vert_shader, frag_shader))
        {
            glGenVertexArrays(1, &m_iVAOID);
            glBindVertexArray(m_iVAOID);
            
            m_loc_u_projection = m_pShader->getUniformLoc("projection");
            m_loc_u_modelview = m_pShader->getUniformLoc("modelview");
        }
        
        ~ShaderModel()
        {
            delete m_pShader;
            glDeleteBuffers(1, &m_iVBOID);
            glDeleteVertexArrays(1, &m_iVAOID);
        }

        void onRender()
        {
            m_pShader->setActive(true);
                beforeRender();

                glUniformMatrix4fv(m_loc_u_projection, 1, GL_TRUE, util::MATRIXSTACK->projection().elements());
                glUniformMatrix4fv(m_loc_u_modelview, 1, GL_TRUE, util::MATRIXSTACK->top().elements());
                
                
                glBindVertexArray(m_iVAOID);
                render();
                glBindVertexArray(0);

                afterRender();
            m_pShader->setActive(false);
        }


        virtual void beforeRender() {}
        virtual void afterRender() {}

        virtual void render() = 0; //renderizacao especifica do modelo.
        virtual void initVBO() = 0;

    protected:
        Glsl *m_pShader;
        GLuint m_iVAOID;
        GLuint m_iVBOID;
        
        GLint m_loc_u_projection;
        GLint m_loc_u_modelview;
    }; //end of class ShaderModel.
} //end of namespace model.

#endif
