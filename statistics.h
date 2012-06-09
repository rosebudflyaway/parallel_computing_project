#ifndef STATISTICS_H
#define STATISTICS_H

#include "Globals.h"
#include "vector3.h"
#include "body.h"
class Stat
{
    private:
        map<int,vector<vector3> > positions;
    public:
        void append_position(map<int,Body> &bodies)
        {
            tr(bodies,it)
            {
                positions[it->first].push_back( (it->second).get_position() );  // it->first is ID; it->second is the Body
            }
        }
        
        void write_position(int rank=1)
        {
            ofstream myfile;
            std::string s;
            std::stringstream out;

            tr(positions,it)
            {
                //
                out<<"./Positions/";
                out<<rank;
                out<<"-";
                out<<it->first;
                myfile.open( (out.str()).c_str() );
                // write the content to the file
                tr(it->second,it2)
                {
                    myfile << it2->x << "\t" << it2->y << "\t" << it2->z << "\n";
                }
                out.str("");
                myfile.close();
            }
        }

};

#endif
