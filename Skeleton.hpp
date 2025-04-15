#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <memory>
#include <iostream>
#include <vector>

#include <QOpenGLWidget>
#include <QString>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLContext>

using std::cout;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

#include "Bone.hpp"

//  cos and sin in degrees
#define Cos(th) cos(3.14159265/180*(th))
#define Sin(th) sin(3.14159265/180*(th))

class Skeleton : public QOpenGLWidget, protected QOpenGLFunctions
{
   Q_OBJECT // magic macro

   // SKELETON
   public:
      Skeleton(QWidget* parent= nullptr) : QOpenGLWidget(parent) {};

      QSize sizeHint() const {return QSize(800,800);}  //  Default size of widget

      void resetAng(); // reset all bone angles
      void resetFlags(); // reset bone visited flags
      void printSkeleton(); // print bones information to terminal - set bool v to true for verbose
      void setAng(int idx, int th, int ph); // increment bone motion angles
      void drawSkeleton(); // draw the complete skeleton
      void drawBone(Bone b); // draw bone at origin
      void drawLeg(int idx, float i); // draw leg: i - +/- axis rotation specifier, ch - starting char signifier
      void drawArm(int idx, float i); // draw arm: i - +/- axis rotation specifier, ch - starting char signifier
      unsigned char getAlpha();
      int getNumBones() {return NUM_BONES;}

