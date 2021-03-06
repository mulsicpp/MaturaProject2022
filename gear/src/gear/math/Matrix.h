#pragma once

#include <gear/core/core.h>
#include "Vector.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iomanip>

_GEAR_START

/*
This class represents a mathematical matrix. The type of the elements is
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
template <class T, int N1, int N2>
class Matrix
{
private:
    T data[N1][N2];

    template <class X1, class X2, class... Xs>
    void put_Elements(int index, X1 first, X2 second, Xs... rest)
    {
        if (index < N1 * N2)
        {
            ((T *)data)[index] = (T)first;
            put_Elements(index + 1, second, rest...);
        }
    }

    template <class X, class... Xs>
    void put_Elements(int index, X first)
    {
        if (index < N1 * N2)
        {
            ((T *)data)[index] = (T)first;
        }
    }

public:
    /*
    Creates a matrix with all elements set to 0.
    */
    Matrix<T, N1, N2>() : data{0} {}

    /*
    Creates a matrix with values.

    @param list a 2D initializer list containing the values
    */
    template <class... Ts>
    Matrix<T, N1, N2>(T first, Ts... rest) : data{0}
    {
        put_Elements(0, first, rest...);
    }

    /*
    @return the amount of rows in the matrix
    */
    constexpr size_t row_Count() const { return N1; }

    /*
    @return the amount of columns in the matrix
    */
    constexpr size_t column_Count() const { return N2; }

    /*
    @param row the row of the element. It has to be greater or equal to 0 and smaller than row count
    @param column the column of the element. It has to be greater or equal to 0 and smaller than column count
    @return a reference to the element in the matrix at the specified row and column.
    */
    T &operator()(int row, int column)
    {
        if (row >= N1 || row < 0)
        {
            error("Matrix row out of bounds: %i", row);
        }
        else if (column >= N2 || column < 0)
        {
            error("Matrix column out of bounds: %i", column);
        }
        else
        {
            return data[row][column];
        }
    }

    /*
    @param row the row of the element. It has to be greater or equal to 0 and smaller than row count
    @param column the column of the element. It has to be greater or equal to 0 and smaller than column count
    @return a const reference to the element in the matrix at the specified row and column.
    */
    const T &operator()(int row, int column) const
    {
        if (row >= N1 || row < 0)
        {
            error("Matrix row out of bounds: %i", row);
        }
        else if (column >= N2 || column < 0)
        {
            error("Matrix column out of bounds: %i", column);
        }
        else
        {
            return data[row][column];
        }
    }

    /*
    @return a copy of the matrix
    */
    Matrix<T, N1, N2> operator+() const
    {
        return *this;
    }

    /*
    @return a negated copy of the matrix
    */
    Matrix<T, N1, N2> operator-() const
    {
        Matrix<T, N1, N2> ret;
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                ret.data[i][j] = -data[i][j];
        return ret;
    }

    /*
    Adds two matrices together.

    @param matrix the matrix, that gets added to this matrix
    @return the result of the addition
    */
    Matrix<T, N1, N2> operator+(const Matrix<T, N1, N2> &matrix) const
    {
        Matrix<T, N1, N2> ret;
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                ret.data[i][j] = data[i][j] + matrix.data[i][j];
        return ret;
    }

    /*
    Subtracts a matrix form this matrix.

    @param matrix the matrix, that gets subtracted this matrix
    @return the result of the subtraction
    */
    Matrix<T, N1, N2> operator-(const Matrix<T, N1, N2> &matrix) const
    {
        Matrix<T, N1, N2> ret;
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                ret.data[i][j] = data[i][j] - matrix.data[i][j];
        return ret;
    }

    /*
    Multiplies the matrix with a scalar value.

    @param k the scalar value
    @return the result of the multiplication
    */
    Matrix<T, N1, N2> operator*(const T &k) const
    {
        Matrix<T, N1, N2> ret;
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                ret.data[i][j] = data[i][j] * k;
        return ret;
    }

    /*
    Devides the matrix with a scalar value.

    @param k the scalar value
    @return the result of the division
    */
    Matrix<T, N1, N2> operator/(const T &k) const
    {
        Matrix<T, N1, N2> ret;
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                ret.data[i][j] = data[i][j] / k;
        return ret;
    }

    /*
    Adds a matrix to the current matrix.

    @param matrix the matrix to be added
    @return a reference to this matrix
    */
    const Matrix<T, N1, N2> &operator+=(const Matrix<T, N1, N2> &matrix) const
    {
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                data[i][j] += matrix.data[i][j];
        return *this;
    }

    /*
    Subtracts a matrix to the current matrix.

    @param matrix the matrix to be subtracted
    @return a reference to this matrix
    */
    const Matrix<T, N1, N2> &operator-=(const Matrix<T, N1, N2> &matrix) const
    {
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                data[i][j] += matrix.data[i][j];
        return *this;
    }

    /*
    Multiplies the current matrix with a scalar value.

    @param k the scalar value t obe multiplied by
    @return a reference to this matrix
    */
    const Matrix<T, N1, N2> &operator*=(const T &k) const
    {
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                data[i][j] *= k;
        return *this;
    }

    /*
    Divides the current matrix with a scalar value.

    @param k the scalar value to be divided by
    @return a reference to this matrix
    */
    const Matrix<T, N1, N2> &operator/=(const T &k) const
    {
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                data[i][j] /= k;
        return *this;
    }

    /*
    Multiplies the matirx with another matrix.

    @param matrix the second matrix
    @return the result of the multiplication
    */
    template <int N3>
    Matrix<T, N1, N3> operator*(const Matrix<T, N2, N3> &matrix) const
    {
        Matrix<T, N1, N3> ret;
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N3; j++)
                for (int k = 0; k < N2; k++)
                    ret.data[i][j] += data[i][k] * matrix.data[k][j];
        return ret;
    }

    /*
    Multiplies the matirx with a vector.

    @param vector the vector
    @return the result of the multiplication
    */
    Vector<T, N1> operator*(const Vector<T, N2> &vector) const
    {
        Vector<T, N1> ret;
        T *p_ret = GEAR_TYPE_PUN_POINTER(ret, T), *p_vector = GEAR_TYPE_PUN_POINTER(vector, T);
        for (int i = 0; i < N1; i++)
            for (int j = 0; j < N2; j++)
                p_ret[i] += data[i][j] * p_vector[j];
        return ret;
    }

    /*
    Cast the matrix to a new matrix with a different element type, row count and column count.
    For this to be possible, there has to be defined a cast from the current to
    the new type of the elements. If the new amount of elements is smaller, only
    elements in that range will be returned. If the new amount of elements is
    greater, every element out of range will be set to 0.

    @return the new matrix with the new row count, column count and/or type
    */
    template <class T2, int N3, int N4>
    Matrix<T2, N3, N4> cast_To() const
    {
        Matrix<T2, N3, N4> ret;
        size_t size_1 = N1 < N3 ? N1 : N3, size_2 = N2 < N4 ? N2 : N4;
        for (int i = 0; i < size_1; i++)
            for (int j = 0; j < size_2; j++)
                ret(i, j) = (T)(data[i][j]);
        return ret;
    }

    /*
    Prints the matrix to an output stream.

    @param ostream the output stream
    @param the matrix to be printed
    */
    friend std::ostream &operator<<(std::ostream &ostream, Matrix<T, N1, N2> matrix)
    {
        std::stringstream ss = std::stringstream();
        std::string numbers[N1][N2];
        for (int i = 0; i < N2; i++)
        {
            for (int j = 0; j < N1; j++)
            {
                ss << std::setprecision(log10(matrix(j, i))) << matrix(j, i);
                ss.str("");
                ss.clear();
            }
        }
        for (int i = 0; i < N1; i++)
        {
            ostream << "|";
            for (int j = 0; j < N2; j++)
            {
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