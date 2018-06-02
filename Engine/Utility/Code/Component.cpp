#include "Component.h"

Engine::CComponent::CComponent()
{
}

Engine::CComponent::~CComponent()
{
}

Engine::CComponent * Engine::CComponent::CloneComponent()
{
#ifdef _DEBUG
	assert(!"Clone Base Component");
#endif
	return nullptr;
}

void Engine::CComponent::Update(float delta_time)
{
}

int Engine::CComponent::AddReferenceCount()
{
	return ++reference_count_;
}
