/*
    Classe utilizada para representar o jogador na aplicação.
    Foi também utilizada para posicionar outros elementos na cena (os outros cubos). Mas que só estão lá
    para serem desenhados.
*/

#ifndef _CGA_T2_PLAYER_HPP_
#define _CGA_T2_PLAYER_HPP_

#include "model/ShaderModel.hpp"
#include "macros.h"
#include "util/MatrixStack.h"
#include "math/math_aux.hpp"

#include <iostream>
#include "util/CommonShader.hpp"

namespace model
{
    class Player : public ShaderModel
    {
    public:
        Player()
            : ShaderModel("res/player.vert", "res/player.frag"), m_fAngle(0.f),m_bSniperOn(false)
        {
            adjustVertexAttribs(COLORVERT);
            setDir(math::vector3f(0.f,0.f,-1.f));
        }
        
        void render() 
        {
            util::setMatrices(m_pShader);

            m_VBO.bind(GL_ARRAY_BUFFER);
            setBufferData();
            m_VBO.unbind();

            GLubyte indices[] = { 
                                  0,1,2, 2,3,0,
                                  0,1,5, 5,4,0,
                                  1,2,6, 6,5,1,
                                  2,3,7, 7,6,2,
                                  3,0,4, 4,7,3,
                                  4,5,6, 6,7,4
                                };
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
        }

        void setBufferData() 
        {
            float r = 10.f;
            float s = 1.f;
            float x = pos()[0];
            float y = pos()[1]+s;
            float z = pos()[2];
                                
            float ang = m_fAngle;
            math::Vector3 v = math::transform(math::vector3f(-1,1,-1),math::rotate(ang,0,1,0));
            math::Vector3 v2 = math::transform(math::vector3f(1,1,-1),math::rotate(ang,0,1,0));
            math::Vector3 v3 = math::transform(math::vector3f(1,1,1),math::rotate(ang,0,1,0));
            math::Vector3 v4 = math::transform(math::vector3f(-1,1,1),math::rotate(ang,0,1,0));

            v[1] = s;
            struct ColorVert d[8] = {
                    { v[0]+x ,  y+v[1], v[2]+z, 1.f,  1.f,1.f,1.f,1.f  },
                    {  v2[0]+x ,  y+v[1], v2[2]+z, 1.f,  1.f,1.f,0.f,1.f  },
                    {  v3[0]+x , y+v[1],  v3[2]+z, 1.f,  1.f,0.f,1.f,1.f  },
                    { v4[0]+x , y+v[1],  v4[2]+z, 1.f,  1.f,0.f,0.f,1.f  },

                    { v[0]+x ,  y-v[1], v[2]+z, 1.f,  0.f,1.f,1.f,1.f  },
                    {  v2[0]+x ,  y-v[1], v2[2]+z, 1.f,  0.f,1.f,0.f,1.f  },
                    {  v3[0]+x , y-v[1],  v3[2]+z, 1.f,  0.f,0.f,1.f,1.f  },
                    { v4[0]+x , y-v[1],  v4[2]+z, 1.f,  0.f,0.f,0.f,1.f }
            };

            glBufferData(GL_ARRAY_BUFFER, sizeof(ColorVert)*8, &d[0], GL_STATIC_DRAW);
        }

        void onUpdate() 
        {
            if(glfwGetKey('I') == GLFW_PRESS)
            {
                setSpeed(.3f);
                move();
            }
            else if(glfwGetKey('K') == GLFW_PRESS)
            {
                setSpeed(-.3f);
                move();
            }
            
            if(glfwGetKey('J') == GLFW_PRESS)
            {
                m_fAngle += math::degreeToRad(1.f);
                setDir(math::normalize(math::transform(dir(),math::rotate(math::degreeToRad(1.f),0,1,0))));
            }
            else if(glfwGetKey('L') == GLFW_PRESS)
            {
                m_fAngle += math::degreeToRad(-1.f);
                setDir(math::normalize(math::transform(dir(),math::rotate(math::degreeToRad(-1.f),0,1,0))));
            }
        }

        void onKeyEvent(int key, int state)
        {
            if(state == GLFW_PRESS)
            {
                if(key == GLFW_KEY_SPACE)
                    m_bSniperOn = !m_bSniperOn;
            }
        }

        bool isSniperOn() { return m_bSniperOn; }
        int sniperMode()
        {
            int mode = 0;
            if(!m_bSniperOn)
                mode = 1;
            return mode;
        }


    private:
        float m_fAngle;
        bool m_bSniperOn;
    }; //end of class Player.
} //end of namespace model.

#endif
