#pragma once

#include <map>
#include <memory>

#include "Component.hpp"

namespace EVA
{
	// Macro for registering a component. Should be put inside the class declaration
	#define REGISTER_COMPONENT_HPP(TYPE) \
    static ComponentRegister<TYPE> m_Register;

	// Macro for registering a component
	#define REGISTER_COMPONENT_CPP(TYPE, NAME) \
    ComponentRegister<TYPE> TYPE::m_Register(NAME);

	/**
	 * \brief Keep track of all registerd components
	 */
	class ComponentMap
	{
	public:

		typedef std::map<std::string, std::shared_ptr<Component>(*)()> map_type;
		static map_type * map;

		template<typename T>
		static Component * CreateInstance()
		{
			return new T;
		}

		static map_type * GetMap() 
		{
			if (!map)
			{
				map = new map_type();
			}
			return map;
		}

		static std::shared_ptr<Component> CreateComponent(const std::string& id)
		{
			const auto it = GetMap()->find(id);

			if (it == GetMap()->end())
				return nullptr;

			return it->second();
		}

		template<typename T>
		static std::shared_ptr<Component> CreateT()
		{
			return std::make_shared<T>();
		}
	};

	template<typename T>
	struct ComponentRegister 
	{
		ComponentRegister(std::string const& s)
		{
			ComponentMap::GetMap()->insert(std::make_pair(s, &ComponentMap::CreateT<T>));
		}
	};
}