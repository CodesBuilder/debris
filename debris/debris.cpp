#include "debris.h"

bool Debris::init() {
	return Debris::BasicWindow::_init();
}

void Debris::deinit()
{
	Debris::BasicWindow::_deinit();
}
