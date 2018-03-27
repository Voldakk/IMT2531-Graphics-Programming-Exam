#pragma once

namespace EVA
{

    class GameObject;

    class Component
    {

    protected:

        GameObject *m_GameObject;

    public:

        Component(GameObject *gameObject);

        virtual void Update(float deltaTime);

        inline GameObject *GetGameObject() const
        { return m_GameObject; }
    };

}