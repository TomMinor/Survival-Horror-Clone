#ifndef INVENTORYWIDGETS_H
#define INVENTORYWIDGETS_H

#include <vector>
#include <assert.h>


#include "Item.h"
#include "RectWidget.h"
#include "Vec2.h"

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

    virtual void update(float _delta);

    virtual void setWidth(int _width);
    virtual void setHeight(int _height);

    void setItem(BaseItem* _item)
    {
      if(_item == nullptr)
      {
        std::cerr << "Attempted to set item to invalid item" << std::endl;
        return;
      }

        m_item = _item;
        m_icon->setTexture( m_item->itemData()->m_icon );
    }

    inline void setActive()     { m_isActive = true; }
    inline void setInactive()   { m_isActive = false; }

private:
    Time m_time; // Copy the world time so we can create a glowing effect over the selected item
    bool m_isActive;
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

    void addItem(BaseItem* _item);

    void draw();

    void update(Time _delta);

    void selectItem(int& _index)
    {
      const int maxIndex = (m_maxRows * m_maxColumns);
      if(_index >= maxIndex || _index < 0)
      {
        std::cout << "Invalid inventory ID " << _index << "\n Limiting to " << maxIndex << std::endl;
        _index = maxIndex - 1;
        return;
      }

      std::cout << m_maxRows * m_maxColumns << "," << _index << std::endl;

      for(auto widget : m_itemWidgets)
      {
          widget->setInactive();
      }

      m_itemWidgets[_index]->setActive();
    }

private:
    const int m_maxRows;
    const int m_maxColumns;

    std::vector<InventoryItemWidget*>::iterator m_currentItem;

    InventoryItemWidget* m_background;
    std::vector<InventoryItemWidget*> m_itemWidgets;
};


#endif // INVENTORYWIDGETS_H
