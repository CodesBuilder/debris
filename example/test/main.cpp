#include <debris/debris.h>
#include <assert.h>

class ExampleWindow : public Debris::BasicWindow {
protected:
	bool closed;

public:
	ExampleWindow() : BasicWindow() {
		closed = false;
	}
	virtual ~ExampleWindow() {
	}
	virtual void onClose() {
		closed = true;
	}
	virtual bool getClosed() {
		return closed;
	}
	virtual void update() {
		renderContext->beginDraw();

		renderContext->clear(Debris::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		renderContext->drawRect(Debris::Pos2D{ 0, 0 }, Debris::Size2D{ 128, 128 }, Debris::ColorF{ 0.5f, 1.0f, 0.5f, 1.0f });
		BasicWindow::update();

		renderContext->endDraw();
	}
};

int main(int argc, char** argv) {
	assert(Debris::init());
	Debris::setRenderer(new DebrisRenderer::D3D11Renderer());

	ExampleWindow* window = new ExampleWindow();
	window->setVisible(true);

	while (!(window->getClosed()))
		window->pollEvents();

	delete window;

	Debris::deinit();
	return 0;
}
