#ifndef GENERATOR_H
#define GENERATOR_H

#include "vector3.h"
#include "body.h"
#include "Globals.h"

// this function is used to generate one sphere and push back to mydomain
void one_sphere(vector<Body> &mydomain, double radius, int rank,int boundary)
{
    // X range for the center of the sphere : 0 to p-1
    // Y range for the center of the sphere  : [rank-rank+1)
    // Z range for the center of the sphere : [0-p)
    float x = RangeFloat(0,boundary);
    float y = RangeFloat(rank,rank+1);
    float z = RangeFloat(0,boundary);
    mydomain.push_back(vector3(x,y,z));
}
void generate_spheres(vector<Body> &mydomain, double radius, int num, int rank, int boundary)
{
    /*
     * mydomain :  holder for all the spheres
     * radius : radius of the sphere
     * num : number of spheres to generate
     * rank : rank of the processor which calls this function
     * boundary: size of the universe = nprocs
     */

    REP(i,0,num)
    {
        // generate a sphere and push it back
        one_sphere(mydomain,radius,num,rank);
    }
}

void prune_layers(vector<Body> &mydomain,float factor)
{
    // remove factor random bodies from the domain
    // the remaining number of bodies is    mydomain.size() * factor
    int retain = mydomain.size()*factor;
    std::random_shuffle(all(mydomain));
    mydomain.erase(mydomain.begin()+retain,mydomain.end());
}

// domain is filled with mydomain
void fill_domain(vector<Body> &bodies, map<int,Body> &domain)
{
    // fill the vector of bodies in the map along with the id
    tr(bodies,it)
    {
        // initially, forced change type to Body(*it), actually it doesn't need to 
        // since *it itself is a Body type
        //domain.insert(make_pair(it->get_id(),Body(*it)));
        domain.insert(make_pair(it->get_id(),*it));
    }
}

void fill_layers(map<int,Body> &mydomain,double r,int rank,int boundary,int startIndex,float retain=0.1)
{
    /*
     * First pack the domain of the current rank with the spheres
     * Random shuffle the elements in the vector
     * Remove the fraction of the elements at the end
     */
    vector<Body> bodies;
    int id = startIndex;
    for(float k=r; k < boundary-r; k+=2*r)   // z coordinate
    {
        for(float j=rank+r; j < rank+1-r; j+= 2*r) // y coordinate 
        {
            for(float i=r;i<boundary-r;i+=2*r) // x coordinate
            {
                // the constructor with position, id, and radius
                bodies.push_back(Body(vector3(i,j,k),id,r));
                id++;
                //std::cout<<i<<","<<j<<","<<k<<std::endl;
            }
        }
    }
    prune_layers(bodies,retain);
    fill_domain(bodies,mydomain);
}


int maxnumber(double r,int boundary)
{
    return ceil(boundary/2*r)  * ceil(1/2*r) * ceil(boundary/2*r);
}

#endif

