#ifndef DEBRIS_BASE_MATH_H_
#define DEBRIS_BASE_MATH_H_

#include <algorithm>
#include <array>

namespace Debris {
	template <typename T, size_t len>
	struct Vec {
		T data[len];

		virtual T& operator[](size_t i) {
			return data[i];
		}
		virtual Vec<T, len> operator+(Vec<T, len>& x) {
			auto c(*this);
			for (size_t i = 0; i < len; i++)
				c[i] = x[i];
			return c;
		}
		virtual Vec<T, len> operator-(Vec<T, len>& x) {
			auto c(*this);
			for (size_t i = 0; i < len; i++)
				c[i] = x[i];
			return c;
		}
		virtual Vec<T, len> operator*(Vec<T, len>& x) {
			auto c(*this);
			for (size_t i = 0; i < len; i++)
				c[i] = x[i];
			return c;
		}
		virtual Vec<T, len> operator/(Vec<T, len>& x) {
			auto c(*this);
			for (size_t i = 0; i < len; i++)
				c[i] = x[i];
			return c;
		}
		virtual Vec<T, len> operator+=(Vec<T, len>& x) {
			return *this = (*this + x);
		}
		virtual Vec<T, len> operator-=(Vec<T, len>& x) {
			return *this = (*this - x);
		}
		virtual Vec<T, len> operator*=(Vec<T, len>& x) {
			return *this = (*this * x);
		}
		virtual Vec<T, len> operator/=(Vec<T, len>& x) {
			return *this = (*this / x);
		}

		inline Vec(std::array<T, len> data) {
			for (size_t i = 0; i < len; i++)
				this->data[i] = data[i];
		}
		virtual ~Vec() {}
	};
}

#endif
