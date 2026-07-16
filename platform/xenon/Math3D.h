#pragma once
#include <cmath>

struct Vec3 { float x{}, y{}, z{}; };
struct Mat4 { float m[16]{}; };

inline Vec3 operator+(Vec3 a, Vec3 b){ return {a.x+b.x,a.y+b.y,a.z+b.z}; }
inline Vec3 operator-(Vec3 a, Vec3 b){ return {a.x-b.x,a.y-b.y,a.z-b.z}; }
inline Vec3 operator*(Vec3 a,float s){ return {a.x*s,a.y*s,a.z*s}; }
inline float dot(Vec3 a,Vec3 b){ return a.x*b.x+a.y*b.y+a.z*b.z; }
inline Vec3 cross(Vec3 a,Vec3 b){ return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x}; }
inline Vec3 normalize(Vec3 v){ float l=std::sqrt(dot(v,v)); return l>0.0001f?v*(1.0f/l):Vec3{}; }

inline Mat4 identity(){ Mat4 r{}; r.m[0]=r.m[5]=r.m[10]=r.m[15]=1; return r; }
inline Mat4 multiply(const Mat4&a,const Mat4&b){ Mat4 r{}; for(int row=0;row<4;++row)for(int col=0;col<4;++col)for(int k=0;k<4;++k)r.m[row*4+col]+=a.m[row*4+k]*b.m[k*4+col]; return r; }
inline Mat4 perspective(float fov,float aspect,float zn,float zf){ Mat4 r{}; float f=1.0f/std::tan(fov*0.5f); r.m[0]=f/aspect; r.m[5]=f; r.m[10]=zf/(zf-zn); r.m[11]=1; r.m[14]=(-zn*zf)/(zf-zn); return r; }
inline Mat4 lookAt(Vec3 eye,Vec3 target,Vec3 up){ Vec3 z=normalize(target-eye); Vec3 x=normalize(cross(up,z)); Vec3 y=cross(z,x); Mat4 r=identity(); r.m[0]=x.x;r.m[1]=y.x;r.m[2]=z.x; r.m[4]=x.y;r.m[5]=y.y;r.m[6]=z.y; r.m[8]=x.z;r.m[9]=y.z;r.m[10]=z.z; r.m[12]=-dot(x,eye);r.m[13]=-dot(y,eye);r.m[14]=-dot(z,eye); return r; }
