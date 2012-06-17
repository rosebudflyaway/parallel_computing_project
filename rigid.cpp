/*
 *
 * This is the main executable for the rigid body simulation 
 */
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <iostream>
#include "Globals.h"
#include "generator.h"
//#include "communication.h"
//#include "message.h"
#include "package.h"
#include "item.h"
#include "boundary.h"
#include "body.h"
#include "vector3.h"
#include "fpsolver.h"
#include "update.h"
#include "statistics.h"
#include "TimeTracker.h"
#include <fstream>
typedef std::map<int,Body> domain; //  key is the unique id of the ball and the value is the body object

namespace mpi = boost::mpi;


float retain; // number of spheres per processor initially
double radius; // radius of each sphere
int iterations; // number of iterations of the entire simulation
bool print=false;

//ofstream myfile("contacts.txt");

void print_usage(char *prog) 
{
    std::cerr<<"Usage: "<<prog<<" -np nprocs -F retain -R radius -I iterations -[P] print"<<std::endl;
    exit(0);
}

void parse_args(int argc, char* argv[]) 
{
    if(argc<4) 
    {
        print_usage(argv[0]);
    }

    for (int i=1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-F") == 0)
        {
            retain = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-R") == 0)
        {
            radius = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-I") == 0)
        {
            iterations = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-P") == 0)
        {
            print = true;
        }
    }
}//end parse_args()

int main(int argc,char *argv[])
{
    // ********************************* Varaibles **************************** //
    int myrank,nprocs;
    domain mydomain;             // all the bodies in the current rank's domain is called mydomain..it is mergeed with the bodies received from the neighbors and the solver is called.
    Item<Body> idomain;          // an item object to hold the spheres
    Item<Body> msg;              // bodies received from the neighbors
    Item<vector3> force;         // force of the local bodies
    Item<vector3> vmsg;          // force in form of vector msg from the neighbors
    int max_number;              // max number of spheres that are present in each rank before pruning i.e number if retain = 1
    mii returnids;               // the destination of the spheres to send the computed the forces
    mii stat;                    // whether there are send or recv
    mpi::environment env(argc, argv);
    mpi::communicator world;
    Stat st;                     // to write out the statistics
    Tracker trk(2070000000);     // stores the times for various operations

    // ******************************* Initialization ************************* //
    parse_args(argc,argv);
    myrank = world.rank();
    nprocs = world.size();
    max_number = maxnumber(radius,nprocs);  // generator.h
    fill_layers(mydomain,radius,myrank,nprocs,myrank*max_number,retain);
    idomain.add_items(mydomain); // all the items inserted into the Item object
    if(myrank==0)
        cout<<"iterations:" << iterations << "\nRadius:" << radius << "\nRetain:" << retain << "\nNumber of spheres:" << idomain.size() << endl;
    Package<Item<Body> > pck(myrank,nprocs); // for sending the bodies
    Package<Item<vector3> > vecpck(myrank,nprocs); // for sending the vector3

    if(myrank==0)
    {
        Body colliding = Body(vector3(0.5,0.95,0.65),1,0.1);
        idomain.item.insert(make_pair(colliding.get_id(),colliding)); // colliding is a body, this is to test the collision
    }


    // ***************************** Iterations ******************************** // 
    REP(i,0,iterations)
    {
        // 0 )  append the positions of the bodies
        st.append_position(idomain.item);  // position is a map of map<int, vector<vector3> >
       
        // 1 ) send the spheres that cross the domain to the corresponding rank
        trk.start("send");
        pck.send(idomain.item,world);
        trk.end("send");

        trk.start("recv");
        pck.recv(world,msg);
        trk.end("recv");
        msg.add_items(idomain.item);
        /*if(myrank==1)
        {
            cout<<" number of spheres received is " <<msg.size()<<endl;
        }*/

        // 2 ) Call the solver
        trk.start("solve");
        solve(msg.item,force.item,returnids);
        trk.end("solve");

        // 3 ) send the forces of the neighbors

        trk.start("send");
        vecpck.vsend(force.item,returnids,world);
        trk.end("send");

        trk.start("recv");
        vecpck.recv(world,vmsg);
        trk.end("recv");
        /*if(myrank==0)
        {
            cout<<" number of vector forces received are "<<vmsg.size()<<endl;
            vmsg.print();
        }*/
        force.merge(vmsg);

        // 4 ) update the velocities

        update_all_velocity(idomain,force);
        manage_bodies(idomain,msg,myrank); // remove the bodies from idomain if it crosses the boundary and add the bodies from the msg to idomain 
        pck.cleanup();
        vecpck.cleanup();
        msg.clear();
        vmsg.clear();
    }
    if(myrank==0)
    {
        st.write_position(myrank);
        trk.print_all();
    }
    return 0;
}
