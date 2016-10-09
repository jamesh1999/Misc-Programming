#ifndef __COMPOSITE_OBJECT_INCLUDED__
#define __COMPOSITE_OBJECT_INCLUDED__

#include <vector>
#include "Component.h"
#include "Transform.h"

#define MAKE_COMPONENT(x)                  \
template                                   \
void CompositeObject::AttachComponent(x*); \
template                                   \
x* CompositeObject::GetComponent();

class Renderer;

class CompositeObject
{
private:
	std::vector<Component*> m_components;
	Transform* m_transform;

public:
	CompositeObject();
	~CompositeObject();

	void Update();

	template<class T>
	T* GetComponent();

	template<class T>
	void AttachComponent(T*);
};

MAKE_COMPONENT(Renderer);

#endif