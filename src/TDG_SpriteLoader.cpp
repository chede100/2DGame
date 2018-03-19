#include "TDG_SpriteLoader.h"

TDG_SpriteLoader::TDG_SpriteLoader(string path, int imgWidth, int imgHight)
{
    this->path = path;
    this->imgWidth = imgWidth;
    this->imgHight = imgHight;
    this->maxRows = 100;
    this->maxColumns = 100;
    this->sprite = NULL;
}

TDG_SpriteLoader::~TDG_SpriteLoader()
{
    if(this->sprite != NULL)
        SDL_DestroyTexture(this->sprite);
}

SDL_Texture* TDG_SpriteLoader::getSpriteImage(TDG_GUI* gui, int imageNumber)
{
    if(imageNumber <= 0)
    {
        cout << "Couldnt load image " << imageNumber << " from "<< this->path << endl;
        cout << "Image number 0 is not valid (image id must be greater 0)!" << endl;
        return NULL;
    }

    imageNumber--;

    if(this->maxColumns == 0)
    {
        cout << "Max column size of sprite " << this->path << " is 0. Not valid!" << endl;
        return NULL;
    }

    double row = imageNumber/this->maxColumns;
    int imageRow = row;

    int imageColumn = imageNumber%(this->maxColumns-1);

    //load sprite sheet
    SDL_Surface* spriteSheet = IMG_Load(this->path.c_str());
    if(spriteSheet == NULL)
    {
        cout << "Couldnt load sprite sheet " << this->path << "!" << endl;
        return NULL;
    }
    SDL_Rect spriteRect = {imageColumn*this->imgWidth, imageRow*this->imgHight, this->imgWidth, this->imgHight};

    //create new image and copy one image of the sprite sheet into it
    SDL_Surface* image = SDL_CreateRGBSurface(0, this->imgWidth, this->imgHight, 32, 0, 0, 0, 0);
    if(image == NULL)
    {
        cout << "Couldnt load image from sprite sheet " << this->path << "!" << endl;
        return NULL;
    }
    SDL_Rect imageRect = {0, 0, this->imgWidth, this->imgHight};

    if(!SDL_BlitSurface(spriteSheet, &spriteRect, image, &imageRect))
    {
        cout << "Could not copy one image from sprite sheet " << this->path << " into a new image!" << endl;
        return NULL;
    }

    SDL_FreeSurface(spriteSheet);

    SDL_Texture* newImage = SDL_CreateTextureFromSurface(gui->getRenderer(), image);
    if(newImage == NULL)
    {
        cout << "Unable to create Texture from Surface!" << endl;
        return NULL;
    }

    return newImage;
}

void TDG_SpriteLoader::setSpriteMaxRows(int rows)
{
    this->maxRows = rows;
}
void TDG_SpriteLoader::setSpriteMaxColumns(int columns)
{
    this->maxColumns = columns;
}
