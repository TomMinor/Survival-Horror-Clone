#ifndef HUD_H
#define HUD_H

#include <vector>

#include "Vec4.h"
#include "Texture.h"
#include "InventoryWidgets.h"

class HUD
{
public:
    HUD();

    void draw();

//private:
    InventoryWidget* m_inventory;
};

#endif // HUD_H
