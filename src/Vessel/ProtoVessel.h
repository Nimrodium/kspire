#pragma once
#include "../globals.h"

class ProtoVessel {
    public:
    uint16_t persistentId;
    bool vesselSpawning;
    bool landed;
    bool splashed;
    std::string vesselName = "Unnamed Vessel";
    int rootIndex;

    //Staging
    int stage;
    int currentStage;

    double distanceTraveled;

    //In scene, rather volatile
    //Transform transform;
    

    float altitude;
    float latitude;
    float longitude;

    linalg::vec<float,3> CoM;

    float gForce;

    float launchTime;

    float heightFromSurface;
    
    float obt_speed;

    //Position and Velocity relative to home body
    linalg::vec<float,3> position;

    linalg::vec<float,3> velocity;

    linalg::vec<float,4> rotation;

    private:
};