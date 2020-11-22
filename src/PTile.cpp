/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
* 
* Date: 11/20/2020
* file: LittleAstronaut PTile.cpp
*/
#include "PTile.h"

PTile::PTile() {
    name = "";
    x = y = r = 0;
    mRenderer = NULL;
    mAligned = clicked = false;
}

PTile::~PTile() {
    // reset PTile
    free();

    // prevent dangling pointers
    if (mRenderer) {
        mRenderer = NULL;
    }
}


bool PTile::loadFromFile(std::string outlinePath, std::string infoPath, std::string algndInfoPath) {
    bool success = true;
    mOutlineText.free();
    mInfoText.free();

    // Make sure the renderer has been initialized
    if (!mRenderer) {
        printf("Renderer has not been initialized for %s!\n", outlinePath.c_str());
        success = false;
    }

    // Initialize the outline texture
    mOutlineText.initLTexture(mRenderer);
    if (!mOutlineText.loadFromFile(outlinePath.c_str())) {
        printf("Failed to load planet %s texture!\n", outlinePath.c_str());
        success = false;
    }
    else { r = mOutlineText.getWidth() / 2; }

    // Initialize the non-aligned information texture
    mInfoText.initLTexture(mRenderer);
    if (!mInfoText.loadFromFile(infoPath.c_str())) {
        printf("Failed to load planet %s texture!\n", infoPath.c_str());
        success = false;
    }

    // Initialize the aligned information texture
    mAlgndInfoText.initLTexture(mRenderer);
    if (!mAlgndInfoText.loadFromFile(algndInfoPath.c_str())) {
        printf("Failed to load planet %s texture!\n", algndInfoPath.c_str());
        success = false;
    }

    return success;
}


void PTile::free() {
    // Enter the texture
    mOutlineText.free();
    mInfoText.free();
    mAlgndInfoText.free();

    // reset the variables
    x = y = r = 0;
    mAligned = clicked = false;
}


void PTile::reset() { mAligned = false; }


void PTile::initPTile(SDL_Renderer *rend, std::string pName, LTexture *text) {
    mRenderer = rend;
    name = pName;
    mText = text;
}


void PTile::render() {
    if (mAligned) {
        mText->render(x - (mText->getWidth() / 2), y + r + 8);
    }
    else {
       mOutlineText.render(x - r, y - r);
    }
}


void PTile::renderInfo() {
    if (mAligned) {
        mAlgndInfoText.render((SCREEN_WIDTH / 2 - mAlgndInfoText.getWidth() / 2), (SCREEN_HEIGHT / 2 - mAlgndInfoText.getHeight() / 2));
    }
}


void PTile::move(int aX, int aY) {
    x = aX;
    y = aY;
}


bool PTile::aligned(LPlanet &planet) {
    // Make sure the planet's name matches the tile's name
    if (planet.name == name) {
        // check to see if the planet overlaps the tile
        bool inside = true;

        // Planet is left of the tile
        if (planet.x < (x - r)) {
            inside = false;
        }
        // Planet is right of the tile
        if (planet.x > (x + r)) {
            inside = false;
        }
        // Planet is above the tile
        else if (planet.y < (y - r)) {
            inside = false;
        }
        // Planet is below the tile
        else if (planet.y > (y + r)) {
            inside = false;
        }

        if (inside) {
            mAligned = planet.aligned = true;
            planet.x = x;
            planet.y = y;
            return true;
        }
    }

    return false;
}


bool PTile::handleEvent(SDL_Event *e) {
    // If the tile is currently clicked
    if (clicked) {
        // Check to see if the mose has been released
        if (e->type == SDL_MOUSEBUTTONUP) {
            // Release the planet if necessary
            clicked = false;
        }
    }
    else {
        // If the mouse was clicked check if the click was on the tile
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            int mosX, mosY;
            SDL_GetMouseState(&mosX, &mosY);
            
            bool inside = true;

            // Mouse is left of the tile
            if (mosX < (x - r)) {
                inside = false;
            }
            // Mouse is right of the tile
            else if (mosX > (x + r)) {
                inside = false;
            }
            // Mouse is above the tile
            else if (mosY < (y - r)) {
                inside = false;
            }
            // Mose is below the tile
            else if (mosY > (y + r)) {
                inside = false;
            }

            clicked = inside;
            printf("Tile %s has been clicked!\n", name.c_str());
        }
    }

    return clicked;
}