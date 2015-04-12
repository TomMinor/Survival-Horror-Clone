#ifndef INVENTORYWIDGETS_H
#define INVENTORYWIDGETS_H

#include <vector>

#include "Item.h"
#include "RectWidget.h"
#include "Vec2.h"
#include "Inventory.h"

class InventoryItemWidget
{
public:
    InventoryItemWidget(const Vec2& _position, const Vec2& _size, std::string _foregroundTexture, std::string _backgroundTexture, int _trim = 2);

    virtual ~InventoryItemWidget()
    {
        delete m_background;
        delete m_foreground;
    }

    virtual void draw();

    virtual void update();

    virtual void setWidth(int _width);
    virtual void setHeight(int _height);

    void setItem(BaseItem* _item)
    {
        m_item = _item;
        m_icon->setTexture( m_item->itemData()->m_icon );
    }

private:
    unsigned int m_time;
    int m_trim;

    BaseItem* m_item;

    RectWidget* m_background;
    RectWidget* m_foreground;
    RectWidget* m_icon;
};


class InventoryWidget
{
public:
    InventoryWidget(int _maxRows, int _maxColumns, const Vec2 &_position, const Vec2 &_size);

    ~InventoryWidget()
    {
        delete m_background;

        for(InventoryItemWidget* widget : m_itemWidgets)
        {
            delete widget;
        }
    }

    void addItem( BaseItem* _item);

    void draw();

    void update();

private:
    const int m_maxRows;
    const int m_maxColumns;

    std::vector<InventoryItemWidget*>::iterator m_currentItem;

    InventoryItemWidget* m_background;
    std::vector<InventoryItemWidget*> m_itemWidgets;
};


#endif // INVENTORYWIDGETS_H
