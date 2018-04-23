#include "TDG_GameBoard.h"

TDG_GameBoard::TDG_GameBoard()
{
    this->backg = NULL;
    this->view = NULL;
    this->entityGraphics = NULL;
    this->entities = NULL;
}

TDG_GameBoard::~TDG_GameBoard()
{
    if(this->backg != NULL)
        delete this->backg;
    if(this->view != NULL)
        delete this->view;
    if(this->entityGraphics != NULL)
        delete this->entityGraphics;
    if(this->entities != NULL)
        delete this->entities;
}

bool TDG_GameBoard::create(TDG_GUI* gui, TDG_GameSpecs* specs)
{
    this->roomName = specs->getRoom()->roomName;

    //Create background
    this->backg = new TDG_Background();
    if(!this->backg->create(gui, specs->getRoom()))
    {
        cout << "Unable to create background!" << endl;
        return false;
    }

    //*************************************************************************************
    //Load and store all necessary entity graphics (animations from Player, NPC, Objects)
    this->entityGraphics = new TDG_StoredEntityAnimations();

    //Store all npc graphics/animations
    list<Entity>::const_iterator it, e;
    for (it = specs->getRoom()->npc.begin(), e = specs->getRoom()->npc.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(Character, it->animationID))
            this->entityGraphics->loadAndAdd(gui, Character, it->animationID);
    }

    //Store all object graphics/animations
    for (it = specs->getRoom()->obj.begin(), e = specs->getRoom()->obj.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(Object, it->animationID))
            this->entityGraphics->loadAndAdd(gui, Object, it->animationID);
    }

    //Store player graphics/animations
    if(!this->entityGraphics->isStored(Character, specs->getRoom()->player.animationID))
        this->entityGraphics->loadAndAdd(gui, Character, specs->getRoom()->player.animationID);

    //***************************************************************************************
    //Create EntityHandler and initialize all entities
    this->entities = new TDG_EntityHandler();
    this->entities->enableCollision(this->backg);

    //create all npc
    for (it = specs->getRoom()->npc.begin(), e = specs->getRoom()->npc.end(); it != e; it++)
    {
        TDG_Character* chara = new TDG_Character();
        chara->init(*it, Character, false);
        //bind npc animations to the npc
        if(!chara->assignAnimations(this->entityGraphics))
        {
            cout << "Unable to bind animations to npc " << it->name << "!" << endl;
            return false;
        }

        //add the npc to the list of all entities
        this->entities->add(chara);
    }

    //create all room objects
    for (it = specs->getRoom()->obj.begin(), e = specs->getRoom()->obj.end(); it != e; it++)
    {
        TDG_Object* obj = new TDG_Object();
        obj->init(*it, Object);
        //bind object animations to the object
        if(!obj->assignAnimations(this->entityGraphics))
        {
            cout << "Unable to bind animations to obj " << it->name << "!" << endl;
            return false;
        }

        //add the object to the list of all entities
        this->entities->add(obj);
    }

    //create player
    this->player = new TDG_Character();
    this->player->init(specs->getRoom()->player, Character, true);
    //bind player animations to the players character
    if(!this->player->assignAnimations(this->entityGraphics))
    {
        cout << "Unable to bind animations to player!" << endl;
        return false;
    }

    //add the player character to the list of all entities
    this->entities->add(this->player);

    //***************************************************************************************

    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = 320;
    int viewHight = 320;
    this->view->init(viewWidth, viewHight);


    //bind the field of view to the players character on the game board
    if(!this->view->bindTo(this->player->getPos()))
    {
        cout << "Unable to bind the field of view to a specific position." << endl;
        return false;
    }

    return true;
}

void TDG_GameBoard::userInput(Direction dir)
{
    this->player->changeMovementStatus(dir);
}

void TDG_GameBoard::startTimer()
{
    this->entities->startAnimations();
    this->entities->startMotion();
}

void TDG_GameBoard::stopTimer()
{
    this->entities->stopAnimations();
    this->entities->stopMotion();
}

bool TDG_GameBoard::render(TDG_GUI* gui)
{
    SDL_RenderClear(gui->getRenderer());

    if(!renderBackground(gui))
        return false;

    this->entities->render(gui, this->view);

    SDL_RenderPresent(gui->getRenderer());

    return true;
}

bool TDG_GameBoard::renderBackground(TDG_GUI* gui)
{
    //position of field of view on the game board
    int xPosView = this->view->getPosX();
    int yPosView = this->view->getPosY();

    //render the background at this position, so the field of view keeps its relative path to the game board.
    int backgRenderPosX = xPosView*(-1);
    int backgRenderPosY = yPosView*(-1);

    if(!this->backg->renderAtPos(gui, backgRenderPosX, backgRenderPosY))
    {
        cout << "Unable to render background at Position X: " << backgRenderPosX << " Y: " << backgRenderPosY << "!" << endl;
        return false;
    }

    return true;
}
