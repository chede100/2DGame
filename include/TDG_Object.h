#ifndef TDG_OBJECT_H
#define TDG_OBJECT_H

#include "TDG_Entity.h"

class TDG_Object : public TDG_Entity
{
    public:
        TDG_Object();
        virtual ~TDG_Object();

        void init(const Entity* entity, EntityTyp typ, TDG_StoredEntityAnimations* storedGraphics);

    protected:

    private:

};

#endif // TDG_OBJECT_H
