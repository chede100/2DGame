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

    int m;
    for(m = 0; m < this->room->tileRows; m++)
    {
        free(this->room->tileRotationDegree[m]);
    }
    if(this->room->tileRotationDegree != NULL)
        free(this->room->tileRotationDegree);


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
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 1))
                {
                    nextString(entries, this->room->roomName);
                }
                else
                {
                    cout << "Invalid count of arguments in " << rPath << " at statement -name-!"<< endl;
                    room.close();
                    return false;
                }
            }
            else if(!entry.compare("tileIDs:"))
            {
                entries.erase(entries.begin());
                while(!entries.empty())
                {
                    int tileID = nextInt(entries);
                    this->room->tileIDs.push_back(tileID);
                }
            }
            else if(!entry.compare("size:"))
            {
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 2))
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
            else if(!entry.compare("rotation:"))
            {
                if(this->room->tileColumns <= 0 || this->room->tileRows <= 0)
                {
                    cout << "Error in " << rPath << " size is not valid!(2)" << endl;
                    room.close();
                    return false;
                }

                int columns = this->room->tileColumns;
                int rows = this->room->tileRows;

                int** tmp;
                if((tmp = (int**) malloc(rows*sizeof(int*))) == NULL)
                {
                    cout << "Rotation degrees of all tiles from file" << rPath << " couldnt be loaded!(1)" << endl;
                    room.close();
                    return false;
                }
                int k;
                for(k = 0; k < rows; k++)
                {
                    if((tmp[k] = (int*) malloc(columns*sizeof(int))) == NULL)
                    {
                        cout << "Rotation degrees of all tiles from file" << rPath << " couldnt be loaded!(2)" << endl;
                        room.close();
                        return false;
                    }
                }
                this->room->tileRotationDegree = tmp;

                vector<string> rotDeg;

                int i, j;
                for(i = 0; i < rows; i++)
                {
                    getline(room, line);
                    rotDeg = split(line, ' ');

                    for(j = 0; j < columns; j++)
                    {
                        this->room->tileRotationDegree[i][j] = atoi(rotDeg.front().c_str());
                        rotDeg.erase(rotDeg.begin());
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
                if(this->room->player.id == 0)
                {
                    cout << "Invalid player ID! ID: 0" << endl;
                    room.close();
                    return false;
                }

                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 3))
                {
                    this->room->player.posX = nextInt(entries);
                    this->room->player.posY = nextInt(entries);

                    string playerStatus;
                    nextString(entries, playerStatus);
                    MovementStatus status = stringToMoveStatus(playerStatus);
                    if(status == noStatus)
                    {
                        cout << "Unable to load player start status." << endl;
                        return false;
                    }
                    this->room->player.firstStatus = status;

                    if(!loadEntity(Character, &this->room->player))
                    {
                        cout << "Unable to load player specifications! ID: " << this->room->player.id << endl;
                        return false;
                    }
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
                entries.erase(entries.begin());
                while(!entries.empty() && (entries.size() >= 4))
                {
                    Entity newNPC;
                    newNPC.id = nextInt(entries);
                    newNPC.posX = nextInt(entries);
                    newNPC.posY = nextInt(entries);

                    string npcStatus;
                    nextString(entries, npcStatus);
                    MovementStatus status = stringToMoveStatus(npcStatus);
                    if(status == noStatus)
                    {
                        cout << "Unable to load npc start status." << endl;
                        return false;
                    }
                    newNPC.firstStatus = status;

                    if(!loadEntity(Character, &newNPC))
                    {
                        cout << "Unable to load npc specifications! ID: " << newNPC.id << endl;
                        return false;
                    }

                    this->room->npc.push_back(newNPC);
                }
            }
            else if(!entry.compare("obj:"))
            {
                entries.erase(entries.begin());
                while(!entries.empty() && (entries.size() >= 4))
                {
                    Entity newObj;
                    newObj.id = nextInt(entries);
                    newObj.posX = nextInt(entries);
                    newObj.posY = nextInt(entries);

                    string objStatus;
                    nextString(entries, objStatus);
                    MovementStatus status = stringToMoveStatus(objStatus);
                    if(status == noStatus)
                    {
                        cout << "Unable to load object start status." << endl;
                        return false;
                    }
                    newObj.firstStatus = status;

                    if(!loadEntity(Object, &newObj))
                    {
                        cout << "Unable to load object specifications! ID: " << newObj.id << endl;
                        return false;
                    }

                    this->room->obj.push_back(newObj);
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
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 1))
                {
                    this->room->player.id = nextInt(entries);
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
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 1))
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
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 2))
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
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 1))
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
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 1))
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

