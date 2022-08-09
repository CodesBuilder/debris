#include <debris/render.h>

static Debris::IRenderer* _renderer=nullptr;

Debris::BasicRenderContext::BasicRenderContext() {
}

Debris::BasicRenderContext::~BasicRenderContext() {
}

void Debris::BasicRenderContext::lock() {
	_lock.lock();
}

void Debris::BasicRenderContext::unlock() {
	_lock.unlock();
}

void Debris::setRenderer(IRenderer* renderer) {
	_renderer = renderer;
}

Debris::IRenderer* Debris::getRenderer() {
	return _renderer;
}
