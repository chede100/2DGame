#ifndef TDG_TILE_H
#define TDG_TILE_H


class TDG_Tile
{
    public:
        TDG_Tile(int id, int rotationDegree, bool impassable);
        virtual ~TDG_Tile();

        int getID();
        int getRotDegree();
        bool isImpassable();

    protected:

    private:
        int id;
        int rotDegree;
        bool impassable;
};

#endif // TDG_TILE_H
