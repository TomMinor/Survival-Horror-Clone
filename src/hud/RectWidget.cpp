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
