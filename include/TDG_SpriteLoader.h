#ifndef TDG_SPRITELOADER_H
#define TDG_SPRITELOADER_H

#include "TDG_GUI.h"
#include "TDG_Animation.h"
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class TDG_SpriteLoader
{
    public:
        TDG_SpriteLoader();
        virtual ~TDG_SpriteLoader();

        bool loadSprite(string pathToFolder, string spriteName);

        SDL_Texture* getImage(TDG_GUI* gui, int row, int column);
        TDG_Animation* getAnimation(TDG_GUI* gui, int row, AnimationTyp typ);

        int getSpriteMaxRows();
        int getSpriteMaxColumns();

        int getImgWidth();
        int getImgHight();

    protected:

    private:
        bool loadSpriteInfo(string path);

        vector<string> split(const string& str, char delimiter);
        int nextInt(vector<string>& entries);

        string path;
        int imgWidth, imgHight;
        int spriteRows, spriteColumns;
        list<int> imgPerRow;

        SDL_Surface* sprite;
};

#endif // TDG_SPRITELOADER_H
