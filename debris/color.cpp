#include "color.h"

Debris::ColorI::ColorI() noexcept {
	r = 0, g = 0, b = 0, a = 0;
}

Debris::ColorI::ColorI(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept {
	this->r = r, this->g = g, this->b = b, this->a = a;
}

Debris::ColorF::ColorF() noexcept {
	r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
}

Debris::ColorF::ColorF(float r, float g, float b, float a) noexcept {
	this->r = r, this->g = g, this->b = b, this->a = a;
}
