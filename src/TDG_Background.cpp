#include "TDG_Background.h"

TDG_Background::TDG_Background()
{
    this->sTiles = NULL;
    this->tileArrangement = NULL;
    this->tileColumns = 0;
    this->tileRows = 0;
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

    //Load needed images from sprite and save them in the TDG_StoredTiles list
    TDG_SpriteLoader* loader = new TDG_SpriteLoader("./data/img/room/room.png", 32, 32);

    int id =  room->tileIDs.front();
    SDL_Texture* newImage = loader->getSpriteImage(gui, id);
    if(newImage == NULL)
    {
        cout << "Unable to load image " << id << " of sprite sheet ./data/img/room/room.png" << endl;
        delete loader;
        return false;
    }
    room->tileIDs.erase(room->tileIDs.begin());

    this->sTiles = new TDG_StoredTiles(newImage, id);

    TDG_StoredTiles* sTiles = this->sTiles;
    while(!room->tileIDs.empty())
    {
        id =  room->tileIDs.front();
        SDL_Texture* newImage = loader->getSpriteImage(gui, id);
        if(newImage == NULL)
        {
            cout << "Unable to load image " << id << " of sprite sheet ./data/img/room/room.png" << endl;
            delete loader;
            return false;
        }
        room->tileIDs.erase(room->tileIDs.begin());

        TDG_StoredTiles* newTileImg = new TDG_StoredTiles(newImage, id);
        sTiles->setNext(newTileImg);
        sTiles = newTileImg;
    }

    delete loader;


    //create tile array (contains all information about the background)
    TDG_Tile*** tmp;
    if((tmp = (TDG_Tile***) malloc(this->tileRows*sizeof(TDG_Tile**))) == NULL)
    {
        cout << "Tile arrangement array couldnt be created!" << endl;
        delete loader;
        return false;
    }
    int k;
    for(k = 0; k < this->tileRows; k++)
    {
        if((tmp[k] = (TDG_Tile**) malloc(this->tileColumns*sizeof(TDG_Tile*))) == NULL)
        {
            cout << "Tile arrangement array couldnt be created!" << endl;
            delete loader;
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
                                                       room->enviromentCollision[i][j]);
        }
    }

    return true;
}

bool TDG_Background::renderAtPos(TDG_GUI* gui, int x, int y, int scaleFaktor)
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

            //render tile with scale 1:1
            SDL_Rect rect = {x + c*(TILE_WIDTH*scaleFaktor), y + r*(TILE_HIGHT*scaleFaktor), TILE_WIDTH*scaleFaktor, TILE_HIGHT*scaleFaktor};
            SDL_RenderCopyEx(gui->getRenderer(), tileImg, NULL, &rect, rotDegree, NULL, SDL_FLIP_NONE);
        }
    }

    return true;
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
