#ifndef __COMPONENT_INCLUDED__
#define __COMPONENT_INCLUDED__

class CompositeObject;
class Component
{
public:
	CompositeObject* obj;
	virtual ~Component() {}
};

#endif