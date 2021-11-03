#pragma once

#include <gear/core/core.h>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <stdio.h>
#include <string.h>

#define X_COORD 0
#define Y_COORD 1
#define Z_COORD 2
#define W_COORD 3

#define U_COORD 0
#define V_COORD 1

_GEAR_START

/*
This class represents a mathematical vector. The type of the elements is
variable, however it has to support the following operations(a, b and c are variables of type T):
a = +b;
a = -b;
a = b + c;
a = b - c;
a = b * c;
a = b / c;
a += b;
a -= b;
a *= b;
a /= b;
*/
template <class T, int N>
class Vector {
private:
  T data[N];

public:
  /*
  Constructs a vector with all elements as 0.
  */
  Vector<T, N>() : data{0} {}

  /*
  Constructs a vector with the values.
  @param list a initializer list containing the values
  */
  Vector<T, N>(std::initializer_list<T> list) : data{0} {
    memcpy(data, list.begin(), (list.size() > N ? N : list.size()) * sizeof(T));
  }

  /*
  @return the element count in the vector.
  */
  constexpr size_t count() const { return N; }

  /*
  @param index the index of the element. It has to be greater or equal to 0 and smaller than element count.
  @return a reference to the element in the vector at the specified index.
  */
  T &operator[](int index) {
    if (index >= N || index < 0) {
      error("Vector index out of bounds: %i", index);
    } else {
      return data[index];
    }
  }

  /*
  @param index the index of the element. It has to be greater than 0 and smaller or equal to element count.
  @return a const reference to the element in the vector at the specified index.
  */
  const T &operator[](int index) const {
    if (index >= N || index < 0) {
      error("Vector index out of bounds: %i", index);
    } else {
      return data[index];
    }
  }

  /*
  @return a copy of the vector
  */
  Vector<T, N> operator+() const { return *this; }

  /*
  @return a negated copy of the vector
  */
  Vector<T, N> operator-() const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = -data[i];
    return ret;
  }

  /*
  Adds two vectors together.

  @param vector the vector, that gets added to this vector
  @return the result of the addition
  */
  Vector<T, N> operator+(const Vector<T, N> &vector) const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = data[i] + vector.data[i];
    return ret;
  }

  /*
  Subtracts the vector from another vector.

  @param vector the vector, that gets subtracted from this vector
  @return the result of the subtraction
  */
  Vector<T, N> operator-(const Vector<T, N> &vector) const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = data[i] - vector.data[i];
    return ret;
  }

  /*
  Multiplies the vector with a scalar value.

  @param k the scalar value
  @return the result of the multiplication
  */
  Vector<T, N> operator*(const T &k) const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = data[i] * k;
    return ret;
  }

  /*
  Divides the vector by a scalar value.

  @param k the scalar value
  @return the result of the division
  */
  Vector<T, N> operator/(const T &k) const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = data[i] / k;
    return ret;
  }

  /*
  Adds a vector to the current vector.

  @param vector the vector to be added
  @return a reference to this vector 
  */
  const Vector<T, N> &operator+=(const Vector<T, N> &vector) const {
    for (int i = 0; i < N; i++)
      data[i] += vector.data[i];
    return *this;
  }

  /*
  Subtracts a vector from the current vector.

  @param vector the vector to be subtracted
  @return a reference to this vector 
  */
  const Vector<T, N> &operator-=(const Vector<T, N> &vector) const {
    for (int i = 0; i < N; i++)
      data[i] -= vector.data[i];
    return *this;
  }

  /*
  Multiplies the current vector with a scalar value.

  @param k the scalar value to be multiplied with
  @return a reference to this vector 
  */
  const Vector<T, N> &operator*=(const T &k) const {
    for (int i = 0; i < N; i++)
      data[i] *= k;
    return *this;
  }

  /*
  Divides the current vector by a scalar value.

  @param k the scalar value to be divided by
  @return a reference to this vector 
  */
  const Vector<T, N> &operator/=(const T &k) const {
    for (int i = 0; i < N; i++)
      data[i] /= k;
    return *this;
  }

  /*
  Calculates the dot product or scalar product of two vectors.
  @param vector the second vector for the dot product
  @return the result of the dot product
  */
  T operator*(const Vector<T, N> &vector) const {
    T ret = 0;
    for (int i = 0; i < N; i++)
      ret += data[i] * vector.data[i];
    return ret;
  }

  /*
  Cast the vector to a new vector with a different element type and count.
  For this to be possible, there has to be defined a cast from the current to
  the new type of the elements. If the new amount of elements is smaller, only
  elements in that range will be returned. If the new amount of elements is
  greater, every element out of range will be set to 0.

  @return the new vector with the new size and/or type
  */
  template <class T2, int N2>
  Vector<T2, N2> cast_To() const {
    Vector<T2, N2> ret;
    size_t size = N < N2 ? N : N2;
    for (int i = 0; i < size; i++)
      ret[i] = (T)(data[i]);
    return ret;
  }

  /*
  Returns a reference to the vector with a new "virtual" element count, which
  means that reference only uses a part of the vector. The new count has to be smaller or
  equal to the current count.

  @return a reference to this vector, with the new "virtual" element count
  */
  template <int N2>
  Vector<T, N2> &use_As() const {
    if (N2 > N) {
      error("Vector element count to big: %i", N2);
    } else
      return *(Vector<T, N2> *)this;
  }

  /*
  Prints the vector to an output stream.

  @param ostream the output stream
  @param vector the vector to be printed
  */
  friend std::ostream &operator<<(std::ostream &ostream,
                                  engine::Vector<T, N> vector) {
    ostream << "[";
    for (int i = 0; i < N; i++) {
      ostream << vector.data[i];
      if (i < N - 1)
        ostream << ", ";
    }
    ostream << "]";
    return ostream;
  }
};

/*
Calculates the cross product of two three-dimensional vectors.

@param vector1 the first vector
@param vector2 the second vector
@return the result of the cross product
*/
template <class T>
Vector<T, 3> crossProduct(const Vector<T, 3> &vector1, const Vector<T, 3> &vector2) {
  Vector<T, 3> ret;
  float *p_ret = (float *)ret, *p_v1 = (float *)vector1, *p_v2 = (float *)vector2;

  p_ret[0] = p_v1[1] * p_v2[2] - p_v1[2] * p_v2[1];
  p_ret[1] = p_v1[2] * p_v2[0] - p_v1[0] * p_v2[2];
  p_ret[2] = p_v1[0] * p_v2[1] - p_v1[1] * p_v2[0];

  return ret;
}

_GEAR_END