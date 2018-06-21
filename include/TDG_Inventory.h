#ifndef TDG_INVENTORY_H
#define TDG_INVENTORY_H

#include "TDG_Item.h"

class TDG_Inventory
{
    public:
        TDG_Inventory();
        virtual ~TDG_Inventory();

        bool create(int maxSize);

        void add(TDG_Window* win, string name, int id, int amount, int maxAmount);

        void render(TDG_Window* win, int borderWidth, int maxRows, int maxColumns, int x, int y, int imgW, int imgH);

        void swap(int posA, int posB);

        void remove(int pos);

        void print();

        TDG_Item* getItem(int pos);
        int getSize();

    protected:

    private:
        int maxSize;

        list<TDG_Item*>  inv;

        bool addAmountTo(TDG_Window* win, int id, int amount, int maxAmount);
        void sort();
};

#endif // TDG_INVENTORY_H
