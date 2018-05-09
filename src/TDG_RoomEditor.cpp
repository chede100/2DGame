#include "TDG_RoomEditor.h"

TDG_RoomEditor::TDG_RoomEditor()
{
    this->gui = NULL;
}

TDG_RoomEditor::~TDG_RoomEditor()
{
    if(this->gui != NULL)
        delete this->gui;
}

bool TDG_RoomEditor::init()
{

    return true;
}
