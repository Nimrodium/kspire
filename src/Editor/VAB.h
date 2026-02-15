#pragma once
#include "../globals.h"
#include "../World/Camera/camera.h"
#include "../Utility/GameTexture.h"


class VAB {
public:
    TEXTURE *screen;
    ModelGroup me;
    //Load ANGEL model
    int load_model(Bundle* assets);
    void destroy_model();
    void render(ProcessedPosition *processed);
private:
    std::vector<ngl_object*> full_scene;
    Camera cam;
    DT clock;
    
    float camera_height = 30;
    float camera_angle = 30;
    float camera_zoom = -20;

    const float MAX_CAMERA_HEIGHT = 740; //Max cam height

    GameTexture side_panel;

    
};