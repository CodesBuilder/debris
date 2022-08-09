#ifndef DEBRIS_WINDOW_H_
#define DEBRIS_WINDOW_H_

#include "element.h"

namespace Debris {
	class Window : public NestedElement {
	public:
		virtual ~Window(){};

		virtual void onClose() = 0;
		virtual void pollEvents() = 0;

		virtual void setTitle(std::wstring title) = 0;
		virtual std::wstring getTitle() = 0;
	};
}

#endif
