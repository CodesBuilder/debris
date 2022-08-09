#ifndef _DEBRIS_NATIVE_WIN32_WINDOW_H
#define _DEBRIS_NATIVE_WIN32_WINDOW_H

#include <debris/window.h>
#include <debris/render.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Debris {
	namespace Native {
		extern LPCWSTR wndClassName;

		class BasicWindow : public Window {
		protected:
			IRenderContext* renderContext;
			HWND hWnd;

		public:
			static LRESULT CALLBACK _wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			BasicWindow();
			virtual ~BasicWindow();

			virtual void update() override;

			virtual void setPos(Pos2D& pos) override;
			virtual Debris::Pos2D getPos() override;

			virtual void setSize(Size2D& pos) override;
			virtual Debris::Size2D getSize() override;

			virtual iterator begin() override;
			virtual iterator end() override;
			virtual void addChild(IElement* e) override;
			virtual void removeChild(iterator& w) override;

			virtual void setVisible(bool visible) override;
			virtual bool getVisible() override;

			virtual Debris::IElement* getParent() override;

			virtual void setTitle(std::wstring title);
			virtual std::wstring getTitle();

			virtual void onParentChanged(Debris::IElement* element) override;
			virtual void onPosChanged(Debris::Pos2D& pos) override;
			virtual void onSizeChanged(Size2D& size) override;
			virtual void onDestroy() override;
			virtual void onMouseHover() override;
			virtual void onMouseLeave() override;
			virtual void onMouseButtonDown(Debris::MouseButton button, const Debris::Pos2D& cursorPos) override;
			virtual void onMouseButtonUp(Debris::MouseButton button, const Debris::Pos2D& cursorPos) override;

			virtual void onClose();

			virtual void pollEvents();

			virtual HWND _getHwnd();

			static bool _init();
			static void _deinit();

			BasicWindow& operator=(const BasicWindow&) = delete;
		};
	}
}

#endif
