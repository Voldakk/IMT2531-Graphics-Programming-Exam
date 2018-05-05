#include "Collider.hpp"

namespace EVA
{
	Collider::~Collider()
	{
		scene->RemoveCollider(this);
	}

	void Collider::Awake()
	{
		scene->AddCollider(this);
	}
}
