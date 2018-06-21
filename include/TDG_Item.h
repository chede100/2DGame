#ifndef TDG_ITEM_H
#define TDG_ITEM_H

#include "TDG_SpriteLoader.h"

class TDG_Item
{
    public:
        TDG_Item();
        virtual ~TDG_Item();

        bool create(TDG_Window* win, string name, int id, int amount, int maxAmount);

        void render(TDG_Window* win, int x, int y, int w, int h);

        int getID();
        int getAmount();
        int getMaxAmount();
        int getPosition();

        void setAmount(int amount);
        void setPosition(int pos);

    protected:

    private:
        string name;
        int id;
        int amount;
        int maxAmount;

        int pos;

        SDL_Texture* img;
};

#endif // TDG_ITEM_H
