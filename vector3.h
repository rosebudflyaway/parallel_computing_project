#ifndef __vector3_h
#define __vector3_h

#include "vector2.h"

class vector3 {
    private:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & x;
                ar & y;
                ar & z;
            }
public:
  float x,y,z;   
  
public:
  vector3():x(),y() {}
  vector3(float xx,float yy,float zz):x(xx),y(yy),z(zz) {}
  vector3(const vector3 &v) {x=v.x;y=v.y;z=v.z;}
  
  float &operator [](int i) {return (&x)[i];}
  const float &operator [](int i) const {return (&x)[i];}
  
  vector3  &operator=(const vector3 &v) {x=v.x;y=v.y;z=v.z;return *this;}
  
  operator vector2() const {return vector2(x,y);}
  
  bool     operator ==(const vector3 &p) const {return ((x==p.x)&&(y==p.y)&&(z==p.z));}
  bool     operator !=(const vector3 &p) const {return !((*this)==p);}
  
  vector3  &operator+=(const vector3 &p) {x+=p.x;y+=p.y;z+=p.z;return *this;}
  vector3  operator + (const vector3 &p) const {return vector3(x+p.x,y+p.y,z+p.z);}
  
  vector3  &operator-=(const vector3 &p) {x-=p.x;y-=p.y;z-=p.z;return *this;}
  vector3  operator - (const vector3 &p) const {return vector3(x-p.x,y-p.y,z-p.z);}
  vector3  operator - () const {return vector3(-x,-y,-z);}
  
  vector3  &operator*=(float f) {x*=f;y*=f;z*=f;return *this;} 
  vector3  operator * (float f) const {return vector3(x*f,y*f,z*f);}

  vector3  &operator/=(float f) {x/=f;y/=f;z/=f;return *this;}
  vector3  operator / (float f) const {return vector3(x/f,y/f,z/f);}

  vector3  mult_elem(const vector3 &p) const {
      return vector3(x*p.x,y*p.y,z*p.z);}

  float  operator * (const vector3 &v2) {return x*v2.x+y*v2.y+z*v2.z;}

  float square() { return x*x+y*y+z*z;}

  float norml() {return sqrt(this->square());}

  void print() const
  {
      std::cout<<x<<","<<y<<","<<z<<std::endl;
  }

  vector3 Normalize()
  {
      float n= this->norml();
      if(n < 1.0E-12 && n > 0)
      {
          std::cout << "Normalize: normalizing a null vector " << std::endl;
      }
      else
      {
          return ((*this) / n);
      }
  }   

};
#endif //__vector3_h

