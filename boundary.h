 
#ifndef BOUNDARY_h
#define BOUNDARY_h

#include "Globals.h"
#include "vector3.h"
#include "body.h"

typedef map<int,Body> domain;

int status(Body &bd,int rank)
{
    /*
     * Returns 0 : if the current position doesnt cross the boundary
     *         1 : if the body belongs to the rank above the current one
     *        -1 : if it belongs to rank below the current one 
     * 
     * Compute (pos.y - radius) and return the value based on the center of the body 
     * */
    float y = bd.get_y();
    if(y+bd.get_rad()>rank+1)
        return 1;
    else if(y-bd.get_rad()<rank)
        return -1;
    else
        return 0;
}

void cross_boundary(map<int,Body> &domain, mii &stat,int myrank)
{
    // for each body find whether it still remains to be in the same domain or needs to sent to neighbors
    // Iterate over each body in the vector and copy if it crosses the boundary
    tr(domain,it)
    {
        stat.insert(make_pair( (it->second).get_id(),status(it->second,myrank)));  // here status() is the function defined above
    }
}

void merge_domain(domain &mydomain, domain &neighbors)
{
    mydomain.insert(all(neighbors));
}

#endif
