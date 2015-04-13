#include "InventoryWidgets.h"
#include <assert.h>

InventoryWidget::InventoryWidget(int _maxRows, int _maxColumns, const Vec2& _position, const Vec2& _size)
    : m_maxRows(_maxRows), m_maxColumns(_maxColumns)
{
    m_itemWidgets.reserve(m_maxRows * m_maxColumns);

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
                                                           6));
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

void InventoryWidget::update(Time _delta)
{
    m_background->update(_delta);

    for(auto widget : m_itemWidgets)
    {
        widget->update(_delta);
    }
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
    ///@todo Fix this terrible 'fix', complicates the rest of the code
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
    // Simple way to get a dropshadow
    const Vec4 shadow(-2, -5, 0);
    const Vec4 shadowColour(0.2, 0.2, 0.2);

    if(m_isActive)
    {
      glDisable(GL_TEXTURE_2D);
      m_background->draw( Vec4(0.75 + (sin(m_time * 4.0f) * 0.25), 0, 0, 1) );
      glEnable(GL_TEXTURE_2D);
    }
    else
    {
      m_background->draw();
    }

    m_foreground->draw();

    if(m_item)
    {
      glPushMatrix();
        shadow.translateGL();
        m_icon->draw(shadowColour);
      glPopMatrix();

      m_icon->draw();
    }
}

void InventoryItemWidget::update(float _delta)
{
    m_time = _delta;
    m_background->update(_delta);
    m_foreground->update(_delta);
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


