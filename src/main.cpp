/*
    Bruno Romero de Azevedo
    2910098

    Simples Relat�rio.

            Este trabalho foi realizado utilizando a vers�o 3.3 do OpenGL. Sendo assim, n�o se encontra em parte alguma
        chamadas a fun��es como gluPerspective ou gluLookAt, tudo � feito utilizando-se VAO's, VBO's, FBO's e afins. Tendo, claro,
        sua utiliza��o nos programas de shader.
            Este programa cont�m algumas caracter�sticas. A primeira � que quando for executado, o usu�rio vai se sentir como se estivesse
        mexendo a c�mera da cena diretamente. Por�m, isto n�o ocorre. O que acontece � que a cena foi colocada exatamente em frente ao
        "personagem" do usu�rio, que no caso n�o passa de um cubo (assim como os outros que podem ser encontrados no cen�rio). Isso
        foi feito para simular um jogo FPS. � bom ter explicado isso, pois pode-se achar que a rota��o da c�mera est� errada pois ela
        n�o gira em torno de si pr�pria, mas gira sempre com o personagem do usu�rio, para sempre ficar na sua posi��o frontal.
            O programa permite ao usu�rio fazer algumas coisas: andar para frente e para tr�s, rotacionar o personagem para a esquerda e
        para a direita, rotacionar a c�mera (aqui sim, � apenas a c�mera que rotaciona) para cima e para baixo, sendo que sua rota��o m�xima
        tanto para cima quanto para baixo � de 90 graus. Tamb�m � poss�vel ao usu�rio dar zoom no que est� sendo visualizado pela escopeta
        (este zoom tamb�m foi limitado), al�m disso pode-se aumentar ou diminuir o di�metro da escopeta.
            Algumas t�cnicas foram utilizadas neste programa. Uma delas foi de deixar com um Blur a parte que n�o est� sendo visualizado pela
        escopeta. Usou-se um bump mapping para a textura do ch�o (foi criado um "sol" que faz ficar mais escuro e mais claro apenas para visualizar
        o bump mapping. Essa funcionalidade pode ser desabilitada). As rota��es foram feitas utilizando-se a t�cnica de eixo arbitr�rio.

            Para fazer com que apenas a parte de dentro da escopeta ficasse sem blur e podendo-se dar zoom foi utilizada a seguinte t�cnica.
        Em cada render, a cena � renderizada para uma textura e essa mesma textura � ent�o desenhada na tela. Por�m, utilizando-se dos shaders,
        � aplicado o efeito de blur nele e os pixels que fazem parte da escopeta s�o descartados (pixels que est�o a uma certa dist�ncia do 
        centro da textura). Sendo assim, a textura � desenhada na tela, mas fica com um buraco no meio onde devia estar a cena n�tida
        e com ou sem zoom. Para preencher este buraco, simplesmente � mandado que a cena seja renderizada normalmente logo ap�s a renderiza��o
        da textura na tela (sendo a cena recriada com o valor do FOV da matriz de proje��o alterado para o zoom desejado).
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

	// Escolhe vers�o do OpenGL
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	// Remover funcionalidades deprecadas
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// Contexto de debug
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	// Remove funcionalidades deprecadas. (PS: Devido a um bug essa fun��o trava no GLFW 2.7.
	// Caso esteja interessado me pe�a uma vers�o corrigida, e o bug tamb�m deve estar arrumado na pr�xima vers�o.)
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Janela n�o redimensionavel
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	// Abre janela, WINDOW_WIDTHxWINDOW_HEIGHT, 8 bits de cor, 8 bits de alpha, 24 bits de depth, 8 bits de stencil
	glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);

	// gl3w � uma mini-biblioteca que inicializa todas as fun��es, e apenas as fun��es presentes no OpenGL 3.3 e 4.1
	// Inicializa a gl3w
	if (gl3wInit() != 0)
	{
		std::cerr << "Failed to initialize gl3w." << std::endl;
		exit(1);
	}
	// Verifica se OpenGL 3.3 � suportado
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
    // Se dispon�vel, registra a callback de erro, que vai imediatamente notificar se ocorrer
	// algum erro em uma chamada OpenGL sem que n�s precisemos verificar isso manualmente.
	if (glDebugMessageCallbackARB) 
    {
		// Habilita debug sincrono: erros s�o sinalizados na fun��o em que ocorrem
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

void help()
{
    std::cout << "SNIPER - Bruno Romero de Azevedo." << std::endl;
    std::cout << "                     OpenGL 3.3  " << std::endl;
    std::cout << "Comandos:                        " << std::endl;
    std::cout << "I/K move o player para frente e para tras." << std::endl;
    std::cout << "J/L rotaciona para esquerda/direita." << std::endl;
    std::cout << "P/U rotaciona para cima/baixo." << std::endl;
    std::cout << "V/B aumenta/diminui o zoom da sniper." << std::endl;
    std::cout << "RSHIFT/RCTRL aumenta/diminui o diametro da sniper." << std::endl;
    std::cout << "F para dar toggle na variacao no chao." << std::endl;
}

int main()
{
	initGLFW();
    initOpenGL();

    setCallBacks();
    
    help();

    util::MATRIXSTACK->setProjection(math::perspective(45.f,800.f/600.f,0.1f,5000.f));
    CONTROLLER->run();

    clean();
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

