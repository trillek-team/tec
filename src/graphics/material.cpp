#include "graphics/material.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/shader.hpp"

namespace tec {
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
		for (GLuint i = 0; i < this->textures.size(); ++ i) {
			this->shader->ActivateTextureUnit(i, this->textures[i]->GetID());
		}
	}

	void Material::Deactivate() {
		for (GLuint i = 0; i < this->textures.size(); ++i) {
			this->shader->DeactivateTextureUnit(i);
		}
	}

}
