#pragma once
/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
* 
* Date: 11/20/2020
* file: LittleAstronaut LPlanet.h
*/
#include "LTexture.h"
#include <ctime>

class LPlanet {
public:
    static constexpr int SCREEN_WIDTH = 1900;
    static constexpr int SCREEN_HEIGHT = 1200;

    // The coordinates, dimension, and the name of the planet
    std::string name;
    int x, y, rx, ry;
    // If this planet is currently being dragged, or has been placed in the right place
    bool clicked;
    bool aligned;

    // Initialize the Planet
    LPlanet();

    // Destroy the planet
    ~LPlanet();

    // Initialize mTexture to the image associated to the parameter path
    bool loadFromFile(std::string path);

    // Deallocates texture
    void free();

    // Reset texture
    void reset();

    // Initialize the planets
    void initLPlanet(SDL_Renderer *rend, std::string pName);

    // Render the planet
    void render();

    // Move the planet
    void move(int x, int y);

    // Handle's the mouse events
    bool handleEvent(SDL_Event *e);

    // Generate random x and y positions in the screen
    inline int randomXPos() { return std::rand() % SCREEN_WIDTH; }
    inline int randomYPos() {
        int num = std::rand() % SCREEN_HEIGHT / 3;
        if (num % 2 == 1) { num += (2 * SCREEN_HEIGHT / 3) - 50; }
        return num;
    }
private:
    // The planets texture and renderer
    LTexture mTexture;
    SDL_Renderer *mRenderer;
};

