#include "TDG_RoomEditor.h"

TDG_RoomEditor::TDG_RoomEditor()
{
    this->event = NULL;
    this->gui = NULL;
    this->board = NULL;

    this->cStatus.create = false;
    this->cStatus.load = false;
    this->cStatus.save = false;
    this->cStatus.roomID = 0;
    this->cStatus.w = 0;
    this->cStatus.h = 0;

    this->consoleInputRunning = false;
}

TDG_RoomEditor::~TDG_RoomEditor()
{
    if(this->consoleInputRunning)
    {
        this->consoleInputRunning = false;
        SDL_DetachThread(this->consoleInput);
    }

    if(this->gui != NULL)
        delete this->gui;
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

    this->gui = new TDG_GUI();
    if(!this->gui->init(specs->getOpt()))
    {
        cout << "Unable to initialize GUI!" << endl;
        delete specs;
        return false;
    }

    delete specs;

    this->board = new TDG_EditorBoard();
    if(!this->board->init())
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

void TDG_RoomEditor::input()
{
    this->board->handleInput(this->event);

    if(this->cStatus.create)
    {
        string path = "./data/spec/room/";
        ostringstream ss;
        ss << this->cStatus.roomID;
        path += ss.str() + "/";

        if(roomExists(path.c_str()))
        {
            cout << "Room does already exist!" << endl;
        }
        else
        {
            if(!this->board->createRoom(this->cStatus.roomID, this->cStatus.w, this->cStatus.h))
                cout << "Unable to create Room!" << endl;
        }

        this->cStatus.create = false;
        this->cStatus.roomID = 0;
        this->cStatus.w = 0;
        this->cStatus.h = 0;
    }
    else if(this->cStatus.load && (this->cStatus.roomID != 0))
    {
        string path = "./data/spec/room/";
        ostringstream ss;
        ss << this->cStatus.roomID;
        path += ss.str() + "/";

        if(roomExists(path.c_str()))
        {
            TDG_FileHandler* fh = new TDG_FileHandler();
            fh->loadRoom(this->cStatus.roomID);

            if(!this->board->loadRoom(this->gui, fh->getRoom()))
            cout << "Unable to load room!" << endl;

            delete fh;
        }
        else
        {
            cout << "Room does not exist!" << endl;
        }

        this->cStatus.load = false;
        this->cStatus.roomID = 0;
    }
    else if(this->cStatus.save)
    {
        if(!this->board->saveRoom())
            cout << "Failed to store room!" << endl;

        this->cStatus.save = false;
    }
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

        this->board->render(this->gui);

        ////////////////////////////////////////////////////////////////////
        delay = 1000/gui->getFPSCap() - ((int) SDL_GetTicks() - before);  //
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
            cout << "1) help                 : Shows help massage." << endl;
            cout << "2) load [id]            : Load the requested room." << endl;
            cout << "3) save                 : Save the current room." << endl;
            cout << "4) create [id] [w] [h]  : Create a new room with id, width and hight." << endl;
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
            if(inst.size() != 3)
                cout << "Invalid count of arguments for [create]. Please use the format: create [id] [width] [hight]" << endl;
            else
            {
                this->cStatus.roomID = nextInt(inst);
                this->cStatus.w = nextInt(inst);
                this->cStatus.h = nextInt(inst);
                this->cStatus.create = true;
            }
        }
        else
        {
            cout << "Write help for information." << endl;
        }

        in = "";
        inst.clear();
    }
}

int TDG_RoomEditor::console_thread(void* param)
{
    ((TDG_RoomEditor*)param)->handleConsoleInput();
    return 0;
}

bool TDG_RoomEditor::roomExists(const string& roomDirectoryPath)
{
  DWORD ftyp = GetFileAttributesA(roomDirectoryPath.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
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
