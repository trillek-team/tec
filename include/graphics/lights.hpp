#pragma once

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <glm/vec3.hpp>
#include "reflection.hpp"

namespace tec {
	struct BaseLight {
		glm::vec3 color;
		float ambient_intensity;
		float diffuse_intensity;

		BaseLight() {
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			ambient_intensity = 0.5f;
			diffuse_intensity = 0.2f;
		}
	};

	struct DirectionalLight : public BaseLight {
		glm::vec3 direction;

		DirectionalLight() {
			direction = glm::vec3(0.0f, 0.0f, -1.0f);
		}

		void Out(proto::Component* target) {
			proto::Light* comp = target->mutable_directionallight();
			comp->set_color_x(this->color.x);
			comp->set_color_y(this->color.y);
			comp->set_color_z(this->color.z);
			comp->set_ambient_intensity(this->ambient_intensity);
			comp->set_diffuse_intensity(this->diffuse_intensity);
			proto::Light::Direction* dir = comp->mutable_direction();
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
				const proto::Light::Direction& direction = comp.direction();
				this->direction.x = direction.x();
				this->direction.y = direction.y();
				this->direction.z = direction.z();
			}
		}

		static ReflectionComponent Reflection(DirectionalLight* val) {
			ReflectionComponent refcomp;
			Property rgbprop(Property::RGB);
			(refcomp.properties["Color"] = rgbprop).Set(val->color);
			refcomp.properties["Color"].update_func = [val] (Property& prop) { val->color = prop.Get<glm::vec3>(); };
			Property fprop(Property::FLOAT);
			(refcomp.properties["Ambient Intensity"] = fprop).Set(val->ambient_intensity);
			refcomp.properties["Ambient Intensity"].update_func = [val] (Property& prop) { val->ambient_intensity = prop.Get<float>(); };
			(refcomp.properties["Diffuse Intensity"] = fprop).Set(val->diffuse_intensity);
			refcomp.properties["Diffuse Intensity"].update_func = [val] (Property& prop) { val->diffuse_intensity = prop.Get<float>(); };
			(refcomp.properties["Direction X"] = fprop).Set(val->direction.x);
			refcomp.properties["Direction X"].update_func = [val] (Property& prop) { val->direction.x = prop.Get<float>(); };
			(refcomp.properties["Direction Y"] = fprop).Set(val->direction.y);
			refcomp.properties["Direction Y"].update_func = [val] (Property& prop) { val->direction.y = prop.Get<float>(); };
			(refcomp.properties["Direction Z"] = fprop).Set(val->direction.z);
			refcomp.properties["Direction Z"].update_func = [val] (Property& prop) { val->direction.z = prop.Get<float>(); };

			return std::move(refcomp);
		}
	};

	struct PointLight : public BaseLight {
		struct {
			float constant;
			float linear;
			float exponential;
		} Attenuation;

		PointLight() {
			Attenuation.constant = 0.0f;
			Attenuation.linear = 0.0f;
			Attenuation.exponential = 0.3f;
		}

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

		static ReflectionComponent Reflection(PointLight* val) {
			ReflectionComponent refcomp;
			Property rgbprop(Property::RGB);
			(refcomp.properties["Color"] = rgbprop).Set(val->color);
			refcomp.properties["Color"].update_func = [val] (Property& prop) { val->color = prop.Get<glm::vec3>(); };
			Property fprop(Property::FLOAT);
			(refcomp.properties["Ambient Intensity"] = fprop).Set(val->ambient_intensity);
			refcomp.properties["Ambient Intensity"].update_func = [val] (Property& prop) { val->ambient_intensity = prop.Get<float>(); };
			(refcomp.properties["Diffuse Intensity"] = fprop).Set(val->diffuse_intensity);
			refcomp.properties["Diffuse Intensity"].update_func = [val] (Property& prop) { val->diffuse_intensity = prop.Get<float>(); };
			(refcomp.properties["Attenuation.constant"] = fprop).Set(val->Attenuation.constant);
			refcomp.properties["Attenuation.constant"].update_func = [val] (Property& prop) { val->Attenuation.constant = prop.Get<float>(); };
			(refcomp.properties["Attenuation.linear"] = fprop).Set(val->Attenuation.linear);
			refcomp.properties["Attenuation.linear"].update_func = [val] (Property& prop) { val->Attenuation.linear = prop.Get<float>(); };
			(refcomp.properties["Attenuation.exponential"] = fprop).Set(val->Attenuation.exponential);
			refcomp.properties["Attenuation.exponential"].update_func = [val] (Property& prop) { val->Attenuation.exponential = prop.Get<float>(); };
			(refcomp.properties["Attenuation.exponential"] = fprop).Set(val->Attenuation.exponential);
			refcomp.properties["Attenuation.exponential"].update_func = [val] (Property& prop) { val->Attenuation.exponential = prop.Get<float>(); };

			return std::move(refcomp);
		}

		float UpdateBoundingRadius() {
			float MaxChannel = fmax(fmax(this->color.x, this->color.y), this->color.z);

			return this->bounding_radius = (-this->Attenuation.linear +
				sqrtf(this->Attenuation.linear *
				this->Attenuation.linear - 4 *
				this->Attenuation.exponential *
				(this->Attenuation.exponential - 256 * MaxChannel *
				this->diffuse_intensity))) / 2 *
				this->Attenuation.exponential;
		}

		float bounding_radius;
	};

	struct SpotLight : public DirectionalLight {
		float cutoff;

		SpotLight() {
			cutoff = 0.0f;
		}
	};
}
