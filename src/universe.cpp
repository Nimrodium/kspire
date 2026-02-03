#include "universe.h"
void Universe::render_celestials(bool mode) {
    if (mode) {
        //Mode 1 PQS
    } else {
        //Mode 0 Distant
    }
}
void Universe::render_nearby_vessels() {

}
void Universe::step_on_rails_orbits() {

}
void Universe::step_physics_orbits() {

}



void Universe::step() {
    


    //RENDER

    for (Vessel& v : vessels) {
        //Step vessel orbit after checking if its on rails or simulated
        if (v.loaded) {
            step_physics_orbits();
        } else {
            step_on_rails_orbits();
        }
    }
}