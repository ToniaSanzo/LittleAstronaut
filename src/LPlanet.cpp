/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
* 
* Date: 11/20/2020
* file: LittleAstronaut LPlanet.cpp
*/
#include "LPlanet.h"


LPlanet::LPlanet() {
    x = y = rx = ry = 0;
    name = "";
    clicked = aligned = false;
    mRenderer = NULL;
}


LPlanet::~LPlanet() {
    // reset the LPlanet
    free();

    // Deallocate mRenderer if it exists
    if (mRenderer) {
        mRenderer = NULL;
    }
}


bool LPlanet::loadFromFile(std::string path) {
    bool success = true;
    mTexture.free();

    // Make sure the renderer has been initialized
    if (!mRenderer) {
        printf("Renderer has not been initialized for %s!\n", path.c_str());
        success = false;
    }

    // Initialize the texture renderer
    mTexture.initLTexture(mRenderer);
    if (!mTexture.loadFromFile(path.c_str())) {
        printf("Failed to load planet %s texture!\n", path.c_str());
        success = false;
    }
    else {
        mTexture.setBlendMode(SDL_BLENDMODE_BLEND);
        mTexture.setAlpha(255);
        rx = mTexture.getWidth() / 2;
        ry = mTexture.getHeight() / 2;
    }

    return success;
}


void LPlanet::free() {
    // Free the texture
    mTexture.free();
    // reset the variables
    x = y = rx = ry = 0;
}

void LPlanet::reset() {
    aligned = false;
    x = randomXPos();
    y = randomYPos();
}


void LPlanet::initLPlanet(SDL_Renderer *gRenderer, std::string pName) {
    mRenderer = gRenderer;
    name = pName;
}


void LPlanet::render() {
    mTexture.render(x - rx, y - ry);
}


void LPlanet::move(int aX, int aY) {
    x = aX;
    y = aY;
}


bool LPlanet::handleEvent(SDL_Event *e) {
    int mosX, mosY;

    // Exit prematurely 
    if (aligned) { return false; }

    // If the user pressed the mouse
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        // Get mouse position
        SDL_GetMouseState(&mosX, &mosY);

        
        // Check if mouse is in the planet
        bool inside = true;

        // Mouse is left of the planet
        if (mosX < (x - rx)) {
            inside = false;
        }
        // Mouse is right of the planet
        else if (mosX > (x + rx)) {
            inside = false;
        }
        // Mouse is above the planet
        else if (mosY < (y - ry)) {
            inside = false;
        }
        // Mouse is below the planet
        else if (mosY > (y + ry)) {
            inside = false;
        }
        
        clicked = inside;
        if (clicked) { return true; }
    }

    // If the user released the mouse
    if (e->type == SDL_MOUSEBUTTONUP) {
        // Release the planet if necessary
        if (clicked) {
            clicked = false;
        }
    }

    // If the user moved the mouse
    if (e->type == SDL_MOUSEMOTION) {
        // Get mouse position
        SDL_GetMouseState(&mosX, &mosY);
        if (clicked) {
            x = mosX;
            y = mosY;
        }
    }
    return false;
}

