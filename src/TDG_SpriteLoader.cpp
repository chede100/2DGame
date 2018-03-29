#include "TDG_SpriteLoader.h"

TDG_SpriteLoader::TDG_SpriteLoader()
{
    this->spriteRows = 0;
    this->spriteColumns = 0;
    this->imgWidth = 0;
    this->imgHight = 0;
    this->sprite = NULL;
}

TDG_SpriteLoader::~TDG_SpriteLoader()
{
    if(this->sprite != NULL)
        SDL_FreeSurface(this->sprite);
}

bool TDG_SpriteLoader::loadSprite(string path, int imgWidth, int imgHight, int spriteRows, int spriteColumns)
{
    this->path = path;
    this->imgWidth = imgWidth;
    this->imgHight = imgHight;
    this->spriteRows = spriteRows;
    this->spriteColumns = spriteColumns;

    if((this->spriteRows <= 0) || (this->spriteColumns <= 0))
    {
        cout << "Number of columns or rows from sprite " << this->path << " is <= 0. Not valid!" << endl;
        return false;
    }

    //load sprite sheet
    this->sprite = IMG_Load(path.c_str());
    if(this->sprite == NULL)
    {
        cout << "Couldnt load sprite sheet " << path << "!" << endl;
        return false;
    }

    return true;
}

SDL_Texture* TDG_SpriteLoader::getImage(TDG_GUI* gui, int row, int column)
{
    if((row <= 0) || (column <= 0))
    {
        cout << "Couldnt load image at position row: " << row << " column: " << column << " from "<< this->path << endl;
        cout << "Position is not valid (sprite sheet starts with row and column 1))!" << endl;
        return NULL;
    }
    if((this->spriteRows < row) || (this->spriteColumns < column))
    {
        cout << "Couldnt load image at position row: " << row << " column: " << column << " from "<< this->path << endl;
        cout << "Position of image is outside of the sprite sheets size!" << endl;
        return NULL;
    }

    row--;
    column--;

    //create new image
    SDL_Surface* image = SDL_CreateRGBSurface(0, this->imgWidth, this->imgHight, 32, 0, 0, 0, 0);
    if(image == NULL)
    {
        cout << "Couldnt load image from sprite sheet " << this->path << "!" << endl;
        return NULL;
    }
    SDL_Rect imageRect = {0, 0, this->imgWidth, this->imgHight};

    //copy one image from sprite sheet into the new image
    SDL_Rect spriteRect = {column*this->imgWidth, row*this->imgHight, this->imgWidth, this->imgHight};
    if(SDL_BlitSurface(this->sprite, &spriteRect, image, &imageRect) != 0)
    {
        cout << "Could not copy one image from sprite sheet " << this->path << " into a new image!" << endl;
        return NULL;
    }

    //create Texture based on the new image
    SDL_Texture* newImage = SDL_CreateTextureFromSurface(gui->getRenderer(), image);
    if(newImage == NULL)
    {
        cout << "Unable to create Texture from Surface!" << endl;
        return NULL;
    }

    return newImage;
}

TDG_Animation* TDG_SpriteLoader::getAnimation(TDG_GUI* gui, int row, int numberOfImages)
{
    return NULL;
}
