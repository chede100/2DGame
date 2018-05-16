#ifndef TDG_GUIELEMENT_H
#define TDG_GUIELEMENT_H

typedef enum
{
    Left, hCenter, Right
}HorizontalPosition;

typedef enum
{
    Top, vCenter, Bottom
}VerticalPosition;

class TDG_GUIElement
{
    public:
        TDG_GUIElement();
        virtual ~TDG_GUIElement();

    protected:

    private:
        HorizontalPosition hPos;
        VerticalPosition vPos;
        int width, hight;
};

#endif // TDG_GUIELEMENT_H
