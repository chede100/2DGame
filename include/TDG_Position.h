#ifndef TDG_POSITION_H
#define TDG_POSITION_H


class TDG_Position
{
    public:
        TDG_Position(double x, double y);
        TDG_Position();
        virtual ~TDG_Position();

        double getPosX();
        double getPosY();

        void setPosX(double newX);
        void setPosY(double newY);

    protected:

    private:
        double x, y;
};

#endif // TDG_POSITION_H
