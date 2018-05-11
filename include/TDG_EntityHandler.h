#ifndef TDG_ENTITYHANDLER_H
#define TDG_ENTITYHANDLER_H

#include <SDL_timer.h>
#include "TDG_EntityList.h"
#include "TDG_Background.h"
#include "TDG_Character.h"
#include "TDG_Object.h"

//Handles animation and movement updates (collision check at movement update)
//Contains a self-sorting list of entities (sort parameter is the y-position of each entity / lowest y-position first)
class TDG_EntityHandler
{
    public:
        TDG_EntityHandler();
        virtual ~TDG_EntityHandler();

        void add(TDG_Entity* newEntity);
        bool remove(TDG_Entity* entity);
        void removeAllExcept(TDG_Entity* expt);

        //sorts and renders the entities
        void render(TDG_GUI* gui, TDG_View* view);

        bool collisionDetectionFor(TDG_Character* chara);

        void startAnimations();
        void stopAnimations();

        void startMotion();
        void stopMotion();

        void enableCollision(TDG_Background* bg);
        void disableCollision();

        TDG_Entity* getEntity(int posX, int posY);

    protected:

    private:
        void sort();
        bool swap(TDG_EntityList* A, TDG_EntityList* B);

        //Animation Timing
        static Uint32 next_image_timer(Uint32 intervall, void* param);
        void updateAllAnimations();

        //Motion Timing
        static Uint32 motion_timer(Uint32 intervall, void* param);
        void moveEntities();

        bool animationTimerRunning;
        SDL_TimerID animationTimer;

        bool motionTimerRunning;
        SDL_TimerID motionTimer;

        TDG_EntityList* first;

        bool collisionDetection;
        //store pointer to TDG_Background for collision detection while moving character with a timer (on a other thread)
        TDG_Background* bg;
};

#endif // TDG_ENTITYHANDLER_H
