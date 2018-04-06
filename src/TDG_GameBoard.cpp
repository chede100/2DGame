#include "TDG_GameBoard.h"

TDG_GameBoard::TDG_GameBoard()
{
    this->backg = NULL;
    this->view = NULL;
    this->entityGraphics = NULL;
}

TDG_GameBoard::~TDG_GameBoard()
{
    if(this->backg != NULL)
        delete this->backg;
    if(this->view != NULL)
        delete this->view;
    if(this->entityGraphics != NULL)
        delete this->entityGraphics;
}

bool TDG_GameBoard::create(TDG_GUI* gui, TDG_GameSpecs* specs)
{
    this->roomName = specs->getRoom()->roomName;

    //*************************************************************************************
    //Load and store all necessary entity graphics (animations from Player, NPC, Objects)
    this->entityGraphics = new TDG_StoredEntityAnimations();

    //Store all npc graphics/animations
    list<Entity>::const_iterator it, e;
    for (it = specs->getRoom()->npc.begin(), e = specs->getRoom()->npc.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(NPC, it->animationID))
            this->entityGraphics->loadAndAdd(gui, NPC, it->animationID);
    }

    //Store all object graphics/animations
    for (it = specs->getRoom()->obj.begin(), e = specs->getRoom()->obj.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(Object, it->animationID))
            this->entityGraphics->loadAndAdd(gui, Object, it->animationID);
    }

    //Store player graphics/animations
    if(!this->entityGraphics->isStored(Player, specs->getRoom()->player.animationID))
        this->entityGraphics->loadAndAdd(gui, Player, specs->getRoom()->player.animationID);
    //*************************************************************************************

    //Create background
    this->backg = new TDG_Background();
    if(!this->backg->create(gui, specs->getRoom()))
    {
        cout << "Unable to create background!" << endl;
        return false;
    }

    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = specs->getOpt()->winWidth;
    int viewHight = specs->getOpt()->winHight;
    this->view->init(viewWidth, viewHight);

    // Test position!!!!!!!!!!!!!!!
    TDG_Position* newPos = new TDG_Position(320, 240);

    //bind the field of view to a specific position on the game board
    if(!this->view->bindTo(newPos))
    {
        cout << "Unable to bind the field of view to a specific position." << endl;
        return false;
    }

    return true;
}

bool TDG_GameBoard::render(TDG_GUI* gui, int scaleFaktor)
{
    SDL_RenderClear(gui->getRenderer());

    if(!renderBackground(gui, scaleFaktor))
        return false;

    SDL_RenderPresent(gui->getRenderer());

    return true;
}

bool TDG_GameBoard::renderBackground(TDG_GUI* gui, int scaleFaktor)
{
    //position of field of view on the game board
    int xPosView = this->view->getPosX();
    int yPosView = this->view->getPosY();

    //render the background at this position, so the field of view keeps its relative path to the game board.
    int backgRenderPosX = xPosView*(-1);
    int backgRenderPosY = yPosView*(-1);

    if(!this->backg->renderAtPos(gui, backgRenderPosX, backgRenderPosY, scaleFaktor))
    {
        cout << "Unable to render background at Position X: " << backgRenderPosX << " Y: " << backgRenderPosY << "!" << endl;
        return false;
    }

    return true;
}
