#ifndef MAP_HPP
#define MAP_HPP

#include<random>

#include"zaxlib.hpp"
#include"Thing.hpp"

class Map {
public:
    Map();
    Map(int width, int height);
    //////////////////////////////////////////////////////////////////////////
    void BuildGenericMap(std::mt19937_64 & rng, 
                    double detail_min, 
                    double detail_max);
    //////////////////////////////////////////////////////////////////////////
    int GetMapWidth();
    int GetMapHeight();
    ///////////////////////////////////////////////////////////////////////////
    sptr<Thing> GetSector(int x, int y);
    ///////////////////////////////////////////////////////////////////////////
private:
    int map_width;
    int map_height;
    vec<vec_sptr<Thing>> sectors;
};


#endif
