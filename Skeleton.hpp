#ifndef SKELETON_HPP
#define SKELETON_HPP

#include "CFunctions.h"

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

using std::cout;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

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

struct angles{ int th,ph; }; // angles/degrees of freedom for bone
struct offset { double x,y,z; }; // offset to the position of a bone
struct buf_t
{
   void* off;
   int type;
   int n;
};
struct vbo_t
{
   int type,n,stride;
   unsigned int buf,ele;
   buf_t vertex,color,normal,texture;
   float dim;
};
struct adj_bone;
struct bone
{
   vbo_t vbo; // vbo for the bone
   //double len; // length of the bone
   char ch; // character signifier for which bone it is
   angles ang; // two angles, th and ph, for two degrees of freedom
   offset off; // offset to the position of a bone
   string name; // bone name
   vector<adj_bone> adj; // adjacent bones
};
enum bone_dir
{
   up = 1, // adj bone is upstream
   down = -1, // adj bone is downstream
   neither = 0 // adj bone is neither upstream nor downstream
};
struct adj_bone
{
   shared_ptr<bone> adj_bone;
   bone_dir dir;
};

// cstr.cpp
   char * toCStr(string str);

// errcheck.cpp
   void ErrCheck(const char* where);

// fatal.cpp
   void Fatal(const char* format , ...);

// loadmodel.cpp
   void DrawVBO(vbo_t vbo);
   void DrawModel(vbo_t vbo);
   float maxdim8(float dim,double xyz[],int n);
   float maxdim4(float dim,float xyz[],int n);
vbo_t LoadModel(const char* file, int inv_norm);

// loadply.c
   // vec3 struct
   // static vec3 sub(vec3 v1,vec3 v2)
   // static vec3 cross(vec3 v1,vec3 v2)
   // static vec3 normalize(vec3 v)
   // static void addvec(float* V,vec3 v)
   // static int findname(char* name[],int Nvar,const char* v[],int n)
   // static buf_t find(char* name[],off_t off[],int type[],int Nvar,const char* v[],int n)
   vbo_t LoadPLY(const char* file, int inv_norm);

// print.cpp
   void Print(const char* format , ...);

// printVBO.cpp
   void printVBO(vbo_t vbo, int v);

// projection.cpp
   void Project(double fov,double asp,double dim);

// read.cpp
   // static int CRLF(char ch)
   char* readline(FILE* f);
   char* getword(char** line);
   void readfloat(char* line,int n,float x[]);
   void readcoord(char* line,int n,float* x[],int* N,int* M);
   char* readstr(char* line,const char* skip);
   int compstr(char* line,char* ref);


class Skeleton{
   public:
      Skeleton();
      ~Skeleton();

      void resetAng(); // reset all bone angles
      const void printSkeleton(bool v); // print bones information to terminal - set bool v to true for verbose
      const void printBone(int idx, bool v);
      void setAng(int idx, int th, int ph); // increment bone motion angles
      void drawSkeleton(); // draw the complete skeleton
      void drawBone(int idx); // draw bone at origin
      void drawBone(bone b); // draw bone at origin
      void drawLeg(int idx, float i); // draw leg: i - +/- axis rotation specifier, ch - starting char signifier
      void drawArm(int idx, float i); // draw arm: i - +/- axis rotation specifier, ch - starting char signifier
      void drawLabel(char ch); // draw char signifier labels at each bone
      void setLabel(char ch); // set currently selected label

   private:
      vbo_t readBoneFile(string, int idx); // read data from bone file and create vbo
      shared_ptr<bone> newBone(int); // retrieve data for and create new bone
      void initBoneAdj(); // initialize bone adjacencies
      bool validIdx(int idx);
      bool validChar(char ch);
      
