/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
*
* Date: 11/20/2020
* file: LittleAstronaut BMusic.cpp
*/
#include "BMusic.h"

BMusic::BMusic() {
    playing = false;
    mRenderer = NULL;
    for (size_t i = 0; i < 2; ++i) {
        mTexClips[i].x = i * BMUSIC_SIDE;
        mTexClips[i].y = 0;
        mTexClips[i].w = BMUSIC_SIDE;
        mTexClips[i].x = BMUSIC_SIDE;
    }
}

BMusic::~BMusic() {
    // redet BMusic
    free();

    // prevent dangling pointers
    if (mRenderer) {
        mRenderer = NULL;
    }
}

bool BMusic::loadFromFile(std::string path) {
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
        printf("MusicBox success.\n");
    }

    return success;
}

void BMusic::free() {
    // Free the texture
    mTexture.free();
}

// Initialize the BMusic object
void BMusic::initBMusic(SDL_Renderer *rend) {
    mRenderer = rend;
}


void BMusic::render() {
    if (playing) {
        mTexture.render(SCREEN_WIDTH - BMUSIC_SIDE, SCREEN_HEIGHT - BMUSIC_SIDE, &mTexClips[0]);
    }
    else {
        mTexture.render(SCREEN_WIDTH - BMUSIC_SIDE, SCREEN_HEIGHT - BMUSIC_SIDE, &mTexClips[1]);
    }
}


void BMusic::handleEvent(SDL_Event *e) {
    int mosX, mosY;
    
    // If the user pressed the mouse
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        // Get mouse position
        SDL_GetMouseState(&mosX, &mosY);


        // Check if mouse is in the planet
        bool inside = true;

        // Mouse is left of the planet
        if (mosX < (SCREEN_WIDTH - BMUSIC_SIDE)) {
            inside = false;
        }
        // Mouse is above the planet
        else if (mosY < (SCREEN_HEIGHT - BMUSIC_SIDE)) {
            inside = false;
        }

        if (inside) { 
            toggleMusic(); 
        }
    }
}

void BMusic::toggleMusic() {
    if (playing) {
        playing = false;
        Mix_PauseMusic();
    }
    else {
        playing = true;
        Mix_ResumeMusic();
    }
}