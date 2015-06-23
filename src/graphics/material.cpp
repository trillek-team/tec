#include "graphics/material.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/shader.hpp"

namespace tec {
	Material::Material(const std::weak_ptr<Shader> shader) : shader(shader),
		polygon_mode(GL_FILL), draw_elements_mode(GL_TRIANGLES) { }

	const GLenum Material::GetPolygonMode() {
		return this->polygon_mode;
	}

	void Material::SetPolygonMode(const GLenum mode) {
		switch (mode) {
			case GL_POINT:
			case GL_LINE:
			case GL_FILL:
				this->polygon_mode = mode;
				break;
			default:
				this->polygon_mode = GL_FILL;
		}
	}

	void Material::SetDrawElementsMode(const GLenum mode) {
		switch (mode) {
			case GL_POINTS:
			case GL_LINE_STRIP:
			case GL_LINE_LOOP:
			case GL_LINES:
			case GL_TRIANGLE_STRIP:
			case GL_TRIANGLE_FAN:
			case GL_TRIANGLES:
			case GL_PATCHES:
				this->draw_elements_mode = mode;
				break;
			default:
				this->draw_elements_mode = GL_TRIANGLES;
		}
	}

	const GLenum Material::GetDrawElementsMode() const {
		return this->draw_elements_mode;
	}

	std::shared_ptr<Shader> Material::GetShader() {
		return this->shader;
	}

	void Material::SetShader(std::shared_ptr<Shader> s) {
		this->shader = s;
	}

	void Material::SetShader(const std::string name) {
		this->shader = ShaderMap::Get(name);
	}

	void Material::AddTexture(std::shared_ptr<TextureObject> tex) {
		this->textures.push_back(tex);
	}

	void Material::RemoveTexture(std::shared_ptr<TextureObject> tex) {
		for (auto texture : this->textures) {
			if (texture == tex) {
				texture.reset();
			}
		}
	}

	std::shared_ptr<Material> Material::Create(const std::string name, std::shared_ptr<Shader> shader) {
		auto m = std::make_shared<Material>(shader);
		MaterialMap::Set(name, m);
		return m;
	}

	void Material::Activate() {
		for (GLuint i = 0; i < this->textures.size(); ++i) {
			this->shader->ActivateTextureUnit(i, this->textures[i]->GetID());
		}
	}

	void Material::Deactivate() {
		for (GLuint i = 0; i < this->textures.size(); ++i) {
			this->shader->DeactivateTextureUnit(i);
		}
	}

}
