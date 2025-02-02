/*
 *  Skeleton Viewer
 *
 *  Key bindings:
 *  p          Toggle between orthogonal/perspective projections
 *  -/+        Decrease/increase skeleton elevation
 *  a          Toggle axes
 *  m          Togle mode
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CFunctions.h"
#include "Skeleton.hpp"

int axes=1;         //  Display axes
int light=1;        //  Toggle light
int proj=1;         //  Projection type
int th=0;           //  Azimuth of view angle
int ph=0;           //  Elevation of view angle
int fov=55;         //  Field of view (for perspective)
int mx=0,my=0;      //  Mouse coordinates
int move=0;    //  Mouse move
offset off={0,0,0}; // xyz offset
double asp=1;       //  Aspect ratio
double dim=60;     //  Size of world
int kol[2]={0,0};   //  Color
int mode[2]={0,0};  //  Display mode
char label = 'A';
int rev=0;          //  Reverse draw order
int scale=0;        //  Adjust scale
unique_ptr<Skeleton> skel = nullptr; 
int gx=0,gy=0,gz=0;

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   
   glClearColor(0,0,0,0);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (proj)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //  Enable lighting if normals are present
   if (light)
   {
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Light colors
      float Emission[]  = {0.0,0.0,0.0,1.0};
      float Ambient[]   = {0.3,0.3,0.3,1.0};
      float Diffuse[]   = {1.0,1.0,1.0,1.0};
      float Specular[]  = {0.5,0.5,0.5,1.0};
      float Position[]  = {3.0,1.0,3.0,1.0};
      float Shinyness[] = {16};
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      //  Set materials
      glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Diffuse);
      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diffuse);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
      glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   }

   //  Draw the model
   glPushMatrix();
   glTranslated(gx,gy,gz);
   skel->drawSkeleton();
   glPopMatrix();
   ErrCheck("skeleton");

   //  Flush skeleton and read pixel color and depth
   glFlush();
   unsigned char mc[4];
   float mz;
   glReadPixels(mx,my,1,1,GL_RGBA,GL_UNSIGNED_BYTE,mc);
   glReadPixels(mx,my,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&mz);
   //  Convert pixel to world coordinates
   int VP[4];
   double PRJ[16],MV[16],X,Y,Z;
   glGetDoublev(GL_PROJECTION_MATRIX,PRJ);
   glGetDoublev(GL_MODELVIEW_MATRIX,MV);
   glGetIntegerv(GL_VIEWPORT,VP);
   gluUnProject(mx,my,mz,MV,PRJ,VP,&X,&Y,&Z);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      float f=30;
      glBegin(GL_LINES);
      glVertex3f(0,0,0);
      glVertex3f(f,0,0);
      glVertex3f(0,0,0);
      glVertex3f(0,f,0);
      glVertex3f(0,0,0);
      glVertex3f(0,0,f);
      glEnd();
      //  Label axes
      glRasterPos3f(f,0,0);
      Print("X");
      glRasterPos3f(0,f,0);
      Print("Y");
      glRasterPos3f(0,0,f);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s",
     th,ph,dim,proj?"Perpective":"Orthogonal");
   Print(" x:%d y:%d z:%d",gx,gy,gz);
   Print(" Bone: %c",label);
   //  Show pixel location and color unless it is the background
   if (mc[0]+mc[1]+mc[2]+mc[3])
   {
      glWindowPos2i(5,25);
      Print("%.3f,%.3f,%.3f = %d,%d,%d,%d",X,Y,Z,mc[0],mc[1],mc[2],mc[3]);
   }
   
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      skel->setAng(label-65,5,0);
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      skel->setAng(label-65,-5,0);
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      skel->setAng(label-65,0,5);
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      skel->setAng(label-65,0,-5);
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 5;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>0.2)
      dim -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27) exit(0);
   //  Reset view angle
   else if (ch == '0') th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X') axes = 1-axes;
   // adjust global xyz offset
   else if (ch == '1') gx -= 5;
   else if (ch == '2') gx += 5;
   else if (ch == '3') gy -= 5;
   else if (ch == '4') gy += 5;
   else if (ch == '5') gz -= 5;
   else if (ch == '6') gz += 5;
   // select bone
   else if ((ch >= 'a' && ch <= 'w') || (ch >= 'A' && ch <= 'W'))
   {
      skel->setLabel(toupper(ch));
      label = toupper(ch);
   }

   //  Reproject
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

// void menu(int m)
// {
//    int model = (m/10)%10;
//    //  Toggle light
//    if (m==1)
//       light = !light;
//    //  Toggle projection
//    else if (m==2)
//    {
//       proj = !proj;
//       Project(proj?fov:0,asp,dim);
//    }
//    //  Swap skeletons
//    else if (m==3)
//       rev = !rev;
//    //  Rescale
//    else if (m==4)
//       scale = !scale;
//    //  Mode
//    else if (m/100==1)
//       mode[model] = m%10;
//    //  Color
//    else if (m/100==2)
//       kol[model] = m%10;
//    glutPostRedisplay();
// }

/*
 *  Mouse coordinates
 */
void motion(int x,int y)
{
   if (move!=1) return;
   th += x-mx;
   ph += y-my;
   mx = x;
   my = y;
   glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
   //  Add a point
   if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
   {
      move = 1;
      mx = x;
      my = y;
   }
   //  Stop move
   else if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
      move = 0;
   // Wheel reports as button 3(scroll up) and button 4(scroll down)
   else if ((button == 3) || (button == 4)) // It's a wheel event
   {
       // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
       if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
       if(button == 3) dim -= 5;
       else dim += 5;
   }
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project(proj?fov:0,asp,dim);
}

// clean up dynamically allocated memory
void onExit()
{
   cout << "EXIT\n";
   skel.reset();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   if (argc != 1) Fatal("Usage: %s\n",argv[0]);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Skeleton");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutMotionFunc(motion);
   glutMouseFunc(mouse);
   atexit(onExit);

   Skeleton * new_skel = new Skeleton();
   skel.reset(new_skel);
   new_skel = nullptr;
   skel->setLabel('A');

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
