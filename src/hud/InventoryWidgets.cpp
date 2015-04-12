#include "InventoryWidgets.h"
#include <assert.h>

InventoryWidget::InventoryWidget(int _maxRows, int _maxColumns, const Vec2& _position, const Vec2& _size)
    : m_maxRows(_maxRows), m_maxColumns(_maxColumns)
{
    m_itemWidgets.reserve(m_maxRows * m_maxColumns);

//    const Vec2 position( 500, 450 );
//    const Vec2 size = Vec2( 250, 400) + Vec2(16,16);


    m_background = new InventoryItemWidget( _position, _size + Vec2(16,16),
                                                 "assets/hud/item_background.jpg",
                                                 "assets/hud/metal_background.jpg",
                                                 16);
    for(int y = 0; y < m_maxRows; y++)
    {
        for(int x = 0; x < m_maxColumns; x++)
        {
            const int width = _size.m_x / m_maxColumns;
            const int height = _size.m_y / m_maxRows;

            m_itemWidgets.push_back(new InventoryItemWidget( Vec2(_position.m_x + (x * width),
                                                                _position.m_y - (y * height) ) + Vec2(8, -8),
                                                           Vec2(width, height),
                                                           "assets/hud/item_background.jpg",
                                                           "assets/hud/item_background_dark.jpg",
                                                           2));
        }
    }

    m_currentItem = m_itemWidgets.begin();
}

void InventoryWidget::addItem( BaseItem* _item)
{
    if(m_currentItem != m_itemWidgets.end())
    {
        (*m_currentItem)->setItem(_item);
        m_currentItem++;
    }
}

void InventoryWidget::update()
{

}

void InventoryWidget::draw()
{
    m_background->draw();

    for(auto widget : m_itemWidgets)
    {
        widget->draw();
    }
}

InventoryItemWidget::InventoryItemWidget(const Vec2 &_position, const Vec2 &_size, std::string _foregroundTexture, std::string _backgroundTexture, int _trim)
    : m_time(0), m_trim(_trim), m_item(NULL)
{
    Vec2 tmpSize(_size.m_x, -_size.m_y); // Invert Y coordinate direction

    m_foreground = new RectWidget(_position + Vec2((_trim / 2), -_trim / 2),
                                  tmpSize + Vec2(-_trim, _trim),
                                  _foregroundTexture);

    m_background = new RectWidget( _position,
                                  tmpSize,
                                  _backgroundTexture);

    m_icon = new IconWidget(_position + Vec2((_trim / 2), -_trim / 2),
                            tmpSize + Vec2(-_trim, _trim)
                            );

    assert(m_background != nullptr);
    assert(m_foreground != nullptr);
}

void InventoryItemWidget::draw()
{
    m_background->draw();
    m_foreground->draw();

    if(m_item)
    {
        m_icon->draw();
    }
}

void InventoryItemWidget::update()
{
    m_background->update();
    m_foreground->update();
}

void InventoryItemWidget::setWidth(int _width)
{
    m_background->setWidth(_width);
    m_foreground->setWidth(_width - m_trim);
}

void InventoryItemWidget::setHeight(int _height)
{
    m_background->setHeight(_height);
    m_foreground->setHeight(_height - m_trim);
}