      // current selected label
      char selected_label = -1;
      // number of bones
      const static int NUM_BONES = 24;
      // min char label
      const char min_char = 'A';
      // max char label
      const char max_char = 'A' + NUM_BONES - 1;
      // min idx
      const int min_idx = 0;
      // max idx
      const int max_idx = NUM_BONES - 1;
      // array of pointers to bones
      shared_ptr<bone> bones[NUM_BONES];
      // path to directory that contains bone files
      const string bones_adr = "./bones"; 
      // file names for bones
      const string bones_f[NUM_BONES] = 
      {
         "sacrum.vtp.ply",          // pelvis/ tailbone/ lower spine
         "lumbar_spine.vtp.ply",    // lumbar/ lower spine
         "thorax.vtp.ply",          // torso/ thorax/ mid-spine
         "cervical_spine.vtp.ply",  // head/ neck/ upper spine

         "scapula_l.vtp.ply",       // left shoulder blade
         "humerus_l.vtp.ply",       // left upper arm
         "ulna_l.vtp.ply",          // left lower arm (ulna)
         "radius_l.vtp.ply",        // left lower arm (radius)
         "lunate_l.vtp.ply",        // left hand

         "scapula.vtp.ply",         // right shoulder blade
         "humerus.vtp.ply",         // right upper arm
         "ulna.vtp.ply",            // right lower arm (ulna)
         "radius.vtp.ply",          // right lower arm (radius)
         "lunate.vtp.ply",          // right hand

         "l_femur.vtp.ply",         // left upper leg
         "l_fibula.vtp.ply",        // left lower leg
         "l_talus.vtp.ply",         // left ball of foot
         "l_foot.vtp.ply",          // left foot
         "l_bofoot.vtp.ply",        // left toes

         "r_femur.vtp.ply",         // right upper leg
         "r_fibula.vtp.ply",        // right lower leg
         "r_talus.vtp.ply",         // right ball of foot
         "r_foot.vtp.ply",          // right foot
         "r_bofoot.vtp.ply"         // right toes
      };
      // names of bones
      const string bones_n[NUM_BONES]
      {
         "pelvis and tailbone",
         "lumbar/ lower spine",
         "torso and thorax/ mid-spine",
         "head and neck/ upper spine",

         "left scapula/ shoulder blade",
         "left humerus/ upper arm",
         "left ulna / lower arm",
         "left radius/ lower arm",
         "left hand",

         "right scapula/ shoulder blade",
         "right humerus/ upper arm",
         "right ulna / lower arm",
         "right radius/ lower arm",
         "right hand",

         "left femur/ upper leg",
         "left tibia and fibula/ lower leg",
         "left talus/ ball of foot",
         "left foot",
         "left toes",

         "right femur/ upper leg",
         "right tibia and fibula/ lower leg",
         "right talus/ ball of foot",
         "right foot",
         "right toes"
      };
      // xyz offset for bones - translation relative to parent limb
      // eg - how does this limb need to be moved in x,y,z directions relative to parent limb
      const offset bones_off[NUM_BONES]
      {
         {0,0,3.25},          // 0.A pelvis/ tailbone
         {0,.75,-3.75},          // 1.B lumbar/ lower spine
         {0,11.75,.45},        // 2.C torso/ thorax/ mid-spine
         {0,-16,2},        // 3.D head/ neck/ upper spine

         {6,0,1},          // 4.E left shoulder blade
         {6,-1.25,.5},     // 5.F left upper arm
         {-.5,-12,.5},     // 6.G left lower arm (ulna)
         {-.5,-12,.5},     // 7.H left lower arm (radius)
         {1.5,-9.75,.75},  // 8.I left hand

         {-6,0,1},         // 9.J right shoulder blade
         {-6,-1.25,.5},    // 10.K right upper arm
         {.5,-12,.5},      // 11.L right lower arm (ulna)
         {.5,-12,.5},      // 12.M right lower arm (radius)
         {-1.5,-9.75,.75}, // 13.N right hand

         {3,-3.25,-2.25},  // 14.O left upper leg
         {0,-16.75,0},     // 15.P left lower leg
         {0,-16.25,0},     // 16.Q left ball of foot
         {0,-2.25,-1.75},  // 17.R left foot
         {0,0,7.5},        // 18.S left toes

         {-3,-3.25,-2.25}, // 19.T right upper leg
         {0,-16.75,0},     // 20.U right lower leg
         {0,-16.25,0},     // 21.V right ball of foot
         {0,-2.25,-1.75},  // 22.W right foot
         {0,0,7.5}         // 23.X right toes
      };
      // bone adjacencies as indices of bones array
      // listed as adj if the movement of one bone directly impacts another - NOT whether they are next to each other
      const vector<vector<int>> bones_adj
      {
         {1,14,19},  // 0.A pelvis/ tailbone
         {0,2},      // 1.B lumbar/ lower spine
         {1,3,4,9},      // 2.C torso/ thorax/ mid-spine
         {2},        // 3.D head/ neck/ upper spine

         {2,5},        // 4.E left shoulder blade
         {4,6,7},    // 5.F left upper arm
         {5,7,8},      // 6.G left lower arm (ulna)
         {5,6,8},      // 7.H left lower arm (radius)
         {6,7},      // 8.I left hand

         {2,10},        // 9.J right shoulder blade
         {9,11,12},  // 10.K right upper arm
         {10,12,13},     // 11.L right lower arm (ulna)
         {10,11,13},     // 12.M right lower arm (radius)
         {11,12},    // 13.N right hand

         {0,15},     // 14.O left upper leg
         {14,16},    // 15.P left lower leg
         {15,17},    // 16.Q left ball of foot
         {16,18},    // 17.R left foot
         {17},       // 18.S left toes

         {0,20},     // 19.T right upper leg
         {19,21},    // 20.U right lower leg
         {20,22},    // 21.V right ball of foot
         {21,23},    // 22.W right foot
         {22}        // 23.X right toes
      };
      // hierarchy/ direction of adjacency
      const vector<vector<bone_dir>> bones_dir
      {
         {down,down,down}, // 0.A pelvis/ tailbone
         {up,down}, // 1.B lumbar/ lower spine
         {up,down,down,down}, // 2.C torso/ thorax/ mid-spine
         {up}, // 3.D head/ neck/ upper spine

         {up,down}, // 4.E left shoulder blade
         {up,down,down}, // 5.F left upper arm
         {up,neither,down}, // 6.G left lower arm (ulna)
         {up,neither,down}, // 7.H left lower arm (radius)
         {up,up}, // 8.I left hand

         {up,down}, // 9.J right shoulder blade
         {up,down,down}, // 10.K right upper arm
         {up,neither,down}, // 11.L right lower arm (ulna)
         {up,neither,down}, // 12.M right lower arm (radius)
         {up,up}, // 13.N right hand

         {up,down}, // 14.O left upper leg
         {up,down}, // 15.P left lower leg
         {up,down}, // 16.Q left ball of foot
         {up,down}, // 17.R left foot
         {up}, // 18.S left toes

         {up,down}, // 19.T right upper leg
         {up,down}, // 20.U right lower leg
         {up,down}, // 21.V right ball of foot
         {up,down}, // 22.W right foot
         {up} // 23.X right toes
      };
      // angles for bone motion
      angles bones_ang[NUM_BONES]
      {
         {0,0},      // 0.A pelvis/ tailbone
         {0,-5},      // 1.B lumbar/ lower spine
         {0,0},      // 2.C torso/ thorax/ mid-spine
         {0,0},      // 3.D head/ neck/ upper spine

         {0,0},      // 4.E left shoulder blade
         {0,92},     // 5.F left upper arm
         {0,0},      // 6.G left lower arm (ulna)
         {0,-10},    // 7.H left lower arm (radius)
         {0,0},      // 8.I left hand

         {0,0},      // 9.J right shoulder blade
         {0,92},     // 10.K right upper arm
         {0,0},      // 11.L right lower arm (ulna)
         {0,-10},    // 12.M right lower arm (radius)
         {0,0},      // 13.N right hand

         {0,0},      // 14.O left upper leg
         {0,0},      // 15.P left lower leg
         {0,0},      // 16.Q left ball of foot
         {0,0},      // 17.R left foot
         {0,0},      // 18.S left toes

         {0,0},      // 19.T right upper leg
         {0,0},      // 20.U right lower leg
         {0,0},      // 21.V right ball of foot
         {0,0},      // 22.W right foot
         {0,0}       // 23.X right toes
      };
      // whether or not to invert the normals for a particular bone - 1:yes, 0:no
      int bones_l[NUM_BONES]
      {
         0, // 0.A pelvis/ tailbone
         0, // 1.B lumbar/ lower spine
         0, // 2.C torso/ thorax/ mid-spine
         0, // 3.D head/ neck/ upper spine

         0, // 4.E left shoulder blade
         0, // 5.F left upper arm
         0, // 6.G left lower arm (ulna)
         0, // 7.H left lower arm (radius)
         0, // 8.I left hand

         1, // 9.J right shoulder blade
         1, // 10.K right upper arm
         1, // 11.L right lower arm (ulna)
         1, // 12.M right lower arm (radius)
         1, // 13.N right hand

         0, // 14.O left upper leg
         1, // 15.P left lower leg
         0, // 16.Q left ball of foot
         0, // 17.R left foot
         0, // 18.S left toes

         1, // 19.T right upper leg
         0, // 20.U right lower leg
         0, // 21.V right ball of foot
         1, // 22.W right foot
         1 // 23.X right toes
      };
};
#endif

