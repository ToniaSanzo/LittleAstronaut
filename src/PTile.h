#pragma once
/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
* 
* Date: 11/20/2020
* file: LittleAstronaut PTile.h
*/
#include "LTexture.h"
#include "LPlanet.h"

class PTile {
public:
    static constexpr int SCREEN_WIDTH = 1900;
    static constexpr int SCREEN_HEIGHT = 1200;

    // The coordinates, dimension, and the name associated with this tile
    std::string name;
    int x, y, r;
    // If this tile has aligned with it's planet
    bool mAligned, clicked;

    // Initialize the tile
    PTile();

    // Destroy the tile
    ~PTile();

    // Initialize PTile's textures
    bool loadFromFile(std::string outlinePath, std::string infoPath, std::string algndInfoPath);
    
    // Deallocates the texture
    void free();

    // Reset the tile
    void reset();

    // Initialize the tile
    void initPTile(SDL_Renderer *rend, std::string pName, LTexture *text);

    // Render the tile
    void render();

    // Render the tile's info
    void renderInfo();

    // Move the tile
    void move(int x, int y);

    // Return a bool based on if the planet matches the tile, and is overlapping the tile 
    bool aligned(LPlanet &planet);

    // Return a bool based on whether this tile has been clicked
    bool handleEvent(SDL_Event *e);

private:
    // The tile's texture and renderer
    LTexture mOutlineText, mInfoText, mAlgndInfoText, *mText;
    SDL_Renderer *mRenderer;
};
