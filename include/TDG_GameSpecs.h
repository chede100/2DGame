#ifndef TDG_GAMESPECS_H
#define TDG_GAMESPECS_H

#include "TDG_Information.h"
#include <cstddef>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <SDL.h>

using namespace std;

typedef struct
{
    int winWidth;
    int winHight;
    int fpsCap;
    bool fullscreen;
}Options;

typedef struct
{
    int roomID;
}SavePoint;

typedef struct
{
    int id;
    int posX;
    int posY;

    string name;
    int animationID;
    int width, hight;
    int speed;

}Entity;

typedef struct
{
    string roomName;
    Entity player;
    list<Entity> npc;
    list<Entity> obj;

    list<int> tileIDs;
    int tileRows;
    int tileColumns;
    int** tileIDArrangement;
    int** tileRotationDegree;
    bool** enviromentCollision;

}Room;

class TDG_GameSpecs
{
    public:
        TDG_GameSpecs();
        virtual ~TDG_GameSpecs();

        bool load();

        Room* getRoom();
        Options* getOpt();
        SavePoint* getSPoint();

    protected:

    private:
        bool loadRoom(int roomID);
        bool loadSPoint();
        bool loadOpt();

        Room* room;
        SavePoint* sPoint;
        Options* opt;

        bool loadEntity(EntityTyp typ, Entity* e);

        int nextInt(vector<string>& entries);
        void nextString(vector<string>& entries, string& input);
        vector<string> split(const string& str, char delimiter);
};

#endif // TDG_GAMESPECS_H