/*
centered at PELVIS
BONES:
0.A pelvis/ tailbone - sacrum.vtp.ply
1.B lumbar/ lower spine - lumbar_spine.vtp.ply
2.C torso/ thorax/ mid-spine - thorax.vtp.ply
3.D head/ neck/ upper spine - cervical_spine.vtp.ply

4.E left shoulder blade - scapula_l.vtp.ply
5.F left upper arm - humerus_l.vtp.ply
6.G left lower arm (ulna) - ulna_l.vtp.ply
7.H left lower arm (radius) - radius_l.vtp.ply
8.I left hand - lunate_l.vtp.ply

9.J right shoulder blade - scapula.vtp.ply
10.K right upper arm - humerus.vtp.ply
11.L right lower arm (ulna) - ulna.vtp.ply
12.M right lower arm (radius) - radius.vtp.ply
13.N right hand - lunate.vtp.ply

14.O left upper leg - l_femur.vtp.ply
15.P left lower leg - l_fibula.vtp.ply
16.Q left ball of foot - l_talus.vtp.ply
17.R left foot - l_foot.vtp.ply
18.S left toes - l_bofoot.vtp.ply

19.T right upper leg - r_femur.vtp.ply
20.U right lower leg - r_fibula.vtp.ply
21.V right ball of foot - r_talus.vtp.ply
22.W right foot - r_foot.vtp.ply
23.X right toes - r_bofoot.vtp.ply
*/

/*
TO-DO:
- add code to skeleton class to call display functions appropriately to draw skeleton
- add better controls with qt
*/
