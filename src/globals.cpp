#include "globals.h"
KSPIRE_Touchpad::KSPIRE_Touchpad() {
    auto tp_i = touchpad_getinfo();
    h = (float)tp_i->height;
    w = (float)tp_i->width;
}
void KSPIRE_Touchpad::Update() {
    touchpad_report_t touchpad;
    touchpad_scan(&touchpad);
    arrow      =      touchpad.arrow;
    contact    =    touchpad.contact;
    dummy      =      touchpad.dummy;
    pressed    =    touchpad.pressed;
    proximity  =  touchpad.proximity;
    x          =          touchpad.x;
    x_velocity = touchpad.x_velocity;
    y          =          touchpad.y;
    y_velocity = touchpad.y_velocity;
    true_contact = (touchpad.proximity >= CONTACT_THRESHOLD) ? true : false;
    

    if (!relative_mode) {
        x_screen_normalized = ((float)x / (float)w);
        y_screen_normalized = ((float)y / (float)h);
        x_screen = x_screen_normalized * (float)SCREEN_WIDTH;
        y_screen = x_screen_normalized * (float)SCREEN_HEIGHT;
    }

    else if (relative_mode) {
        

        int yvsigned = (y_velocity > 127) ? y_velocity - 256 : y_velocity;
        y_screen += yvsigned / 2;
        int xvsigned = (x_velocity > 127) ? x_velocity - 256 : x_velocity;
        x_screen += xvsigned / 2;
        
        
        if (y_screen < 0) y_screen = 0;
        if (y_screen > SCREEN_HEIGHT) y_screen = SCREEN_HEIGHT;

        printf("yv%d\n",y_velocity);

    }

    
}

KSPIRE_Touchpad kspire_pad;