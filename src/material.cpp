#include "material.hpp"
#include "shader.hpp"

namespace vv {
	GLenum Material::GetFillMode() {
		return this->fill_mode;
	}

	void Material::SetFillMode(const GLenum mode) {
		switch (mode) {
			case GL_LINE:
			case GL_FILL:
				this->fill_mode = mode;
				break;
			default:
				this->fill_mode = GL_FILL;
		}
	}

	std::weak_ptr<Shader> Material::GetShader() {
		return this->shader;
	}

	void Material::SetShader(std::weak_ptr<Shader> s) {
		this->shader = s;
	}

	void Material::SetShader(std::string name) {
		this->shader = ShaderMap::Get(name);
	}

	std::weak_ptr<Material> Material::Create(const std::string name, std::weak_ptr<Shader> shader) {
		auto m = std::make_shared<Material>(shader);
		MaterialMap::Set(name, m);
		return m;
	}
}
