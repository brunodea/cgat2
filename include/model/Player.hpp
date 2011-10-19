#ifndef _CGA_T2_PLAYER_HPP_
#define _CGA_T2_PLAYER_HPP_

#include "model/ShaderModel.hpp"
#include "macros.h"
#include "math/math_aux.hpp"

#include <iostream>

namespace model
{
    class Player : public ShaderModel
    {
    public:
        Player()
            : ShaderModel("res/player.vert", "res/player.frag"), m_fAngle(0.f)
        {
            setDir(math::vector3f(0.f,0.f,-1.f));
        }
        
        void render() 
        {
            initVBO();
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

        void initVBO() 
        {
            glGenVertexArrays(1, &m_iVAOID);
                glBindVertexArray(m_iVAOID);

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
                struct vertex_data d[8] = {
                        { v[0]+x ,  y+v[1], v[2]+z, 1.f,  1.f,1.f,1.f,1.f  },
                        {  v2[0]+x ,  y+v[1], v2[2]+z, 1.f,  1.f,1.f,0.f,1.f  },
                        {  v3[0]+x , y+v[1],  v3[2]+z, 1.f,  1.f,0.f,1.f,1.f  },
                        { v4[0]+x , y+v[1],  v4[2]+z, 1.f,  1.f,0.f,0.f,1.f  },

                        { v[0]+x ,  y-v[1], v[2]+z, 1.f,  0.f,1.f,1.f,1.f  },
                        {  v2[0]+x ,  y-v[1], v2[2]+z, 1.f,  0.f,1.f,0.f,1.f  },
                        {  v3[0]+x , y-v[1],  v3[2]+z, 1.f,  0.f,0.f,1.f,1.f  },
                        { v4[0]+x , y-v[1],  v4[2]+z, 1.f,  0.f,0.f,0.f,1.f }
                };
            
                m_Data = &d[0];
            
                glGenBuffers(1, &m_iVBOID);
                glBindBuffer(GL_ARRAY_BUFFER, m_iVBOID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data)*8, m_Data, GL_STATIC_DRAW);
            
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data),  (char*)0 + offsetof(vertex_data, pos));
		        glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(vertex_data),  (char*)0 + offsetof(vertex_data, color));
		        glEnableVertexAttribArray(1);
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
            else if(glfwGetKey('J') == GLFW_PRESS)
            {
                m_fAngle += math::degreeToRad(1.f);
                setDir(math::transform(dir(),math::rotate(math::degreeToRad(1.f),0,1,0)));
            }
            else if(glfwGetKey('L') == GLFW_PRESS)
            {
                m_fAngle += math::degreeToRad(-1.f);
                setDir(math::transform(dir(),math::rotate(math::degreeToRad(-1.f),0,1,0)));
            }
        }

        void onKeyEvent(int key, int state) {}

    private:
        struct vertex_data
        {
            float pos[4]; //x,y,z,w
            float color[4];
        };
    private:
        struct vertex_data *m_Data;
        float m_fAngle;
    }; //end of class Player.
} //end of namespace model.

#endif
