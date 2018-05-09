#ifndef TDG_FILEHANDLER_H
#define TDG_FILEHANDLER_H

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
    int id;
    int posX;
    int posY;
    MovementStatus firstStatus;

    string name;
    int animationID;
    double scale;
    int width, hight;
    double speed;
}Entity;

typedef struct
{
    int id;
    int row;
    int column;
    MovementStatus arriveStatus;
    int destinationRoomID;
    int destinationGateID;
}Gate;

typedef struct
{
    int roomID;
    Entity player;
}SavePoint;

typedef struct
{
    string roomName;
    int roomID;
    list<Entity> npc;
    list<Entity> obj;

    list<Gate> gates;

    list<int> tileIDs;
    int tileRows;
    int tileColumns;
    int** tileIDArrangement;
    int** tileRotationDegree;
    bool** enviromentCollision;
    int** flipTile;

}Room;

class TDG_FileHandler
{
    public:
        TDG_FileHandler();
        virtual ~TDG_FileHandler();

        bool load();
        bool loadRoom(int roomID);
        bool loadOpt();
        bool loadSPoint();

        Room* getRoom();
        Options* getOpt();
        SavePoint* getSPoint();

    protected:

    private:
        Room* room;
        SavePoint* sPoint;
        Options* opt;

        bool loadEntity(EntityTyp typ, Entity* e);
        bool loadNPCs(string path);
        bool loadOBJs(string path);

        double nextDouble(vector<string>& entries);
        int nextInt(vector<string>& entries);
        void nextString(vector<string>& entries, string& input);
        vector<string> split(const string& str, char delimiter);

        MovementStatus stringToMoveStatus(string& status);
        bool tileIMGLoaded(int id);
};

#endif // TDG_FILEHANDLER_H