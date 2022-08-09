#ifndef DEBRIS_DEBRIS_H_
#define DEBRIS_DEBRIS_H_

#include "window.h"
#include "renderers.h"

#ifdef _WIN32
#include "native/win32/window.h"
#endif

namespace Debris {
	bool init();
	void deinit();

	using Native::BasicWindow;
}

#endif
