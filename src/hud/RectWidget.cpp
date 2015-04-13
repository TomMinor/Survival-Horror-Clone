#include "RectWidget.h"

#include <algorithm>

void RectWidget::move(const Vec2& _offset)
{
    m_position += _offset;
}

void RectWidget::draw(const Vec4& _colour)
{
    m_texture->bind();

    // Quick fix for icons
    float slide = 0.15;
    float ratio = (-m_size.m_y / m_size.m_x) + slide;
    if(ratio > 1)
    {
        ratio = 1;
        slide = 0;
    }

    glBegin(GL_QUADS);
      _colour.colourGL();
      glTexCoord2f(0, slide);   glVertex3f(m_position.m_x,                m_position.m_y,                 0);
      glTexCoord2f(0, ratio);   glVertex3f(m_position.m_x,                m_position.m_y + m_size.m_y,    0);
      glTexCoord2f(1, ratio);   glVertex3f(m_position.m_x + m_size.m_x,   m_position.m_y + m_size.m_y,    0);
      glTexCoord2f(1, slide);   glVertex3f(m_position.m_x + m_size.m_x,   m_position.m_y,                 0);
    glEnd();

    m_texture->unbind();
}

void RectWidget::update(Time _delta)
{

}

void RectWidget::setTexture(const std::string _textureName)
{
    delete m_texture;

    m_texture = new Texture(_textureName);
}

void RectWidget::setTexture(Texture * const _texture)
{
    delete m_texture;

    m_texture = _texture;
}

void IconWidget::draw(const Vec4 &_colour)
{
    // Draw transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    RectWidget::draw(_colour);

    glDisable(GL_ALPHA_TEST);
    glDisable( GL_BLEND );
}
