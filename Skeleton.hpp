#ifndef SKELETON_HPP
#define SKELETON_HPP

#include "CFunctions.h"

#include <iostream>
#include <vector>

using std::cout;
using std::string;
using std::vector;

char * toCStr(string str); // defined in file cstr.cpp
bool str2bool(string str) {return str != "";}; // return bool for whether or not string is empty

struct angles{ int th,ph; }; // angles/degrees of freedom for bone
struct offset { int x,y,z; }; // offset to the position of a bone
struct bone
{
   vbo_t vbo; // vbo for the bone
   unsigned int len; // length of the bone
   char ch; // character signifier for which bone it is
   angles ang; // two angles, th and ph, for two degrees of freedom
   offset off; // offset to the position of a bone
   vector<bone*> adj; // adjacent bones
};

class Skeleton{
   public:
      Skeleton();
      ~Skeleton();

      void resetAng(); // reset all bone angles
      void printBones(); // print bones information to terminal
      void updateAng(int idx, int th, int ph); // update bone motion angles
      void drawSkeleton(); // draw the complete skeleton
      void drawBone(int idx); // draw bone at origin
      void drawLeg(int idx, float i, char ch, angles ang_l[5]); // draw leg: i - +/- axis rotation specifier, ch - starting char signifier
      void drawArm(int idx, float i, char ch, angles ang_a[5]); // draw arm: i - +/- axis rotation specifier, ch - starting char signifier
      void drawLabel(char ch); // draw char signifier labels at each bone

   private:
      vbo_t readBoneFile(string); // read data from bone file and create vbo
      bone* newBone(int); // retrieve data for and create new bone
      void initBoneAdj(); // initialize bone adjacencies
      
