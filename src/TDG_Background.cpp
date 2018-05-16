#include "TDG_Background.h"

TDG_Background::TDG_Background()
{
    this->sTiles = NULL;
    this->tileArrangement = NULL;
    this->tileColumns = 0;
    this->tileRows = 0;
    this->tileWidth = 0;
    this->tileHight = 0;
}

TDG_Background::~TDG_Background()
{
    this->gates.clear();

    if(this->tileArrangement != NULL)
    {
        for(int k = 0; k < this->tileRows; k++)
            delete [] this->tileArrangement[k];

        delete [] this->tileArrangement;
    }

    if(this->sTiles != NULL)
    {
        TDG_StoredTiles* tmp = this->sTiles;
        TDG_StoredTiles* next;
        while(tmp != NULL)
        {
            next = tmp->getNext();
            delete tmp;
            tmp = next;
        }
    }
}

bool TDG_Background::create(TDG_Window* win, Room* room)
{
    //********************************************************************************************************/
    //Load needed images from sprite and save them in the TDG_StoredTiles list
    if(!this->initRoom(win, room->tileRows, room->tileColumns))
    {
        cout << "Unable to initialize room!" << endl;
        return false;
    }

    //Load sprite sheet for loading all tile images except the default one
    TDG_SpriteLoader* sprite = new TDG_SpriteLoader();
    if(!sprite->loadSprite("./data/img/room/", "room"))
    {
        cout << "Unable to load background sprite!" << endl;
        return false;
    }

    while(!room->tileIDs.empty())
    {
        int id =  room->tileIDs.front();

        //load all tile images and skip the tile images which are already stored
        if(addImage(win, sprite, id))
            cout << "Stored tile image with ID: " << id << endl;

        room->tileIDs.erase(room->tileIDs.begin());
    }

    delete sprite;

    //**************************************************************************************************************
    //fill array
    int i, j;
    for(i = 0; i < this->tileRows; i++)
    {
        for(j = 0; j < this->tileColumns; j++)
        {
            this->tileArrangement[i][j].set(room->tileIDArrangement[i][j],
                                             room->tileRotationDegree[i][j],
                                             room->enviromentCollision[i][j],
                                             room->flipTile[i][j]);
        }
    }

    //************************************************************************
    //save gates
    this->gates = room->gates;

    return true;
}

bool TDG_Background::createEmpty(TDG_Window* win, int rows, int columns)
{
    if(!this->initRoom(win, rows, columns))
    {
        cout << "Unable to initialize room!" << endl;
        return false;
    }

    //fill tile information array with default settings
    int i, j;
    for(i = 0; i < this->tileRows; i++)
    {
        for(j = 0; j < this->tileColumns; j++)
        {
            this->tileArrangement[i][j].set(1, 0, false, 0);
        }
    }
    return true;
}

bool TDG_Background::initRoom(TDG_Window* win, int rows, int columns)
{
    this->tileRows = rows;
    this->tileColumns = columns;

    //Load sprite sheet and all important information from a file.
    TDG_SpriteLoader* sprite = new TDG_SpriteLoader();
    if(!sprite->loadSprite("./data/img/room/", "room"))
    {
        cout << "Unable to load background sprite!" << endl;
        return false;
    }

    this->tileWidth = sprite->getImgWidth();
    this->tileHight = sprite->getImgHight();

    //Load the default tile image: 1
    if(addImage(win, sprite, 1))
        cout << "Stored tile image with ID: " << "1" << endl;

    delete sprite;

     //create tile array (contains information about the background)
    this->tileArrangement = new TDG_Tile*[this->tileRows];
    for(int u = 0; u < this->tileRows; u++)
        this->tileArrangement[u] = new TDG_Tile[this->tileColumns];

    return true;
}

