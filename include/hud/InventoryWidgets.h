#ifndef INVENTORYWIDGETS_H
#define INVENTORYWIDGETS_H

#include "RectWidget.h"
#include "Vec2.h"
#include "Inventory.h"

class BaseInventoryWidget
{
public:
    BaseInventoryWidget(const Vec2& _position, const Vec2& _size, int _trim = 2);

    virtual ~BaseInventoryWidget()
    {
        delete m_background;
        delete m_foreground;
    }

    virtual void draw();

    virtual void update();

    virtual void setWidth(int _width);
    virtual void setHeight(int _height);

private:
    unsigned int m_time;
    int m_trim;

    RectWidget* m_background;
    RectWidget* m_foreground;
};

#endif // INVENTORYWIDGETS_H
