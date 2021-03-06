#include "TDG_CollisionBox.h"

TDG_CollisionBox::TDG_CollisionBox()
{
    this->pos = NULL;
    this->width = 0;
    this->hight = 0;
    this->xCorrection = 0;
    this->yCorrection = 0;
}

TDG_CollisionBox::~TDG_CollisionBox()
{
    //dtor
}

void TDG_CollisionBox::bindToPosition(TDG_Position* pos, int xCorrection, int yCorrection)
{
    this->pos = pos;
    this->xCorrection = xCorrection;
    this->yCorrection = yCorrection;
}

bool TDG_CollisionBox::collisionWith(TDG_CollisionBox* box, int speedX, int speedY)
{
    if((box == NULL) || (this->pos == NULL))
    {
        cout << "Error: Try to check collision with a Nullpointer." << endl;
        return false;
    }

    int posXBox1 = this->getPosX() + speedX;
    int posYBox1 = this->getPosY() + speedY;
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

bool TDG_CollisionBox::collisionWith(TDG_Background* background, int speedX, int speedY)
{
    //top left corner position
    int tlX = this->getPosX() + speedX;
    int tlY = this->getPosY() + speedY;

    //bottom right corner position
    int brX = this->getPosX() + this->width + speedX;
    int brY = this->getPosY() + this->hight + speedY;

    //tile width and hight
    int tWidth = background->getTileWidth();
    int tHight = background->getTileHight();

    //tiles per row and column
    int tRows = background->getTileRows();
    int tColumns = background->getTileColumns();

    //check tiles for collision on which the collision box stands
    int row = tlY/tHight;
    while((row < tRows) && (row*tHight < brY))
    {
        int column = tlX/tWidth;
        while((column < tColumns) && (column*tWidth < brX))
        {
            if(background->isTileImpassable(row, column))
                return true;

            column++;
        }

        row++;
    }

    return false;
}

void TDG_CollisionBox::setSize(int width, int hight)
{
    this->width = width;
    this->hight = hight;
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
