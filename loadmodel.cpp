//  CSCIx239 library
//  Willem A. (Vlakkies) Schreuder
#include "Skeleton.hpp"

//
//  Draw VBO data
//
// void Skeleton::DrawVBO(vbo_t vbo)
// {
//    //  Bind VBO
//    glBindBuffer(GL_ARRAY_BUFFER,vbo.buf);
//    //  Set arrays
//    if (vbo.vertex.n)
//    {
//       glVertexPointer(vbo.vertex.n,vbo.vertex.type,vbo.stride,vbo.vertex.off);
//       glEnableClientState(GL_VERTEX_ARRAY);
//    }
//    if (vbo.normal.n)
//    {
//       glNormalPointer(vbo.normal.type,vbo.stride,vbo.normal.off);
//       glEnableClientState(GL_NORMAL_ARRAY);
//    }
//    if (vbo.color.n)
//    {
//       glColorPointer(vbo.color.n,vbo.color.type,vbo.stride,vbo.color.off);
//       glEnableClientState(GL_COLOR_ARRAY);
//    }
//    //  Draw elements
//    if (vbo.ele)
//    {
//       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo.ele);
//       glDrawElements(GL_TRIANGLES,vbo.n,GL_UNSIGNED_INT,(void*)0);
//       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
//    }
//    //  Draw arrays
//    else
//       glDrawArrays(GL_TRIANGLES,0,vbo.n);
//    //  Unset arrays
//    if (vbo.vertex.n) glDisableClientState(GL_VERTEX_ARRAY);
//    if (vbo.normal.n) glDisableClientState(GL_NORMAL_ARRAY);
//    if (vbo.color.n)  glDisableClientState(GL_COLOR_ARRAY);
//    //  Unbind VBO
//    glBindBuffer(GL_ARRAY_BUFFER,0);
// }
void Skeleton::DrawVBO(vbo_t vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo.buf);
    ErrCheck("glBindBuffer");

    if (vbo.vertex.n) {
        glVertexPointer(vbo.vertex.n, vbo.vertex.type, vbo.stride, vbo.vertex.off);
        ErrCheck("glVertexPointer");
        glEnableClientState(GL_VERTEX_ARRAY);
        ErrCheck("glEnableClientState (vertex)");
    }
    if (vbo.normal.n) {
        glNormalPointer(vbo.normal.type, vbo.stride, vbo.normal.off);
        ErrCheck("glNormalPointer");
        glEnableClientState(GL_NORMAL_ARRAY);
        ErrCheck("glEnableClientState (normal)");
    }
    if (vbo.color.n) {
        glColorPointer(vbo.color.n, vbo.color.type, vbo.stride, vbo.color.off);
        ErrCheck("glColorPointer");
        glEnableClientState(GL_COLOR_ARRAY);
        ErrCheck("glEnableClientState (color)");
    }

   //  Draw elements
   if (vbo.ele)
   {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo.ele);
      glDrawElements(GL_TRIANGLES,vbo.n,GL_UNSIGNED_INT,(void*)0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
   }
   //  Draw arrays
   else
      glDrawArrays(GL_TRIANGLES,0,vbo.n);
   //  Unset arrays
   if (vbo.vertex.n) glDisableClientState(GL_VERTEX_ARRAY);
   if (vbo.normal.n) glDisableClientState(GL_NORMAL_ARRAY);
   if (vbo.color.n)  glDisableClientState(GL_COLOR_ARRAY);
   //  Unbind VBO
   glBindBuffer(GL_ARRAY_BUFFER,0);
    ErrCheck("glBindBuffer 0");
}

//  Draw a model file
void Skeleton::DrawModel(vbo_t vbo)
{
   if (vbo.type==0)
      glCallList(vbo.buf);
   else
      DrawVBO(vbo);
}

//  Maximum dimensions
float Skeleton::maxdim8(float dim,double xyz[],int n)
{
   if (n==4)
   {
      for (int k=0;k<3;k++)
         if (fabs(xyz[k]/xyz[3])>dim)
            dim = fabs(xyz[k]/xyz[3]);
   }
   else
   {
      for (int k=0;k<n;k++)
         if (fabs(xyz[k])>dim)
            dim = fabs(xyz[k]);
   }
   return dim;
}
float Skeleton::maxdim4(float dim,float xyz[],int n)
{
   if (n==4)
   {
      for (int k=0;k<3;k++)
         if (fabs(xyz[k]/xyz[3])>dim)
            dim = fabs(xyz[k]/xyz[3]);
   }
   else
   {
      for (int k=0;k<n;k++)
         if (fabs(xyz[k])>dim)
            dim = fabs(xyz[k]);
   }
   return dim;
}


//  Load an Model file
vbo_t Skeleton::LoadModel(const char* file, int inv_norm)
{
   vbo_t vbo = {};
   vbo = LoadPLY(file,inv_norm);
   fprintf(stderr,"%s = %f\n",file,vbo.dim);
   return vbo;
}
