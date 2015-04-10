#include "GLinc.h"
#include "BoundingBox.h"
#include "GLFunctions.h"
#include <iostream>

#include "3dUtilities.h"

bool BBox::m_debugDraw = true;

/**
 * @brief BBox::checkIntersection
 * @param _b The bounding box to check this instance against
 * @return True if there is intersection, false if there is not
 */
bool BBox::checkIntersection(const BBox& _b) const
{
  return ( ((m_max.m_x > _b.m_min.m_x) && (m_min.m_x < _b.m_max.m_x)) &&
           ((m_max.m_y > _b.m_min.m_y) && (m_min.m_y < _b.m_max.m_y)) &&
           ((m_max.m_z > _b.m_min.m_z) && (m_min.m_z < _b.m_max.m_z)) );
}

/**
 * @brief BBox::checkIntersectOrTouch
 * @param _b
 * @return True if the two bounding boxes touch or intersect, false if not
 */
bool BBox::checkIntersectOrTouch(const BBox& _b) const
{
  return ( ((m_max.m_x >= _b.m_min.m_x) && (m_min.m_x <= _b.m_max.m_x)) &&
           ((m_max.m_y >= _b.m_min.m_y) && (m_min.m_y <= _b.m_max.m_y)) &&
           ((m_max.m_z >= _b.m_min.m_z) && (m_min.m_z <= _b.m_max.m_z)) );
}

/**
 * @brief BBox::checkTouch
 * @param _b
 * @return True if the two bounding boxes touch, false if they do not or they intersect
 */
bool BBox::checkTouch(const BBox& _b) const
{
  return (checkIntersectOrTouch(_b) && !checkIntersection(_b));


}

Vec4 BBox::getIntersectOffset(const BBox& _b) const
{
  float X1 = (m_min.m_x - _b.m_max.m_x);
  float X2 = (m_max.m_x - _b.m_min.m_x);
  float X = (X1>X2) ? X1 : X2;

  float Y1 = (m_min.m_y - _b.m_max.m_y);
  float Y2 = (m_max.m_y - _b.m_min.m_y);
  float Y = (Y1>Y2) ? Y1 : Y2;

  float Z1 = (m_min.m_z - _b.m_max.m_z);
  float Z2 = (m_max.m_z - _b.m_min.m_z);
  float Z = (Z1>Z2) ? Z1 : Z2;

  return Vec4( X, Y, Z, 0.0f );
}

/**
 * @brief BBox::getTouchingFace
 * @param _b  BBox to check against
 * @return The face from this BBox instance that _b is touching
 */
BBox::Face BBox::getTouchingFace(const BBox& _b) const
{
  Face tmp = NONE;

  const static float delta = 0.0001f; // Fuzzy comparison

  #define equal(max,min,delta) ((max-delta) < (min)) && ((max) > (min))

  // Positive xyz axis
  if( ((m_max.m_x-delta) < (_b.m_min.m_x)) && ((m_max.m_x) > (_b.m_min.m_x)) ) { tmp = RIGHT;   }
  if( ((m_max.m_y-delta) < (_b.m_min.m_y)) && ((m_max.m_y) > (_b.m_min.m_y)) ) { tmp = TOP;     }
  if( ((m_max.m_z-delta) < (_b.m_min.m_z)) && ((m_max.m_z) > (_b.m_min.m_z)) ) { tmp = FRONT;   }

  // Negative xyz axis
  if( ((m_min.m_x-delta) < (_b.m_max.m_x)) && ((m_min.m_x) > (_b.m_max.m_x)) ) { tmp = LEFT;    }
  if( ((m_min.m_y-delta) < (_b.m_max.m_y)) && ((m_min.m_y) > (_b.m_max.m_y)) ) { tmp = BOTTOM;  }
  if( ((m_min.m_z-delta) < (_b.m_max.m_z)) && ((m_min.m_z) > (_b.m_max.m_z)) ) { tmp = BACK;    }

  return tmp;
}

Vec4 BBox::getCenter() const
{
  return Vec4( (m_max.m_x+m_min.m_x)/2,
               (m_max.m_y+m_min.m_y)/2,
               (m_max.m_z+m_min.m_z)/2 );
}

void BBox::move(Vec4 _pos)
{
  m_max += _pos;
  m_min += _pos;
}

void BBox::draw() const
{
  if(m_debugDraw)
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glPushMatrix();
      glTranslatef( (m_max.m_x+m_min.m_x)/2 ,
                    (m_max.m_y+m_min.m_y)/2,
                    (m_max.m_z+m_min.m_z)/2);
      glPushMatrix();
        GLFunctions::cube(fabs(m_max.m_x - m_min.m_x),
                          fabs(m_max.m_y - m_min.m_y),
                          fabs(m_max.m_z - m_min.m_z));
      glPopMatrix();
    glPopMatrix();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }
}

std::ostream & operator<<(std::ostream &_output, const BBox &_b)
{
  _output << "Min x: " << _b.m_min.m_x
        << "\nMin y: " << _b.m_min.m_y
        << "\nMin z: " << _b.m_min.m_z
        << "\nMax x: " << _b.m_max.m_x
        << "\nMax y: " << _b.m_max.m_y
        << "\nMax z: " << _b.m_max.m_z;

  return _output;
}
