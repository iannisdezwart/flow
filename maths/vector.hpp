#ifndef FLOW_VECTOR_HEADER
#define FLOW_VECTOR_HEADER

#include <bits/stdc++.h>

using namespace std;

namespace flow {
	template <size_t size, typename num_t = double>
	class Vector {
		public:
			num_t values[size];

			Vector() {}

			/**
			 *  @brief  Initialises this Vector with an initializer_list like:
			 *  { 1, 2, 3 }
			 */
			Vector(initializer_list<num_t> init)
			{
				size_t i = 0;

				for (num_t num : init) {
					values[i] = num;
					if (++i == size) break;
				}
			}

			/**
			 *  @brief  Gets the n-th value of the Vector.
			 *  @param  n  n.
			 */
			num_t operator[](size_t n) const
			{
				return values[n];
			}

			/**
			 *  @brief  Returns a reference to the n-th value of the Vector.
			 *  @param  n  n.
			 */
			num_t& operator[](size_t n)
			{
				return values[n];
			}

			/**
			 *  @brief  Creates a new Vector that is the sum of two other Vectors.
			 */
			Vector<size, num_t> operator+(const Vector<size, num_t>& other_vector)
			{
				Vector<size, num_t> res;

				for (size_t i = 0; i < size; i++) {
					res[i] = values[i] + other_vector[i];
				}

				return res;
			}

			/**
			 *  @brief  Adds another Vector to this Vector.
			 */
			void operator+=(const Vector<size, num_t>& other_vector)
			{
				for (size_t i = 0; i < size; i++) {
					values[i] += other_vector[i];
				}
			}

			/**
			 *  @brief  Creates a new Vector that is the product of a Vector and a
			 * 	scalar.
			 */
			Vector<size, num_t> operator*(num_t scalar)
			{
				Vector<size, num_t> res;

				for (size_t i = 0; i < size; i++) {
					res[i] = values[i] * scalar;
				}

				return res;
			}

			/**
			 *  @brief  Multiplies this Vector with a scalar.
			 */
			void operator*=(num_t scalar)
			{
				for (size_t i = 0; i < size; i++) {
					values[i] *= scalar;
				}
			}

			/**
			 *  @brief  Creates a new Vector that is the quotient of a Vector and a
			 * 	scalar.
			 */
			Vector<size, num_t> operator/(num_t scalar)
			{
				Vector<size, num_t> res;

				for (size_t i = 0; i < size; i++) {
					res[i] = values[i] / scalar;
				}

				return res;
			}

			/**
			 *  @brief  Divides this Vector by a scalar.
			 */
			void operator/=(num_t scalar)
			{
				for (size_t i = 0; i < size; i++) {
					values[i] /= scalar;
				}
			}

			/**
			 *  @brief  Computes the dot product two Vectors.
			 */
			num_t operator*(const Vector<size, num_t>& other_vector)
			{
				num_t res = 0;

				for (size_t i = 0; i < size; i++) {
					res += values[i] * other_vector[i];
				}

				return res;
			}

			/**
			 *  @brief  Computes the cross product of two 3D Vectors.
			 */
			static Vector<3, num_t> cross_product(
				const Vector<3, num_t>& v1, const Vector<3, num_t>& v2)
			{
				Vector<3, num_t> v = {
					v1[1] * v2[2] - v1[2] * v2[1],
					v1[2] * v2[0] - v1[0] * v2[2],
					v1[0] * v2[1] - v1[1] * v2[0]
				};

				return v;
			}

			/**
			 *  @brief  Returns a String representation of this Vector.
			 */
			String to_string()
			{
				String str = "{";

				for (size_t i = 0; i < size; i++) {
					if (i == 0) str += " "; // Leading space

					String num = String::from_num(values[i]);
					str += num;

					if (i != size - 1) str += ", "; // Comma seperator
					else str += " "; // Trailing space
				}

				str += "}";
				return str;
			}
	};
};

#endif