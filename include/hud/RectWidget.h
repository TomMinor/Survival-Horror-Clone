#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <string>

#include "EngineGlobals.h"

#include "Vec4.h"
#include "Vec2.h"
#include "Texture.h"

class RectWidget
{
public:
    RectWidget(const Vec2& _position, const Vec2& _size, const std::string& _textureName)
        : m_texture(new Texture(_textureName)), m_position(_position), m_size(_size)
    {;}

    RectWidget(const Vec2& _position, const Vec2& _size)
        : m_texture(nullptr), m_position(_position), m_size(_size)
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

    void setTexture(const std::string _textureName);
    void setTexture(Texture * const _texture);

    virtual void draw(const Vec4 &_colour = Vec4(1,1,1,1));
    virtual void update(Time _delta);

protected:
    Texture* m_texture;
    Vec2 m_position;
    Vec2 m_size;
};

// Draw transparently
class IconWidget : public RectWidget
{
public:
    IconWidget(const Vec2& _position, const Vec2& _size, const std::string& _textureName)
        : RectWidget(_position, _size, _textureName)
    {;}

    IconWidget(const Vec2& _position, const Vec2& _size)
        : RectWidget(_position, _size)
    {;}

    virtual void draw(const Vec4 &_colour = Vec4(1,1,1,1));
};

#endif // BASEWIDGET_H
