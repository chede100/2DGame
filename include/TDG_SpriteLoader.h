#ifndef TDG_SPRITELOADER_H
#define TDG_SPRITELOADER_H

#include "TDG_GUI.h"
#include "TDG_Animation.h"
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class TDG_SpriteLoader
{
    public:
        TDG_SpriteLoader();
        virtual ~TDG_SpriteLoader();

        bool loadSprite(string path, int imgWidth, int imgHight, int spriteRows, int spriteColumns);

        SDL_Texture* getImage(TDG_GUI* gui, int row, int column);
        TDG_Animation* getAnimation(TDG_GUI* gui, int row, int numberOfImages);

    protected:

    private:
        string path;
        int imgWidth, imgHight;

        //Maximum number of images in a row and column of the sprite sheet
        int spriteRows, spriteColumns;

        SDL_Surface* sprite;
};

#endif // TDG_SPRITELOADER_H
