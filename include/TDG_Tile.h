#ifndef TDG_TILE_H
#define TDG_TILE_H


class TDG_Tile
{
    public:
        TDG_Tile(int id, bool impassable);
        virtual ~TDG_Tile();

        int getID();
        bool isImpassable();

    protected:

    private:
        int id;
        bool impassable;
};

#endif // TDG_TILE_H
