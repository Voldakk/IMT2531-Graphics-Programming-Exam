#pragma once

#include <map>
#include <memory>

#include "Component.hpp"
#include <vector>

namespace EVA
{
	// Macro for registering a component. Should be put inside the class declaration
	#define REGISTER_COMPONENT_HPP(TYPE) \
    static EVA::ComponentRegister<TYPE> m_Register;\
	public:\
	std::string GetTypeId() override;\
	private:

	// Macro for registering a component
	#define REGISTER_COMPONENT_CPP(TYPE, NAME) \
    EVA::ComponentRegister<TYPE> TYPE::m_Register(NAME);\
	std::string TYPE::GetTypeId()\
	{\
		return m_Register.typeId;\
	}

	/**
	 * \brief Keeps track of all registerd components
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

		static std::vector<std::string> GetComponentIds()
		{
			std::vector<std::string> ids;

			for (map_type::const_iterator it = map->begin(); it != map->end(); ++it)
			{
				ids.push_back(it->first);
			}

			return ids;
		}
	};

	template<typename T>
	struct ComponentRegister 
	{
		std::string typeId;
		explicit ComponentRegister(std::string const& s)
		{
			typeId = s;
			ComponentMap::GetMap()->insert(std::make_pair(s, &ComponentMap::CreateT<T>));
		}
	};
}
