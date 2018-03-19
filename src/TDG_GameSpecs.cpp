#include "TDG_GameSpecs.h"

TDG_GameSpecs::TDG_GameSpecs()
{
    this->opt = new Options();
    this->room = new Room();
    this->sPoint = new SavePoint();

    this->opt->fpsCap = 0;
    this->opt->fullscreen = false;
    this->opt->winHight = 0;
    this->opt->winWidth = 0;

    this->room->enviromentCollision = NULL;
    this->room->tileIDArrangement = NULL;
    this->room->player.id = 0;
    this->room->player.posX = 0;
    this->room->player.posY = 0;
    this->room->tileColumns = 0;
    this->room->tileRows = 0;

    this->sPoint->playerCharID = 0;
    this->sPoint->roomID = 0;
}

TDG_GameSpecs::~TDG_GameSpecs()
{
    this->room->npc.clear();
    this->room->obj.clear();
    this->room->tileIDs.clear();

    int k;
    for(k = 0; k < this->room->tileRows; k++)
    {
        free(this->room->tileIDArrangement[k]);
    }
    if(this->room->tileIDArrangement != NULL)
        free(this->room->tileIDArrangement);

    int l;
    for(l = 0; l < this->room->tileRows; l++)
    {
        free(this->room->enviromentCollision[l]);
    }
    if(this->room->enviromentCollision != NULL)
        free(this->room->enviromentCollision);

    delete this->opt;
    delete this->room;
    delete this->sPoint;
}

bool TDG_GameSpecs::load()
{
    if(loadOpt())
    {
        if(loadSPoint())
        {
            if(this->sPoint->roomID != 0)
            {
                if(!loadRoom(this->sPoint->roomID))
                {
                    cout << "Error while loading room file!" << endl;
                    return false;
                }
            }
            //default room
            else
            {
                if(!loadRoom(1))
                {
                    cout << "Error while loading room file!" << endl;
                    return false;
                }
            }
        }
        else
        {
            cout << "Error while loading save-point file!" << endl;
            return false;
        }
    }
    else
    {
        cout << "Error while loading option file!" << endl;
        return false;
    }

    return true;
}

