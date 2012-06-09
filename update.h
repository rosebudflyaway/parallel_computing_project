// update the velocity of the bodies after computing the total force
#ifndef UPDATE_H
#define UPDATE_H

void update_vel(Body &b,const vector3 f)
{
    b.add_vel(f);
}
void update_pos(Body &b,const vector3 f)
{
    b.add_pos(f);
}
void update_all_velocity(Item<Body> &mydomain, const Item<vector3> finalforce)
{
    tr(finalforce.item,it)
    {
        if( present ( mydomain.item,it->first))
        {
            update_vel(mydomain.item[it->first], it->second);
            update_pos(mydomain.item[it->first], it->second);
        }
    }
}


void manage_bodies(Item<Body> &mydomain, Item<Body> &msg,int rank)
{
    // for bodies in mydomain remove if the center crosses the boundary
    map<int,Body>::iterator it = mydomain.item.begin();
    while(it!= mydomain.item.end() )
    {
        if( (it->second).get_y() > rank+1)
        {
            mydomain.item.erase(it++);
        }
        else
        {
            ++it;
        }
    }
    // for bodies in msg add it to the mydomain if its in the domain
    /*float y;
    tr(msg.item,it)
    {
        y = (it->second).get_y();
        if( y >= rank && y < rank+1)
            mydomain.add_item(it->first,it->second);
    }*/
}
#endif
