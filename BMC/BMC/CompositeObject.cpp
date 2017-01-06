#include "stdafx.h"
#include "CompositeObject.h"
#include "Renderer.h"

CompositeObject::CompositeObject()
{
}


CompositeObject::~CompositeObject()
{
}

template <>
Transform* CompositeObject::GetComponent<Transform>()
{
	return m_transform;
}

template <class T>
T* CompositeObject::GetComponent()
{
	for (int i = 0; i < m_components.size(); ++i)
	{
		if (dynamic_cast<T*>(m_components[i]) != nullptr)
			return dynamic_cast<T*>(m_components[i]);
	}

	return nullptr;
}

template<>
void CompositeObject::AttachComponent<Transform>(Transform* t)
{
	m_transform = t;
	t->obj = this;
}

template<class T>
void CompositeObject::AttachComponent(T* component)
{
	m_components.push_back(component);
	reinterpret_cast<Component*>(component)->obj = this;
}