#include "TDG_EditorGUI.h"

TDG_EditorGUI::TDG_EditorGUI()
{
    this->pos = NULL;
    this->view = NULL;
    this->tp = NULL;
}

TDG_EditorGUI::~TDG_EditorGUI()
{
    if(this->view != NULL)
        delete this->view;
    if(this->tp != NULL)
        delete this->tp;
}

bool TDG_EditorGUI::init(TDG_Window* win, TDG_Position* pos)
{
    this->view = new TDG_View();

    // set the field of view to the size of the window
    int viewWidth = win->getLogicalWidth();
    int viewHight = win->getLogicalHight();
    this->view->init(viewWidth, viewHight, 0, 0);

    this->pos = pos;

    //bind the field of view to the players character on the game board
    if(!this->view->bindTo(this->pos, 0, 0))
    {
        cout << "Unable to bind field of view!" << endl;
        return false;
    }

    return true;
}

bool TDG_EditorGUI::create()
{
    if(this->tp != NULL)
        delete this->tp;

    this->tp = new TDG_TPalette();
    this->tp->create(10, 16, 16, 2);
    this->tp->setPosition(hCenter, Bottom, LeftRight);

    list<SDL_Event> keys;
    this->addKey(keys, 771, SDL_SCANCODE_1);
    this->addKey(keys, 771, SDL_SCANCODE_2);
    this->addKey(keys, 771, SDL_SCANCODE_3);
    this->addKey(keys, 771, SDL_SCANCODE_4);
    this->addKey(keys, 771, SDL_SCANCODE_5);
    this->addKey(keys, 771, SDL_SCANCODE_6);
    this->addKey(keys, 771, SDL_SCANCODE_7);
    this->addKey(keys, 771, SDL_SCANCODE_8);
    this->addKey(keys, 771, SDL_SCANCODE_9);
    this->addKey(keys, 771, SDL_SCANCODE_0);

    tp->bindToKeys(keys);

    return true;
}

void TDG_EditorGUI::render(TDG_Window* win)
{
    this->tp->render(win, this->view);
}

void TDG_EditorGUI::addTileToPalette(TDG_Window* win, int tileID, unsigned int selectorNumber)
{
    this->tp->store(win, tileID, selectorNumber);
}

void TDG_EditorGUI::handleKeyboardInput(SDL_Event* event, TDG_Window* win, TDG_Mouse* mouse, TDG_Background* bg)
{
    this->tp->handleInupt(event, win, this->view, mouse, bg);
}

void TDG_EditorGUI::addKey(list<SDL_Event>& keys, const Uint32 eventTyp, SDL_Scancode key)
{
    SDL_Event e;
    e.type = eventTyp;
    e.key.keysym.scancode = key;
    keys.push_back(e);
}

TDG_View* TDG_EditorGUI::getView()
{
    return this->view;
}

TDG_Position* TDG_EditorGUI::getPos()
{
    return this->pos;
}
