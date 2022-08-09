#include <debris/element.h>

void Debris::NestedElement::addChild(IElement* e) {
	children.insert(e);
}

void Debris::NestedElement::removeChild(ChildList::iterator& w) {
	children.erase(w);
}
