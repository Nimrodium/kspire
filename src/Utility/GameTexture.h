#pragma once
#include "../globals.h"

class GameTexture {
public:
    std::vector<uint16_t> tex_data;
    TEXTURE tex;
    int x, y;
    int init(Bundle* resources,std::string path,TEXTURE *_screen);
    
    //Most basic draw function, no fancy slicing here
    void draw(int x, int y);
private:
    TEXTURE *screen;
};
    