/*
    Classe que lida com toda a renderização da cena. Tanto na textura quanto na tela.
*/
#ifndef _CGA_T2_TEXTURED_SCENE_HPP_
#define _CGA_T2_TEXTURED_SCENE_HPP_

class Controller;

#include <iostream>
#include <cstdlib>
#include "macros.h"
#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"
#include "util/MatrixStack.h"
#include "Controller.h"
#include "util/CommonShader.hpp"

#include "gl/GLObjects.hpp"


class TexturedScene
{
public:
    static TexturedScene *instance()
    {
        if(m_sInstance == NULL)
        {
            m_sInstance = new TexturedScene();
        }
        return m_sInstance;
    }

    ~TexturedScene()
    {
        delete m_pShader;
        glDeleteFramebuffers(1,&m_iFBO);
        glDeleteTextures(1,&m_iRenderedTexture);
        glDeleteRenderbuffers(1,&m_iDepthRenderBuffer);
    }

    void initVBO()
    {
        m_VAO.bind();

        struct vertex_data d[4] =
        {
            /*    Pos     */  /* UV */
            {-1.f,-1.f, .0f,  .0f,.0f},
            {-1.f, 1.f, .0f,  .0f,1.f},
            { 1.f, 1.f, .0f,  1.f,1.f},
            { 1.f, -1.f,.0f,  1.f,.0f}
        };

        m_VBO.bind(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data)*4,&d[0],GL_STATIC_DRAW);
                
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_data),(void*)offsetof(vertex_data, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vertex_data),(void*)offsetof(vertex_data, uv));
    }

    void onUpdate()
    {
        if(glfwGetKey('V') == GLFW_PRESS)
        {
            m_fZoom += 1.f;
            if(m_fZoom > 45.f)
                m_fZoom = 45.f;
        }
        else if(glfwGetKey('B') == GLFW_PRESS)
        {
            m_fZoom -= 1.f;
            if(m_fZoom < 5.f)
                m_fZoom = 5.f;
        }

        if(glfwGetKey(GLFW_KEY_RCTRL) == GLFW_PRESS)
        {
            m_fScopeDiameter += 0.01f;
            if(m_fScopeDiameter > .5f)
                m_fScopeDiameter = .5f;
        }
        else if(glfwGetKey(GLFW_KEY_RSHIFT) == GLFW_PRESS)
        {
            m_fScopeDiameter -= 0.01f;
            if(m_fScopeDiameter < .05f)
                m_fScopeDiameter = .05f;
        }
    }

    void renderScene()
    {

        renderSceneToTexture();
        renderSceneToScreen();
    }

    void renderSceneToTexture()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,m_iFBO);
        glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        util::MATRIXSTACK->setProjection(math::perspective(math::degreeToRad(45.f),WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,5000.f));
        CONTROLLER->drawScene();
    }

    void renderSceneToScreen()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_pShader->setActive(true);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_iRenderedTexture);

        glUniform1i(m_loc_u_renderedTexture,0);
        glUniform1f(m_loc_u_scope_diameter,m_fScopeDiameter);
        
        GLubyte indices[] = { 0,1,2, 2,3,0 };
        
        m_VAO.bind();
        m_IBO.bind(GL_ELEMENT_ARRAY_BUFFER);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, indices, GL_STATIC_DRAW);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        
        util::MATRIXSTACK->setProjection(math::perspective(math::degreeToRad(m_fZoom),WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,5000.f));
        CONTROLLER->drawScene();

        m_pShader->setActive(false);
    }

    GLuint renderedTexture() { return m_iRenderedTexture; }
    gl::VAO vao() { return m_VAO; }
    gl::VBO ibo() { return m_IBO; }

    Glsl *shader() { return m_pShader; }

private:
    TexturedScene()
        : m_pShader(new Glsl("res/scene.vert", "res/scene.frag")), m_fZoom(45.f),m_VBO(),m_VAO(),m_IBO(), m_fScopeDiameter(.25f)
    {
        glGenFramebuffers(1,&m_iFBO);
        glBindFramebuffer(GL_FRAMEBUFFER,m_iFBO);
        
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1,&m_iRenderedTexture);
        glBindTexture(GL_TEXTURE_2D,m_iRenderedTexture);

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,WINDOW_WIDTH,WINDOW_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,0);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_iRenderedTexture,0);
        glGenRenderbuffers(1,&m_iDepthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER,m_iDepthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,WINDOW_WIDTH,WINDOW_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_iDepthRenderBuffer);

        glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,m_iRenderedTexture,0);

        GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT};
        glDrawBuffers(2, DrawBuffers);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Problem when setting up FBO." << std::endl;

        m_loc_u_renderedTexture = m_pShader->getUniformLoc("renderedTexture");
        m_loc_u_scope_diameter = m_pShader->getUniformLoc("scopeDiameter");
        initVBO();
    }

private:
    struct vertex_data
    {
        float pos[3];
        float uv[2];
    };

private:
    static TexturedScene *m_sInstance;

    GLuint m_iFBO;
    GLuint m_iRenderedTexture;
    GLuint m_iDepthRenderBuffer;
    gl::VAO m_VAO;
    gl::VBO m_VBO;
    gl::VBO m_IBO;

    Glsl *m_pShader;
    GLuint m_loc_u_renderedTexture;
    GLuint m_loc_u_scope_diameter;
    float m_fZoom;
    float m_fScopeDiameter;
};

#endif