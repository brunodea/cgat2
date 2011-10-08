//---------------------------------------------
// Author: Cesar Tadeu Pozzer
// UFSM - 06/2010
// http://www-usr.inf.ufsm.br/~pozzer/
//
// Version 2
//---------------------------------------------
#include "GL/gl3w.hpp"
#include "GL/glfw.h"
#include <stdio.h>
#include <stdlib.h>


static bool showInfo = false;

class Glsl
{
   GLuint p, f, v;
   bool isActive;
public: 
   Glsl(char *vert, char *frag);
   ~Glsl();
   void setActive(bool b);
   GLint getUniformLoc(char *);
   GLint getAttribLoc(char *);
   void  bindAttribLocation(int index, char *nome);
   void  bindFragDataLocation(int index, char *nome);

   void setMatrix(GLint loc, GLfloat *p);  //sets a given matrix to the shader program
 
private:
   void setShaders(char *vert, char *frag);
   char *textFileRead(char *fn);
   int  printOglError(char *file, int line);
   void printProgramInfoLog(GLuint obj);
   void printShaderInfoLog(GLuint obj);
};