#pragma once

#include <gear/core/core.h>
#include "Vector.h"
#include <exception>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iomanip>

_GEAR_START

template<class T, int N1, int N2>
class Matrix{
private:
  T data[N1][N2];
public:
  Matrix<T, N1, N2>() : data{0} {}
  Matrix<T, N1, N2>(std::initializer_list<std::initializer_list<T>> list) : data{0} {
    int size = list.size() > N1 ? N1 : list.size();
    const std::initializer_list<T> *lists = list.begin();
    for(int i = 0; i < size; i++)
      memcpy(data[i], lists[i].begin(), (lists[i].size() > N2 ? N2 : lists[i].size()) * sizeof(T));
  }

  constexpr size_t rowCount() const { return N1; }
  constexpr size_t columnCount() const { return N2; }

  T& operator()(int row, int column){
    if (row >= N1 || row < 0) {
      sprintf(error_buffer, "Matrix row out of bounds: %i", row);
      throw std::exception(error_buffer);
    } else if (column >= N2 || column < 0) {
      sprintf(error_buffer, "Matrix column out of bounds: %i", column);
      throw std::exception(error_buffer);
    } else {
      return data[row][column];
    }
  }

  const T& operator()(int row, int column) const {
    if (row >= N1 || row < 0) {
      sprintf(error_buffer, "Matrix row out of bounds: %i", row);
      throw std::exception(error_buffer);
    } else if (column >= N2 || column < 0) {
      sprintf(error_buffer, "Matrix column out of bounds: %i", column);
      throw std::exception(error_buffer);
    } else {
      return data[row][column];
    }
  }



  Matrix<T, N1, N2> operator+() const {
    return *this;
  }

  Matrix<T, N1, N2> operator-() const {
    Matrix<T, N1, N2> ret;
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        ret.data[i][j] = -data[i][j];
    return ret;
  }



  Matrix<T, N1, N2> operator+(const Matrix<T, N1, N2> &matrix) const {
    Matrix<T, N1, N2> ret;
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        ret.data[i][j] = data[i][j] + matrix.data[i][j];
    return ret;
  }

  Matrix<T, N1, N2> operator-(const Matrix<T, N1, N2> &matrix) const {
    Matrix<T, N1, N2> ret;
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        ret.data[i][j] = data[i][j] - matrix.data[i][j];
    return ret;
  }

  Matrix<T, N1, N2> operator*(const T &k) const {
    Matrix<T, N1, N2> ret;
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        ret.data[i][j] = data[i][j] * k;
    return ret;
  }

  Matrix<T, N1, N2> operator/(const T &k) const {
    Matrix<T, N1, N2> ret;
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        ret.data[i][j] = data[i][j] / k;
    return ret;
  }



  const Matrix<T, N1, N2> &operator+=(const Matrix<T, N1, N2> &matrix) const {
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        data[i][j] += matrix.data[i][j];
    return *this;
  }

  const Matrix<T, N1, N2> &operator-=(const Matrix<T, N1, N2> &matrix) const {
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        data[i][j] += matrix.data[i][j];
    return *this;
  }

  const Matrix<T, N1, N2> &operator*=(const T &k) const {
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        data[i][j] *= k;
    return *this;
  }

  const Matrix<T, N1, N2> &operator/=(const T &k) const {
    for(int i = 0; i < N1; i++)
      for(int  j= 0; j < N2; j++)
        data[i][j] /= k;
    return *this;
  }



  template<int N3>
  Matrix<T, N1, N3> operator*(const Matrix<T, N2, N3> &matrix) const {
    Matrix<T, N1, N3> ret;
    for(int i = 0; i < N1; i++)
      for(int j = 0; j < N3; j++)
        for(int k = 0; k < N2; k++)
          ret.data[i][j] += data[i][k] * matrix.data[k][j];
    return ret;
  }

  Vector<T, N1> operator*(const Vector<T, N2> &vector) const {
    Vector<T, N1> ret;
    T *p_ret = TYPE_PUN_POINTER(ret, T), *p_vector = TYPE_PUN_POINTER(vector, T);
    for(int i = 0; i < N1; i++)
      for(int j = 0; j < N2; j++)
          p_ret[i] += data[i][j] * p_vector[j];
    return ret;
  }

  friend std::ostream &operator<<(std::ostream &ostream, Matrix<T, N1, N2> matrix) {
    std::stringstream ss = std::stringstream();
    std::string numbers[N1][N2];
    for(int i = 0; i < N2; i++){
      for(int j = 0; j < N1; j++){
        ss << std::setprecision(log10(matrix(j, i))) << matrix(j, i);
        ss.str("");
        ss.clear();
      }
    }
    for(int i = 0; i < N1; i++){
      ostream << "|";
      for (int j = 0; j < N2; j++) {
        ostream << matrix.data[i][j];
        if (j < N2 - 1)
         ostream << " ";
      }
      ostream << "|\n";
    }
    return ostream;
  }
};

_GEAR_END