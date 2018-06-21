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

bool TDG_GameBoard::init(TDG_Window* win, TDG_FileHandler* specs)
{
    this->entityGraphics = new TDG_StoredEntityAnimations();

    this->entities = new TDG_EntityHandler();

    //create the room and init all entities
    if(!createRoom(win, specs->getRoom()))
    {
        cout << "Unable to create room." << endl;
        return false;
    }

    if(!createPlayer(win, specs->getSPoint()))
    {
        cout << "Unable to create player character!" << endl;
        return false;
    }

    //***************************************************************************************

    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = win->getLogicalWidth();
    int viewHight = win->getLogicalHight();
    int bgWidth = this->backg->getTileWidth()*this->backg->getTileColumns();
    int bgHight = this->backg->getTileHight()*this->backg->getTileRows();
    this->view->init(viewWidth, viewHight, bgWidth, bgHight);


    //bind the field of view to the players character on the game board
    if(!this->view->bindTo(this->player->getPos(), this->player->getImageWidth()/2, this->player->getImageHight()/2))
    {
        cout << "Unable to bind the field of view to a specific position." << endl;
        return false;
    }

    return true;
}

bool TDG_GameBoard::createRoom(TDG_Window* win, Room* room)
{
    this->roomName = room->roomName;
    this->roomID = room->roomID;

    //Create background
    this->backg = new TDG_Background();
    if(!this->backg->create(win, room))
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
            this->entityGraphics->loadAndAdd(win, Character, it->animationID);
    }

    //Store all object graphics/animations
    for (it = room->obj.begin(), e = room->obj.end(); it != e; it++)
    {
        if(!this->entityGraphics->isStored(Object, it->animationID))
            this->entityGraphics->loadAndAdd(win, Object, it->animationID);
    }
    //***************************************************************************************
    //create all npc
    for (it = room->npc.begin(), e = room->npc.end(); it != e; it++)
    {
        TDG_Character* chara = new TDG_Character();
        chara->init(&(*it), Character, this->entityGraphics, false);

        chara->createInventory(win, &(*it));

        //add the npc to the list of all entities
        this->entities->add(chara);
    }

    //create all room objects
    for (it = room->obj.begin(), e = room->obj.end(); it != e; it++)
    {
        TDG_Object* obj = new TDG_Object();
        obj->init(&(*it), Object, this->entityGraphics);

        obj->createInventory(win, &(*it));

        //add the object to the list of all entities
        this->entities->add(obj);
    }

    return true;
}

bool TDG_GameBoard::createPlayer(TDG_Window* win, SavePoint* sp)
{
    //Store player graphics/animations
    if(!this->entityGraphics->isStored(Character, sp->player.animationID))
        this->entityGraphics->loadAndAdd(win, Character, sp->player.animationID);

    //create player
    this->player = new TDG_Player();
    this->player->init(&sp->player, Character, this->entityGraphics, true);

    player->createInventory(win, &sp->player);

    //add the player character to the list of all entities
    this->entities->add(this->player);

    return true;
}

void TDG_GameBoard::userInput(TDG_EventHandler* event)
{
    this->player->handlePlayerEvents(event);
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

bool TDG_GameBoard::changeRoom(TDG_Window* win, Gate* enterGate)
{
    TDG_FileHandler* newRoom = new TDG_FileHandler();
    newRoom->loadRoom(enterGate->destinationRoomID);

    this->entityGraphics->removeAllExcept(Character, this->player->getAnimationID());
    this->entities->removeAllExcept(this->player);

    delete this->backg;
    this->backg = NULL;

    if(!createRoom(win, newRoom->getRoom()))
    {
        cout << "Unable to change room. New room cant be created!" << endl;
        return false;
    }

    //new field of view collision with background
    int bgWidth = this->backg->getTileWidth()*this->backg->getTileColumns();
    int bgHight = this->backg->getTileHight()*this->backg->getTileRows();
    this->view->updateMovementInterval(bgWidth, bgHight);

    //search arrival gate
    for(list<Gate>::const_iterator it = newRoom->getRoom()->gates.begin(), end = newRoom->getRoom()->gates.end(); it != end; it++)
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

    delete newRoom;

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

bool TDG_GameBoard::render(TDG_Window* win)
{
    SDL_RenderClear(win->getRenderer());

    if(!renderBackground(win))
        return false;

    this->entities->render(win, this->view);

    SDL_RenderPresent(win->getRenderer());

    return true;
}

bool TDG_GameBoard::renderBackground(TDG_Window* win)
{
    //position of field of view on the game board
    int xPosView = this->view->getPosX();
    int yPosView = this->view->getPosY();

    //render the background at this position, so the field of view keeps its relative path to the game board.
    int backgRenderPosX = xPosView*(-1);
    int backgRenderPosY = yPosView*(-1);

    if(!this->backg->renderAtPos(win, backgRenderPosX, backgRenderPosY))
    {
        cout << "Unable to render background at Position X: " << backgRenderPosX << " Y: " << backgRenderPosY << "!" << endl;
        return false;
    }

    return true;
}

void TDG_GameBoard::save()
{
    ofstream savePoint;
    savePoint.open("./game.savepoint");

    savePoint << "roomID: " << this->roomID << endl;

    string startAnimation;
    if(this->player->getMovementStatus() == s_north) startAnimation = "s_north";
    else if(this->player->getMovementStatus() == s_north_east) startAnimation = "s_north_east";
    else if(this->player->getMovementStatus() == s_east) startAnimation = "s_east";
    else if(this->player->getMovementStatus() == s_south_east) startAnimation = "s_south_east";
    else if(this->player->getMovementStatus() == s_south) startAnimation = "s_south";
    else if(this->player->getMovementStatus() == s_south_west) startAnimation = "s_south_west";
    else if(this->player->getMovementStatus() == s_west) startAnimation = "s_west";
    else if(this->player->getMovementStatus() == s_north_west) startAnimation = "s_north_west";
    else if(this->player->getMovementStatus() == m_north) startAnimation = "s_north";
    else if(this->player->getMovementStatus() == m_north_east) startAnimation = "s_north_east";
    else if(this->player->getMovementStatus() == m_east) startAnimation = "s_east";
    else if(this->player->getMovementStatus() == m_south_east) startAnimation = "s_south_east";
    else if(this->player->getMovementStatus() == m_south) startAnimation = "s_south";
    else if(this->player->getMovementStatus() == m_south_west) startAnimation = "s_south_west";
    else if(this->player->getMovementStatus() == m_west) startAnimation = "s_west";
    else if(this->player->getMovementStatus() == m_north_west) startAnimation = "s_north_west";
    else startAnimation = "s_south";

    savePoint << "player: " << this->player->getID() << " " << this->player->getPos()->getPosX() << " " << this->player->getPos()->getPosY() << " " << startAnimation << endl;

    savePoint << "inventorySize: " << this->player->getInventory()->getSize() << endl;
    savePoint << "<item_list>" << endl;

    int invSize = this->player->getInventory()->getSize();
    for(int i = 0; i < invSize; i++)
    {
        TDG_Item* item = this->player->getInventory()->getItem(i);
        if(item != NULL)
        {
            int id = item->getID();
            int amount = item->getAmount();
            int pos = item->getPosition();

            savePoint << id << " " << amount << " " << pos << " ";
        }

        if(i%17 == 16)
            savePoint << endl;
    }
    savePoint << "<item_list_end>" << endl;
}
