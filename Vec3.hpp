#ifndef VEC3_H
#define VEC3_H

#include <math.h>

//
//  3D vector type
//
class vec3
{
   public:
      float x,y,z;
      vec3(void)
      {
         x = y = z = 0;
      }
      vec3(float v[3])
      {
         x = v[0];
         y = v[1];
         z = v[2];
      }
      vec3(float X,float Y,float Z)
      {
         x = X;
         y = Y;
         z = Z;
      }
   inline vec3& operator+=(const vec3& v) {x+=v.x;y+=v.y;z+=v.z; return *this;}
   inline vec3  operator-() {return vec3(-x,-y,-z);}
   inline vec3 invert();
};

//  vec3 operators
inline vec3 operator+(const vec3& v1  , const vec3& v2) {return vec3(v1.x+v2.x , v1.y+v2.y , v1.z+v2.z);}
inline vec3 operator-(const vec3& v1  , const vec3& v2) {return vec3(v1.x-v2.x , v1.y-v2.y , v1.z-v2.z);}
inline vec3 operator*(const vec3& v1  , const vec3& v2) {return vec3(v1.y*v2.z-v2.y*v1.z,v1.z*v2.x-v2.z*v1.x,v1.x*v2.y-v2.x*v1.y);}
inline vec3 operator*(float f , const vec3& v)  {return vec3(f*v.x,f*v.y,f*v.z);} 
inline float abs(vec3 v) {return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);}
inline float area(vec3 v0,vec3 v1, vec3 v2) {return 0.5*abs((v0-v1)*(v0-v2));}

//  Normalize vec3
inline vec3 normalize(vec3 v)
{
   float len = abs(v);
   return (len==0) ? vec3(1,0,0) : (1/len)*v;
}

//  Max absolute by component
inline float amax(vec3 v)
{
   float m = abs(v.x);
   if (abs(v.y)>m) m = abs(v.y);
   if (abs(v.z)>m) m = abs(v.z);
   return m;
}

//  Unproject
vec3 unproject3(vec3 p,const float mv[16],const float proj[16],const int view[4]);

vec3 vec3::invert()
{
   return vec3(-x,-y,-z);
}

#endif
