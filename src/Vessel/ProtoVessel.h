#pragma once
#include "../globals.h"

class ProtoVessel {
    public:
    uint16_t persistentId = 0;
    bool vesselSpawning = true;
    bool landed = false;
    bool splashed = false;
    std::string vesselName = "Unnamed Vessel";
    int rootIndex = 0;

    //Staging
    int stage = 0;
    int currentStage = 0;

    double distanceTraveled = 0;

    //In scene, rather volatile
    //Transform transform;
    

    float altitude = 0;
    float latitude = 0;
    float longitude = 0;

    linalg::vec<float,3> CoM = {0,0,0};

    float gForce = 0;

    float launchTime = 0;

    float heightFromSurface = 0;
    
    float obt_speed = 0;

    //Position and Velocity relative to home body
    linalg::vec<float,3> position = {0,0,0};

    linalg::vec<float,3> velocity = {0,0,0};

    linalg::vec<float,4> rotation = {0,0,0,0};

    private:
};