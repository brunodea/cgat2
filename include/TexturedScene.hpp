#ifndef _CGA_T2_TEXTURED_SCENE_HPP_
#define _CGA_T2_TEXTURED_SCENE_HPP_

#include <iostream>
#include <cstdlib>
#include "macros.h"
#include "GL/glfw.h"
#include "GL/glsl.h"
#include "util/MatrixStack.h"

#include "gl/GLObjects.hpp"
#include "model/VertexStructs.hpp"
#include "model/ShaderModel.hpp"

class Controller;

typedef void (Controller::*ControllerFuncPtr)(void);

class TexturedScene : public model::ShaderModel
{
public:
    TexturedScene()
        : ShaderModel("res/scene.vert", "res/scene.frag"), m_fZoom(45.f),
          m_FBO(GL_FRAMEBUFFER,GL_TEXTURE1,gl::FBO::GOOD,WINDOW_WIDTH,WINDOW_HEIGHT),
          m_VAO(),m_VBO(),m_IBO()
    {  
        //glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,WINDOW_WIDTH,WINDOW_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,0);      

        m_loc_u_renderedTexture = m_pShader->getUniformLoc("renderedTexture");
        adjustVertexAttribs(model::TEXVERT);
    }

    ~TexturedScene()
    {
        delete m_pShader;
    }

    void onUpdate() 
    {
        if(glfwGetKey('V') == GLFW_PRESS)
        {
            m_fZoom += 1.f;
           // std::cout << "FOV: " << m_fZoom << std::endl;
        }
        else if(glfwGetKey('B') == GLFW_PRESS)
        {
            m_fZoom -= 1.f;
          //  std::cout << "FOV: " << m_fZoom << std::endl;
        }
    }

    //useless functions.
    void render() {}
    void onKeyEvent(int key, int state) {}

    void setBufferData()
    {
        struct model::TexVert d[4] =
        {
            /*    Pos     */  /* UV */
            {-1.f,-1.f, .0f,  .0f,.0f},
            {-1.f, 1.f, .0f,  .0f,1.f},
            { 1.f, 1.f, .0f,  1.f,1.f},
            { 1.f, -1.f,.0f,  1.f,.0f}
        };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model::TexVert)*4, &d[0], GL_STATIC_DRAW);
    }

    void renderTexture(Controller *ctrl, ControllerFuncPtr pt)
    {
        m_FBO.bind();
        glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        util::MATRIXSTACK->setProjection(math::perspective(math::degreeToRad(45.f),WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,5000.f));
        (ctrl->*pt)();

        m_FBO.unbind();
        glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_pShader->setActive(true);
        
        m_FBO.bind();

        glUniform1i(m_loc_u_renderedTexture,0);
        
        GLubyte indices[] = { 0,1,2, 2,3,0 };
        
        m_VAO.bind();
        m_IBO.bind(GL_ELEMENT_ARRAY_BUFFER);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, indices, GL_STATIC_DRAW);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        
        m_VAO.unbind();
        m_IBO.unbind();

        util::MATRIXSTACK->setProjection(math::perspective(math::degreeToRad(m_fZoom),WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,5000.f));
        (ctrl->*pt)();

        m_pShader->setActive(false);
    }

private:
    gl::FBO m_FBO;
    gl::VAO m_VAO;
    gl::VBO m_VBO;
    gl::VBO m_IBO;

    Glsl *m_pShader;
    GLuint m_loc_u_renderedTexture;
    float m_fZoom;
};

#endif
