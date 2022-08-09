#ifndef DEBRIS_RENDER_H_
#define DEBRIS_RENDER_H_

#include "color.h"
#include "window.h"

#include <mutex>

namespace Debris {
	template <typename T>
	class IBitmap {
	public:
		virtual ~IBitmap(){};
		virtual Size2D size() = 0;
		virtual void resize(Size2D& size) = 0;
		virtual T& operator[](const Pos2D& pos) = 0;
		virtual T& operator[](size_t index) = 0;

		IBitmap& operator=(const IBitmap&) = delete;
	};

	class IRenderContext {
	public:
		virtual ~IRenderContext(){};

		virtual void lock() = 0;
		virtual void unlock() = 0;

		virtual void beginDraw() = 0;
		virtual void endDraw() = 0;

		virtual void drawRect(Pos2D& pos, Size2D& size, ColorI& color) = 0;
		virtual void drawRect(Pos2D& pos, Size2D& size, ColorF& color) = 0;

		virtual void clear(ColorI& color) = 0;
		virtual void clear(ColorF& color) = 0;

		virtual void onResize() = 0;

		IRenderContext& operator=(const IRenderContext&) = delete;

		template <typename T>
		static inline void typeCheck(IRenderContext* renderer) {
			if (typeid(*renderer) != typeid(T))
				throw std::runtime_error("Incompatible renderer type");
		}
	};

	class IRenderer {
	public:
		virtual ~IRenderer() {}

		virtual IRenderContext* createContext(Window* window) = 0;
		IRenderer& operator=(const IRenderer&) = delete;
	};

	class BasicRenderContext : public IRenderContext {
	protected:
		std::mutex _lock;

	public:
		BasicRenderContext();
		virtual ~BasicRenderContext();
		virtual void lock();
		virtual void unlock();
	};

	void setRenderer(IRenderer* _renderer);
	IRenderer* getRenderer();
}

#endif
