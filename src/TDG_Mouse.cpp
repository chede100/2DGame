#include "TDG_Mouse.h"

TDG_Mouse::TDG_Mouse()
{
    this->sel = NULL;
    this->typ = noSelection;
}

TDG_Mouse::~TDG_Mouse()
{
    if(this->typ == TILE)
    {
        if(this->sel != NULL)
            free((SelectedTile*)this->sel);
    }
}

void TDG_Mouse::renderSelRect(TDG_GUI* gui, TDG_View* view)
{
    int x, y;
    if(this->typ == TILE)
    {
        SelectedTile* sT = (SelectedTile*) this->sel;
        x = sT->c*sT->w - view->getPosX();
        y = sT->r*sT->h - view->getPosY();
        SDL_Rect rect = {x, y, sT->w, sT->h};
        if(SDL_RenderDrawRect(gui->getRenderer(), &rect))
            cout << "Failed to render mouse selection marking." << endl;
    }
    else if(this->typ == ENTITY)
    {
        TDG_Entity* ent = (TDG_Entity*) this->sel;
        x = ent->getPos()->getPosX() - view->getPosX();
        y = ent->getPos()->getPosY() - view->getPosY();
        SDL_Rect rect = {x, y, ent->getImageWidth(), ent->getImageHight()};
        if(SDL_RenderDrawRect(gui->getRenderer(), &rect))
            cout << "Failed to render mouse selection marking." << endl;
    }
}

void TDG_Mouse::handleEvent(SDL_Event* event, TDG_EntityHandler* eh, TDG_Background* bg, TDG_View* view)
{
    if((eh == NULL) || (bg == NULL));

    //If mouse event happened
    else if( event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP )
    {
        //get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        int vX = view->getPosX();
        int vY = view->getPosY();

        x = x/2 + vX;
        y = y/2 + vY;

        int maxHight = bg->getTileRows()*bg->getTileHight();
        int maxWidth = bg->getTileColumns()*bg->getTileWidth();
        if(((x > 0) && (y > 0)) && ((x < maxWidth) && (y < maxHight)))
        {
            //move selected entity or tile to the position of the mouse
            if(this->sel != NULL)
            {
                if(this->typ == ENTITY)
                {
                    TDG_Entity* ent = (TDG_Entity*) this->sel;

                    ent->getPos()->setPosX(x);
                    ent->getPos()->setPosY(y);
                }
                else if(this->typ == TILE)
                {
                    int tWidth = bg->getTileWidth();
                    int tHight = bg->getTileHight();
                    if((tWidth != 0) && (tHight != 0))
                    {
                        //on which tile is the mouse positioned
                        int mouseRow = y/tHight;
                        int mouseColumn = x/tWidth;

                        SelectedTile* tile = (SelectedTile*) this->sel;

                        //move the tile to the position of the mouse if their positions are not the same
                        if((tile->r != mouseRow) || (tile->c != mouseColumn))
                        {
                            //reset old tile
                            TDG_Tile* prevTile = bg->getTile(tile->r, tile->c);
                            prevTile->set(tile->prev.id, tile->prev.rotDegree, tile->prev.impassable, tile->prev.flip);

                            //tile at mouse position
                            TDG_Tile* moveTile = bg->getTile(mouseRow, mouseColumn);

                            //temporary store previous tile infos
                            tile->prev.id = moveTile->getID();
                            tile->prev.rotDegree = moveTile->getRotDegree();
                            tile->prev.impassable = moveTile->isImpassable();
                            if(moveTile->flipTile() == SDL_FLIP_HORIZONTAL)
                                tile->prev.flip = 2;
                            else if(moveTile->flipTile() == SDL_FLIP_VERTICAL)
                                tile->prev.flip = 1;
                            else
                                tile->prev.flip = 0;

                            //move tile to mouse position
                            moveTile->set(tile->id, tile->rotDegree, tile->impassable, tile->flip);

                            //update the tiles position
                            tile->r = mouseRow;
                            tile->c = mouseColumn;
                        }
                    }
                    else
                        cout << "Error: Tile width or hight is zero!" << endl;
                }
            }

            if(event->type == SDL_MOUSEBUTTONDOWN)
            {
                if(event->button.button == SDL_BUTTON_RIGHT)
                {
                    //when something is selected and a left click is observed, place the selected at the current mouse position
                    if((this->sel != NULL) && (this->typ != noSelection))
                    {
                        if(this->typ == ENTITY)
                        {
                            this->sel = NULL;
                            this->typ = noSelection;
                        }
                        else if(this->typ == TILE)
                        {
                            free(this->sel);
                            this->sel = NULL;
                            this->typ = noSelection;
                        }
                    }
                }
                else if(event->button.button == SDL_BUTTON_LEFT)
                {
                    //nothing selected => check if mouse is on a entity or a tile and select it
                    if((this->sel == NULL) && (this->typ == noSelection))
                    {
                        if(!selectEntity(eh, x, y))
                            selectTile(bg, x, y);
                    }
                }
            }
        }
    }
}

bool TDG_Mouse::selectEntity(TDG_EntityHandler* eh, int posX, int posY)
{
    TDG_Entity* ent = eh->getEntity(posX, posY);
    if(ent != NULL)
    {
        this->sel = ent;
        this->typ = ENTITY;

        return true;
    }

    return false;
}

bool TDG_Mouse::selectTile(TDG_Background* bg, int posX, int posY)
{
    int tWidth = bg->getTileWidth();
    int tHight = bg->getTileHight();
    if((tWidth != 0) && (tHight != 0))
    {
        int mouseRow = posY/tHight;
        int mouseColumn = posX/tWidth;

        TDG_Tile* t = bg->getTile(mouseRow, mouseColumn);
        if(t == NULL)
            return false;
        else
        {
            SelectedTile* sTile = (SelectedTile*) malloc(sizeof(SelectedTile));

            //put a empty tile on the previous position
            sTile->prev.id = 1;
            sTile->prev.rotDegree = 0;
            sTile->prev.impassable = false;
            sTile->prev.flip = 0;

            sTile->r = mouseRow;
            sTile->c = mouseColumn;
            sTile->id = t->getID();
            sTile->rotDegree = t->getRotDegree();
            sTile->impassable = t->isImpassable();
            sTile->w = tWidth;
            sTile->h = tHight;
            if(t->flipTile() == SDL_FLIP_HORIZONTAL)
                sTile->flip = 2;
            else if(t->flipTile() == SDL_FLIP_VERTICAL)
                sTile->flip = 1;
            else
                sTile->flip = 0;

            this->sel = sTile;
            this->typ = TILE;

            return true;
        }
    }
    return false;
}
