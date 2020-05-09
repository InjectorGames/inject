#pragma once
#include <inject/keyboard_event.hpp>
#include <inject/camera_component.hpp>
#include <inject/mouse_motion_event.hpp>
#include <inject/mouse_button_event.hpp>
#include <inject/transform_component.hpp>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

namespace inject
{
	class FirstPersonSystem final :
		public entityx::System<FirstPersonSystem>,
		public entityx::Receiver<FirstPersonSystem>
	{
	private:
		entityx::Entity camera;
		bool rotating;
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 eulerAngles;
	public:
		bool clampPitch;
		float speed;
		float sensitivity;

		FirstPersonSystem() :
			camera(),
			rotating(false),
			rotation(glm::vec3(0.0f)),
			translation(glm::vec3(0.0f)),
			eulerAngles(glm::vec3(0.0f)),
			clampPitch(true),
			speed(2.0f),
			sensitivity(0.15f)
		{}

		void configure(entityx::EntityManager& entities, entityx::EventManager& events) override
		{
			events.subscribe<KeyboardEvent>(*this);
			events.subscribe<MouseMotionEvent>(*this);
			events.subscribe<MouseButtonEvent>(*this);

			camera = entities.create();
			camera.assign<CameraComponent>();
			camera.assign<TransformComponent>(glm::vec3(-1.0f), glm::vec3(0.0f, 0.0f, 2.0f));
		}

		void update(entityx::EntityManager& entities,
			entityx::EventManager& events,
			entityx::TimeDelta deltaTime) override
		{
			if (camera.has_component<TransformComponent>())
			{
				auto& transformComponent = *camera.component<TransformComponent>();
				transformComponent.position += translation * transformComponent.rotation * speed * static_cast<float>(deltaTime);
				transformComponent.changed = true;

				if (rotating)
				{
					eulerAngles += rotation * sensitivity * static_cast<float>(deltaTime);

					if (clampPitch)
						eulerAngles.x = std::clamp(eulerAngles.x, -1.57f, 1.57f);

					transformComponent.rotation =
						glm::angleAxis(eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
						glm::angleAxis(eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}

			rotation = glm::vec3(0.0f);
		}

		void receive(const KeyboardEvent& event)
		{
			if (event.data.keysym.scancode == SDL_Scancode::SDL_SCANCODE_A)
			{
				if (event.data.state == KeyboardEvent::State::Released && translation.x < 0.0f)
					translation.x = 0.0f;
				else if (event.data.state == KeyboardEvent::State::Pressed)
					translation.x = -1.0f;
			}
			else if (event.data.keysym.scancode == SDL_Scancode::SDL_SCANCODE_D)
			{
				if (event.data.state == KeyboardEvent::State::Released && translation.x > 0.0f)
					translation.x = 0.0f;
				else if (event.data.state == KeyboardEvent::State::Pressed)
					translation.x = 1.0f;
			}
			else if (event.data.keysym.scancode == SDL_Scancode::SDL_SCANCODE_LCTRL)
			{
				if (event.data.state == KeyboardEvent::State::Released && translation.y < 0.0f)
					translation.y = 0.0f;
				else if (event.data.state == KeyboardEvent::State::Pressed)
					translation.y = -1.0f;
			}
			else if (event.data.keysym.scancode == SDL_Scancode::SDL_SCANCODE_SPACE)
			{
				if (event.data.state == KeyboardEvent::State::Released && translation.y > 0.0f)
					translation.y = 0.0f;
				else if (event.data.state == KeyboardEvent::State::Pressed)
					translation.y = 1.0f;
			}
			else if (event.data.keysym.scancode == SDL_Scancode::SDL_SCANCODE_W)
			{
				if (event.data.state == KeyboardEvent::State::Released && translation.z < 0.0f)
					translation.z = 0.0f;
				else if (event.data.state == KeyboardEvent::State::Pressed)
					translation.z = -1.0f;
			}
			else if (event.data.keysym.scancode == SDL_Scancode::SDL_SCANCODE_S)
			{
				if (event.data.state == KeyboardEvent::State::Released && translation.z > 0.0f)
					translation.z = 0.0f;
				else if (event.data.state == KeyboardEvent::State::Pressed)
					translation.z = 1.0f;
			}
		}
		void receive(const MouseMotionEvent& event)
		{
			rotation += glm::vec3(
				static_cast<float>(event.data.yrel),
				static_cast<float>(event.data.xrel),
				0.0f);
		}
		void receive(const MouseButtonEvent& event)
		{
			if (event.data.button == MouseButtonEvent::Button::Right)
			{
				if (event.data.state == MouseButtonEvent::State::Released)
				{
					SDL_SetRelativeMouseMode(SDL_FALSE);
					rotating = false;
				}
				else if (event.data.state == MouseButtonEvent::State::Pressed)
				{
					SDL_SetRelativeMouseMode(SDL_TRUE);
					rotating = true;
				}
			}
		}
	};
}