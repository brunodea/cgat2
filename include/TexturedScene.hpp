#ifndef _CGA_T2_TEXTURED_SCENE_HPP_
#define _CGA_T2_TEXTURED_SCENE_HPP_

#include "macros.h"
#include "model/TexturedModel.hpp"
#include "model/ShaderModel.hpp"
#include "GL/glfw.h"

class Controller;
typedef void (Controller::*ControllerFuncPtr)(void);

class TexturedScene : public model::TexturedModel, model::ShaderModel
{
public:
    TexturedScene()
        : model::TexturedModel(WINDOW_WIDTH, WINDOW_HEIGHT, GL_TEXTURE1),
          model::ShaderModel("res/player.vert","res/player.frag")
    { initVBO(); }

    void beforeRender()
    {
        renderTexture();
    }

    void afterRender()
    {
        afterRenderTexture();
    }

    void onRender()
    {
        m_pShader->setActive(true);
            beforeRender();

            glUniformMatrix4fv(m_loc_u_projection, 1, GL_TRUE, util::MATRIXSTACK->projection().elements());
            glUniformMatrix4fv(m_loc_u_modelview, 1, GL_TRUE, util::MATRIXSTACK->top().elements());

            //m_fAngle = (m_fAngle > 1000000.f) ? 4.5f : m_fAngle + .01f;

            float s = sin((double)0.5f);
            float c = cos((double)0.789f);

            glUniform3f(m_loc_u_sunlight_pos, c,s,c);
                
            glBindVertexArray(m_iVAOID);
            render();
            glBindVertexArray(0);

            afterRender();
        m_pShader->setActive(false);
    }

    void render()
    {
        GLubyte indices[] = { 0,1,2,
                              2,3,0 };
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
    }

    void setTexture(Controller *c, ControllerFuncPtr fptr)
    {
        glDrawBuffer(GL_BACK);
        glReadBuffer(GL_BACK);

        glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);

        (c->*fptr)();

        glFlush();

        glBindTexture(GL_TEXTURE_2D, m_iTextureID);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0); //Tambem pode ser usado: glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void initVBO()
    {
        glGenVertexArrays(1, &m_iVAOID);
        glBindVertexArray(m_iVAOID);

        float r = 10.f;
        float w = WINDOW_WIDTH;
        float h = WINDOW_HEIGHT;

        struct vertex_data d[4] = {
                { 0,0 },
                { 0,h },
                { w,h },
                { w,0 }
        };
            
        m_Data = &d[0];
            
        initTOB(1);

        glGenBuffers(1, &m_iVBOID);
        glBindBuffer(GL_ARRAY_BUFFER, m_iVBOID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data)*4, m_Data, GL_STATIC_DRAW);
            
		glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data), 0);
    }

    void onUpdate() {}
    void onKeyEvent(int key, int state) {}

private:
    struct vertex_data
    {
        float pos[2]; //x,y
    };
private:
    struct vertex_data *m_Data;
};

#endif
