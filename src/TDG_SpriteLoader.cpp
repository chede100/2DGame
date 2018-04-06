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

    this->imgPerRow.clear();
}

bool TDG_SpriteLoader::loadSprite(string pathToFolder, string spriteName)
{
    this->path = pathToFolder + spriteName + ".png";

    string iPath = pathToFolder + spriteName + ".info";
    if(!loadSpriteInfo(iPath))
    {
        cout << "Unable to load information file for room sprite sheet!" << endl;
        return false;
    }

    int maxRows = this->imgPerRow.size();
    if(maxRows != this->spriteRows)
    {
        cout << maxRows << " " << this->spriteRows << endl;
        cout << "Incorrectly created list with number of sprite images at every row. Sprite: " << this->path << endl;
        return false;
    }

    if((this->spriteRows <= 0) || (this->spriteColumns <= 0))
    {
        cout << "Number of columns or rows from sprite " << this->path << " is <= 0. Not valid!" << endl;
        return false;
    }

    if((this->imgWidth <= 0) || (this->imgHight <= 0))
    {
        cout << "Size of images from sprite sheet " << this->path << " is <= 0. Not valid!" << endl;
        return false;
    }

    //load sprite sheet
    this->sprite = IMG_Load(this->path.c_str());
    if(this->sprite == NULL)
    {
        cout << "Couldnt load sprite sheet " << this->path << "!" << endl;
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

    //create new image
    SDL_Surface* image = SDL_CreateRGBSurface(0, this->imgWidth, this->imgHight, 32, 0, 0, 0, 0);
    if(image == NULL)
    {
        cout << "Couldnt load image from sprite sheet " << this->path << "!" << endl;
        return NULL;
    }
    SDL_Rect imageRect = {0, 0, this->imgWidth, this->imgHight};

    //copy one image from sprite sheet into the new image
    row--;
    column--;
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

TDG_Animation* TDG_SpriteLoader::getAnimation(TDG_GUI* gui, int row, AnimationTyp typ)
{
    int imagesAtRow;

    if(!this->imgPerRow.empty())
    {
        if((row <= this->spriteRows))
        {
            if(row <= 0)
            {
                cout << "Spriteloader could not load animation, because the given position in the sprite sheet is invalid (<= 0). Sprite: " << this->path << " Row: " << row << endl;
                return NULL;
            }

            list<int>::const_iterator it = this->imgPerRow.begin();

            //get the iterator in list that equals the number of row
            advance(it, row-1);

            //get the value of the iterator
            imagesAtRow = *it;
        }
        else
        {
            cout << "Requested row for animation creation is out of bound. Sprite: " << this->path << " Row: " << row << endl;
            return NULL;
        }
    }
    else
    {
        cout << "Spriteloader couldnt correctly load the number of images at a specific row. Error at sprite: " << this->path << endl;
        return NULL;
    }

    TDG_Animation* newAni = new TDG_Animation();
    newAni->setTyp(typ);

    int i;
    for(i = 1; i <= imagesAtRow; i++)
    {
        SDL_Texture* img = getImage(gui, row, i);
        if(img == NULL)
        {
            cout << "Unable to load image from sprite " << this->path << " Row:" << row << " Column: " << i << endl;
            return NULL;
        }
        newAni->addImg(img);
    }

    return newAni;
}

bool TDG_SpriteLoader::loadSpriteInfo(string path)
{
    ifstream sprite;
    sprite.open(path.c_str(), ios::out);
    if(!sprite.is_open())
    {
        cout << "Path " << path << " to Sprite information file is unvalid!" << endl;
        return false;
    }
    else
    {
        string line;
        while(getline(sprite, line))
        {
            vector<string> entries = split(line, ' ');
            string entry = entries.front();

            if(!entry.compare("imgSize:"))
            {
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 2))
                {
                    this->imgWidth = nextInt(entries);
                    this->imgHight = nextInt(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << path << " at statement -imgSize-!"<< endl;
                    sprite.close();
                    return false;
                }
            }
            else if(!entry.compare("spriteSize:"))
            {
                entries.erase(entries.begin());
                if(!entries.empty() && (entries.size() == 2))
                {
                    this->spriteRows = nextInt(entries);
                    this->spriteColumns = nextInt(entries);
                }
                else
                {
                    cout << "Invalid count of arguments in " << path << " at statement -spriteSize-!"<< endl;
                    sprite.close();
                    return false;
                }
            }
            else if(!entry.compare("imgPerRow:"))
            {
                entries.erase(entries.begin());
                while(!entries.empty())
                {
                    this->imgPerRow.push_back(nextInt(entries));
                }
            }
        }
    }
    sprite.close();

    return true;
}

int TDG_SpriteLoader::getSpriteMaxRows()
{
    return this->spriteRows;
}

int TDG_SpriteLoader::getSpriteMaxColumns()
{
    return this->spriteColumns;
}

int TDG_SpriteLoader::getImgWidth()
{
    return this->imgWidth;
}

int TDG_SpriteLoader::getImgHight()
{
    return this->imgHight;
}

int TDG_SpriteLoader::nextInt(vector<string>& entries)
{
    int result = atoi(entries.front().c_str());
    entries.erase(entries.begin());
    return result;
}

vector<string> TDG_SpriteLoader::split(const string& str, char delimiter)
{
    istringstream is(str);
    vector<string> result;
    for(string cur; getline(is, cur, delimiter); result.push_back(cur));
    return result;
}
