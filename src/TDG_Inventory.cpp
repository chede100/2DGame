#include "TDG_Inventory.h"

TDG_Inventory::TDG_Inventory()
{
    this->maxSize = 0;
}

TDG_Inventory::~TDG_Inventory()
{
    for(list<TDG_Item*>::const_iterator it = this->inv.begin(), end = this->inv.end(); it != end; it++)
    {
         if(*it != NULL)
            delete *it;
    }
    this->inv.clear();
}

bool TDG_Inventory::create(int maxSize)
{
    this->maxSize = maxSize;

    return true;
}

void TDG_Inventory::add(TDG_Window* win, string name, int id, int amount, int maxAmount)
{
    if((id > 0) && (maxAmount > 0) && (amount >= 0))
    {
        if(addAmountTo(win, id, amount, maxAmount));
        else
        {
            int freeSpot = 1;

            if(this->inv.size() == 0)
            {
                if(this->maxSize >= freeSpot)
                {
                    TDG_Item* item = new TDG_Item();
                    item->create(win, name, id, amount, maxAmount);

                    this->inv.push_back(item);
                    item->setPosition(freeSpot);
                }
            }
            else
            {
                //try to get a spot in between the inventory list (insert a item) which is free
                bool spotInbetween = false;
                for(list<TDG_Item*>::const_iterator it = this->inv.begin(), end = this->inv.end(); it != end; it++)
                {
                    if(this->maxSize >= freeSpot)
                    {
                        if((*it)->getPosition() > freeSpot)
                        {
                            spotInbetween = true;
                            TDG_Item* item = new TDG_Item();
                            item->create(win, name, id, amount, maxAmount);

                            this->inv.push_back(item);
                            item->setPosition(freeSpot);

                            break;
                        }
                    }

                    freeSpot++;
                }

                //add item to the end of inventory list, if all entries before are filled
                if(!spotInbetween && (this->maxSize >= freeSpot))
                {
                    TDG_Item* item = new TDG_Item();
                    item->create(win, name, id, amount, maxAmount);

                    this->inv.push_back(item);
                    item->setPosition(freeSpot);
                }
            }
        }
    }
    else
        cout << "Inventory Error: Invalid arguments for adding item to list!" << endl;
}

void TDG_Inventory::render(TDG_Window* win, int borderWidth, int maxRows, int maxColumns, int x, int y, int imgW, int imgH)
{
    //row and column start by 1 (not 0)
    if(maxRows > 0)
    {
        for(list<TDG_Item*>::const_iterator it = this->inv.begin(), end = this->inv.end(); it != end; it++)
        {
            int pos = (*it)->getPosition()-1;
            int c = pos%maxColumns + 1;
            int r = pos/maxColumns + 1;
            if(r <= maxRows)
            {
                int posX = x + (c - 1)*imgW + borderWidth*c;
                int posY = y + (r - 1)*imgH + borderWidth*r;
                (*it)->render(win, posX, posY, imgW, imgH);
            }
        }
    }
}

void TDG_Inventory::swap(int posA, int posB)
{
    if((posA > 0) && (posB > 0) && (this->inv.size() >= (unsigned) posA) && (this->inv.size() >= (unsigned) posB))
    {
        list<TDG_Item*>::iterator a = this->inv.begin();
        list<TDG_Item*>::iterator b = this->inv.begin();

        for(int k = 0; k < posA; k++)
            a++;
        for(int l = 0; l < posB; l++)
            b++;

        TDG_Item* tmp = *a;
        *a = *b;
        *b = tmp;
    }
    else
        cout << "Inventory Error: Invalid swap position!" << endl;
}

bool TDG_Inventory::addAmountTo(TDG_Window* win, int id, int amount, int maxAmount)
{
    for(list<TDG_Item*>::const_iterator it = this->inv.begin(), end = this->inv.end(); it != end; it++)
    {
        if((*it)->getID() == id)
        {
            //is the item stack already full?/at maximum?
            if((*it)->getMaxAmount() != (*it)->getAmount())
            {
                (*it)->setAmount((*it)->getAmount() + amount);
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

void TDG_Inventory::remove(int pos)
{
    if((pos > 0) && (this->inv.size() >= (unsigned) pos))
    {
        for(list<TDG_Item*>::const_iterator it = this->inv.begin(), end = this->inv.end(); it != end; it++)
        {
            if((*it)->getPosition() == pos)
            {
                delete *it;
                this->inv.erase(it);
            }
        }
    }
    else
        cout << "Inventory Error: Invalid remove position!" << endl;
}

void TDG_Inventory::print()
{
    list<TDG_Item*>::const_iterator it, e;
    for (it = this->inv.begin(), e = this->inv.end(); it != e; it++)
    {
        cout << (*it)->getID() << " ";
    }
    cout << endl;
}

TDG_Item* TDG_Inventory::getItem(int pos)
{
    if((pos > 0) && (this->inv.size() != 0))
    {
        for(list<TDG_Item*>::const_iterator it = this->inv.begin(), end = this->inv.end(); it != end; it++)
        {
            if((*it)->getPosition() == pos)
                return *it;
        }
    }

    return NULL;
}

int TDG_Inventory::getSize()
{
    return this->maxSize;
}

void TDG_Inventory::sort()
{
    if(this->inv.size() > 1)
    {
        bool finished = false;

        while(!finished)
        {
            finished = true;

            list<TDG_Item*>::iterator a = this->inv.begin();
            list<TDG_Item*>::iterator b = a++;
            for(list<TDG_Item*>::const_iterator end = this->inv.end(); b != end;)
            {
                if((*a)->getPosition() > (*b)->getPosition())
                {
                    finished = false;
                    TDG_Item* tmp = *a;
                    *a = *b;
                    *b = tmp;
                }
                a++, b++;
            }
        }
    }
}
