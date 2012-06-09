
#ifndef __vector2_h
#define __vector2_h

#include <stdio.h>   
#include <math.h>    
#include <iostream>
#include <limits.h>

class vector2 {
private:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & x;
                ar & y;
            }
public:
  float   x,y;
  
public:
  vector2():x(),y() {}
  vector2(float xx,float yy):x(xx),y(yy) {}
  vector2(const vector2 &v):x(v.x),y(v.y) {}
  
  float &operator [](int i) {return (&x)[i];}
  const float &operator [](int i) const {return (&x)[i];}
  
  vector2   &operator =(const vector2 &v) {x=v.x; y=v.y; return *this;}
  
  bool     operator ==(const vector2 &p) const {return ((x==p.x)&&(y==p.y));}
  bool     operator !=(const vector2 &p) const {return !((*this)==p);}
  
  vector2  &operator+=(const vector2 &p) {x+=p.x;y+=p.y;return *this;}
  vector2  operator + (const vector2 &p) const {return vector2(x+p.x,y+p.y);}
  
  vector2  &operator-=(const vector2 &p) {x-=p.x;y-=p.y;return *this;}
  vector2  operator - (const vector2 &p) const {return vector2(x-p.x,y-p.y);}
  vector2  operator - () const {return vector2(-x,-y);}
  
  vector2  &operator*=(float f) {x*=f;y*=f;return *this;} 
  vector2  operator * (float f) const {return vector2(x*f,y*f);}
  
  vector2  &operator/=(float f) {x/=f;y/=f;return *this;}
  vector2  operator / (float f) const {return vector2(x/f,y/f);}
  
};

inline vector2 operator *(float f, const vector2 &v) {return v*f;}
#endif //__vector2_h

