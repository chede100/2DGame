#ifndef TDG_ENTITYLIST_H
#define TDG_ENTITYLIST_H

#include "TDG_Entity.h"

class TDG_EntityList
{
    public:
        TDG_EntityList();
        virtual ~TDG_EntityList();

        void setEntity(TDG_Entity* entity);
        void setNext(TDG_EntityList* next);
        void setPrevious(TDG_EntityList* pre);

        TDG_Entity* getEntity();
        TDG_EntityList* getNext();
        TDG_EntityList* getPrevious();

    protected:

    private:
        TDG_Entity* entry;

        TDG_EntityList* next;
        TDG_EntityList* previous;
};

#endif // TDG_ENTITYLIST_H