bool TDG_Background::renderAtPos(TDG_Window* win, int x, int y)
{
    int r, c;
    for(r = 0; r < this->tileRows; r++)
    {
        for(c = 0; c < this->tileColumns; c++)
        {
            //get image from stored background tiles
            SDL_Texture* tileImg = getTileImage(this->tileArrangement[r][c].getID());
            if(tileImg == NULL)
            {
                cout << "Unable to find tile image in the list of stored ones!" << endl;
                return false;
            }
            //get the degree in which the tile should be rotated
            int rotDegree = this->tileArrangement[r][c].getRotDegree();

            //render tile on rect
            SDL_Rect rect = {x + c*this->tileWidth, y + r*this->tileHight,
                            this->tileWidth, this->tileHight};

            SDL_RenderCopyEx(win->getRenderer(), tileImg, NULL, &rect, rotDegree, NULL, this->tileArrangement[r][c].flipTile());
        }
    }

    return true;
}

bool TDG_Background::addImage(TDG_Window* win, TDG_SpriteLoader* sprite, int id)
{
    if(this->isTileImgStored(id))
        return false;

    SDL_Texture* newImage = sprite->getImage(win, (int) (id-1)/sprite->getSpriteMaxColumns() + 1, (int) id%(sprite->getSpriteMaxColumns() + 1));
    if(newImage == NULL)
    {
        cout << "Unable to load image " << id << " of sprite sheet ./data/img/room/room.png" << endl;
        delete sprite;
        return false;
    }

    if(this->sTiles != NULL)
    {
        TDG_StoredTiles* newTileImg = new TDG_StoredTiles(newImage, id);
        newTileImg->setNext(this->sTiles->getNext());
        this->sTiles->setNext(newTileImg);
    }
    else
        this->sTiles = new TDG_StoredTiles(newImage, id);

    return true;
}

bool TDG_Background::isTileImgStored(int id)
{
    TDG_StoredTiles*  tmp = this->sTiles;
    while(tmp != NULL)
    {
        if(tmp->getTileID() == id)
            return true;
        tmp = tmp->getNext();
    }
    return false;
}

void TDG_Background::removeUnusedImages()
{
    list<int> usedT;
    for(int i = 0; i < this->tileRows; i++)
    {
        for(int j = 0; j < this->tileColumns; j++)
        {
            bool alreadyStored = false;
            for(list<int>::const_iterator it = usedT.begin(), end = usedT.end(); it != end; it++)
            {
                if(*it == this->tileArrangement[i][j].getID())
                {
                    alreadyStored = true;
                    it = usedT.end();
                }
            }
            if(!alreadyStored)
                usedT.push_back(this->tileArrangement[i][j].getID());
        }
    }

    TDG_StoredTiles* allT = this->sTiles;
    TDG_StoredTiles* prevT = NULL;
    while(allT != NULL)
    {
        bool inUse = false;
        for(list<int>::const_iterator it = usedT.begin(), end = usedT.end(); it != end; it++)
        {
            if(*it == allT->getTileID())
            {
                inUse = true;
                it = usedT.end();
            }
        }
        if(!inUse)
        {
            if(prevT == NULL)
            {
                TDG_StoredTiles* next = this->sTiles->getNext();
                delete this->sTiles;
                this->sTiles = next;
            }
            else
            {
                prevT->setNext(allT->getNext());
                delete allT;
            }
        }
        prevT = allT;
        allT = allT->getNext();
    }
}

bool TDG_Background::isTileImpassable(int row, int column)
{
    if(this->tileArrangement == NULL)
    {
        cout << "Error: Tryed to access background which was not created." << endl;
        return false;
    }

    if((this->tileRows <= row) || (this->tileColumns <= column))
    {
        cout << "Error: Tile access was out of bound." << endl;
        return false;
    }

    return this->tileArrangement[row][column].isImpassable();
}

bool TDG_Background::isGate(int row, int column)
{
    for(list<Gate>::const_iterator it = this->gates.begin(), end = this->gates.end(); it != end; it++)
    {
        if(it->row == row)
        {
            if(it->column == column)
                return true;
        }
    }

    return false;
}

