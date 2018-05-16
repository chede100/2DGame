#ifndef TDG_ROOMEDITOR_H
#define TDG_ROOMEDITOR_H

#include "TDG_Window.h"
#include "TDG_EventHandler.h"
#include "TDG_EditorBoard.h"

typedef struct
{
    bool load, create, save;
    int roomID;
    string rName;
    int rows, columns;

}ConsoleStatus;

class TDG_RoomEditor
{
    public:
        TDG_RoomEditor();
        virtual ~TDG_RoomEditor();

        bool init();

        bool start();
        void stop();

        void programLoop();

    protected:

    private:
        void input();

        static int console_thread(void* param);
        void handleConsoleInput();
        bool consoleInputRunning;
        SDL_Thread* consoleInput;

        ConsoleStatus cStatus;

        TDG_Window* win;
        TDG_EventHandler* event;

        TDG_EditorBoard* board;

        int nextInt(vector<string>& entries);
        vector<string> split(const string& str, char delimiter);
        bool roomExists(const string& roomDirectoryPath);
};

#endif // TDG_ROOMEDITOR_H
