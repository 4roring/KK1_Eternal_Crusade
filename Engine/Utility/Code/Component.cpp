#include "Component.h"

Engine::CComponent::CComponent()
{
}

Engine::CComponent::~CComponent()
{
}

Engine::CComponent * Engine::CComponent::CloneComponent()
{
	assert(!"Clone Base Component");
	return nullptr;
}

void Engine::CComponent::Update(float delta_time)
{
}

int Engine::CComponent::AddReferenceCount()
{
	return ++reference_count_;
}
