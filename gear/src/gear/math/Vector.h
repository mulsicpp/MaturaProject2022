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

static char error_buffer[100];

/*
This class represents a mathematical Vector. The type of the elements is
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
  Constructs a vector with the values of the elements in an initalizer list.
  */
  Vector<T, N>(std::initializer_list<T> list) : data{0} {
    memcpy(data, list.begin(), (list.size() > N ? N : list.size()) * sizeof(T));
  }

  /*
  Returns the element count in the vector.
  */
  constexpr size_t count() const { return N; }

  /*
  Returns a reference to the element in the vector at the specified index.
  The index has to be greater or equal to 0 and smaller than the element count
  or an exception will be thrown.
  */
  T &operator[](int index) {
    if (index >= N || index < 0) {
      sprintf(error_buffer, "Vector index out of bounds: %i", index);
      throw std::exception(error_buffer);
    } else {
      return data[index];
    }
  }

  const T &operator[](int index) const {
    if (index >= N || index < 0) {
      sprintf(error_buffer, "Vector index out of bounds: %i", index);
      throw std::exception(error_buffer);
    } else {
      return data[index];
    }
  }

  /*
  Leaves the vector as it is.
  */
  Vector<T, N> operator+() const { return *this; }

  /*
  Negates the vector.
  */
  Vector<T, N> operator-() const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = -data[i];
    return ret;
  }

  /*
  Adds two vectors together.
  */
  Vector<T, N> operator+(const Vector<T, N> &vector) const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = data[i] + vector.data[i];
    return ret;
  }

  /*
  Subtracts a vector from another vector.
  */
  Vector<T, N> operator-(const Vector<T, N> &vector) const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = data[i] - vector.data[i];
    return ret;
  }

  /*
  Multiplies the vector with a scalar value.
  */
  Vector<T, N> operator*(const T &k) const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = data[i] * k;
    return ret;
  }

  /*
  Devides the vector by a scalar value.
  */
  Vector<T, N> operator/(const T &k) const {
    Vector<T, N> ret;
    for (int i = 0; i < N; i++)
      ret.data[i] = data[i] / k;
    return ret;
  }

  /*
  Adds a vector to the current vector.
  */
  const Vector<T, N> &operator+=(const Vector<T, N> &vector) const {
    for (int i = 0; i < N; i++)
      data[i] += vector.data[i];
    return *this;
  }

  /*
  Subtracts a vector from the current vector.
  */
  const Vector<T, N> &operator-=(const Vector<T, N> &vector) const {
    for (int i = 0; i < N; i++)
      data[i] -= vector.data[i];
    return *this;
  }

  /*
  Multiplies the current vector with a scalar value.
  */
  const Vector<T, N> &operator*=(const T &factor) const {
    for (int i = 0; i < N; i++)
      data[i] *= factor;
    return *this;
  }

  /*
  Devides the current vector by a scalar value.
  */
  const Vector<T, N> &operator/=(const T &factor) const {
    for (int i = 0; i < N; i++)
      data[i] /= factor;
    return *this;
  }

  /*
  Calculates the dot product or scalar product of two vectors.
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
  */
  template <class T2, int N2>
  Vector<T2, N2> castTo() const {
    Vector<T2, N2> ret;
    size_t size = N < N2 ? N : N2;
    for (int i = 0; i < size; i++)
      ret[i] = (T)(data[i]);
    return ret;
  }

  /*
  Returns a reference to the vector with a new "virtual" element count, which
  means that reference only uses a part of the vector. The new count has to be
  smaller or equal to the current count or an exception will be thrown.
  */
  template <int N2>
  Vector<T, N2> &useAs() const {
    if (N2 > N) {
      sprintf(error_buffer, "Vector element count to big: %i", N2);
      throw std::exception(error_buffer);
    } else
      return *(Vector<T, N2> *)this;
  }

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