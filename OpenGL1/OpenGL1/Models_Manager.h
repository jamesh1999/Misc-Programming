#ifndef __MODELS_MANAGER_INCLUDED__
#define __MODELS_MANAGER_INCLUDED__

#include <map>
#include "Shader_Manager.h"
#include "IGameObject.h"
#include "Triangle.h"
#include "Quad.h"

using namespace Rendering;

namespace Managers
{

	class Models_Manager
	{
	public:
		Models_Manager();
		~Models_Manager();

		void Draw();
		void Update();
		void DeleteModel(const std::string& gameModelName);
		const IGameObject& GetModel(const std::string& gameModelName) const;

	private:
		std::map<std::string, IGameObject*> gameModelList;
	};

}

#endif