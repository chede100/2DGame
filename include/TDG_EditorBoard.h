#ifndef TDG_EDITORBOARD_H
#define TDG_EDITORBOARD_H

#include "TDG_EntityHandler.h"
#include "TDG_Player.h"

class TDG_EditorBoard
{
    public:
        TDG_EditorBoard();
        virtual ~TDG_EditorBoard();

        bool init();

        void start();

        void handleUserInput(TDG_EventHandler* event);

    protected:

    private:
        static Uint32 console_timer(Uint32 intervall, void* param);
        void handleConsoleInput();
        bool consoleTimerRunning;
        SDL_TimerID consoleTimer;

        bool load, create;
        int newRoomID;

        string roomName;
        int roomID;

        TDG_Position* viewPos;
        TDG_View* view;

        TDG_Background* backg;

        TDG_StoredEntityAnimations* entityGraphics;

        TDG_Player* player;
        TDG_EntityHandler* entities;

        vector<string> split(const string& str, char delimiter);
};

#endif // TDG_EDITORBOARD_H
