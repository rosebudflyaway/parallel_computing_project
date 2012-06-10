#include "Globals.h"

void print_vi(const vi& x)
{
    tr(x,it)
    {
        cout<<","<<*it<<",";
    }
    cout<<endl<<"------------------"<<endl;
}
void print_vd(const vd& x)
{
    tr(x,it)
    {
        cout<<","<<*it<<",";
    }
    cout<<endl<<"------------------"<<endl;
}

// this is to print vector<vector <int> >
void print_vvi(const vvi& x)
{
    tr(x,it)
    {
        print_vi(*it);
    }
}

float RangeFloat(float min, float max)
{
    // generate a float between min and maximum
    float random = ((float) rand() ) / (float) RAND_MAX;
    float range = max - min;
    return (random*range) + min;
}

// pair<int, int>
void print_ii(const ii& x)
{
    cout << "(" << x.first<< "," << x.second << ")" << endl;
}

// vector <pair<int, int> >
void print_vii(const vii& x)
{
    tr(x,it)
    {
        print_ii(*it);
    }
}
