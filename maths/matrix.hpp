#ifndef FLOW_MATRIX_HEADER
#define FLOW_MATRIX_HEADER

#include <bits/stdc++.h>
#include "vector.hpp"

using namespace std;

namespace flow {
	template <size_t size_y, size_t size_x, typename num_t = double>
	class Matrix {
		public:
			Vector<size_x, num_t> values[size_y];

			Matrix() {}

			/**
			 *  @brief  Initialises this Matrix with an initializer_list like:
			 *  { { 1, 2 }, { 3, 4 } }
			 */
			Matrix(initializer_list<initializer_list<num_t>> init)
			{
				size_t y = 0;

				for (initializer_list<num_t> init_row : init) {
					size_t x = 0;

					for (num_t num : init_row) {
						values[y][x] = num;

						if (++x == size_x) break;
					}

					if (++y == size_y) break;
				}
			}

			/**
			 *  @brief  Initialises all elements of this Matrix with a value.
			 *  @param  value  The initial value of each element.
			 */
			Matrix(num_t value)
			{
				for (size_t y = 0; y < size_y; y++)
					for (size_t x = 0; x < size_x; x++)
						values[y][x] = value;
			}

			/**
			 *  @brief  Returns a reference to the y-th row of the Matrix.
			 */
			Vector<size_x, num_t>& operator[](size_t y)
			{
				return values[y];
			}

			/**
			 *  @brief  Returns a copy of the x-th column of this Matrix as a Vector.
			 */
			Vector<size_y, num_t> get_col(size_t x)
			{
				Vector<size_y, num_t> v;

				for (size_t y = 0; y < size_y; y++) {
					v[y] = values[y][x];
				}

				return v;
			}

			/**
			 *  @brief  Returns a copy of the x-th row of this Matrix as a Vector.
			 */
			Vector<size_x, num_t> get_row(size_t y)
			{
				Vector<size_x, num_t> v;

				for (size_t x = 0; x < size_x; x++) {
					v[x] = values[y][x];
				}

				return v;
			}

			/**
			 *  @brief  Creates a new Matrix which is the product of this Matrix
			 *  and a scalar.
			 */
			Matrix<size_y, size_x, num_t> operator*(num_t scalar)
			{
				Matrix<size_y, size_x, num_t> m;

				for (size_t y = 0; y < size_y; y++) {
					for (size_t x = 0; x < size_x; x++) {
						m[y][x] = values[y][x] * scalar;
					}
				}

				return m;
			}

			/**
			 *  @brief  Multiplies this Matrix by a scalar.
			 */
			void operator*=(num_t scalar)
			{
				for (size_t y = 0; y < size_y; y++) {
					for (size_t x = 0; x < size_x; x++) {
						values[y][x] *= scalar;
					}
				}
			}

			/**
			 *  @brief  Creates a new Matrix which is the quotient of this Matrix
			 *  and a scalar.
			 */
			Matrix<size_y, size_x, num_t> operator/(num_t scalar)
			{
				Matrix<size_y, size_x, num_t> m;

				for (size_t y = 0; y < size_y; y++) {
					for (size_t x = 0; x < size_x; x++) {
						m[y][x] = values[y][x] / scalar;
					}
				}

				return m;
			}

			/**
			 *  @brief  Divides this Matrix by a scalar.
			 */
			void operator/=(num_t scalar)
			{
				for (size_t y = 0; y < size_y; y++) {
					for (size_t x = 0; x < size_x; x++) {
						values[y][x] /= scalar;
					}
				}
			}

			/**
			 *  @brief  Creates a Vector that is the product of this Matrix
			 *  and another Vector.
			 */
			Vector<size_y, num_t> operator*(Vector<size_x, num_t> vector)
			{
				Vector<size_y, num_t> v;

				for (size_t y = 0; y < size_y; y++) {
					v[y] = 0;

					for (size_t x = 0; x < size_x; x++) {
						v[y] += vector[x] * values[y][x];
					}
				}

				return v;
			}

			/**
			 *  @brief  Creates a new Matrix that is the product of this Matrix
			 *  and another Matrix.
			 */
			template <size_t size_o>
			Matrix<size_y, size_o, num_t> operator*(
				Matrix<size_x, size_o, num_t> other_matrix)
			{
				Matrix<size_y, size_o, num_t> m;

				for (size_t i = 0; i < size_y; i++) {
					for (size_t j = 0; j < size_o; j++) {
						m[i][j] = 0;

						for (size_t k = 0; k < size_x; k++) {
							m[i][j] += values[i][k] * other_matrix[k][j];
						}
					}
				}

				return m;
			}

			/**
			 *  @brief  Returns a String representation of this Matrix.
			 */
			String to_string()
			{
				String str = "{";

				for (size_t y = 0; y < size_y; y++) {
					if (y == 0) str += " "; // Leading space

					String vector_str = values[y].to_string();
					str += vector_str;

					if (y != size_y - 1) str += ", "; // Comma seperator
					else str += " "; // Trailing space
				}

				str += "}";
				return str;
			}

			/**
			 *  @brief  Returns a String representation of this Matrix with
			 * 	each row on its own line.
			 */
			String to_string_with_newlines()
			{
				String str = "{";

				for (size_t y = 0; y < size_y; y++) {
					if (y == 0) str += "\n\t"; // Leading newline and tab

					String vector_str = values[y].to_string();
					str += vector_str;

					if (y != size_y - 1) str += ",\n\t"; // Comma seperator
					else str += "\n"; // Trailing newline
				}

				str += "}";
				return str;
			}
	};
};

#endif