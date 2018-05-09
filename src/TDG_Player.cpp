#include "TDG_Player.h"

TDG_Player::TDG_Player() : TDG_Character()
{
    //ctor
}

TDG_Player::~TDG_Player()
{
    //dtor
}

void TDG_Player::init(Entity entity, EntityTyp typ, bool moveable)
{
    this->TDG_Character::init(entity, typ, moveable);
}

void TDG_Player::handlePlayerEvents(TDG_EventHandler* event)
{
    int keysPressed = 0;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_UP])
    {
        up = true;
        keysPressed++;
    }
    if(state[SDL_SCANCODE_DOWN])
    {
        down = true;
        keysPressed++;
    }
    if(state[SDL_SCANCODE_RIGHT])
    {
        right =  true;
        keysPressed++;
    }
    if(state[SDL_SCANCODE_LEFT])
    {
        left = true;
        keysPressed++;
    }

    //cout << "Up: " << up << " Down: " << down << " Right: " << right << " Left: " << left << endl;

    if(keysPressed == 0)
    {
        //no input for player movement
        if(this->getMovementStatus() == m_north)
            this->setMovementStatus(s_north);
        else if(this->getMovementStatus() == m_east)
            this->setMovementStatus(s_east);
        else if(this->getMovementStatus() == m_south)
            this->setMovementStatus(s_south);
        else if(this->getMovementStatus() == m_west)
            this->setMovementStatus(s_west);
        else if(this->getMovementStatus() == m_north_east)
            this->setMovementStatus(s_north_east);
        else if(this->getMovementStatus() == m_south_east)
            this->setMovementStatus(s_south_east);
        else if(this->getMovementStatus() == m_south_west)
            this->setMovementStatus(s_south_west);
        else if(this->getMovementStatus() == m_north_west)
            this->setMovementStatus(s_north_west);
    }
    else if(keysPressed == 1)
    {
        if(up)
            this->setMovementStatus(m_north);
        else if(down)
            this->setMovementStatus(m_south);
        else if(right)
            this->setMovementStatus(m_east);
        else if(left)
            this->setMovementStatus(m_west);
    }
    else if(keysPressed == 2)
    {
        if(up && down)
            this->setMovementStatus(s_south);
        else if(left && right)
            this->setMovementStatus(s_south);
        else if(up && left)
            this->setMovementStatus(m_north_west);
        else if(up && right)
            this->setMovementStatus(m_north_east);
        else if(down && left)
            this->setMovementStatus(m_south_west);
        else if(down && right)
            this->setMovementStatus(m_south_east);
    }
    else
    {
        this->setMovementStatus(s_south);
    }
}
