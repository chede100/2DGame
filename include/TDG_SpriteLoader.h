#ifndef TDG_SPRITELOADER_H
#define TDG_SPRITELOADER_H

#include "TDG_GUI.h"
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class TDG_SpriteLoader
{
    public:
        TDG_SpriteLoader(string path, int imgWidth, int imgHight);
        virtual ~TDG_SpriteLoader();

        SDL_Texture* getSpriteImage(TDG_GUI* gui, int imageNumber);

        void setSpriteMaxRows(int rows);
        void setSpriteMaxColumns(int columns);

    protected:

    private:
        string path;
        int imgWidth, imgHight;

        //Maximum number of images in a row and column of the sprite sheet
        int maxRows, maxColumns;

        SDL_Texture* sprite;
};

#endif // TDG_SPRITELOADER_H
