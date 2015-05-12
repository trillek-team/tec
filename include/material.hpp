#pragma once

#include <memory>

#include "multiton.hpp"

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

namespace vv {
	class Shader;
	class Material;

	typedef Multiton<std::string, std::shared_ptr<Material>> MaterialMap;

	class Material {
	public:
		Material(const std::weak_ptr<Shader> shader) : shader(shader), fill_mode(GL_FILL) { }

		GLenum GetFillMode();

		void SetFillMode(const GLenum mode);

		std::weak_ptr<Shader> GetShader();

		void SetShader(std::weak_ptr<Shader> s);

		void SetShader(std::string name);

		// Creates a Material from the given shader MaterialMap under name.
		// return is a weak_ptr to the created Material.
		static std::weak_ptr<Material> Create(const std::string name, std::weak_ptr<Shader> shader);
	private:
		GLenum fill_mode;
		std::weak_ptr<Shader> shader;
	};
}
