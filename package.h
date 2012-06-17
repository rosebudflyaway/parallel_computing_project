#ifndef PACKAGE_H
#define PACKAGE_H

#include "Globals.h"
#include "body.h"
#include "vector3.h"
#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include "item.h"
typedef map<int,Body> domain;
namespace mpi = boost::mpi;

template <typename T>
class Package
{
    private:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & send_msg;
                ar & recv_msg;
                ar & rk;
                ar & neighbors;
            }
        map<int,T > send_msg; // int is the rank to which the item must be sent
        map<int,T > recv_msg; // int is the rank the message recieved from
        int rk;  // rank
        int maxrank; // max rank
        vector<int> neighbors; // all the neighbors of the current rank
        void initialize_send();
        void remove_dummy();
        void prepare_send(domain &items,const boost::mpi::communicator &world); // for each item add it to the corresponding Item
        void vprepare_send(const map<int,vector3> &vecs, mii &returnids,const boost::mpi::communicator &world);
        void prepare_recv(const map<int,T> &items);
        void merge(T &total_msg);

    public:
        Package(int rank,int max); // max is the size of the world
        void send(domain &items, const boost::mpi::communicator &world); 
        void vsend(const map<int,vector3> &vecs, mii &returnids, const boost::mpi::communicator &world);
        void recv(const boost::mpi::communicator &world, T &total_msg);
        void print();
        void cleanup();
};

// initialize: to find the two neighbors 
template<typename T>
Package<T>::Package(int rank,int max)
{
    maxrank = max;
    rk = rank;
    // add the two neighbors to the vector
    if(rk+1 < max)
        neighbors.push_back(rk+1);
    if(rk-1 >= 0)
        neighbors.push_back(rk-1);
}


// add a dummy message before sending
template<typename T>
void Package<T>::initialize_send()
{
    // add dummy messages to all the neighbors
    tr(neighbors,it)
    {
        // T.add_dummy();
        // no define function of add_dummy()
        // but here T is something of class Item,
        // so  the add_dummy() is defined there
        send_msg[*it].add_dummy();
    }
}

// remove the dummy message after receiving
template<typename T>
void Package<T>::remove_dummy()
{
    // remove dummy messages to all the neighbors
    tr(neighbors,it)
    {
        recv_msg[*it].remove_dummy();
    }
}


// prepare to send the body before calling the solver
template<>
void Package<Item<Body> >::prepare_send(domain &items, const boost::mpi::communicator &world)
{
    // scan each item and add it to the correct destination
    float y;
    double rad;
    tr(items,it)
    {
        // decide the destination of this item
        y = (it->second).get_y();
        rad = (it->second).get_rad();
        if(y+rad > rk+1 && rk+1<maxrank)
            send_msg[rk+1].add_item(it->first,it->second);
        else if(y-rad< rk && rk-1 >=0)
            send_msg[rk-1].add_item(it->first,it->second);
    }
    // now send the messages
    this->initialize_send();  // add dummy to make sure the message sent is not a NULL message
    tr(send_msg,it)
    {
        world.send(it->first,0,it->second);  // world.send(destination, tag, message)
    }
}

// prepare to send the vector3 force after calling the solver 
template<>
void Package<Item<vector3> >::vprepare_send(const map<int,vector3> &vecs, mii &returnids,const boost::mpi::communicator &world)
{
    // scan each item and add it to the correct destination
    int dest;
    tr(vecs,it)
    {
        dest = returnids[it->first];  // returnids is a map, the first one is the id, the second one is the dest
        if(dest!=rk) // if the return id is not same as the current one
        send_msg[dest].add_item(it->first,it->second);
    }
    // now send the messages
    this->initialize_send();
    tr(send_msg,it)
    {
        world.send(it->first,0,it->second);
    }
}

// send the vector3 force after calling the solver
// every time before sending, we will clear the vector3 force from last time step
template<>
void Package<Item<vector3> >::vsend(const map<int,vector3> &vecs, mii &returnids,const boost::mpi::communicator &world)
{
    send_msg.clear();  // the map<int, T>
    this->vprepare_send(vecs,returnids,world);
}


// send the Body before calling the solver
// every time before sending, we will clear the map of Body from last time step
template<>
void Package<Item<Body> >::send(domain &items,const boost::mpi::communicator &world)
{
    send_msg.clear();
    this->prepare_send(items,world);
}


// recv the map of Body and the vector3 force from the destination processor
template<typename T>
void Package<T>::recv(const boost::mpi::communicator &world, T &total_msg)
{
    // recv messages from the neighbors
    mpi::request req[neighbors.size()];
    REP(i,0,neighbors.size())
    {
        req[i] = world.irecv(neighbors[i],0,recv_msg[neighbors[i]]);
    }
    mpi::wait_all(req,req+neighbors.size());  // wait for all  the requests in a single line
    remove_dummy();
    merge(total_msg);
}

template <typename T>
void Package<T>::merge(T &total_msg)
{
    tr(recv_msg,it)
    {
        // for it->second, it is a type of IteTyp
        //
        total_msg.merge(it->second); // it->first is the rank from which to recive the message
    }
}

template<typename T>
void Package<T>::print()
{
    tr(send_msg,it)
    {
        cout<<" to rank "<<it->first<<endl;  // the it->first is the rank
        (it->second).print();
    }
    tr(recv_msg,it)
    {
        cout<<" from rank "<<it->first<<endl;
        (it->second).print();
    }
}

template<typename T>
void Package<T>::cleanup()
{
    send_msg.clear();
    recv_msg.clear();
}


#endif
