#include "TDG_Mouse.h"

TDG_Mouse::TDG_Mouse()
{
    this->sel = NULL;
    this->typ = noSelection;
    this->selected = false;
}

TDG_Mouse::~TDG_Mouse()
{
    if(this->typ == TILE)
    {
        if(this->sel != NULL)
            free((SelectedTile*)this->sel);
    }
}

void TDG_Mouse::renderSelRect(TDG_Window* win, TDG_View* view)
{
    int x, y;
    if(this->typ == TILE)
    {
        SelectedTile* sT = (SelectedTile*) this->sel;
        x = sT->c*sT->w - view->getPosX();
        y = sT->r*sT->h - view->getPosY();
        SDL_Rect rect = {x, y, sT->w, sT->h};
        if(SDL_RenderDrawRect(win->getRenderer(), &rect))
            cout << "Failed to render mouse selection marking." << endl;
    }
    else if(this->typ == ENTITY)
    {
        TDG_Entity* ent = (TDG_Entity*) this->sel;
        x = ent->getPos()->getPosX() - view->getPosX();
        y = ent->getPos()->getPosY() - view->getPosY();
        SDL_Rect rect = {x, y, ent->getImageWidth(), ent->getImageHight()};
        if(SDL_RenderDrawRect(win->getRenderer(), &rect))
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

                        //check if the row and column are valid. maybe the room changed
                        if(((tile->c >= 0) && (tile->r >= 0)) && ((tile->c < bg->getTileColumns()) && (tile->r < bg->getTileRows())))
                        {
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
                        {
                            free(this->sel);
                            this->sel = NULL;
                            this->typ = noSelection;
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

                            this->selected = false;
                        }
                        if(this->typ == TILE)
                        {
                            //dont place tile on current position
                            SelectedTile* t = (SelectedTile*) this->sel;
                            TDG_Tile* tile = bg->getTile(t->r, t->c);
                            tile->set(t->prev.id, t->prev.rotDegree, t->prev.impassable, t->prev.flip);

                            free(this->sel);
                            this->sel = NULL;
                            this->typ = noSelection;

                            //expensive operation to delete unused images (garbage collection)
                            bg->removeUnusedImages();

                            this->selected = false;
                        }
                    }
                }
                else if(event->button.button == SDL_BUTTON_LEFT)
                {
                    //nothing selected => check if mouse is on a entity or a tile and select it
                    if((this->sel == NULL) && (this->typ == noSelection))
                    {
                        if(!this->selected)
                        {
                            if(!selectEntity(eh, x, y))
                                selectTile(bg, x, y);
                        }
                    }
                    else if((this->sel != NULL) && (this->typ != noSelection))
                    {
                        if(this->selected)
                        {
                            if(this->typ == TILE)
                            {
                                SelectedTile* t = (SelectedTile*) this->sel;
                                if(!t->multipleTimesPlaceable)
                                {
                                    //save the selected tile and reset the mouse selection
                                    free(this->sel);
                                    this->sel = NULL;
                                    this->typ = noSelection;

                                    //expensive operation to delete unused images (garbage collection)
                                    bg->removeUnusedImages();
                                }
                                else
                                {
                                    //save the selected tile on the current position
                                    t->prev.id = t->id;
                                    t->prev.rotDegree = t->rotDegree;
                                    t->prev.impassable = t->impassable;
                                    t->prev.flip = t->flip;
                                }
                            }
                            else if(this->typ == ENTITY)
                            {
                                this->sel = NULL;
                                this->typ = noSelection;
                            }
                        }
                    }
                }
            }
            else if(event->type == SDL_MOUSEBUTTONUP)
            {
                if(event->button.button == SDL_BUTTON_LEFT)
                {
                    if((this->sel != NULL) && (this->typ != noSelection))
                        this->selected = true;
                    else if((this->sel == NULL) && (this->typ == noSelection))
                        this->selected = false;
                }
            }
        }
    }
}

bool TDG_Mouse::selectTile(TDG_Window* win, TDG_View* view, TDG_Background* bg, int tileID)
{
    if(((this->typ == noSelection) && (this->sel == NULL)) || (this->typ == TILE))
    {
        if(!bg->isTileImgStored(tileID))
        {
            TDG_SpriteLoader* sprite = new TDG_SpriteLoader();
            if(!sprite->loadSprite("./data/img/room/", "room"))
            {
                cout << "Unable to load background sprite!" << endl;
                return false;
            }

            if(!bg->addImage(win, sprite, tileID))
                return false;

            delete sprite;
        }

        //get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        int vX = view->getPosX();
        int vY = view->getPosY();

        x = x/2 + vX;
        y = y/2 + vY;

        int tWidth = bg->getTileWidth();
        int tHight = bg->getTileHight();

        //mouse points to tile on background at row and column:
        int mouseRow = y/tHight;
        int mouseColumn = x/tWidth;

        if(((mouseColumn >= 0) && (mouseRow >= 0)) && ((mouseColumn < bg->getTileColumns()) && (mouseRow < bg->getTileRows())))
        {
            SelectedTile* sTile;
            TDG_Tile* t = bg->getTile(mouseRow, mouseColumn);

            if(this->typ == TILE)
            {
                sTile = (SelectedTile*) this->sel;
                t->set(sTile->prev.id, sTile->prev.rotDegree, sTile->prev.impassable, sTile->prev.flip);
            }
            else if(this->typ == noSelection)
                sTile = (SelectedTile*) malloc(sizeof(SelectedTile));

            if(t != NULL)
            {
                //save the previous tile on which the mouse points
                sTile->prev.id = t->getID();
                sTile->prev.rotDegree = t->getRotDegree();
                sTile->prev.impassable = t->isImpassable();
                if(t->flipTile() == SDL_FLIP_HORIZONTAL)
                    sTile->prev.flip = 2;
                else if(t->flipTile() == SDL_FLIP_VERTICAL)
                    sTile->prev.flip = 1;
                else
                    sTile->prev.flip = 0;

                t->set(tileID, 0, false, 0);

                //the tile on which the mouse points, shall be:
                sTile->id = tileID;
                sTile->rotDegree = 0;
                sTile->impassable = false;
                sTile->flip = 0;
                sTile->r = mouseRow;
                sTile->c = mouseColumn;
                sTile->w = tWidth;
                sTile->h = tHight;
                sTile->multipleTimesPlaceable = true;

                this->sel = sTile;
                this->typ = TILE;

                return true;
            }
        }
    }

    return false;
}

bool TDG_Mouse::selectEntity(TDG_Entity* e)
{
    if((this->typ == noSelection) && (this->sel == NULL))
    {
        if(e != NULL)
        {
            this->sel = e;
            this->typ = ENTITY;

            return true;
        }
    }

    return false;
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

            sTile->id = t->getID();
            sTile->rotDegree = t->getRotDegree();
            sTile->impassable = t->isImpassable();
            sTile->r = mouseRow;
            sTile->c = mouseColumn;
            sTile->w = tWidth;
            sTile->h = tHight;
            sTile->multipleTimesPlaceable = false;
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

void TDG_Mouse::deselect()
{
    if(this->typ == TILE)
    {
        free(this->sel);
        this->sel = NULL;
        this->typ = noSelection;

    }
    else if(this->typ == ENTITY)
    {
        this->sel = NULL;
        this->typ = noSelection;
    }
}

bool TDG_Mouse::selectedSomething()
{
    if(this->typ == noSelection)
        return false;

    return true;
}
