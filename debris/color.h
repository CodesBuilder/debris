#ifndef DEBRIS_BASE_COLOR_H_
#define DEBRIS_BASE_COLOR_H_

#include <stdint.h>

namespace Debris {

	struct ColorF;
	struct ColorI;

	struct ColorI final {
		uint8_t r, g, b, a;
		ColorI() noexcept;
		ColorI(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;
		inline ColorI& operator=(ColorI& x) noexcept;
		inline bool operator==(ColorI& x) noexcept;
		inline ColorI operator+(ColorI& x) noexcept;
		inline ColorI operator-(ColorI& x) noexcept;
		inline ColorI& operator+=(ColorI& x) noexcept;
		inline ColorI& operator-=(ColorI& x) noexcept;
		inline operator ColorF() noexcept;
	};

	struct ColorF final {
		float r, g, b, a;
		ColorF() noexcept;
		ColorF(float r, float g, float b, float a) noexcept;
		inline ColorF& operator=(ColorF& x) noexcept;
		inline bool operator==(ColorF& x) noexcept;
		inline ColorF operator+(ColorF& x) noexcept;
		inline ColorF operator-(ColorF& x) noexcept;
		inline ColorF& operator+=(ColorF& x) noexcept;
		inline ColorF& operator-=(ColorF& x) noexcept;
		inline operator ColorI() noexcept;
		inline ColorF& alphaBlendZero(ColorF& x, ColorF& y) noexcept;
		inline ColorF& alphaBlendOne(ColorF& x, ColorF& y) noexcept;
		inline ColorF& alphaBlendSource(ColorF& x, ColorF& y) noexcept;
		inline ColorF& alphaBlendDest(ColorF& x, ColorF& y) noexcept;
	};

	inline ColorI& ColorI::operator=(ColorI& x) noexcept {
		r = x.r, g = x.g, b = x.b, a = x.a;
		return *this;
	}
	inline bool ColorI::operator==(ColorI& x) noexcept {
		return (this->r == x.r) && (this->g == x.g) && (this->b == x.b) && (this->a == x.a);
	}
	inline ColorI ColorI::operator+(ColorI& x) noexcept {
		return ColorI(r + x.r, g + x.g, b + x.b, a + x.a);
	}
	inline ColorI ColorI::operator-(ColorI& x) noexcept {
		return ColorI(r - x.r, g - x.g, b - x.b, a - x.a);
	}
	inline ColorI& ColorI::operator+=(ColorI& x) noexcept {
		r += x.r, g += x.g, b += x.b, a += x.a;
		return *this;
	}
	inline ColorI& ColorI::operator-=(ColorI& x) noexcept {
		r -= x.r, g -= x.g, b -= x.b, a -= x.a;
		return *this;
	}
	inline ColorI::operator ColorF() noexcept {
		return ColorF((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
	}

	inline ColorF& ColorF::operator=(ColorF& x) noexcept {
		r = x.r, g = x.g, b = x.b, a = x.a;
		return *this;
	}
	inline bool ColorF::operator==(ColorF& x) noexcept {
		return (this->r == x.r) && (this->g == x.g) && (this->b == x.b) && (this->a == x.a);
	}
	inline ColorF ColorF::operator+(ColorF& x) noexcept {
		return ColorF(r + x.r, g + x.g, b + x.b, a + x.a);
	}
	inline ColorF ColorF::operator-(ColorF& x) noexcept {
		return ColorF(r - x.r, g - x.g, b - x.b, a - x.a);
	}
	inline ColorF& ColorF::operator+=(ColorF& x) noexcept {
		r += x.r, g += x.g, b += x.b, a += x.a;
		return *this;
	}
	inline ColorF& ColorF::operator-=(ColorF& x) noexcept {
		r -= x.r, g -= x.g, b -= x.b, a -= x.a;
		return *this;
	}
	inline ColorF::operator ColorI() noexcept {
		return ColorI((uint8_t)(r * 255.0f), (uint8_t)(g * 255.0f), (uint8_t)(b * 255.0f), (uint8_t)(a * 255.0f));
	}

	//
	// Alpha blend functions
	//
	inline ColorF& ColorF::alphaBlendZero(ColorF& x, ColorF& y) noexcept {
		return x;
	}

	inline ColorF& ColorF::alphaBlendOne(ColorF& x, ColorF& y) noexcept {
		return y;
	}

	inline ColorF& ColorF::alphaBlendSource(ColorF& x, ColorF& y) noexcept {
		return ColorF(
			x.r * x.a * y.r * (1.0f - x.a),
			x.g * x.a * y.g * (1.0f - x.a),
			x.b * x.a * y.b * (1.0f - x.a),
			1.0f);
	}

	inline ColorF& ColorF::alphaBlendDest(ColorF& x, ColorF& y) noexcept {
		return ColorF(
			x.r * (1.0f - y.a) * y.r,
			x.g * (1.0f - y.a) * y.g,
			x.b * (1.0f - y.a) * y.b,
			1.0f);
	}
}

#endif
