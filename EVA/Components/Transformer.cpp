#include "Transformer.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(Transformer, "EVA::Transformer")

	void Transformer::Update(const float deltaTime)
	{
		transform->Translate(m_Translation * deltaTime);

		transform->SetScale(transform->localScale + m_Scaling * deltaTime);

		transform->Rotate(m_Rotation * deltaTime);
	}

	void Transformer::Load(const DataObject data)
	{
		m_Translation = data.GetVec3("translation", m_Translation);
		m_Scaling = data.GetVec3("scaling", m_Scaling);
		m_Rotation = data.GetVec3("rotation", m_Rotation);
	}
}
