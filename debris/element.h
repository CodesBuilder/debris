#ifndef DEBRIS_ELEMENT_H_
#define DEBRIS_ELEMENT_H_

#include "math.hpp"
#include <unordered_set>

namespace Debris {
	struct Pos2D : public Vec<int, 2> {
		int& x = data[0];
		int& y = data[1];
		inline Pos2D(Pos2D& data) : Vec<int, 2>(data) {
		}
		inline Pos2D(int x, int y) : Vec<int, 2>({ x, y }) {
		}
	};
	struct Pos3D : public Vec<int, 3> {
		int& x = data[0];
		int& y = data[1];
		int& z = data[2];
		inline Pos3D(Pos3D& data) : Vec<int, 3>(data) {
		}
	};
	using Size2D = Pos2D;
	using Size3D = Pos3D;

	enum MouseButton {
		MOUSEBUTTON_LEFT = 0,
		MOUSEBUTTON_RIGHT,
		MOUSEBUTTON_MIDDLE
	};

	class IElement {
	public:
		virtual ~IElement() {}

		virtual void update() = 0;

		virtual void setPos(Pos2D& pos) = 0;
		virtual Pos2D getPos() = 0;

		virtual void setSize(Size2D& pos) = 0;
		virtual Size2D getSize() = 0;

		virtual bool getVisible() = 0;
		virtual void setVisible(bool visible) = 0;

		virtual IElement* getParent() = 0;

		// Events
		virtual void onParentChanged(IElement* element) = 0;
		virtual void onPosChanged(Pos2D& pos) = 0;
		virtual void onSizeChanged(Size2D& size) = 0;
		virtual void onDestroy() = 0;
		virtual void onMouseHover() = 0;
		virtual void onMouseLeave() = 0;
		virtual void onMouseButtonDown(MouseButton button, const Pos2D& cursorPos) = 0;
		virtual void onMouseButtonUp(MouseButton button, const Pos2D& cursorPos) = 0;

		IElement& operator=(IElement& a) = delete;
	};

	class NestedElement : public IElement {
	public:
		using ChildList = std::unordered_set<IElement*>;
		using iterator = ChildList::iterator;
		virtual ~NestedElement() {}

		virtual ChildList::iterator begin() = 0; // Get iterator of first child.
		virtual ChildList::iterator end() = 0;	 // Get iterator of last child.
		virtual void addChild(IElement* e);
		virtual void removeChild(ChildList::iterator& w);

		NestedElement& operator=(NestedElement& a) = delete;

	protected:
		ChildList children;
	};
}

#endif
