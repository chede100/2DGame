#include "TDG_EditorBoard.h"

TDG_EditorBoard::TDG_EditorBoard()
{
    this->backg = NULL;
    this->gui = NULL;
    this->entityGraphics = NULL;
    this->entities = NULL;
    this->guiPos = NULL;
    this->mouse = NULL;

    this->roomID = 0;
}

TDG_EditorBoard::~TDG_EditorBoard()
{
    if(this->backg != NULL)
        delete this->backg;
    if(this->gui != NULL)
        delete this->gui;
    if(this->entityGraphics != NULL)
        delete this->entityGraphics;
    if(this->entities != NULL)
        delete this->entities;
    if(this->guiPos != NULL)
        delete this->guiPos;
    if(this->mouse != NULL)
        delete this->mouse;
}

bool TDG_EditorBoard::init(TDG_Window* win)
{
    this->mouse = new TDG_Mouse();
    this->gui = new TDG_GUI();

    //bind view by default to the left upper corner
    this->guiPos = new TDG_Position(0, 0);

    if(!this->gui->init(win, guiPos))
    {
        cout << "Unable to initialize graphical user interface!" << endl;
        return false;
    }

    return true;
}

bool TDG_EditorBoard::render(TDG_Window* win)
{
    SDL_RenderClear(win->getRenderer());

    if(this->backg != NULL)
    {
        if(!renderBackground(win))
            return false;
    }

    if(this->entities != NULL)
    {
        this->entities->render(win, this->gui->getView());
    }

    if(this->mouse != NULL)
    {
        this->mouse->renderSelRect(win, this->gui->getView());
    }

    SDL_RenderPresent(win->getRenderer());

    return true;
}

bool TDG_EditorBoard::renderBackground(TDG_Window* win)
{
    //position of field of view on the game board
    int xPosView = this->gui->getView()->getPosX();
    int yPosView = this->gui->getView()->getPosY();

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

bool TDG_EditorBoard::createRoom(TDG_Window* win, string& name, int id, int rows, int columns)
{
    if(this->entityGraphics != NULL)
        delete this->entityGraphics;
    if(this->entities != NULL)
        delete this->entities;
    if(this->backg != NULL)
        delete this->backg;

    this->entityGraphics = new TDG_StoredEntityAnimations();
    this->entities = new TDG_EntityHandler();

    this->roomName = name;
    this->roomID = id;

    this->backg = new TDG_Background();
    if(!this->backg->createEmpty(win, rows, columns))
    {
        cout << "Unable to create background!" << endl;
        return false;
    }

    //collision enabled
    this->entities->enableCollision(this->backg);

    //new field of view collision with background
    int bgWidth = this->backg->getTileWidth()*this->backg->getTileColumns();
    int bgHight = this->backg->getTileHight()*this->backg->getTileRows();
    this->gui->getView()->updateMovementInterval(bgWidth, bgHight);

    double bgCenterX = bgWidth/2;
    double bgCenterY = bgHight/2;
    this->gui->getPos()->setPosX(bgCenterX);
    this->gui->getPos()->setPosY(bgCenterY);

    return true;
}

bool TDG_EditorBoard::loadRoom(TDG_Window* win, Room* room)
{
    if(this->entityGraphics != NULL)
        delete this->entityGraphics;
    if(this->entities != NULL)
        delete this->entities;
    if(this->backg != NULL)
        delete this->backg;

    this->entities = new TDG_EntityHandler();
    this->entityGraphics = new TDG_StoredEntityAnimations();

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

    //new field of view collision with background
    int bgWidth = this->backg->getTileWidth()*this->backg->getTileColumns();
    int bgHight = this->backg->getTileHight()*this->backg->getTileRows();
    this->gui->getView()->updateMovementInterval(bgWidth, bgHight);

    double bgCenterX = bgWidth/2;
    double bgCenterY = bgHight/2;
    this->gui->getPos()->setPosX(bgCenterX);
    this->gui->getPos()->setPosY(bgCenterY);

    return true;
}

bool TDG_EditorBoard::saveRoom()
{
    string path = "./data/spec/room/";
    ostringstream ss;
    ss << this->roomID;
    path += ss.str() + "/";

    //check if directory exists
    bool dirExists = false;
    DWORD ftyp = GetFileAttributesA(path.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        dirExists = false;

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        dirExists = true;

    if(CreateDirectory(path.c_str(), NULL));
    else
    {
        if(!dirExists)
        {
            cout << "Invalid path. Unable to save room" << endl;
            return false;
        }
    }

    //Create npc file
    string npc = path + ss.str() + ".npcs";
    ofstream onpc;
    onpc.open(npc.c_str(), ios::out);
    this->entities->print(Character, &onpc);
    onpc.close();

    //create object file
    string obj = path + ss.str() + ".objects";
    ofstream oobj;
    oobj.open(obj.c_str(), ios::out);
    this->entities->print(Object, &oobj);
    oobj.close();

    //create room file
    string room = path + ss.str() + ".room";
    ofstream oroom;
    oroom.open(room.c_str(), ios::out);
    oroom << "name: " << this->roomName << endl;
    this->backg->print(&oroom);
    oroom.close();

    return true;
}

void TDG_EditorBoard::addTile(int id)
{
    if(this->mouse->selectedSomething())
        cout << "Unselect the current tile to add a new one!" << endl;
    else
    {

    }
}

void TDG_EditorBoard::addEntity(EntityTyp typ, int id)
{

}

void TDG_EditorBoard::startTimer()
{
    if(this->entities != NULL)
        this->entities->startAnimations();
}

void TDG_EditorBoard::stopTimer()
{
    if(this->entities != NULL)
        this->entities->stopAnimations();
}

void TDG_EditorBoard::handleInput(TDG_EventHandler* event)
{
    this->mouse->handleEvent(event->getEvent(), this->entities, this->backg, this->gui->getView());
}

bool TDG_EditorBoard::roomStored()
{
    if(this->roomID != 0)
        return true;
    return false;
}
