#ifndef VEC4_H__
#define VEC4_H__

#include <ostream>

class Vec4
{
public:
  //------------------------------------------
  /// @brief Ctor
  /// @param [in] _x The x value default 0
  /// @param [in] _y The y value default 0
  /// @param [in] _z The z value default 0
  /// @param [in] _w The w value default 0
  //------------------------------------------
  Vec4(float _x=0.f, float _y=0.f, float _z=0.f, float _w=1.f)
        : m_x(_x), m_y(_y), m_z(_z), m_w(_w) {;}

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

  //------------------------------------------
  /// @brief calculate dot product of 2 Vec4's
  //------------------------------------------
  float dot(const Vec4 &_b) const;

  //------------------------------------------
  /// @brief calculate dot product of 2 Vec4's
  //------------------------------------------
  float dot(const Vec4 &_a, const Vec4 &_b) const;

  //------------------------------------------
  /// @brief cross product of this and b
  /// @param [in] _b the vectir the cross with
  /// @returns a new vector this cross _b
  //------------------------------------------
  Vec4 cross(const Vec4 &_b) const;

  //------------------------------------------
  /// @brief cross product of this and b
  /// @param [in] _a a vector the cross with
  /// @param [in] _b a vector the cross with
  /// @returns a new vector this cross _b
  //------------------------------------------
  Vec4 cross(const Vec4 &_a, const Vec4 &_b) const;

  void colourGL() const;
  void normalGL() const;
  void vertexGL() const;

  void set(float _x, float _y, float _z, float _w=1.0f);

  //------------------------------------------
  // Operator Overloads
  //------------------------------------------
  Vec4 operator *(float _rhs) const; // rhs = right hand side
  void operator *=(float _rhs);

  Vec4 operator -(const Vec4 &_rhs) const;
  void operator -=(const Vec4 &_rhs);

  Vec4 operator+(const Vec4 &_rhs) const;
  void operator+=(const Vec4 &_rhs);

  bool operator==(const Vec4 &_rhs) const;

  float* getVector()
  {
    return m_openGL;
  }

  union
  {
    //XYZW
    struct
    {
      /// @brief The x element of the vector
      float m_x;
      /// @brief The y element of the vector
      float m_y;
      /// @brief The z element of the vector
      float m_z;
      /// @brief The w element of the vector
      float m_w;
    };

    //RGBA
    struct
    {
      /// @brief The r element of the vector
      float m_r;
      /// @brief The g element of the vector
      float m_g;
      /// @brief The b element of the vector
      float m_b;
      /// @brief The a element of the vector
      float m_a;
    };

    float m_openGL[4];
  };

};



std::ostream & operator<<(std::ostream &_output, const Vec4 &_v);

#endif
