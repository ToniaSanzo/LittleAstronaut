/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
*
* Date: 11/20/2020
* file: LittleAstronaut LPlanet.cpp
*/
#include "TStar.h"

TStar::TStar() {
    // Initialize variables to default values
    index = x = y = rx = ry = 0;
    clicked = false;
}

TStar::TStar(int aindex, int ax, int ay, int arx, int ary) {
    // Initialize variables to parameter values
    index = aindex;
    x = ax;
    y = ay;
    rx = arx;
    ry = ary;
    clicked = false;
}

TStar::~TStar() {
    index = x = y = rx = ry = 0;
}

Uint8 TStar::handleEvent(SDL_Event *e) {
    int mosX, mosY;


    // If the mouse was clicked
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        // Get mouse position
        SDL_GetMouseState(&mosX, &mosY);
        printf("x: %d, y: %d\n", mosX, mosY);

        // Check if mouse is in the star
        bool inside = true;

        // Mouse is left of the star
        if (mosX < (x - rx)) {
            inside = false;
        }
        // Mouse is right of the star
        else if (mosX > (x + rx)) {
            inside = false;
        }
        // Mouse is above the star
        else if (mosY < (y - ry)) {
            inside = false;
        }
        // Mouse is below the star
        else if (mosY > (y + ry)) {
            inside = false;
        }

        clicked = inside;
        if (inside) { return STAR_CLICKED; }
    }
    // if the mosue was released
    else if (e->type == SDL_MOUSEBUTTONUP) {
        clicked = false;

        // Get mouse position
        SDL_GetMouseState(&mosX, &mosY);

        // Check if mouse is in the star
        bool inside = true;

        // Mouse is left of the star
        if (mosX < (x - rx)) {
            inside = false;
        }
        // Mouse is right of the star
        else if (mosX > (x + rx)) {
            inside = false;
        }
        // Mouse is above the star
        else if (mosY < (y - ry)) {
            inside = false;
        }
        // Mouse is below the star
        else if (mosY > (y + ry)) {
            inside = false;
        }
        if (inside) { return STAR_RELEASED; }
    }

    if (clicked) {
        return STAR_CLICKED;
    }
    else {
        return STAR_OUT_OF_RANGE;
    }
}