bool TDG_GameSpecs::loadRoom(int roomID)
{
    if(roomID <= 0)
    {
        cout << "Room ID not valid. Couldnt load room!" << endl;
        return false;
    }

    string rPath = "./data/spec/room/";
    ostringstream ss;
    ss << roomID;
    rPath += ss.str() + ".room";

    ifstream room;
    room.open(rPath.c_str(), ios::out);
    if(!room.is_open())
    {
        cout << "Path " << rPath << " to Room-spezification-file is unvalid!" << endl;
        return false;
    }
    else
    {
        string line;
        while(getline(room, line))
        {
            vector<string> entries = split(line, ' ');
            string entry = entries.front();

            if(!entry.compare("name:"))
            {
                if(!entries.empty() && (entries.size() == 2))
                {
                    this->room->roomName = nextString(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << rPath << " at statement -name-!"<< endl;
                    room.close();
                    return false;
                }
            }
            else if(!entry.compare("tileIDs"))
            {
                while(!entries.empty())
                {
                    int tileID = nextInt(entries);
                    this->room->tileIDs.push_back(tileID);
                }
            }
            else if(!entry.compare("size:"))
            {
                if(!entries.empty() && (entries.size() == 3))
                {
                    this->room->tileColumns = nextInt(entries);
                    this->room->tileRows = nextInt(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << rPath << " at statement -size-!"<< endl;
                    room.close();
                    return false;
                }
            }
            else if(!entry.compare("background:"))
            {
                if(this->room->tileColumns <= 0 || this->room->tileRows <= 0)
                {
                    cout << "Error in " << rPath << " size is not valid!(1)" << endl;
                    room.close();
                    return false;
                }
                int columns = this->room->tileColumns;
                int rows = this->room->tileRows;

                //two dimensional tile-array which contains the tile IDs of the background
                int** tmp;
                if((tmp = (int**) malloc(rows*sizeof(int*))) == NULL)
                {
                    cout << "Background-Array from file" << rPath << " couldnt be initialized!(1)" << endl;
                    room.close();
                    return false;
                }
                int k;
                for(k = 0; k < rows; k++)
                {
                    if((tmp[k] = (int*) malloc(columns*sizeof(int))) == NULL)
                    {
                        cout << "Background-Array from file" << rPath << " couldnt be initialized!(2)" << endl;
                        room.close();
                        return false;
                    }
                }
                this->room->tileIDArrangement = tmp;

                vector<string> tileIDs;

                int i, j;
                for(i = 0; i < rows; i++)
                {
                    getline(room, line);
                    tileIDs = split(line, ' ');

                    for(j = 0; j < columns; j++)
                    {
                        this->room->tileIDArrangement[i][j] = atoi(tileIDs.front().c_str());
                        tileIDs.erase(tileIDs.begin());
                    }
                }
            }
            else if(!entry.compare("collision:"))
            {
                if(this->room->tileColumns <= 0 || this->room->tileRows <= 0)
                {
                    cout << "Error in " << rPath << " size is not valid!(2)" << endl;
                    room.close();
                    return false;
                }

                int columns = this->room->tileColumns;
                int rows = this->room->tileRows;

                bool** tmp;
                if((tmp = (bool**) malloc(rows*sizeof(bool*))) == NULL)
                {
                    cout << "Background-Collision-Array from file" << rPath << " couldnt be initialized!(1)" << endl;
                    room.close();
                    return false;
                }
                int k;
                for(k = 0; k < rows; k++)
                {
                    if((tmp[k] = (bool*) malloc(columns*sizeof(bool))) == NULL)
                    {
                        cout << "Background-Collision-Array from file" << rPath << " couldnt be initialized!(2)" << endl;
                        room.close();
                        return false;
                    }
                }
                this->room->enviromentCollision = tmp;

                vector<string> coll;

                int i, j;
                for(i = 0; i < rows; i++)
                {
                    getline(room, line);
                    coll = split(line, ' ');

                    for(j = 0; j < columns; j++)
                    {
                        if(atoi(coll.front().c_str()) == 1)
                            this->room->enviromentCollision[i][j] = true;
                        else
                            this->room->enviromentCollision[i][j] = false;
                        coll.erase(coll.begin());
                    }
                }
            }
            else if(!entry.compare("player:"))
            {
                //default player character
                this->room->player.id = 1;

                if(!entries.empty() && (entries.size() == 3))
                {
                    this->room->player.posX = nextInt(entries);
                    this->room->player.posY = nextInt(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << rPath << " at statement -player-!"<< endl;
                    room.close();
                    return false;
                }
            }
            else if(!entry.compare("npc:"))
            {
                while(!entries.empty() && (entries.size() >= 3))
                {
                    int id = nextInt(entries);
                    int x = nextInt(entries);
                    int y = nextInt(entries);

                    Entity newEntity = {id, x, y};
                    this->room->npc.push_back(newEntity);
                }
            }
            else if(!entry.compare("obj:"))
            {
                while(!entries.empty() && (entries.size() >= 3))
                {
                    int id = nextInt(entries);
                    int x = nextInt(entries);
                    int y = nextInt(entries);

                    Entity newEntity = {id, x, y};
                    this->room->obj.push_back(newEntity);
                }
            }
        }
        room.close();
    }
    return true;
}

bool TDG_GameSpecs::loadSPoint()
{
    string spPath = "./game.savepoint";
    ifstream saveP;
    saveP.open(spPath.c_str(), ios::out);
    if(!saveP.is_open())
    {
        cout << "Path " << spPath << " to Save-Point-file is unvalid!" << endl;
        return false;
    }
    else
    {
        string line;
        while(getline(saveP, line))
        {
            vector<string> entries = split(line, ' ');
            string entry =  entries.front();

            if(!entry.compare("playerCharacterID:"))
            {
                if(!entries.empty() && (entries.size() == 2))
                {
                    this->sPoint->playerCharID = nextInt(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << spPath << " at statement -playerCharacterID-!"<< endl;
                    saveP.close();
                    return false;
                }
            }
            else if(!entry.compare("roomID:"))
            {
                if(!entries.empty() && (entries.size() == 2))
                {
                    this->sPoint->roomID = nextInt(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << spPath << " at statement -roomID-!"<< endl;
                    saveP.close();
                    return false;
                }
            }
        }
        saveP.close();
    }
    return true;
}

bool TDG_GameSpecs::loadOpt()
{
    string oPath = "./game.options";
    ifstream opt;
    opt.open(oPath.c_str(), ios::out);
    if(!opt.is_open())
    {
        cout << "Path " << oPath << " to Option-file is unvalid!" << endl;
        return false;
    }
    else
    {
        string line;
        while(getline(opt, line))
        {
            vector<string> entries = split(line, ' ');
            string entry = entries.front();

            if(!entry.compare("windowSize:"))
            {
                if(!entries.empty() && (entries.size() == 3))
                {
                    this->opt->winWidth = nextInt(entries);
                    this->opt->winHight = nextInt(entries);
                }
                //default window size settings
                else
                {
                    this->opt->winWidth = 640;
                    this->opt->winHight = 480;
                    cout << "Invalid count of arguments in " << oPath << " at statement -windowSize-!"<< endl;
                }
            }
            else if(!entry.compare("fullscreen:"))
            {
                if(!entries.empty() && (entries.size() == 2))
                {
                    if(nextInt(entries) == 1)
                        this->opt->fullscreen = true;
                    else
                        this->opt->fullscreen = false;
                }
                //default fullscreen setting
                else
                {
                    this->opt->fullscreen = false;
                    cout << "Invalid count of arguments in " << oPath << " at statement -fullscreen-!"<< endl;
                }
            }
            else if(!entry.compare("fpsCap:"))
            {
                if(!entries.empty() && (entries.size() == 2))
                {
                    this->opt->fpsCap = nextInt(entries);
                }
                //default fpsCap setting
                else
                {
                    this->opt->fpsCap = 60;
                    cout << "Invalid count of arguments in " << oPath << " at statement -fpsCap-!"<< endl;
                }
            }
        }
        opt.close();
    }
    return true;
}

Room* TDG_GameSpecs::getRoom()
{
    return this->room;
}
Options* TDG_GameSpecs::getOpt()
{
    return this->opt;
}
SavePoint* TDG_GameSpecs::getSPoint()
{
    return this->sPoint;
}

int TDG_GameSpecs::nextInt(vector<string>& entries)
{
    entries.erase(entries.begin());
    return atoi(entries.front().c_str());
}

string& TDG_GameSpecs::nextString(vector<string>& entries)
{
    entries.erase(entries.begin());
    return entries.front();
}

vector<string> TDG_GameSpecs::split(const string& str, char delimiter)
{
    istringstream is(str);
    vector<string> result;
    for(string cur; getline(is, cur, delimiter); result.push_back(cur));
    return result;
}
