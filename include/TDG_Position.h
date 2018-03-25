#ifndef TDG_POSITION_H
#define TDG_POSITION_H


class TDG_Position
{
    public:
        TDG_Position(int x, int y);
        virtual ~TDG_Position();

        int getPosX();
        int getPosY();

        void setPosX(int newX);
        void setPosY(int newY);

    protected:

    private:
        int x, y;
};

#endif // TDG_POSITION_H
