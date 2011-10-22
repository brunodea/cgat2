/*
    Bruno Romero de Azevedo
    2910098

    Simples Relatório.

            Este trabalho foi realizado utilizando a versão 3.3 do OpenGL. Sendo assim, não se encontra em parte alguma
        chamadas a funções como gluPerspective ou gluLookAt, tudo é feito utilizando-se VAO's, VBO's, FBO's e afins. Tendo, claro,
        sua utilização nos programas de shader.
            Este programa contém algumas características. A primeira é que quando for executado, o usuário vai se sentir como se estivesse
        mexendo a câmera da cena diretamente. Porém, isto não ocorre. O que acontece é que a cena foi colocada exatamente em frente ao
        "personagem" do usuário, que no caso não passa de um cubo (assim como os outros que podem ser encontrados no cenário). Isso
        foi feito para simular um jogo FPS. É bom ter explicado isso, pois pode-se achar que a rotação da câmera está errada pois ela
        não gira em torno de si própria, mas gira sempre com o personagem do usuário, para sempre ficar na sua posição frontal.
            O programa permite ao usuário fazer algumas coisas: andar para frente e para trás, rotacionar o personagem para a esquerda e
        para a direita, rotacionar a câmera (aqui sim, é apenas a câmera que rotaciona) para cima e para baixo, sendo que sua rotação máxima
        tanto para cima quanto para baixo é de 90 graus. Também é possível ao usuário dar zoom no que está sendo visualizado pela escopeta
        (este zoom também foi limitado), além disso pode-se aumentar ou diminuir o diâmetro da escopeta.
            Algumas técnicas foram utilizadas neste programa. Uma delas foi de deixar com um Blur a parte que não está sendo visualizado pela
        escopeta. Usou-se um bump mapping para a textura do chão (foi criado um "sol" que faz ficar mais escuro e mais claro apenas para visualizar
        o bump mapping. Essa funcionalidade pode ser desabilitada). As rotações foram feitas utilizando-se a técnica de eixo arbitrário.

            Para fazer com que apenas a parte de dentro da escopeta ficasse sem blur e podendo-se dar zoom foi utilizada a seguinte técnica.
        Em cada render, a cena é renderizada para uma textura e essa mesma textura é então desenhada na tela. Porém, utilizando-se dos shaders,
        é aplicado o efeito de blur nele e os pixels que fazem parte da escopeta são descartados (pixels que estão a uma certa distância do 
        centro da textura). Sendo assim, a textura é desenhada na tela, mas fica com um buraco no meio onde devia estar a cena nítida
        e com ou sem zoom. Para preencher este buraco, simplesmente é mandado que a cena seja renderizada normalmente logo após a renderização
        da textura na tela (sendo a cena recriada com o valor do FOV da matriz de projeção alterado para o zoom desejado).
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

