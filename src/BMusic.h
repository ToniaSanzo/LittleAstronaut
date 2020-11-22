#pragma once
/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
*
* Date: 11/21/2020
* file: LittleAstronaut BMusic.h
*/
#include "LTexture.h"
#include <SDL_mixer.h>

class BMusic {
public:
    // Screen dimensions
    static constexpr int SCREEN_WIDTH = 1900;
    static constexpr int SCREEN_HEIGHT = 1200;
    static constexpr int BMUSIC_SIDE = 105;

    // The current state of the music
    bool playing;

    // Initialize the variables
    BMusic();

    // Deallocate the varaibles
    ~BMusic();

    // Initialize mTexture to the image associated to the parameter path
    bool loadFromFile(std::string path);

    // Deallocates texture
    void free();

    // Initialize the the BMusic object
    void initBMusic(SDL_Renderer *rend);

    // Render the sound icon
    void render();

    // Handle the mouse events
    void handleEvent(SDL_Event *e);
    
    // pauses or plays the music
    void toggleMusic();

private:
    // The planets texture, renderer, and music
    LTexture mTexture;
    SDL_Rect mTexClips[2];
    SDL_Renderer *mRenderer;
};