#ifndef TDG_MOUSE_H
#define TDG_MOUSE_H

#include "TDG_Position.h"
#include "TDG_EntityHandler.h"

typedef struct
{
    int id;
    int rotDegree;
    bool impassable;
    int flip;
}PreviousTile;

typedef struct
{
    PreviousTile prev;
    int r, c;
    int id;
    int rotDegree;
    bool impassable;
    int flip;
    int w, h;
}SelectedTile;

typedef enum
{
    TILE, ENTITY, noSelection
}SelectionTyp;

class TDG_Mouse
{
    public:
        TDG_Mouse();
        virtual ~TDG_Mouse();

        void handleEvent(SDL_Event* event, TDG_EntityHandler* eh, TDG_Background* bg, TDG_View* view);

        void renderSelRect(TDG_Window* win, TDG_View* view);

        bool selectedSomething();

    protected:

    private:
        bool selectEntity(TDG_EntityHandler* eh, int posX, int posY);
        bool selectTile(TDG_Background* bg, int posX, int posY);

        SelectionTyp typ;
        void* sel;
};

#endif // TDG_MOUSE_H
