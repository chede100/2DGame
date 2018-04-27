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
    if(this->tileArrangement != NULL)
    {
        int i, j;
        for(i = 0; i < this->tileRows; i++)
        {
            for(j = 0; j < this->tileColumns; j++)
            {
                if(this->tileArrangement[i][j] != NULL)
                    delete this->tileArrangement[i][j];
            }
        }

        int k;
        for(k = 0; k < this->tileRows; k++)
        {
            if(this->tileArrangement[k] != NULL)
                free(this->tileArrangement[k]);
        }
        if(this->tileArrangement != NULL)
            free(this->tileArrangement);
    }

    if(this->sTiles != NULL)
    {
        TDG_StoredTiles* tmp = this->sTiles;
        TDG_StoredTiles* next = NULL;
        while(tmp->getNext() != NULL)
        {
            next = tmp->getNext();
            delete tmp;
            tmp = next;
        }
    }
}

bool TDG_Background::create(TDG_GUI* gui, Room* room)
{
    this->tileRows = room->tileRows;
    this->tileColumns = room->tileColumns;

    //********************************************************************************************************/
    //Load needed images from sprite and save them in the TDG_StoredTiles list

    //Load sprite sheet and all important information from a file.
    TDG_SpriteLoader* sprite = new TDG_SpriteLoader();
    if(!sprite->loadSprite("./data/img/room/", "room"))
    {
        cout << "Unable to load background sprite!" << endl;
        return false;
    }

    this->tileWidth = sprite->getImgWidth();
    this->tileHight = sprite->getImgHight();

    //Get the tile id.
    int id = room->tileIDs.front();

    //Adjust the tile ID to the location of the image on the sprite sheet (row, column)
    //Load first image from sprite sheet and init StoredTiles list
    SDL_Texture* newImage = sprite->getImage(gui, (int) id/sprite->getSpriteMaxColumns() + 1 , (int) id%sprite->getSpriteMaxColumns());
    if(newImage == NULL)
    {
        cout << "Unable to load image " << id << " of sprite sheet ./data/img/room/room.png" << endl;
        delete sprite;
        return false;
    }
    room->tileIDs.erase(room->tileIDs.begin());
    this->sTiles = new TDG_StoredTiles(newImage, id);

    cout << "Stored tile image with ID: " << id << endl;

    //continuously load all tile images from sprite sheet
    TDG_StoredTiles* sTiles = this->sTiles;
    while(!room->tileIDs.empty())
    {
        id =  room->tileIDs.front();

        newImage = sprite->getImage(gui, (int) (id-1)/sprite->getSpriteMaxColumns() + 1, (int) id%(sprite->getSpriteMaxColumns() + 1));
        if(newImage == NULL)
        {
            cout << "Unable to load image " << id << " of sprite sheet ./data/img/room/room.png" << endl;
            delete sprite;
            return false;
        }
        room->tileIDs.erase(room->tileIDs.begin());

        TDG_StoredTiles* newTileImg = new TDG_StoredTiles(newImage, id);
        sTiles->setNext(newTileImg);
        sTiles = newTileImg;

        cout << "Stored tile image with ID: " << id << endl;
    }

    delete sprite;

    //**************************************************************************************************************/
    //create tile array (contains information about the background)

    TDG_Tile*** tmp;
    if((tmp = (TDG_Tile***) malloc(this->tileRows*sizeof(TDG_Tile**))) == NULL)
    {
        cout << "Tile arrangement array couldnt be created!" << endl;
        return false;
    }
    int k;
    for(k = 0; k < this->tileRows; k++)
    {
        if((tmp[k] = (TDG_Tile**) malloc(this->tileColumns*sizeof(TDG_Tile*))) == NULL)
        {
            cout << "Tile arrangement array couldnt be created!" << endl;
            return false;
        }
    }
    this->tileArrangement = tmp;

    //fill array
    int i, j;
    for(i = 0; i < this->tileRows; i++)
    {
        for(j = 0; j < this->tileColumns; j++)
        {
            this->tileArrangement[i][j] = new TDG_Tile(room->tileIDArrangement[i][j],
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

bool TDG_Background::renderAtPos(TDG_GUI* gui, int x, int y)
{
    int r, c;
    for(r = 0; r < this->tileRows; r++)
    {
        for(c = 0; c < this->tileColumns; c++)
        {
            //get image from stored background tiles
            SDL_Texture* tileImg = getTileImage(this->tileArrangement[r][c]->getID());
            if(tileImg == NULL)
            {
                cout << "Unable to find tile image in the list of stored ones!" << endl;
                return false;
            }
            //get the degree in which the tile should be rotated
            int rotDegree = this->tileArrangement[r][c]->getRotDegree();

            //render tile on rect
            SDL_Rect rect = {x + c*(this->tileWidth*gui->getScaleFactor()), y + r*(this->tileHight*gui->getScaleFactor()),
                            this->tileWidth*gui->getScaleFactor(), this->tileHight*gui->getScaleFactor()};

            SDL_RenderCopyEx(gui->getRenderer(), tileImg, NULL, &rect, rotDegree, NULL, this->tileArrangement[r][c]->flipTile());
        }
    }

    return true;
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

    return this->tileArrangement[row][column]->isImpassable();
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