      // number of bones
      const static int NUM_BONES = 23;
      // array of pointers to bones
      bone* bones[NUM_BONES];
      // path to directory that contains bone files
      const string bones_adr = "./bones"; 
      // file names for bones
      const string bones_f[NUM_BONES] = 
      {
         "sacrum.vtp.ply",          // pelvis/ tailbone/ lower spine
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
         "pelvis and tailbone/ lower spine",
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
      // length of bones in inches TODO: *LENGTHS ARE ESTIMATED, FIX LATER*
      const unsigned int bones_l[NUM_BONES]
      {
         3,    // 0.A pelvis/ tailbone/ lower spine
         17,   // 1.B torso/ thorax/ mid-spine
         10,   // 2.C head/ neck/ upper spine

         0,    // 3.D left shoulder blade
         11.5, // 4.E left upper arm
         9,    // 5.F left lower arm (ulna)
         9,    // 6.G left lower arm (radius)
         5.5,  // 7.H left hand

         0,    // 8.I right shoulder blade
         11.5, // 9.J right upper arm
         9,    // 10.K right lower arm (ulna)
         9,    // 11.L right lower arm (radius)
         5.5,  // 12.M right hand

         17.5, // 13.N left upper leg
         15.5, // 14.O left lower leg
         2,    // 15.P left ball of foot
         4,    // 16.Q left foot
         3,    // 17.R left toes

         17.5, // 18.S right upper leg
         15.5, // 19.T right lower leg
         2,    // 20.U right ball of foot
         4,    // 21.V right foot
         3     // 22.W right toes
      };
      // xyz offset for bones
      const offset bones_off[NUM_BONES]
      {
         {0,0,0}, // 0.A pelvis/ tailbone/ lower spine
         {0,0,0}, // 1.B torso/ thorax/ mid-spine
         {0,0,0}, // 2.C head/ neck/ upper spine

         {0,0,0}, // 3.D left shoulder blade
         {0,0,0}, // 4.E left upper arm
         {0,0,0}, // 5.F left lower arm (ulna)
         {0,0,0}, // 6.G left lower arm (radius)
         {0,0,0}, // 7.H left hand

         {0,0,0}, // 8.I right shoulder blade
         {0,0,0}, // 9.J right upper arm
         {0,0,0}, // 10.K right lower arm (ulna)
         {0,0,0}, // 11.L right lower arm (radius)
         {0,0,0}, // 12.M right hand

         {0,0,0}, // 13.N left upper leg
         {0,0,0}, // 14.O left lower leg
         {0,0,0}, // 15.P left ball of foot
         {0,0,0}, // 16.Q left foot
         {0,0,0}, // 17.R left toes

         {0,0,0}, // 18.S right upper leg
         {0,0,0}, // 19.T right lower leg
         {0,0,0}, // 20.U right ball of foot
         {0,0,0}, // 21.V right foot
         {0,0,0} // 22.W right toes
      };
      // bone adjacencies as indices of bones array
      // listed as adj if the movement of one bone directly impacts another - NOT whether they are next to each other
      const vector<vector<int>> bones_adj
      {
         {1,13,18},  // 0.A pelvis/ tailbone/ lower spine
         {0,2},      // 1.B torso/ thorax/ mid-spine
         {1},        // 2.C head/ neck/ upper spine

         {4},        // 3.D left shoulder blade
         {3,5,6},    // 4.E left upper arm
         {4,6},      // 5.F left lower arm (ulna)
         {4,6},      // 6.G left lower arm (radius)
         {5,6},      // 7.H left hand

         {9},        // 8.I right shoulder blade
         {8,10,11},  // 9.J right upper arm
         {9,12},     // 10.K right lower arm (ulna)
         {9,12},     // 11.L right lower arm (radius)
         {10,11},    // 12.M right hand

         {0,14},     // 13.N left upper leg
         {13,15},    // 14.O left lower leg
         {14,16},    // 15.P left ball of foot
         {15,17},    // 16.Q left foot
         {16},       // 17.R left toes

         {0,19},     // 18.S right upper leg
         {18,20},    // 19.T right lower leg
         {19,21},    // 20.U right ball of foot
         {20,22},    // 21.V right foot
         {21}        // 22.W right toes
      };
      // angles for bone motion
      angles bones_ang[NUM_BONES]
      {
         {0,0},   // 0.A pelvis/ tailbone/ lower spine
         {0,0},   // 1.B torso/ thorax/ mid-spine
         {0,0},   // 2.C head/ neck/ upper spine

         {0,0},   // 3.D left shoulder blade
         {0,0},   // 4.E left upper arm
         {0,0},   // 5.F left lower arm (ulna)
         {0,0},   // 6.G left lower arm (radius)
         {0,0},   // 7.H left hand

         {0,0},   // 8.I right shoulder blade
         {0,0},   // 9.J right upper arm
         {0,0},   // 10.K right lower arm (ulna)
         {0,0},   // 12.M right hand

         {0,0},   // 13.N left upper leg
         {0,0},   // 14.O left lower leg
         {0,0},   // 15.P left ball of foot
         {0,0},   // 16.Q left foot
         {0,0},   // 17.R left toes

         {0,0},   // 18.S right upper leg
         {0,0},   // 19.T right lower leg
         {0,0},   // 20.U right ball of foot
         {0,0},   // 21.V right foot
         {0,0}    // 22.W right toes
      };
};
#endif

/*
centered at PELVIS
BONES:
0.A pelvis/ tailbone/ lower spine - sacrum.vtp.ply
1.B torso/ thorax/ mid-spine - thorax.vtp.ply
2.C head/ neck/ upper spine - cervical_spine.vtp.ply

3.D left shoulder blade - scapula_l.vtp.ply
4.E left upper arm - humerus_l.vtp.ply
5.F left lower arm (ulna) - ulna_l.vtp.ply
6.G left lower arm (radius) - radius_l.vtp.ply
7.H left hand - lunate_l.vtp.ply

8.I right shoulder blade - scapula.vtp.ply
9.J right upper arm - humerus.vtp.ply
10.K right lower arm (ulna) - ulna.vtp.ply
11.L right lower arm (radius) - radius.vtp.ply
12.M right hand - lunate.vtp.ply

13.N left upper leg - l_femur.vtp.ply
14.O left lower leg - l_fibula.vtp.ply
15.P left ball of foot - l_talus.vtp.ply
16.Q left foot - l_foot.vtp.ply
17.R left toes - l_bofoot.vtp.ply

18.S right upper leg - r_femur.vtp.ply
19.T right lower leg - r_fibula.vtp.ply
20.U right ball of foot - r_talus.vtp.ply
21.V right foot - r_foot.vtp.ply
22.W right toes - r_bofoot.vtp.ply
*/

/*
TO-DO:
- add code to skeleton class to call display functions appropriately to draw skeleton
- add better controls with qt
*/
