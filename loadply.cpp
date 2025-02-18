//  CSCIx239 library
//  Willem A. (Vlakkies) Schreuder
//

#include "Skeleton.hpp"
#include <sys/types.h>

//  Maximum number of fields
#define MAX 256
const char* xyzw[4] = {"x","y","z","w"};
const char* nxyz[3] = {"nx","ny","nz"};
const char* rgba[4] = {"red","green","blue","alpha"};

//  Vector 3 math
typedef struct {float x,y,z;} vec3;
static vec3 sub(vec3 v1,vec3 v2) {return (vec3){v1.x-v2.x , v1.y-v2.y , v1.z-v2.z};}
static vec3 cross(vec3 v1,vec3 v2) {return (vec3){v1.y*v2.z-v2.y*v1.z , v1.z*v2.x-v2.z*v1.x , v1.x*v2.y-v2.x*v1.y};}
static vec3 normalize(vec3 v)
{
   float d = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
   if (d>0) v = (vec3){v.x/d , v.y/d , v.z/d};
   return v;
}
//  Get double vector
static vec3 getvec8(double xyzw[],int n)
{
   vec3 v;
   if (n==4)
   {
      v.x = xyzw[0]/xyzw[3];
      v.y = xyzw[1]/xyzw[3];
      v.z = xyzw[2]/xyzw[3];
   }
   else if (n==3)
   {
      v.x = xyzw[0];
      v.y = xyzw[1];
      v.z = xyzw[2];
   }
   else
   {
      v.x = xyzw[0];
      v.y = xyzw[1];
      v.z = 0;
   }
   return v;
}
//  Get float vector
static vec3 getvec4(float xyzw[],int n)
{
   vec3 v;
   if (n==4)
   {
      v.x = xyzw[0]/xyzw[3];
      v.y = xyzw[1]/xyzw[3];
      v.z = xyzw[2]/xyzw[3];
   }
   else if (n==3)
   {
      v.x = xyzw[0];
      v.y = xyzw[1];
      v.z = xyzw[2];
   }
   else
   {
      v.x = xyzw[0];
      v.y = xyzw[1];
      v.z = 0;
   }
   return v;
}
//  In place vector summation
static void addvec(float* V,vec3 v)
{
   V[0] += v.x;
   V[1] += v.y;
   V[2] += v.z;
}

//
//  Find matching vertex group
//
static int findname(char* name[],int Nvar,const char* v[],int n)
{
   for (int k=0;k<=Nvar-n;k++)
   {
      int i=0;
      for (int j=0;j<n;j++)
         if (strcmp(v[j],name[k+j])) i++;
      if (!i) return k;
   }
   return -1;
}

//
//  Find matching vertex group
//
static buf_t find(char* name[],off_t off[],int type[],int Nvar,const char* v[],int n)
{
   buf_t buf = {};
   int k = findname(name,Nvar,v,n);
   //  Mark as found
   if (k>=0)
   {
      buf.n    = n;
      buf.off  = (void*)off[k];
      buf.type = type[k];
   }
   return buf;
}

