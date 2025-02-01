#ifndef CFUNCTIONS_H
#define CFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

// GLEW _MUST_ be included first
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  Get all GL prototypes
#define GL_GLEXT_PROTOTYPES
//  Select SDL, SDL2, GLFW or GLUT
#if defined(SDL2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif defined(SDL)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#elif defined(GLFW)
#include <GLFW/glfw3.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//  Make sure GLU and GL are included
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif

//  cos and sin in degrees
#define Cos(th) cos(3.14159265/180*(th))
#define Sin(th) sin(3.14159265/180*(th))

//  VBO struct
typedef struct {void* off;int type;int n;} buf_t;
typedef struct {
   int type,n,stride;
   unsigned int buf,ele;
   buf_t vertex,color,normal,texture;
   float dim;
   } vbo_t;

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
//vbo_t LoadOBJ(const char* file);
vbo_t LoadPLY(const char* file);
//vbo_t LoadSTL(const char* file);
vbo_t LoadModel(const char* file);
void DrawModel(vbo_t vbo);
char* readline(FILE* f);
char* getword(char** line);
void readfloat(char* line,int n,float x[]);
void readcoord(char* line,int n,float* x[],int* N,int* M);
char* readstr(char* line,const char* skip);
int compstr(char* line,char* ref);
float maxdim8(float dim,double xyz[],int n);
float maxdim4(float dim,float xyz[],int n);
void printVBO(vbo_t vbo, int v);

#ifdef __cplusplus
}
#endif

#endif