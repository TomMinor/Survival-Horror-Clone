#include "RectWidget.h"

void RectWidget::move(const Vec2& _offset)
{
    m_position += _offset;
}

void RectWidget::draw()
{
    m_texture->bind();

    glBegin(GL_QUADS);
      glColor3f(1, 1, 1);
      glTexCoord2f(0, 1); glVertex3f(m_position.m_x,                m_position.m_y,                 0);
      glTexCoord2f(0, 0); glVertex3f(m_position.m_x,                m_position.m_y + m_size.m_y,    0);
      glTexCoord2f(1, 0); glVertex3f(m_position.m_x + m_size.m_x,   m_position.m_y + m_size.m_y,    0);
      glTexCoord2f(1, 1); glVertex3f(m_position.m_x + m_size.m_x,   m_position.m_y,                 0);
    glEnd();

    m_texture->unbind();
}

void RectWidget::update()
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

void IconWidget::draw()
{
    // Draw transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    RectWidget::draw();

    glDisable(GL_ALPHA_TEST);
    glDisable( GL_BLEND );
}