//
//  Load PLY file
//
vbo_t Skeleton::LoadPLY(const char* file, int inv_norm)
{
   //  Open file
   FILE* f = fopen(file,"r");
   if (!f) Fatal("Cannot open file %s\n",file);
   //  Check initial line for header
   char* line = readline(f);
   if (compstr(line,toCStr("ply"))) Fatal("Invalid PLY header in %s\n",file);
   //  Expect binary_little_endian or ascii
   line = readline(f);
   int bin = !compstr(line,toCStr("format binary_little_endian 1.0"));
   if (!bin && compstr(line,toCStr("format ascii 1.0"))) Fatal("Invalid PLY format %s\n",line);
   //  Skip comments
   while ((line = readline(f)) && compstr(line,toCStr("comment "))==0) {}
   //  Get vertex count
   int Nv;
   if (compstr(line,toCStr("element vertex ")))
      Fatal("Expected element vertex %s\n",line);
   else if (sscanf(line+strlen("element vertex "),"%d",&Nv)!=1)
      Fatal("Error reading vertex count: %s\n",line);
   else if (Nv<1)
      Fatal("Invalid vertex count: %d\n",Nv);
      
   //  Process fields
   int   type[MAX];
   off_t off[MAX];
   char* name[MAX];
   int N=0,Nvar=0;
   for (int k=0 ; (line=readline(f)) && compstr(line,toCStr("property "))==0 && k<MAX ; k++)
   {
      Nvar++;
      off[k] = N;
      if (compstr(line,toCStr("property float "))==0)
      {
         name[k] = strdup(line+strlen("property float "));
         type[k] = GL_FLOAT;
         N += 4;
      }
      else if (compstr(line,toCStr("property double "))==0)
      {
         name[k] = strdup(line+strlen("property double "));
         type[k] = GL_DOUBLE;
         N += 8;
      }
      else if (compstr(line,toCStr("property uchar "))==0)
      {
         name[k] = strdup(line+strlen("property uchar "));
         type[k] = GL_UNSIGNED_BYTE;
         N += 1;
      }
      else if (compstr(line,toCStr("property "))==0)
         Fatal("Not implemented %s\n",line);
   }

   //  Get face count
   int Nf;
   if (compstr(line,toCStr("element face ")))
      Fatal("Expected element face %s\n",line);
   else if (sscanf(line+strlen("element face "),"%d",&Nf)!=1)
      Fatal("Error reading face count: %s\n",line);
   else if (Nf<1)
      Fatal("Invalid face count: %d\n",Nf);
   //  Get face lists
   line = readline(f);
   if (compstr(line,toCStr("property list uchar int vertex_ind")) &&
       compstr(line,toCStr("property list uchar uint vertex_ind")))
      Fatal("Expected property list uchar int vertex_ind: %s\n",line);
   //  Skip until end_header
   while ((line = readline(f)) && compstr(line,toCStr("end_header"))) {}
   if (!line || compstr(line,toCStr("end_header")))
      Fatal("Cannot find end_header: %s\n",line);

   //  Pad with normal if required
   int N0    = N;
   int Nvar0 = Nvar;
   if (findname(name,Nvar,nxyz,3)<0)
   {
      for (int k=0;k<3;k++)
      {
         off[Nvar] = N;
         name[Nvar] = strdup(nxyz[k]);
         type[Nvar] = GL_FLOAT;
         Nvar++;
         N += 4;
      }
   }

   //  Vertex buffer structure
   vbo_t vbo = {.type=1,.n=3*Nf,.stride=N};
   //  Vertex
   vbo.vertex = find(name,off,type,Nvar,xyzw,4);
   if (!vbo.vertex.n) vbo.vertex = find(name,off,type,Nvar,xyzw,3);
   if (!vbo.vertex.n) Fatal("No vertex coordinates in PLY\n");
   //  Normals
   vbo.normal = find(name,off,type,Nvar,nxyz,3);
   //  Colors
   vbo.color = find(name,off,type,Nvar,rgba,4);
   if (!vbo.color.n) vbo.color = find(name,off,type,Nvar,rgba,3);

   //  Allocate vertex and element memory
   // unsigned char* V = malloc(Nv*N);
   unsigned char* V = new unsigned char[Nv * N];
   if (!V) Fatal("Cannot allocate vertex memory\n");
   // unsigned int* E = malloc(3*Nf*sizeof(int));
   unsigned int* E = new unsigned int[3 * Nf];
   if (!E) Fatal("Cannot allocate VBO data\n");

   // Snarf vertex data
   if (!bin)
   {
      unsigned char* v=V;
      for (int k=0;k<Nv;k++)
      {
         line = readline(f);
         if (!line) Fatal("Error reading vertex line %d\n",k);
         for (int i=0;i<Nvar0;i++)
         {
            char* word = getword(&line);
            switch (type[i])
            {
               case GL_FLOAT:
                  if (sscanf(word,"%f",(float*)(v+off[i]))!=1)
                     Fatal("Error reading float %s vertex %d\n",name[i],k);
                  break;
               case GL_DOUBLE:
                  if (sscanf(word,"%lf",(double*)(v+off[i]))!=1)
                     Fatal("Error reading double %s vertex %d\n",name[i],k);
                  break;
               case GL_UNSIGNED_BYTE:
                  if (sscanf(word,"%hhd",v+off[i])!=1)
                     Fatal("Error reading uchar %s vertex %d\n",name[i],k);
                  break;
            }
         }
         v += N;
      }
   }
   //  Binary includes normals
   else if (N==N0)
   {
      // if (fread(V,N,Nv,f)!=Nv) Fatal("Cannot read %d vertexes\n",Nv);
   }
   //  Need to calculate normals
   else
   {
      memset(V,0,Nv*N);
      unsigned char* v=V;
      for (int k=0;k<Nv;k++)
      {
         if (fread(v,N0,1,f)!=1) Fatal("Cannot read vertex %d\n",k);
         v += N;
      }
   }

   //  Read faces and assemble EBO
   if (bin)
   {
      for (int k=0;k<Nf;k++)
      {
         //  Must be triangles
         unsigned char n;
         if (fread(&n,1,1,f)!=1) Fatal("Cannot read facet count %d\n",k);
         if (n!=3) Fatal("Invalid facet count1 %d for %d\n",n,k);
         //  Read index
         if (fread(E+3*k,sizeof(int),3,f)!=3) Fatal("Cannot read facet index %d\n",k);
      }
   }
   else
   {
      unsigned int* e=E;
      for (int k=0;k<Nf;k++)
      {
         line = readline(f);
         if (!line) Fatal("Error reading face line %d\n",k);
         unsigned char n;
         if (sscanf(line,"%hhd %d %d %d",&n,e,e+1,e+2)!=4)
           Fatal("Error reading index: %s\n",line);
         else if (n!=3)
           Fatal("Invalid facet count2 %d for %d\n",n,k);
         e += 3;
      }
   }
   fclose(f);

   //  Calculate normals
   if (N!=N0)
   {
      for (int l=0;l<Nf;l++)
      {
         //  Get vertex index
         int i = *(int*)(E+3*l+0);
         int j = *(int*)(E+3*l+1);
         int k = *(int*)(E+3*l+2);
         //  Get vertex
         vec3 I,J,K;
         if (vbo.vertex.type==GL_DOUBLE)
         {
            I = getvec8((double*)(V+(long)vbo.vertex.off+N*i),vbo.vertex.n);
            J = getvec8((double*)(V+(long)vbo.vertex.off+N*j),vbo.vertex.n);
            K = getvec8((double*)(V+(long)vbo.vertex.off+N*k),vbo.vertex.n);
         }
         else
         {
            I = getvec4((float*)(V+(long)vbo.vertex.off+N*i),vbo.vertex.n);
            J = getvec4((float*)(V+(long)vbo.vertex.off+N*j),vbo.vertex.n);
            K = getvec4((float*)(V+(long)vbo.vertex.off+N*k),vbo.vertex.n);
         }
         //  Compute normal for triangle
         vec3 A = sub(J,I);
         vec3 B = sub(K,J);
         vec3 nml = normalize(cross(A,B));
         if(inv_norm)
         {
            nml.x = -nml.x;
            nml.y = -nml.y;
            nml.z = -nml.z;
         }
         //  Add triangle normal to vertexes
         addvec((float*)(V+(long)vbo.normal.off+N*i),nml);
         addvec((float*)(V+(long)vbo.normal.off+N*j),nml);
         addvec((float*)(V+(long)vbo.normal.off+N*k),nml);
      }
   }

   //  Calculate dimensions
   if (vbo.vertex.type==GL_DOUBLE)
      for (int k=0;k<Nv;k++)
         vbo.dim = maxdim8(vbo.dim,(double*)(V+(long)vbo.vertex.off+N*k),vbo.vertex.n);
   else
      for (int k=0;k<Nv;k++)
         vbo.dim = maxdim4(vbo.dim,(float*)(V+(long)vbo.vertex.off+N*k),vbo.vertex.n);

   //  Copy vertex data to VBO
   glGenBuffers(1,&vbo.buf);
   glBindBuffer(GL_ARRAY_BUFFER,vbo.buf);
   glBufferData(GL_ARRAY_BUFFER,Nv*N,V,GL_STATIC_DRAW);
   //  Copy element data to EBO
   glGenBuffers(1,&vbo.ele);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo.ele);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*Nf*sizeof(int),E,GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   //  Free arrays
   // free(V);
   // free(E);
   delete[] V;
   delete[] E;
   for (int k=0;k<Nvar;k++)
      free(name[k]);

   return vbo;
}
