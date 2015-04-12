#include "HUD.h"
#include <iostream>

#include "GLFunctions.h"

#include "InventoryWidgets.h"

//void HUD::drawRect(int _x, int _y, int _w, int _h, Vec4 _colour)
//{
//    glBegin(GL_QUADS);
//      glColor3f(_colour.m_r, _colour.m_g, _colour.m_b);
//      glTexCoord2f(0, 1); glVertex3f(_x,        _y,         0);
//      glTexCoord2f(0, 0); glVertex3f(_x,        _y + _h,    0);
//      glTexCoord2f(1, 0); glVertex3f(_x + _w,   _y + _h,    0);
//      glTexCoord2f(1, 1); glVertex3f(_x + _w,   _y,         0);
//    glEnd();
//}

HUD::HUD()
{
    // Bad - Hard code all the things

    // Inventory

    const Vec2 position( 500, 450 );
    const Vec2 size = Vec2( 250, 400) + Vec2(16,16);

    const int rowCount = 5;
    const int columnCount = 2;

    m_widgets.push_back(new BaseInventoryWidget( position, size + Vec2(16,16), 16));
    for(int x = 0; x < columnCount; x++)
    {
        for(int y = 0; y < rowCount; y++)
        {
            const int width = size.m_x / columnCount;
            const int height  = size.m_y / rowCount;

            m_widgets.push_back(new BaseInventoryWidget( Vec2(position.m_x + (x * width),
                                                              position.m_y - (y * height) ) + Vec2(8, -8),
                                                         Vec2(width, height),
                                                         1)
                                );
        }
    }
}

void HUD::draw()
{
    glClearColor(0.078, 0.137, 0.353,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_2D, 0);

      // Draw in orthographic projection
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, 800, 0.0, 600, -500.0, 500.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
          glLoadIdentity();

          glDisable(GL_LIGHTING);
          glEnable(GL_TEXTURE_2D);

          glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
          glColor3f(1.0f, 1.0f, 1.0f);
          // Draw the facade so that it fills the screen
          glTranslatef(0, 0, 0);

          for(auto widget : m_widgets)
          {
              widget->draw();
          }

          glDisable(GL_TEXTURE_2D);
          glEnable(GL_LIGHTING);

          glPopMatrix();
        glMatrixMode(GL_PROJECTION);
      glPopMatrix();

      glMatrixMode(GL_MODELVIEW);
}