void TDG_Background::print(ofstream* out)
{
    *out << "size: " << this->tileColumns << " " << this->tileRows << endl;

    *out << "background:" << endl;
    for(int i = 0; i < this->tileRows; i++)
    {
        for(int j = 0; j < this->tileColumns; j++)
        {
            *out << this->tileArrangement[i][j].getID() << " ";
        }
        *out << endl;
    }

    *out << "rotation:" << endl;
    for(int i = 0; i < this->tileRows; i++)
    {
        for(int j = 0; j < this->tileColumns; j++)
        {
            *out << this->tileArrangement[i][j].getRotDegree() << " ";
        }
        *out << endl;
    }

    *out << "collision:" << endl;
    for(int i = 0; i < this->tileRows; i++)
    {
        for(int j = 0; j < this->tileColumns; j++)
        {
            *out << this->tileArrangement[i][j].isImpassable() << " ";
        }
        *out << endl;
    }

    *out << "flip:" << endl;
    for(int i = 0; i < this->tileRows; i++)
    {
        for(int j = 0; j < this->tileColumns; j++)
        {
            if(this->tileArrangement[i][j].flipTile() == SDL_FLIP_HORIZONTAL)
                *out << "2" << " ";
            else if(this->tileArrangement[i][j].flipTile() == SDL_FLIP_VERTICAL)
                *out << "1" << " ";
            else if(this->tileArrangement[i][j].flipTile() == SDL_FLIP_NONE)
                *out << "0" << " ";
        }
        *out << endl;
    }

    *out << "gates: ";
    for(list<Gate>::const_iterator it = this->gates.begin(), end = this->gates.end(); it != end; it++)
    {
        string arrivalDir = "s_south";
        if(it->arriveStatus == s_south) arrivalDir = "s_south";
        else if(it->arriveStatus  == s_south_west) arrivalDir = "s_south_west";
        else if(it->arriveStatus  == s_west) arrivalDir = "s_west";
        else if(it->arriveStatus  == s_north_west) arrivalDir = "s_north_west";
        else if(it->arriveStatus  == s_north) arrivalDir = "s_north";
        else if(it->arriveStatus  == s_north_east) arrivalDir = "s_north_east";
        else if(it->arriveStatus  == s_east) arrivalDir = "s_east";
        else if(it->arriveStatus  == s_south_east) arrivalDir = "s_south_east";
        *out << it->id << " " << it->row << " " << it->column << " " << arrivalDir << " " << it->destinationRoomID << " " << it->destinationGateID << " ";
    }
}

TDG_Tile* TDG_Background::getTile(int row, int column)
{
    if(this->tileArrangement == NULL)
    {
        cout << "Error: Tryed to access background which was not created." << endl;
        return NULL;
    }

    if((this->tileRows <= row) || (this->tileColumns <= column))
    {
        cout << "Error: Tile access was out of bound." << endl;
        return NULL;
    }

    return &this->tileArrangement[row][column];
}

SDL_Texture* TDG_Background::getTileImage(int id)
{
    TDG_StoredTiles* tmp = this->sTiles;
    while(tmp != NULL)
    {
        if(id == tmp->getTileID())
            return tmp->getImage();
        else
            tmp = tmp->getNext();
    }

    cout << "List of background tile images contains no image with ID: " << id << endl;

    return NULL;
}

int TDG_Background::getTileWidth()
{
   return this->tileWidth;
}

int TDG_Background::getTileHight()
{
    return this->tileHight;
}

int TDG_Background::getTileRows()
{
    return this->tileRows;
}

int TDG_Background::getTileColumns()
{
    return this->tileColumns;
}

void TDG_Background::getGate(Gate* gate, int row, int column)
{
    for(list<Gate>::const_iterator it = this->gates.begin(), end = this->gates.end(); it != end; it++)
    {
        if(it->row == row)
        {
            if(it->column == column)
                *gate = *it;
        }
    }
}
