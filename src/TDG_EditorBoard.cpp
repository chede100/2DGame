#include "TDG_EditorBoard.h"

TDG_EditorBoard::TDG_EditorBoard()
{
    this->backg = NULL;
    this->view = NULL;
    this->entityGraphics = NULL;
    this->entities = NULL;
    this->viewPos = NULL;

    this->consoleTimerRunning = false;

    this->load = false;
    this->create = false;
    this->newRoomID = 0;

    this->roomID = 0;
}

TDG_EditorBoard::~TDG_EditorBoard()
{
    if(this->consoleTimerRunning)
        SDL_RemoveTimer(this->consoleTimer);

    if(this->backg != NULL)
        delete this->backg;
    if(this->view != NULL)
        delete this->view;
    if(this->entityGraphics != NULL)
        delete this->entityGraphics;
    if(this->entities != NULL)
        delete this->entities;
    if(this->viewPos != NULL)
        delete this->viewPos;
}

bool TDG_EditorBoard::init()
{

    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = 420;
    int viewHight = 320;
    this->view->init(viewWidth, viewHight, 0, 0);

    //bind view to center of the window
    this->viewPos = new TDG_Position(210, 160);

    //bind the field of view to the players character on the game board
    if(!this->view->bindTo(this->viewPos, 0, 0))
    {
        cout << "Unable to bind field of view!" << endl;
        return false;
    }

    return true;
}

void TDG_EditorBoard::start()
{
    if(!this->consoleTimerRunning)
    {
        this->consoleTimer = SDL_AddTimer(125, &console_timer, this);
        this->consoleTimerRunning = true;
    }
}

void TDG_EditorBoard::handleUserInput(TDG_EventHandler* event)
{

}

void TDG_EditorBoard::handleConsoleInput()
{
    string in;
    cout << "Next instruction: " << endl;
    getline(cin, in);

    vector<string> inst = split(in, ' ');

    if(!inst.front().compare("help"))
    {
        cout << "List of instructions:" << endl;
        cout << "1) help             : Shows help massage." << endl;
        cout << "2) load [roomID]    : Load the requested room." << endl;
        cout << "3) save             : Save the current room." << endl;
        cout << "4) create [roomID]  : Create a new room." << endl;
    }
    else if(!inst.front().compare("load"))
    {
        inst.erase(inst.begin());
        if(inst.size() != 1)
            cout << "Invalid count of arguments for [load]. Please use the format: load [roomID]" << endl;
        else
        {
            this->newRoomID = atoi(inst.front().c_str());
            this->load = true;
        }
    }
    else if(!inst.front().compare("save"))
    {
        //implement save function
    }
    else if(!inst.front().compare("create"))
    {
        inst.erase(inst.begin());
        if(inst.size() != 1)
            cout << "Invalid count of arguments for [create]. Please use the format: create [roomID]" << endl;
        else
        {
            this->newRoomID = atoi(inst.front().c_str());
            this->create = true;
        }
    }
    else
    {
        cout << "Write help for information." << endl;
    }

    inst.clear();
}

Uint32 TDG_EditorBoard::console_timer(Uint32 intervall, void* param)
{
    ((TDG_EditorBoard*)param)->handleConsoleInput();
    return intervall;
}

vector<string> TDG_EditorBoard::split(const string& str, char delimiter)
{
    istringstream is(str);
    vector<string> result;
    for(string cur; getline(is, cur, delimiter); )
    {
        //ignore all spaces
        if(cur.compare(""))
            result.push_back(cur);
    }
    return result;
}
