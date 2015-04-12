#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <string>

#include "Vec2.h"
#include "Texture.h"

class RectWidget
{
public:
    RectWidget(const Vec2& _position, const Vec2& _size, const std::string& _textureName)
        : m_texture(new Texture(_textureName)), m_position(_position), m_size(_size)
    {;}

    virtual ~RectWidget()
    {
        delete m_texture;
    }

    void move(const Vec2& _offset);

    void setWidth(int _width) { m_size.m_x = _width; }
    void setHeight(int _height) { m_size.m_y = _height; }

    int getWidth() const { return (int)m_size.m_x; }
    int getHeight() const { return (int)m_size.m_y; }

    virtual void draw();
    virtual void update();

protected:
    Texture* m_texture;
    Vec2 m_position;
    Vec2 m_size;
};

#endif // BASEWIDGET_H
