#ifndef TDG_PLAYER_H
#define TDG_PLAYER_H

#include "TDG_Character.h"
#include "TDG_EventHandler.h"

class TDG_Player : public TDG_Character
{
    public:
        TDG_Player();
        virtual ~TDG_Player();

        void init(const Entity* entity, EntityTyp typ, TDG_StoredEntityAnimations* storedGraphics, bool moveable);

        void handlePlayerEvents(TDG_EventHandler* event);

    protected:

    private:
};

#endif // TDG_PLAYER_H
