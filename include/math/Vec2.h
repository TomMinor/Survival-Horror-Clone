#ifndef VEC2_H
#define VEC2_H

#include <iostream>
#include "GLinc.h"

class Vec2
{
public:
  //------------------------------------------
  /// @brief Ctor
  /// @param [in] _x The x value default 0
  /// @param [in] _y The y value default 0
  //------------------------------------------
  Vec2(float _x=0.f, float _y=0.f)
        : m_x(_x), m_y(_y) {;}

  //------------------------------------------
  /// @brief method to print attributes
  //------------------------------------------
  void print() const;

  //------------------------------------------
  /// @brief calculate length of vector
  //------------------------------------------
  float length() const;

  //------------------------------------------
  /// @brief length^2
  //------------------------------------------
  float lengthSquared() const;

  //------------------------------------------
  /// @brief normalize the attributes
  //------------------------------------------
  void normalize();

  void set(float _x, float _y);

  //------------------------------------------
  // Operator Overloads
  //------------------------------------------
  Vec2 operator *(float _rhs) const; // rhs = right hand side
  void operator *=(float _rhs);

  Vec2 operator -() const;
  Vec2 operator -(const Vec2 &_rhs) const;
  void operator -=(const Vec2 &_rhs);

  Vec2 operator+(const Vec2 &_rhs) const;
  void operator+=(const Vec2 &_rhs);

  bool operator==(const Vec2 &_rhs) const;

  inline float* getVector() { return m_openGL; }

  union // Various varieties of accessor names
  {
    float m_openGL[2];

    //XYZW
    struct
    {
      /// @brief The x element of the position vector
      float m_x;
      /// @brief The y element of the position vector
      float m_y;
    };
  };

};


std::ostream & operator<<(std::ostream &_output, const Vec2 &_v);


#endif // VEC2_H
