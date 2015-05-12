#include "components/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "os.hpp"
#include "render-system.hpp"
#include "transform.hpp"

namespace vv {
	void CameraMover::Update(double delta) {
		ProcessEventQueue();
	}

	void CameraMover::On(std::shared_ptr<vv::KeyboardEvent> data) {
		auto camera = this->cam.lock();
		if (!camera) {
			this->cam = e.Get<Camera>();
			return;
		}
		auto transforms = e.GetList<Position, Orientation>();
		if (!std::get<0>(transforms).lock()) {
			return;
		}
		auto position = std::make_shared<Position>(*std::get<0>(transforms).lock().get());
		if (!std::get<1>(transforms).lock()) {
			return;
		}
		auto orientation = std::make_shared<Orientation>(*std::get<1>(transforms).lock().get());

		switch (data->action) {
			case vv::KeyboardEvent::KEY_UP:
				switch (data->key) {
					case GLFW_KEY_A:
						orientation->OrientedRotate(glm::vec3(0.0, glm::radians(10.0f), 0.0));
						break;
					case GLFW_KEY_D:
						orientation->OrientedRotate(glm::vec3(0.0, glm::radians(-10.0f), 0.0));
						break;
					case GLFW_KEY_W:
						position->OrientedTranslate(orientation->value, glm::vec3(0.0, 0.0, -1.0));
						break;
					case GLFW_KEY_S:
						position->OrientedTranslate(orientation->value, glm::vec3(0.0, 0.0, 1.0));
						break;
					case GLFW_KEY_SPACE:
						camera->MakeActive();
						break;
				}
				break;
			default:
				break;
		}
		this->e.Update<Position>(position);
		this->e.Update<Orientation>(orientation);
	}
	Camera::Camera(eid entity_id) : e(entity_id) {
		e.Add<View>();
	}

	Camera::~Camera() {
		this->e.Remove<Position>();
		this->e.Remove<Orientation>();
	}

	bool Camera::MakeActive() {
		if (this->e.Has<View>()) {
			auto view = this->e.Get<View>().lock();
			if (!view->active) {
				auto new_view = std::make_shared<View>();
				new_view->active = true;
				new_view->view_matrix = view->view_matrix;
				this->e.Update<View>(new_view);
			}
			return true;
		}
		return false;
	}
}
