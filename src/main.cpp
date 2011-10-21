/*
    Bruno Romero de Azevedo
    2910098
 */
#include <cstdlib>
#include <iostream>

#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include "GL/glsl.h"

#include "macros.h"
#include "Controller.h"
#include "TexturedScene.hpp"
#include "util/MatrixStack.h"
#include "math/matrix_functions.hpp"


// Callback de mensagens de debug
void APIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	std::cout << message << std::endl;
}

void initGLFW()
{
	// Inicializa a GLFW
	if (glfwInit() != GL_TRUE)
	{
		std::cerr << "Failed to initialized GLFW." << std::endl;
		exit(1);
	}

	// Escolhe versão do OpenGL
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	// Remover funcionalidades deprecadas
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// Contexto de debug
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	// Remove funcionalidades deprecadas. (PS: Devido a um bug essa função trava no GLFW 2.7.
	// Caso esteja interessado me peça uma versão corrigida, e o bug também deve estar arrumado na próxima versão.)
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Janela não redimensionavel
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	// Abre janela, WINDOW_WIDTHxWINDOW_HEIGHT, 8 bits de cor, 8 bits de alpha, 24 bits de depth, 8 bits de stencil
	glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);

	// gl3w é uma mini-biblioteca que inicializa todas as funções, e apenas as funções presentes no OpenGL 3.3 e 4.1
	// Inicializa a gl3w
	if (gl3wInit() != 0)
	{
		std::cerr << "Failed to initialize gl3w." << std::endl;
		exit(1);
	}
	// Verifica se OpenGL 3.3 é suportado
	else if (!gl3wIsSupported(3, 3))
	{
		std::cerr << "OpenGL 3.3 not supported." << std::endl;
		exit(1);
	}
}

/*
 * Callback functions
 */
void GLFWCALL keyEventCallback(int key, int state)
{
    CONTROLLER->keyEvent(key, state);
}

void GLFWCALL mousePosCallback(int x, int y)
{
    CONTROLLER->mouseMoved(x,y);
}

void GLFWCALL handleResize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void setCallBacks()
{
    glfwSetKeyCallback(keyEventCallback);
    glfwSetMousePosCallback(mousePosCallback);
    glfwSetWindowSizeCallback(handleResize);
    // Se disponível, registra a callback de erro, que vai imediatamente notificar se ocorrer
	// algum erro em uma chamada OpenGL sem que nós precisemos verificar isso manualmente.
	if (glDebugMessageCallbackARB) 
    {
		// Habilita debug sincrono: erros são sinalizados na função em que ocorrem
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		// Configura filtro para aceitar qualquer mensagem
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
		// Registra callback
		glDebugMessageCallbackARB(debug_callback, 0);
	}
}

void initOpenGL()
{
    handleResize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
 //   glEnable(GL_CULL_FACE);

    float v = 172.f/255.f;
    glClearColor(v,v,v,1.f);
}

void clean()
{
    delete util::MATRIXSTACK;
    delete CONTROLLER;
    delete TEXTUREDSCENE;
}

int main()
{
	initGLFW();
    initOpenGL();

    setCallBacks();

    util::MATRIXSTACK->setProjection(math::perspective(45.f,800.f/600.f,0.1f,5000.f));
    CONTROLLER->run();

    clean();
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

