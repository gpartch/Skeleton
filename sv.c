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

int axes=1;         //  Display axes
int light=1;        //  Toggle light
int proj=1;         //  Projection type
int th=0;           //  Azimuth of view angle
int ph=0;           //  Elevation of view angle
int fov=55;         //  Field of view (for perspective)
int mx=0,my=0;      //  Mouse coordinates
double off=0;       //  Z offset
double asp=1;       //  Aspect ratio
double dim=1.5;     //  Size of world
int Nskel=1;        //  Number of skeletons
int kol[2]={0,0};   //  Color
int mode[2]={0,0};  //  Display mode
int rev=0;          //  Reverse draw order
int scale=0;        //  Adjust scale
vbo_t skel[2];      //  Object display list

//
//  Draw skeleton
//
void skeleton(int k)
{
   glPushMatrix();
   if (scale) glScalef(1/skel[k].dim,1/skel[k].dim,1/skel[k].dim);
   float color[4][4] = {{1,1,1,1},{1,0,0,1},{0,1,0,1},{0,0,1,1}};
   if (mode[k]==0)
   {
      glColor4fv(color[kol[k]]);
      DrawModel(skel[k]);
   }
   else if (mode[k]==1)
   {
      glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(1,1);
      glColorMask(0,0,0,0);
      DrawModel(skel[k]);
      glDisable(GL_POLYGON_OFFSET_FILL);
      glColorMask(1,1,1,1);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glColor4fv(color[kol[k]]);
      DrawModel(skel[k]);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   }
   else
   {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glColor4fv(color[kol[k]]);
      DrawModel(skel[k]);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   }
   glPopMatrix();
}

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
   glTranslated(0,off,0);
   //  Draw skeleton
   if (Nskel==1)
      skeleton(0);
   else if (rev)
   {
      skeleton(1);
      skeleton(0);
   }
   else
   {
      skeleton(0);
      skeleton(1);
   }

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

   //  Reset transform
   glPopMatrix();

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      float f=0.3;
      glBegin(GL_LINES);
      glVertex3f(0,0,0);
      glVertex3f(f,0,0);
      glVertex3f(0,0,0);
      glVertex3f(0,1,0);
      glVertex3f(0,0,0);
      glVertex3f(0,0,f);
      glEnd();
      //  Label axes
      glRasterPos3f(f,0,0);
      Print("X");
      glRasterPos3f(0,1,0);
      Print("Y");
      glRasterPos3f(0,0,f);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s",
     th,ph,dim,proj?"Perpective":"Orthogonal");
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
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>0.2)
      dim -= 0.1;
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
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Offset
   else if (ch == '-')
      off -= 0.05;
   else if (ch == '+')
      off += 0.05;
   //  Reproject
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void menu(int m)
{
   int model = (m/10)%10;
   //  Toggle light
   if (m==1)
      light = !light;
   //  Toggle projection
   else if (m==2)
   {
      proj = !proj;
      Project(proj?fov:0,asp,dim);
   }
   //  Swap skeletons
   else if (m==3)
      rev = !rev;
   //  Rescale
   else if (m==4)
      scale = !scale;
   //  Mode
   else if (m/100==1)
      mode[model] = m%10;
   //  Color
   else if (m/100==2)
      kol[model] = m%10;
   glutPostRedisplay();
}

/*
 *  Mouse coordinates
 */
void motion(int x,int y)
{
   mx = x;
   my = glutGet(GLUT_WINDOW_HEIGHT)-y;
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

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   if (argc!=2 && argc!=3) Fatal("Usage: %s <model>\n",argv[0]);
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
   glutPassiveMotionFunc(motion);
   //  Load skeletons
   skel[0] = LoadModel(argv[1]);
   if (argc==3)
   {
      skel[1] = LoadModel(argv[2]);
      Nskel = 2;
   }
   //  Set menus
   int menuid[2];
   for (int k=0;k<Nskel;k++)
   {
      // Mode Setting
      char* mode[] = {"Solid","WireHidden","Wireframe"};
      int modid = glutCreateMenu(menu);
      for (int i=0;i<3;i++)
         glutAddMenuEntry(mode[i],100+10*k+i);
      // Color setting
      char* color[] = {"white","red","green","blue"};
      int colid = glutCreateMenu(menu);
      for (int i=0;i<4;i++)
         glutAddMenuEntry(color[i],200+10*k+i);
      menuid[k] = glutCreateMenu(menu);
      glutAddSubMenu("Mode",modid);
      glutAddSubMenu("Color",colid);
   }
   glutCreateMenu(menu);
   glutAddMenuEntry("Toggle lighting",1);
   glutAddMenuEntry("Toggle projection",2);
   if (Nskel>1) glutAddMenuEntry("Swap draw order",3);
   glutAddMenuEntry("Rescale",4);
   for (int k=0;k<Nskel;k++)
      glutAddSubMenu(argv[k+1],menuid[k]);
   glutAttachMenu(GLUT_RIGHT_BUTTON);

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
