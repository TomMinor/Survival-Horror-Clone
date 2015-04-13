#ifndef HUD_H
#define HUD_H

#include <vector>

#include "EngineGlobals.h"

#include "Vec4.h"
#include "Texture.h"
#include "InventoryWidgets.h"

class HUD
{
public:
    HUD();

    void draw();

    void update(Time _delta);

//private:
    InventoryWidget* m_inventory;
};

#endif // HUD_H