bool TDG_GameSpecs::loadEntity(EntityTyp typ, Entity* e)
{
    //Objects dont have a speed value so its by default zero
    e->speed = 0.0;

    string path;
    if(typ == Character)
        path = "./data/spec/entity/char/";
    else if(typ == Object)
        path = "./data/spec/entity/object/";
    else
    {
        cout << "Unable to load entity because the entity typ is invalid." << endl;
        return false;
    }

    ostringstream ss;
    ss << e->id;

    if(typ == Character)
        path += ss.str() + ".character";
    else if(typ == Object)
        path += ss.str() + ".object";

    ifstream entity;
    entity.open(path.c_str(), ios::out);
    if(!entity.is_open())
    {
        if(typ == Character)
            cout << "Unable to load a specific character! ID: " << e->id << endl;
        else if(typ == Object)
            cout << "Unable to load a specific object! ID: " << e->id << endl;
        return false;
    }
    else
    {
        string line;
        while(getline(entity, line))
        {
            vector<string> entries = split(line, ' ');
            string entry = entries.front();

            if(!entry.compare("name:"))
            {
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 1))
                {
                    nextString(entries, e->name);
                }
                else
                {
                    cout << "Invalid count of arguments in " << path << " at statement -name-!"<< endl;
                    entity.close();
                    return false;
                }
            }
            else if(!entry.compare("animationID:"))
            {
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 1))
                {
                    e->animationID = nextInt(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << path << " at statement -animationID-!"<< endl;
                    entity.close();
                    return false;
                }
            }
            else if(!entry.compare("size:"))
            {
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 2))
                {
                    e->width = nextInt(entries);
                    e->hight = nextInt(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << path << " at statement -size-!"<< endl;
                    entity.close();
                    return false;
                }
            }
            else if(!entry.compare("speed:"))
            {
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 1))
                {
                    e->speed = nextDouble(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << path << " at statement -speed-!"<< endl;
                    entity.close();
                    return false;
                }
            }
        }
        entity.close();
    }

    return true;
}

double TDG_GameSpecs::nextDouble(vector<string>& entries)
{
    double result = atof(entries.front().c_str());
    entries.erase(entries.begin());
    return result;
}

int TDG_GameSpecs::nextInt(vector<string>& entries)
{
    int result = atoi(entries.front().c_str());
    entries.erase(entries.begin());
    return result;
}

void TDG_GameSpecs::nextString(vector<string>& entries, string& input)
{
    input = entries.front();
    entries.erase(entries.begin());
}

vector<string> TDG_GameSpecs::split(const string& str, char delimiter)
{
    istringstream is(str);
    vector<string> result;
    for(string cur; getline(is, cur, delimiter); result.push_back(cur));
    return result;
}

MovementStatus TDG_GameSpecs::stringToMoveStatus(string& status)
{
    if(!status.compare("s_north")) return s_north;
    else if(!status.compare("s_north_east")) return s_north_east;
    else if(!status.compare("s_east")) return s_east;
    else if(!status.compare("s_south_east")) return s_south_east;
    else if(!status.compare("s_south")) return s_south;
    else if(!status.compare("s_south_west")) return s_south_west;
    else if(!status.compare("s_west")) return s_west;
    else if(!status.compare("s_north_west")) return s_north_west;

    else if(!status.compare("m_north")) return m_north;
    else if(!status.compare("m_north_east")) return m_north_east;
    else if(!status.compare("m_east")) return m_east;
    else if(!status.compare("m_south_east")) return m_south_east;
    else if(!status.compare("m_south")) return m_south;
    else if(!status.compare("m_south_west")) return m_south_west;
    else if(!status.compare("m_west")) return m_west;
    else if(!status.compare("m_north_west")) return m_north_west;

    return noStatus;
}
