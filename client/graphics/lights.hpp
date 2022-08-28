#pragma once

#include <cmath>
#include <glm/vec3.hpp>

#include "components/transforms.hpp"

namespace tec {
struct BaseLight {
	glm::vec3 color{1.f, 1.f, 1.f};
	float ambient_intensity{0.5f};
	float diffuse_intensity{0.2f};

	BaseLight() = default;
};

struct DirectionalLight : public BaseLight {
	glm::vec3 direction{0.f, 0.f, -1.f};

	DirectionalLight() = default;

	void Out(proto::Component* target) {
		proto::Light* comp = target->mutable_directionallight();
		comp->set_color_x(this->color.x);
		comp->set_color_y(this->color.y);
		comp->set_color_z(this->color.z);
		comp->set_ambient_intensity(this->ambient_intensity);
		comp->set_diffuse_intensity(this->diffuse_intensity);
		proto::NormalVector* dir = comp->mutable_direction();
		dir->set_x(this->direction.x);
		dir->set_y(this->direction.y);
		dir->set_z(this->direction.z);
	}

	void In(const proto::Component& source) {
		const proto::Light& comp = source.directionallight();
		if (comp.has_color_x()) {
			this->color.x = comp.color_x();
		}
		if (comp.has_color_y()) {
			this->color.y = comp.color_y();
		}
		if (comp.has_color_z()) {
			this->color.z = comp.color_z();
		}
		if (comp.has_ambient_intensity()) {
			this->ambient_intensity = comp.ambient_intensity();
		}
		if (comp.has_diffuse_intensity()) {
			this->diffuse_intensity = comp.diffuse_intensity();
		}
		if (comp.has_direction()) {
			const proto::NormalVector& _direction = comp.direction();
			this->direction.x = _direction.x();
			this->direction.y = _direction.y();
			this->direction.z = _direction.z();
		}
	}
};

struct PointLight : public BaseLight {
	struct {
		float constant{0.f};
		float linear{0.f};
		float exponential{0.3f};
	} Attenuation;

	PointLight() = default;

	void Out(proto::Component* target) {
		proto::Light* comp = target->mutable_pointlight();
		comp->set_color_x(this->color.x);
		comp->set_color_y(this->color.y);
		comp->set_color_z(this->color.z);
		comp->set_ambient_intensity(this->ambient_intensity);
		comp->set_diffuse_intensity(this->diffuse_intensity);
		proto::Light::Attenuation* atten = comp->mutable_attenuation();
		atten->set_constant(this->Attenuation.constant);
		atten->set_linear(this->Attenuation.linear);
		atten->set_exponential(this->Attenuation.exponential);
	}

	void In(const proto::Component& source) {
		const proto::Light& comp = source.pointlight();
		if (comp.has_color_x()) {
			this->color.x = comp.color_x();
		}
		if (comp.has_color_y()) {
			this->color.y = comp.color_y();
		}
		if (comp.has_color_z()) {
			this->color.z = comp.color_z();
		}
		if (comp.has_ambient_intensity()) {
			this->ambient_intensity = comp.ambient_intensity();
		}
		if (comp.has_diffuse_intensity()) {
			this->diffuse_intensity = comp.diffuse_intensity();
		}
		if (comp.has_attenuation()) {
			const proto::Light::Attenuation& atten = comp.attenuation();
			if (atten.has_constant()) {
				this->Attenuation.constant = atten.constant();
			}
			if (atten.has_linear()) {
				this->Attenuation.linear = atten.linear();
			}
			if (atten.has_exponential()) {
				this->Attenuation.exponential = atten.exponential();
			}
		}
	}

	float UpdateBoundingRadius() {
		float MaxChannel = fmaxf(fmaxf(this->color.x, this->color.y), this->color.z);
		const float CUTOFF_FACTOR = 255.0 / 10.0;
		float const_atten = this->Attenuation.constant;
		float linear_atten = this->Attenuation.linear;
		float linear_atten_sq = linear_atten * linear_atten;
		float exp_atten = this->Attenuation.exponential;
		// this nastiness is the quadratic equation, with tons of lighting params plugged in
		auto exp_con_term = 4 * exp_atten * (const_atten - CUTOFF_FACTOR * MaxChannel * this->diffuse_intensity);
		this->bounding_radius = (sqrtf(linear_atten_sq - exp_con_term) - linear_atten) / (2.f * exp_atten);
		return this->bounding_radius;
	}

	float bounding_radius{0.f};
};

struct SpotLight : public DirectionalLight {
	float cutoff{0.f};

	SpotLight() = default;
};
} // namespace tec
