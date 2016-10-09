#ifndef __SCRIPT_INCLUDED__
#define __SCRIPT_INCLUDED__

#include "Component.h"

class Script : public Component
{
public:
	unsigned m_id;
	virtual void Update() = 0;
};

#endif