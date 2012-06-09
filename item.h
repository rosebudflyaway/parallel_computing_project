#ifndef ITEM_H
#define ITEM_H

#include "Globals.h"
#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include "vector3.h"

template <typename ItmTyp>
class Item
{
	// this is for the boost::mpi serialization 
	// class Archive
    private:
        friend class boost::serialization::access;
        template<class Archive>
            void serialize(Archive & ar, const unsigned int version)
            {
                ar & item;
            }
    public:
        map<int,ItmTyp> item;
        Item(){}
        Item(const map<int,ItmTyp> &init)   // (id, body)  && (id, force)
        {
            item.insert(all(init));
        }
        
        inline void add_item(const int id, const ItmTyp &it)
        {
            item.insert(make_pair(id,it));   // (id, body)  && (id, force)
        }
      
        inline void add_dummy()
        {
            item.insert(make_pair(-1,ItmTyp())); // add a dummy object
        }
        
        inline void remove_dummy()
        {
            item.erase(-1);
        }
        void merge(const Item &it) // the variable is a Item itself
        {
            tr(it.item,x)    // item is a member of Item class
            {
                add_item(x->first,x->second);
            }
        }
        
        int size(){ return item.size();}
        
        void print()
        {
            cout<<" the items are "<<endl;
            tr(item,it)
            {
                cout<<it->first<<":"<<endl;
                (it->second).print();
            }
        }
     
        void add_items(const map<int,ItmTyp> &it)
        {
            item.insert(all(it));
        }
        
        inline void clear()
        {
            item.clear();
        }
};

template<>
void Item<vector3>::merge(const Item<vector3> &it)
{
    tr(it.item,I)  // this is the variable in the function to add 
    {
        if( present(item,I->first)) // add the forces  // the item here is the member of the class, so the it.item of I will be added to the item, which is the member directly
        {
            item[I->first] += I->second;
        }
        else
        {
            item[I->first] = I->second;
        }
    }
}
#endif

