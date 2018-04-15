#include "TDG_CollisionBox.h"

TDG_CollisionBox::TDG_CollisionBox()
{
    //ctor
}

TDG_CollisionBox::~TDG_CollisionBox()
{
    //dtor
}

bool TDG_CollisionBox::collisionWith(TDG_CollisionBox* box)
{
    if((box == NULL) || (this->pos == NULL))
    {
        cout << "Error: Try to check collision with a Nullpointer." << endl;
        return false;
    }

    int posXBox1 = this->getPosX();
    int posYBox1 = this->getPosY();
    int widthBox1 = this->width;
    int hightBox1 =  this->hight;

    int posXBox2 = box->getPosX();
    int posYBox2 = box->getPosY();
    int widthBox2 = box->getWidth();
    int hightBox2 = box->getHight();

    //collision detected
    if(!(posXBox1 + widthBox1 < posXBox2) &&
       !(posXBox1 > posXBox2 + widthBox2) &&
       !(posYBox1 > posYBox2 + hightBox2) &&
       !(posYBox1 + hightBox1 < posYBox2))
        return true;

    return false;
}

bool TDG_CollisionBox::collisionWithBackground(TDG_Background* background)
{
    return false;
}

int TDG_CollisionBox::getWidth()
{
    return this->width;
}

int TDG_CollisionBox::getHight()
{
    return this->hight;
}

int TDG_CollisionBox::getPosX()
{
    return this->pos->getPosX() + this->xCorrection;
}

int TDG_CollisionBox::getPosY()
{
    return this->pos->getPosY() + this->yCorrection;
}
