#pragma once
#include "../globals.h"


//Staging icons
enum Staging_Icons {
    CAPSULE,
    ENGINE,
    BOOSTER,
    NONE
};


class Node {
    
};

class Part {
    public:
    //Attachment
    bool attached;
    linalg::vec<float,3> attPos;
    linalg::vec<float,4> attRotation;
    
    //Attach nodes
    // ?????

    


    //SYMM
    std::vector<Part*> symmetry_tochters;
    int symmetry_mode;
    

    //Family
    std::vector<Part*> children;    
    Part* parent = nullptr;

    //Other
    int stage;
    Staging_Icons icon = Staging_Icons::NONE;

    //Physical Properties
    float mass;
    float aerodynamicArea;
    
    enum DragModel
	{
		DEFAULT,
		CONIC,
		CYLINDRICAL
	};

    float drag_max = 0.2f;
    float drag_min = 0.1f;
    float drag_ang = 0.1f;

    //Pascals
    float static_pressure = 100.0f;
    float dynamic_pressure = 0.0f;

    //K
    float temp_max = 2400.0f;
    float temp     = 300.0f;

    
};
