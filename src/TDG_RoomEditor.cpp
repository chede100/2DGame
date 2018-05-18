#include "TDG_RoomEditor.h"

TDG_RoomEditor::TDG_RoomEditor()
{
    this->event = NULL;
    this->win = NULL;
    this->board = NULL;

    this->cStatus.create = false;
    this->cStatus.load = false;
    this->cStatus.save = false;
    this->cStatus.roomID = 0;
    this->cStatus.rName = "";
    this->cStatus.rows = 0;
    this->cStatus.columns = 0;
    this->cStatus.addC = false;
    this->cStatus.addT = false;
    this->cStatus.addO = false;
    this->cStatus.addID = 0;
    this->cStatus.palettePos = 0;

    this->consoleInputRunning = false;
}

TDG_RoomEditor::~TDG_RoomEditor()
{
    if(this->consoleInputRunning)
    {
        this->consoleInputRunning = false;
        SDL_DetachThread(this->consoleInput);
    }

    if(this->win != NULL)
        delete this->win;
    if(this->event != NULL)
        delete this->event;
    if(this->board != NULL)
        delete this->board;
}

bool TDG_RoomEditor::init()
{
    TDG_FileHandler* specs = new TDG_FileHandler();

    if(!specs->loadOpt())
    {
        cout << "Unable to load option file!" << endl;
        delete specs;
        return false;
    }

    this->win = new TDG_Window();
    if(!this->win->init(specs->getOpt()))
    {
        cout << "Unable to initialize window!" << endl;
        delete specs;
        return false;
    }

    delete specs;

    this->board = new TDG_EditorBoard();
    if(!this->board->init(this->win))
    {
        cout << "Unable to initialize editor board!" << endl;
        return false;
    }

    this->event = new TDG_EventHandler();

    return true;
}

bool TDG_RoomEditor::start()
{
    if(!this->consoleInputRunning)
    {
        this->consoleInputRunning = true;
        this->consoleInput = SDL_CreateThread(console_thread, "ConsoleInput", (void*) this);
        if(this->consoleInput == NULL)
            return false;
        else
            return true;
    }
    return false;
}
void TDG_RoomEditor::stop()
{
    if(this->consoleInputRunning)
    {
        this->consoleInputRunning = false;
        SDL_DetachThread(this->consoleInput);
    }
}

void TDG_RoomEditor::input()
{
    this->board->handleInput(this->win, this->event, &this->cStatus);
}

void TDG_RoomEditor::programLoop()
{
    while(!this->event->quit())
    {
        input();

        ///////////// FPS //////////////////////////////////////////////////
        int before, delay;                                                //
        before = (int)SDL_GetTicks();                                     //
        ////////////////////////////////////////////////////////////////////

        this->board->render(this->win);

        ////////////////////////////////////////////////////////////////////
        delay = 1000/win->getFPSCap() - ((int) SDL_GetTicks() - before);  //
                                                                          //
        if(delay > 0)                                                     //
            SDL_Delay(delay);                                             //
        ////////////////////////////////////////////////////////////////////
    }
}

void TDG_RoomEditor::handleConsoleInput()
{
    string in;
    cout << "Next instruction: " << endl;

    while(this->consoleInputRunning)
    {
        getline(cin, in);
        vector<string> inst = split(in, ' ');

        if(!inst.front().compare("help"))
        {
            cout << "List of instructions:" << endl;
            cout << "1) help                       : Shows help massage." << endl;
            cout << "2) load [id]                  : Load the requested room." << endl;
            cout << "3) save                       : Save the current room." << endl;
            cout << "4) create [name] [id] [r] [c] : Create a new room with name, id, rows and columns." << endl;
        }
        else if(!inst.front().compare("load"))
        {
            inst.erase(inst.begin());
            if(inst.size() != 1)
                cout << "Invalid count of arguments for [load]. Please use the format: load [id]" << endl;
            else
            {
                this->cStatus.roomID = nextInt(inst);
                this->cStatus.load = true;
            }
        }
        else if(!inst.front().compare("save"))
        {
            this->cStatus.save = true;
        }
        else if(!inst.front().compare("create"))
        {
            inst.erase(inst.begin());
            if(inst.size() != 4)
                cout << "Invalid count of arguments for [create]. Please use the format: create [name] [id] [r] [c]" << endl;
            else
            {
                string rName = inst.front();
                inst.erase(inst.begin());
                int roomID = nextInt(inst);
                int rows = nextInt(inst);
                int columns = nextInt(inst);

                string path = "./data/spec/room/";
                ostringstream ss;
                ss << roomID;
                path += ss.str() + "/";

                if(this->board->roomExists(path.c_str()))
                {
                    cout << "Room does already exist!" << endl;
                }
                else
                {
                    this->cStatus.rName = rName;
                    this->cStatus.roomID = roomID;
                    this->cStatus.rows = rows;
                    this->cStatus.columns = columns;
                    this->cStatus.create = true;
                }
            }
        }
        else if(this->board != NULL)
        {
            if(this->board->roomStored())
            {
                if(!inst.front().compare("addT"))
                {
                    inst.erase(inst.begin());
                    if(inst.size() != 2)
                        cout << "Invalid count of arguments for [addT]. Please use the format: addT [id] [palette position]" << endl;
                    else
                    {
                        this->cStatus.addID = nextInt(inst);
                        this->cStatus.palettePos = nextInt(inst);
                        this->cStatus.addT = true;
                    }
                }
                else if(!inst.front().compare("addC"))
                {
                    inst.erase(inst.begin());
                    if(inst.size() != 1)
                        cout << "Invalid count of arguments for [addC]. Please use the format: addC [id]" << endl;
                    else
                    {
                        this->cStatus.addID = nextInt(inst);
                        this->cStatus.addC = true;
                    }
                }
                else if(!inst.front().compare("addO"))
                {
                    inst.erase(inst.begin());
                    if(inst.size() != 1)
                        cout << "Invalid count of arguments for [addO]. Please use the format: addO [id]" << endl;
                    else
                    {
                        this->cStatus.addID = nextInt(inst);
                        this->cStatus.addO = true;
                    }
                }
            }
        }
        else
        {
            cout << "Write help for information." << endl;
        }

        inst.clear();
    }
}

int TDG_RoomEditor::console_thread(void* param)
{
    ((TDG_RoomEditor*)param)->handleConsoleInput();
    return 0;
}

int TDG_RoomEditor::nextInt(vector<string>& entries)
{
    int result = atoi(entries.front().c_str());
    entries.erase(entries.begin());
    return result;
}

vector<string> TDG_RoomEditor::split(const string& str, char delimiter)
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
