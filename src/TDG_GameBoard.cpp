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

bool TDG_GameBoard::init(TDG_GUI* gui, TDG_GameSpecs* specs)
{
    this->entityGraphics = new TDG_StoredEntityAnimations();

    this->entities = new TDG_EntityHandler();

    //create the room and init all entities
    if(!createRoom(gui, specs->getRoom()))
    {
        cout << "Unable to create room." << endl;
        return false;
    }
    //Store player graphics/animations
    if(!this->entityGraphics->isStored(Character, specs->getSPoint()->player.animationID))
        this->entityGraphics->loadAndAdd(gui, Character, specs->getSPoint()->player.animationID);

    //create player
    this->player = new TDG_Character();
    this->player->init(specs->getSPoint()->player, Character, true);
    //bind player animations to the players character
    if(!this->player->assignAnimations(this->entityGraphics))
    {
        cout << "Unable to bind animations to player!" << endl;
        return false;
    }
    //bind cBox to player character
    player->bindCBox();

    //add the player character to the list of all entities
    this->entities->add(this->player);

    //***************************************************************************************

    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = 420;
    int viewHight = 320;
    this->view->init(viewWidth, viewHight);


    //bind the field of view to the players character on the game board
    if(!this->view->bindTo(this->player->getPos(), this->player->getImageWidth()/2, this->player->getImageHight()/2))
    {
        cout << "Unable to bind the field of view to a specific position." << endl;
        return false;
    }

    return true;
}

bool TDG_GameBoard::createRoom(TDG_GUI* gui, Room* room)
{
    this->roomName = room->roomName;
    this->roomID = room->roomID;

    //Create background
    this->backg = new TDG_Background();
    if(!this->backg->create(gui, room))
    {
        cout << "Unable to create background!" << endl;
        return false;
    }

    //collision enabled
    this->entities->enableCollision(this->backg);

    //*************************************************************************************
    //Load and store all necessary entity graphics (animations from Player, NPC, Objects)
    //Store all npc graphics/animations
    list<Entity>::const_iterator it, e;
    for (it = room->npc.begin(), e = room->npc.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(Character, it->animationID))
            this->entityGraphics->loadAndAdd(gui, Character, it->animationID);
    }

    //Store all object graphics/animations
    for (it = room->obj.begin(), e = room->obj.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(Object, it->animationID))
            this->entityGraphics->loadAndAdd(gui, Object, it->animationID);
    }
    //***************************************************************************************
    //create all npc
    for (it = room->npc.begin(), e = room->npc.end(); it != e; it++)
    {
        TDG_Character* chara = new TDG_Character();
        chara->init(*it, Character, false);
        //bind npc animations to the npc
        if(!chara->assignAnimations(this->entityGraphics))
        {
            cout << "Unable to bind animations to npc " << it->name << "!" << endl;
            return false;
        }
        //bind cBox to character
        chara->bindCBox();

        //add the npc to the list of all entities
        this->entities->add(chara);
    }

    //create all room objects
    for (it = room->obj.begin(), e = room->obj.end(); it != e; it++)
    {
        TDG_Object* obj = new TDG_Object();
        obj->init(*it, Object);
        //bind object animations to the object
        if(!obj->assignAnimations(this->entityGraphics))
        {
            cout << "Unable to bind animations to obj " << it->name << "!" << endl;
            return false;
        }
        //bind cBox to object
        obj->bindCBox();

        //add the object to the list of all entities
        this->entities->add(obj);
    }

    return true;
}

void TDG_GameBoard::userInput(Direction dir)
{
    this->player->changeMovementStatus(dir);
}

bool TDG_GameBoard::throughGate(Gate* enterGate)
{
    //get the tiles position (row, column) on which the center, of the player, stands
    int playerRow = (this->player->getCBox()->getPosY() + this->player->getCBox()->getHight()/2)/this->backg->getTileHight();
    int playerColumn = (this->player->getCBox()->getPosX() + this->player->getCBox()->getWidth()/2)/this->backg->getTileWidth();
    if(this->backg->isGate(playerRow, playerColumn))
    {
        this->backg->getGate(enterGate, playerRow, playerColumn);
        return true;
    }

    return false;
}

bool TDG_GameBoard::changeRoom(TDG_GUI* gui, Room* newRoom, Gate* enterGate)
{
    this->entityGraphics->removeAllExcept(Character, this->player->getAnimationID());
    this->entities->removeAllExcept(this->player);

    delete this->backg;

    if(!createRoom(gui, newRoom))
    {
        cout << "Unable to change room. New room cant be created!" << endl;
        return false;
    }

    //search arrival gate
    for(list<Gate>::const_iterator it = newRoom->gates.begin(), end = newRoom->gates.end(); it != end; it++)
    {
        if(it->id == enterGate->destinationGateID)
        {
            int posXGate = it->column*this->backg->getTileHight();
            int posYGate = it->row*this->backg->getTileWidth();
            int tileWidth = this->backg->getTileWidth();
            int tileHight = this->backg->getTileHight();
            //place player above the arrival gate
            if(it->arriveStatus == s_north)
                this->player->adjust(posXGate + tileWidth/2 - this->player->getImageWidth()/2, posYGate - this->player->getImageHight() - 1, s_north);
            //place the player right next to the arrival gate
            else if(it->arriveStatus == s_east)
                this->player->adjust(posXGate + tileWidth + 1, posYGate + tileHight/2 - this->player->getImageHight()/2 , s_east);
            //place the player below the arrival gate
            else if(it->arriveStatus == s_south)
                this->player->adjust(posXGate + tileWidth/2 - this->player->getImageWidth()/2, posYGate + tileHight + 1, s_south);
            //place the player left next to the arrival gate
            else if(it->arriveStatus == s_west)
                this->player->adjust(posXGate - this->player->getImageWidth() - 1, posYGate + tileHight/2 - this->player->getImageHight()/2, s_west);
        }
    }

    return true;
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
