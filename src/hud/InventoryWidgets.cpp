#include "InventoryWidgets.h"
#include <assert.h>

BaseInventoryWidget::BaseInventoryWidget(const Vec2 &_position, const Vec2 &_size, int _trim)
    : m_time(0), m_trim(_trim)
{
    Vec2 tmpSize(_size.m_x, -_size.m_y); // Invert Y coordinate direction

    m_background = new RectWidget( _position,
                                  tmpSize,
                                  "assets/hud/metal_background.jpg");

    m_foreground = new RectWidget(_position + Vec2((_trim / 2), -_trim / 2),
                                  tmpSize + Vec2(-_trim, _trim),
                                  "assets/hud/item_background.jpg");

    assert(m_background != nullptr);
    assert(m_foreground != nullptr);
}

void BaseInventoryWidget::draw()
{
    m_background->draw();
    m_foreground->draw();
}

void BaseInventoryWidget::update()
{
    m_background->update();
    m_foreground->update();
}

void BaseInventoryWidget::setWidth(int _width)
{
    m_background->setWidth(_width);
    m_foreground->setWidth(_width - m_trim);
}

void BaseInventoryWidget::setHeight(int _height)
{
    m_background->setHeight(_height);
    m_foreground->setHeight(_height - m_trim);
}


