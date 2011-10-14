#ifndef _CGA_T2_SHADER_MODEL_HPP_
#define _CGA_T2_SHADER_MODEL_HPP_

#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "model/GameObject.hpp"

namespace model
{
    class ShaderModel : public GameObject
    {
    public:
        ShaderModel(char *vert_shader, char *frag_shader)
            : GameObject(), m_pShader(new Glsl(vert_shader, frag_shader)), m_fAngle(4.25f)
        {
            glGenVertexArrays(1, &m_iVAOID);
            glBindVertexArray(m_iVAOID);
            
            m_loc_u_projection = m_pShader->getUniformLoc("projection");
            m_loc_u_modelview = m_pShader->getUniformLoc("modelview");
            m_loc_u_sunlight_pos = m_pShader->getUniformLoc("sunlightpos");
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

                //m_fAngle = (m_fAngle > 1000000.f) ? 4.5f : m_fAngle + .01f;

                float s = sin((double)m_fAngle);
                float c = cos((double)m_fAngle);

                glUniform3f(m_loc_u_sunlight_pos, c,s,c);
                
                glBindVertexArray(m_iVAOID);
                render();
                glBindVertexArray(0);

                afterRender();
            m_pShader->setActive(false);
        }

        virtual void beforeRender() {}
        virtual void afterRender() {}

        virtual void render() = 0; //renderizacao especifica do modelo.
        virtual void initVBO() = 0; //inicializacao de vertices especifica do modelo.

        virtual void onUpdate() = 0;
        virtual void onKeyEvent(int key, int state) = 0;

    protected:
        Glsl *m_pShader;
        GLuint m_iVAOID;
        GLuint m_iVBOID;
        
        GLint m_loc_u_projection;
        GLint m_loc_u_modelview;
        GLint m_loc_u_sunlight_pos;

    private:
        float m_fAngle;
    }; //end of class ShaderModel.
} //end of namespace model.

#endif
