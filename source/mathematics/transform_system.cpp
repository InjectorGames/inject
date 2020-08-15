#include <injector/mathematics/transform_system.hpp>
#include <injector/engine.hpp>

#include <iostream>

namespace INJECTOR_NAMESPACE
{
	TransformSystem::TransformSystem() :
		transforms()
	{}
	TransformSystem::~TransformSystem()
	{}

	void TransformSystem::update()
	{
		auto deltaTime = static_cast<float>(Engine::getUpdateDeltaTime());

		for (auto& transform : transforms)
		{
			TransformComponent* transformComponent;

			if (!transform->getComponent(transformComponent))
				continue;

			TranslateComponent* translateComponent;
			if (transform->getComponent(translateComponent))
			{
				transformComponent->position +=
					translateComponent->translation * deltaTime;
				transformComponent->changed = true;
			}

			RotateComponent* rotateComponent;
			if (transform->getComponent(rotateComponent))
			{
				transformComponent->rotation *=
					rotateComponent->rotation * deltaTime;
				transformComponent->changed = true;
			}

			if (!transformComponent->changed)
				continue;

			transformComponent->rotation = transformComponent->rotation.getNormalized();

			Matrix4 matrix;

			if (transformComponent->type == TransformComponent::Type::Spin)
			{
				matrix = Matrix4::identity.getTranslated(transformComponent->position);
				matrix *= transformComponent->rotation.getMatrix4();
				matrix *= Matrix4::identity.getScaled(transformComponent->scale);
			}
			else
			{
				matrix = transformComponent->rotation.getMatrix4();
				matrix *= Matrix4::identity.getTranslated(transformComponent->position);
				matrix *= Matrix4::identity.getScaled(transformComponent->scale);
			}

			transformComponent->matrix = matrix;
		}

		// TODO: make parent childrens pair

		/*for (auto& transform : transforms)
		{
			TransformComponent* transformComponent;

			if (!transform->getComponent(transformComponent) ||
				!transformComponent->changed)
				continue;

			auto matrix = transformComponent->matrix;
			auto relative = transformComponent->parent;

			while (relative)
			{
				TransformComponent* relativeTransform;

				if (!relative->getComponent(relativeTransform))
					break;

				matrix = relativeTransform->matrix * matrix;
				relative = relativeTransform->parent;
			}

			transformComponent->matrix = matrix;
			transformComponent->changed = false;
		}*/
	}

	size_t TransformSystem::getTransformCount() const noexcept
	{
		return transforms.size();
	}

	bool TransformSystem::addTransform(const EntityHandle& entity) noexcept
	{
		if (entity == nullptr || !entity->containsComponent<TransformComponent>())
			return false;

		return transforms.emplace(entity).second;
	}
	bool TransformSystem::removeTransform(const EntityHandle& entity) noexcept
	{
		if (entity == nullptr)
			return false;

		auto iterator = transforms.find(entity);

		if (iterator == transforms.end())
			return false;

		transforms.erase(iterator);
		return true;
	}
	void TransformSystem::removeTransforms() noexcept
	{
		transforms.clear();
	}
}
