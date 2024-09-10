/* esse arquivo contem os cabeçalhos e macros mais comuns
que podem ser utilizados nos varios programas graficos de OpenGL */

/* repare que o uso deve ser exclusivo para programas com include de "GL/glew", "GLFW/glfw3", "string" */

#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <string>

/* PADRAO -----------------------------------------
tipo nome_fucao(tipo_arguemnto); //numero_funcao */

void printShaderLog(GLuint); // 1

void printProgramLog(int); // 2

bool checkOpenGLError(); // 3

std::string readShaderSource(const char*, bool); // 4

GLuint createShaderProgram(const char*, const char*, bool); // 5

void setConsoleColor(WORD);

void resetConsoleColor();

#endif // UTILS_H