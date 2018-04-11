#pragma once

namespace EVA
{

	class GameObject;
	class Transform;

    class Component
    {

    protected:

        GameObject* m_GameObject;
		Transform* m_Transform;

    public:
	    
	    explicit Component(GameObject *gameObject);

		virtual ~Component() = default;

        virtual void Update(float deltaTime);

        inline GameObject *GetGameObject() const
        { return m_GameObject; }
    };

}
