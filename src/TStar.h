#pragma once
/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
*
* Date: 11/21/2020
* file: LittleAstronaut TStar.h
*/
#include "LTexture.h"

class TStar {
public:
    static constexpr Uint8 STAR_OUT_OF_RANGE = 0;
    static constexpr int STAR_CLICKED = 1;
    static constexpr int STAR_RELEASED = 2;

    // The coordinates, dimension of the star
    int index, x, y, rx, ry;
    bool clicked;

    // Initialize the star's variables
    TStar();
    TStar(int aindex, int ax, int ay, int arx, int ary);

    // Deallocate the star
    ~TStar();

    // Handle mouse events, 0 - not on star, 1 - star clicked, 2 - mouse released on star
    Uint8 handleEvent(SDL_Event *e);
};