// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "material.hpp"
#include "texture-object.hpp"
#include "shader.hpp"

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

	void Material::AddTexture(std::shared_ptr<TextureObject> tex) {
		this->textures.push_back(tex);
	}
	
	std::shared_ptr<TextureObject> Material::GetTexutre(std::size_t index) {
		if (index < this->textures.size()) {
			return this->textures[index];
		}
		return nullptr;
	}

	void Material::RemoveTexture(std::shared_ptr<TextureObject> tex) {
		for (auto texture : this->textures) {
			if (texture == tex) {
				texture.reset();
			}
		}
	}

	std::shared_ptr<Material> Material::Create(const std::string name) {
		auto m = std::make_shared<Material>();
		MaterialMap::Set(name, m);
		return m;
	}

	void Material::Activate() {
		for (GLuint i = 0; i < this->textures.size(); ++i) {
			Shader::ActivateTextureUnit(i, this->textures[i]->GetID());
		}
	}

	void Material::Deactivate() {
		for (GLuint i = 0; i < this->textures.size(); ++i) {
			Shader::DeactivateTextureUnit(i);
		}
	}
}
