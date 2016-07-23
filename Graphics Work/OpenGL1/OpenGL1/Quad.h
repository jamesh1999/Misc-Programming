#ifndef __QUAD_INCLUDED__
#define __QUAD_INCLUDED__

#include "Model.h"

namespace Rendering
{
	namespace Models
	{
		class Quad : public Models::Model
		{
		public:
			Quad();
			~Quad();

			void Create();
			virtual void Draw() override final;
			virtual void Update() override final;
		};
	}
}

#endif