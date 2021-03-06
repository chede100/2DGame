#ifndef TDG_SPRITELOADER_H
#define TDG_SPRITELOADER_H

#include "TDG_Window.h"
#include "TDG_Animation.h"

class TDG_SpriteLoader
{
    public:
        TDG_SpriteLoader();
        virtual ~TDG_SpriteLoader();

        bool loadSprite(string pathToFolder, string spriteName);

        SDL_Texture* getImage(TDG_Window* win, int row, int column);
        TDG_Animation* getAnimation(TDG_Window* win, int row);

        int getSpriteMaxRows();
        int getSpriteMaxColumns();

        int getImgWidth();
        int getImgHight();

    protected:

    private:
        bool loadSpriteInfo(string path);

        AnimationTyp stringToAnimationTyp(string typ);

        vector<string> split(const string& str, char delimiter);
        int nextInt(vector<string>& entries);
        void nextString(vector<string>& entries, string& input);

        string path;
        int imgWidth, imgHight;
        int spriteRows, spriteColumns;

        list<int> imgPerRow;
        list<AnimationTyp> aniAtRow;

        SDL_Surface* sprite;
};

#endif // TDG_SPRITELOADER_H
