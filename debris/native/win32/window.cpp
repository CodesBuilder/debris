#include "window.h"
#include <comdef.h>

LPCWSTR Debris::Native::wndClassName = L"DEBRISWINDOW";
static WNDCLASSW _wndClass = { 0 };
static std::unordered_map<HWND, Debris::Native::BasicWindow*> _windows; // All windows created by Debris.

LRESULT Debris::Native::BasicWindow::_wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	auto window = _windows[hWnd];
	switch (uMsg) {
		case WM_SIZE:
			window->onSizeChanged(Size2D(LOWORD(lParam), HIWORD(lParam)));
			break;
		case WM_MOVE:
			window->onPosChanged(Pos2D(LOWORD(lParam), HIWORD(lParam)));
			window->update();
			break;
		case WM_LBUTTONDOWN:
			switch (wParam) {
				case MK_LBUTTON:
					window->onMouseButtonDown(MOUSEBUTTON_LEFT, { LOWORD(lParam), HIWORD(lParam) });
				case MK_CONTROL:
					break;
				case MK_SHIFT:
					break;
			}
			break;
		case WM_LBUTTONUP:
			window->onMouseButtonUp(MOUSEBUTTON_LEFT, { LOWORD(lParam), HIWORD(lParam) });
			break;
		case WM_SHOWWINDOW:
			window->setVisible(wParam == TRUE);
			break;
		case WM_CLOSE:
			window->onClose();
			break;
		case WM_DESTROY:
			_windows.erase(hWnd);
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Debris::Native::BasicWindow::BasicWindow() {
	hWnd = CreateWindowW(
		Debris::Native::wndClassName,
		L"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	if (!hWnd)
		throw std::runtime_error("Error creating window");
	renderContext = Debris::getRenderer()->createContext(this);
	_windows[hWnd] = this;
}

Debris::Native::BasicWindow::~BasicWindow() {
	onDestroy();
	delete renderContext;

	DestroyWindow(hWnd);
	_windows.erase(hWnd);
}

void Debris::Native::BasicWindow::update() {
	for (auto i : children)
		i->update();
	UpdateWindow(hWnd);
}

void Debris::Native::BasicWindow::setPos(Pos2D& pos) {
	RECT rect;
	GetWindowRect(hWnd, &rect);
	rect.right = pos.x + (rect.right - rect.left);
	rect.bottom = pos.y + (rect.bottom - rect.top);
	rect.left = pos.x;
	rect.top = pos.y;
}

Debris::Pos2D Debris::Native::BasicWindow::getPos() {
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return Pos2D(rect.left, rect.top);
}

void Debris::Native::BasicWindow::setSize(Debris::Size2D& pos) {
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = pos.x;
	rect.bottom = pos.y;
	AdjustWindowRect(&rect, GetWindowLong(hWnd, GWL_STYLE), FALSE);
}

Debris::Size2D Debris::Native::BasicWindow::getSize() {
	RECT rect;
	GetClientRect(hWnd, &rect);
	return Size2D(rect.right - rect.left, rect.bottom - rect.top);
}

void Debris::Native::BasicWindow::setVisible(bool visible) {
	ShowWindowAsync(hWnd, SW_SHOW);
}

bool Debris::Native::BasicWindow::getVisible() {
	return (bool)IsWindowVisible(hWnd);
}

Debris::IElement* Debris::Native::BasicWindow::getParent() {
	const HWND parentHwnd = GetParent(hWnd);

	if (_windows.count(parentHwnd))
		return _windows[parentHwnd];
	return nullptr;
}

void Debris::Native::BasicWindow::setTitle(std::wstring title) {
	SetWindowTextW(hWnd, title.c_str());
}

std::wstring Debris::Native::BasicWindow::getTitle() {
	int len = GetWindowTextLengthW(hWnd) + 1;
	WCHAR* title = new WCHAR[len];
	GetWindowTextW(hWnd, title, len);

	std::wstring titleStr(title);
	delete[] title;

	return titleStr;
}

Debris::Native::BasicWindow::iterator Debris::Native::BasicWindow::begin() {
	return children.begin();
}

Debris::Native::BasicWindow::iterator Debris::Native::BasicWindow::end() {
	return children.end();
}

void Debris::Native::BasicWindow::addChild(IElement* e) {
	children.insert(e);
}

void Debris::Native::BasicWindow::removeChild(iterator& w) {
	children.erase(w);
}

void Debris::Native::BasicWindow::onParentChanged(IElement* element) {
	auto window = std::find_if(
		_windows.begin(), _windows.end(),
		[element](std::pair<const HWND, Debris::Native::BasicWindow*>& i) -> bool {
			if (i.second == element)
				return true;
			return false;
		});
	if (window == _windows.end())
		throw std::runtime_error("");
	SetParent(hWnd, window->first);
}

void Debris::Native::BasicWindow::onDestroy() {
	for (auto i : children)
		delete i;
}

void Debris::Native::BasicWindow::onMouseHover() {
}

void Debris::Native::BasicWindow::onMouseLeave() {
}

void Debris::Native::BasicWindow::onClose() {
	delete this;
}

void Debris::Native::BasicWindow::onMouseButtonDown(MouseButton button, const Pos2D& cursorPos) {
	for (auto i : children) {
		if (!(i->getVisible()))
			continue;
		Pos2D pos = i->getPos();
		Size2D size = i->getSize();
		if ((cursorPos.x >= pos.x) && (cursorPos.x <= pos.x + size.x) &&
			(cursorPos.y >= pos.y) && (cursorPos.y <= pos.y + size.y)) {
			i->onMouseButtonDown(button, cursorPos);
			break;
		}
	}
}

void Debris::Native::BasicWindow::onMouseButtonUp(MouseButton button, const Pos2D& cursorPos) {
	for (auto i : children) {
		if (!(i->getVisible()))
			continue;
		Pos2D pos = i->getPos();
		Size2D size = i->getSize();
		if ((cursorPos.x >= pos.x) && (cursorPos.x <= pos.x + size.x) &&
			(cursorPos.y >= pos.y) && (cursorPos.y <= pos.y + size.y)) {
			i->onMouseButtonUp(button, cursorPos);
			break;
		}
	}
}

void Debris::Native::BasicWindow::onPosChanged(Pos2D& pos) {
}

void Debris::Native::BasicWindow::onSizeChanged(Size2D& size) {
	renderContext->onResize();
}

void Debris::Native::BasicWindow::pollEvents() {
	MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND Debris::Native::BasicWindow::_getHwnd() {
	return hWnd;
}

bool Debris::Native::BasicWindow::_init() {
	if (FAILED(CoInitialize(0)))
		return false;

	_wndClass.lpszClassName = wndClassName;
	_wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	_wndClass.hCursor = LoadIcon((HINSTANCE)GetModuleHandle(NULL), IDC_ARROW);
	_wndClass.hInstance = GetModuleHandle(NULL);
	_wndClass.lpfnWndProc = _wndProc;
	_wndClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

	if (!RegisterClassW(&_wndClass))
		return false;

	return true;
}

void Debris::Native::BasicWindow::_deinit() {
	for (auto i : _windows)
		DestroyWindow(i.second->hWnd);
	_windows.clear();
	UnregisterClassW(wndClassName, GetModuleHandle(NULL));

	CoUninitialize();
}
