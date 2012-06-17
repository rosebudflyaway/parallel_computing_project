#ifndef BODY_H
#define BODY_H

#include "vector3.h"
#include <iostream>

class Body
{
    private:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & position;
                ar & force;
                ar & velocity; 
                ar & lambda;
                ar & mass;
                ar & radius;
                ar & id;
            }
        vector3 position;
        vector3 force;
        vector3 velocity; 
        vd lambda;
        double mass;
        double radius;
        int id;
    public:
        Body(){}
        Body(const vector3 &p,int uniqueid=1,double rad=0)
        {
            position = p;
            radius = rad;
            //force = vector3(0.0,0.0,-9.8);
            id = uniqueid;
            //force = vector3(0.0,0.0,-0.0098*(id+1)); //  testing
            force = vector3(0.0,0.0,-0.0098); //  testing
            mass = 1.0;
            velocity = vector3(0,0,0);
        }
        Body(const vector3 &p, const vector3 &f)
        {
            // p,f,v are position force and veclocity respectively
            position = p; force= f;
        }
        Body(const vector3 &p, const vector3 &f,const vector3 &v)
        {
            // p,f,v are position force and veclocity respectively
            position = p; force= f; velocity = v;
        }
        void add_vel(const vector3 &f)
        {
            velocity = velocity + f;
        }
        void zero_vel()
        {
            velocity = vector3(0,0,0);
        }

        void add_pos(const vector3 &f)
        {
            position = position + f;
        }
        inline float get_y()
        {
            return position.y;
        }
        inline float get_x()
        {
            return position.x;
        }
        inline float get_z()
        {
            return position.z;
        }
        inline double get_rad()
        {
            return radius;
        }
        inline int get_id()
        {
            return id;
        }
        double get_mass()
        {
            return mass;
        }
        double get_inertia()
        {
            return 0.4*mass*radius*radius;
        }
        vector3 get_position()
        {
            return vector3(position);
        }
        vector3 get_velocity()
        {
            return velocity;
        }
        vector3 get_ext()
        {
            return force;
        }
        void print()
        {
            std::cout<<id<<":"<<"("<<position.x<<","<<position.y<<","<<position.z<<")"<<"-->"<<force.z<<"mass ="<<mass<<"radius="<<radius<<std::endl;
        }
};
#endif