   private:
      void initBoneAdj(); // initialize bone adjacencies
      bool validIdx(int idx);
      bool validChar(char ch);
      
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
      shared_ptr<Bone> bones[NUM_BONES];
      // path to directory that contains bone files
      const QString bones_adr = "../bones"; 
      // file names for bones
      const QString bones_f[NUM_BONES] = 
      {
         "sacrum.vtp.ply",          // pelvis/ tailbone/ lower spine
         "lumbar_spine.vtp.ply",    // lumbar/ lower spine
         "./replaced_bones/thorax_new.ply",          // torso/ thorax/ mid-spine
         "./replaced_bones/cervical_spine_new.ply",  // head/ neck/ upper spine

         "./replaced_bones/scapula_L_new.ply",       // left shoulder blade
         "humerus_l.vtp.ply",       // left upper arm
         "./replaced_bones/ulna_L_new.ply",          // left lower arm (ulna)
         "./replaced_bones/radius_L_new.ply",        // left lower arm (radius)
         "./replaced_bones/lunate_L_new.ply",        // left hand

         "./replaced_bones/scapula_R_new.ply",         // right shoulder blade
         "humerus.vtp.ply",         // right upper arm
         "./replaced_bones/ulna_R_new.ply",            // right lower arm (ulna)
         "./replaced_bones/radius_R_new.ply",          // right lower arm (radius)
         "./replaced_bones/lunate_R_new.ply",          // right hand

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
      const QString bones_n[NUM_BONES]
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
         {0,0,3.25},       // 0.A pelvis/ tailbone
         {0,.75,-3.75},    // 1.B lumbar/ lower spine
         {0,1.5,0},    // 2.C torso/ thorax/ mid-spine
         {0,10.5,.75},        // 3.D head/ neck/ upper spine

         {2.5,8,1.5},          // 4.E left shoulder blade
         {4,1,-.5},    // 5.F left upper arm
         {-1,-11.75,.25},     // 6.G left lower arm (ulna)
         {-1,-11.75,.25},     // 7.H left lower arm (radius)
         {0,-9.8,0},  // 8.I left hand

         {-2.5,8,1.5},         // 9.J right shoulder blade
         {-4,1,-.5},    // 10.K right upper arm
         {-1,-11.75,.25},      // 11.L right lower arm (ulna)
         {-1,-11.75,.25},      // 12.M right lower arm (radius)
         {0,-9.8,0}, // 13.N right hand

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
         {1,3,4,9},  // 2.C torso/ thorax/ mid-spine
         {2},        // 3.D head/ neck/ upper spine

         {2,5},      // 4.E left shoulder blade
         {4,6,7},    // 5.F left upper arm
         {5,7,8},    // 6.G left lower arm (ulna)
         {5,6,8},    // 7.H left lower arm (radius)
         {6,7},      // 8.I left hand

         {2,10},     // 9.J right shoulder blade
         {9,11,12},  // 10.K right upper arm
         {10,12,13}, // 11.L right lower arm (ulna)
         {10,11,13}, // 12.M right lower arm (radius)
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
      // join type - conjoined (con): move together or disjoined(dis) - move separately
      // eg whether or not to transfer the rotation of a bone to another
      const vector<vector<std::shared_ptr<join>>> bones_j
      { 
         {nullptr,nullptr,nullptr}, // 0.A pelvis/ tailbone
         {nullptr,nullptr}, // 1.B lumbar/ lower spine
         {nullptr,nullptr,nullptr,nullptr}, // 2.C torso/ thorax/ mid-spine
         {nullptr}, // 3.D head/ neck/ upper spine

         {nullptr,nullptr}, // 4.E left shoulder blade
         {nullptr,nullptr,nullptr}, // 5.F left upper arm
         {nullptr,make_shared<join>(pheta,1),nullptr}, // 6.G left lower arm (ulna)
         {nullptr,make_shared<join>(pheta,1),make_shared<join>(theta,1)}, // 7.H left lower arm (radius)
         {nullptr,make_shared<join>(theta,1)}, // 8.I left hand

         {nullptr,nullptr}, // 9.J right shoulder blade
         {nullptr,nullptr,nullptr}, // 10.K right upper arm
         {nullptr,make_shared<join>(pheta,1),nullptr}, // 11.L right lower arm (ulna)
         {nullptr,make_shared<join>(pheta,1),make_shared<join>(theta,1)}, // 12.M right lower arm (radius)
         {nullptr,make_shared<join>(theta,1)}, // 13.N right hand

         {nullptr,nullptr}, // 14.O left upper leg
         {nullptr,nullptr}, // 15.P left lower leg
         {nullptr,nullptr}, // 16.Q left ball of foot
         {nullptr,nullptr}, // 17.R left foot
         {nullptr}, // 18.S left toes

         {nullptr,nullptr}, // 19.T right upper leg
         {nullptr,nullptr}, // 20.U right lower leg
         {nullptr,nullptr}, // 21.V right ball of foot
         {nullptr,nullptr}, // 22.W right foot
         {nullptr} // 23.X right toes
      };
      // initial angles for bone motion
      angles bones_ang[NUM_BONES]
      {
         {0,0},         // 0.A pelvis/ tailbone
         {0,-5},        // 1.B lumbar/ lower spine
         {0,0},         // 2.C torso/ thorax/ mid-spine
         {0,0},         // 3.D head/ neck/ upper spine

         {0,0},         // 4.E left shoulder blade
         {0,85},        // 5.F left upper arm
         {0,0},         // 6.G left lower arm (ulna)
         {0,0},     // 7.H left lower arm (radius)
         {0,0},         // 8.I left hand

         {0,0},         // 9.J right shoulder blade
         {0,85},        // 10.K right upper arm
         {0,0},     // 11.L right lower arm (ulna)
         {0,0},     // 12.M right lower arm (radius)
         {0,0},         // 13.N right hand

         {0,0},         // 14.O left upper leg
         {0,0},         // 15.P left lower leg
         {0,0},         // 16.Q left ball of foot
         {0,0},         // 17.R left foot
         {0,0},         // 18.S left toes

         {0,0},         // 19.T right upper leg
         {0,0},         // 20.U right lower leg
         {0,0},         // 21.V right ball of foot
         {0,0},         // 22.W right foot
         {0,0}          // 23.X right toes
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
         1, // 6.G left lower arm (ulna)
         1, // 7.H left lower arm (radius)
         1, // 8.I left hand

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
      angle_limits bones_lim[NUM_BONES] // limits on bone rotation {{min:th,ph},{max:th,ph}}
      {
         {{0,0},{0,0}}, // 0.A pelvis/ tailbone
         {{0,-30},{0,60}}, // 1.B lumbar/ lower spine
         {{0,-10},{0,30}}, // 2.C torso/ thorax/ mid-spine
         {{0,-30},{0,45}}, // 3.D head/ neck/ upper spine

         {{},{}}, // 4.E left shoulder blade
         {{-120,0},{45,135}}, // 5.F left upper arm
         {{0,-170},{0,0}}, // 6.G left lower arm (ulna)
         {{-170,-150},{0,0}}, // 7.H left lower arm (radius)
         {{-170,-80},{0,30}}, // 8.I left hand

         {{},{}}, // 9.J right shoulder blade
         {{-120,0},{45,135}}, // 10.K right upper arm
         {{0,-170},{0,0}}, // 11.L right lower arm (ulna)
         {{-170,-150},{0,0}}, // 12.M right lower arm (radius)
         {{-170,-80},{0,30}}, // 13.N right hand

         {{},{}}, // 14.O left upper leg
         {{},{}}, // 15.P left lower leg
         {{},{}}, // 16.Q left ball of foot
         {{},{}}, // 17.R left foot
         {{},{}}, // 18.S left toes

         {{},{}}, // 19.T right upper leg
         {{},{}}, // 20.U right lower leg
         {{},{}}, // 21.V right ball of foot
         {{},{}}, // 22.W right foot
         {{},{}} // 23.X right toes
      };
      
      // QT WIDGET
      public:
         // cstr.cpp
            char * toCStr(string str);

         // errcheck.cpp
            void ErrCheck(const char* where); //*
            void ErrCheck(string where);

         // print.cpp
            void Print(const char* format , ...); //*

         int axes=1;         //  Display axes
         int light=1;        //  Toggle light
         int proj=1;         //  Projection type
         int th=0;           //  Azimuth of view angle
         int ph=0;           //  Elevation of view angle
         int fov=55;         //  Field of view (for perspective)
         int mx=0,my=0;      //  Mouse coordinates
         bool   mouse = 0;       //  Mouse pressed
         QPoint pos;         //  Mouse position
         double asp=1;       //  Aspect ratio
         double dim=60;     //  Size of world
         char label = 'A';
         int selected_bone = -1;
         int gx=0,gy=0,gz=0;
      
      public slots:
         void setGX(double X);      //  Slot to set x0
         void setGY(double Y);      //  Slot to set y0
         void setGZ(double Z);      //  Slot to set z0
         void setDIM(double DIM);    //  Slot to set dim
         void reset(void);           //  Reset view
         void setSelectedBone(int idx); // set selected bone to idx, or if idx is already selected set to unselected (-1)
         void resetBones(void);
         

      signals:
         void setAngles(QString text);  //  Signal for display angles
         void setDim(double dim);     //  Signal for display dimensions
         void setWidgetGX(double dim);
         void setWidgetGY(double dim);
         void setWidgetGZ(double dim);
         void resetBoneSelectedBtn(int b); // reset all buttons checked value to false except idx
      
      // GL functionality
      protected:
         void initializeGL() override;                   //  Initialize widget
         void resizeGL(int width, int height) override;  //  Resize widget
         void paintGL() override;                        //  Draw widget
         void mousePressEvent(QMouseEvent*);             //  Mouse pressed
         void mouseReleaseEvent(QMouseEvent*);           //  Mouse released
         void mouseMoveEvent(QMouseEvent*);              //  Mouse moved
         void wheelEvent(QWheelEvent*);                  //  Mouse wheel
      private:
         void project();   
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

*/
