#include "Controller.h"

#include "GL/glfw.h"

#include "util/MatrixStack.h"
#include "macros.h"

Controller *Controller::m_sInstance = NULL;

Controller::Controller()
    : m_Camera(math::vector3f(0.f,5.f,5.f), math::vector3f(0.f,5.f,0.f), math::vector3f(0.f,1.f,0.f)), 
      m_pGround(new model::Ground(100.f, "res/ground1.vert", "res/ground1.frag","res/normal_2.bmp")),
      m_pPlayer(new model::Player()), m_fYaw(0.f)
{
    m_Camera.setSpeed(.05f);
    /* cria a textura da cena. */
}

Controller::~Controller()
{
    delete m_pGround;
    delete m_pPlayer;
}

Controller *Controller::instance()
{
    if(m_sInstance == NULL)
        m_sInstance = new Controller();
    return m_sInstance;
}

void Controller::run()
{
    double last_render = 0;
    double last_update = 0;

    double frame_interval = 1.f/60.f;
    double update_interval = 1.f/90.f;

    double cur_time = 0;
    while(glfwGetWindowParam(GLFW_OPENED) && glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS)
    {
        cur_time = glfwGetTime();
        if(cur_time - last_update > update_interval)
        {
            onUpdate();
            last_update = glfwGetTime();
        }
        if(cur_time - last_render > frame_interval) 
        {
            last_render = glfwGetTime();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            onRender();
            glfwSwapBuffers();
        }
        glfwSleep(update_interval - (cur_time + glfwGetTime()));
    }
}

void Controller::keyEvent(int key, int state)
{
}

void Controller::mouseMoved(int x, int y)
{
}

void Controller::onUpdate()
{
    cameraMove();
    m_pPlayer->onUpdate();
}

void Controller::onRender()
{
    float v = 195.f/255.f;
    glClearColor(v,v,v,1.f);
    
    util::MATRIXSTACK->pushMatrix();
        util::MATRIXSTACK->loadIdentity();
        
        math::Vector3 pos = m_pPlayer->pos();
        pos[1] += .3f;
        math::Vector3 dir = m_pPlayer->dir();

        //util::MATRIXSTACK->transform(m_Camera.lookAt(pos+(dir*2.f),pos+(dir*3.f),math::vector3f(0.f,1.f,0.f)));
        util::MATRIXSTACK->translate(m_Camera.eye()*-1.f);
        util::MATRIXSTACK->scale(math::Vector3(10.f));
        util::MATRIXSTACK->transform(math::rotate(m_fYaw, m_Camera.up()));

        m_pGround->onRender();
        m_pPlayer->onRender();
    util::MATRIXSTACK->popMatrix();
}

void Controller::cameraRotate()
{
}

void Controller::cameraMove()
{
    float ang = math::degreeToRad(1.f);
    if(glfwGetKey('W') == GLFW_PRESS)
        m_Camera.moveForward();
    else if(glfwGetKey('S') == GLFW_PRESS)
        m_Camera.moveBackwards();

    if(glfwGetKey('A') == GLFW_PRESS)
    {
        m_fYaw -= ang;
        m_Camera.transform(math::rotate(m_fYaw, m_Camera.up()));
    }
    else if(glfwGetKey('D') == GLFW_PRESS)
    {
        m_fYaw += ang;
        m_Camera.transform(math::rotate(m_fYaw, m_Camera.up()));
    }

    if(glfwGetKey('Z') == GLFW_PRESS)
        m_Camera.moveDown();
    else if(glfwGetKey('X') == GLFW_PRESS)
        m_Camera.moveUp();
}
