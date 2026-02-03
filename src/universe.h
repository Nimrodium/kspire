#pragma once
#include "globals.h"
#include "Vessel/Vessel.h"
#include "flight.h"

//Contains all vessels.
class Universe {
    public:

    //Physics range things
    Flight flight;



    void step();

    private:
    //All vessels in world. Does not contain part data, until loaded.
    //NTS: I would prefer you to do emplace back. Read up on emplace back.
    std::vector<Vessel> vessels;


    //Mode 0 : Distant, up to LO. Mode 1: PQS
    void render_celestials(bool mode);
    //Render focused vessel & any surrounding vessels
    void render_nearby_vessels();

    //Step on rails orbit sim
    void step_on_rails_orbits();
    //Step physics orbits
    void step_physics_orbits();



};