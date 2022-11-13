#include "texture-object.hpp"

#include "gl-symbol.hpp"
#include "render-system.hpp"

#include <spdlog/spdlog.h>

namespace tec {
TextureObject::~TextureObject() { Destroy(); }

TextureObject::TextureObject(const PixelBuffer& image) {
	this->texture_id = 0;
	Load(image);
}

TextureObject::TextureObject(std::weak_ptr<PixelBuffer> pbp) : source_ptr(pbp) {
	this->texture_id = 0;
	auto locked_ptr = pbp.lock();
	if (locked_ptr) {
		Load(*locked_ptr.get());
	}
}

void TextureObject::Update() {
	std::shared_ptr<PixelBuffer> locked_ptr = this->source_ptr.lock();
	if (locked_ptr) {
		if (locked_ptr->IsDirty()) {
			Load(*locked_ptr);
			locked_ptr->Validate();
		}
	}
}

TextureObject::TextureObject(TextureObject&& other) {
	this->texture_id = other.texture_id;
	other.texture_id = 0;
}

TextureObject& TextureObject::operator=(TextureObject&& other) {
	this->texture_id = other.texture_id;
	other.texture_id = 0;
	return *this;
}

void TextureObject::Destroy() {
	if (this->texture_id) {
		glDeleteTextures(1, &this->texture_id);
		this->texture_id = 0;
	}
}

void TextureObject::Load(const PixelBuffer& image) {
	auto _log = spdlog::get("console_log");
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");

	struct {
		GLenum to;
		GLenum from;
	} format;
	switch (image.GetFormat()) {
	case ImageColorMode::GAMMA_RGBA: format = {GL_SRGB_ALPHA, GL_RGBA}; break;
	case ImageColorMode::GAMMA_RGB: format = {GL_SRGB, GL_RGB}; break;
	case ImageColorMode::COLOR_RGBA: format = {GL_RGBA, GL_RGBA}; break;
	case ImageColorMode::COLOR_RGB: format = {GL_RGB, GL_RGB}; break;
	case ImageColorMode::MONOCHROME_A: format = {GL_RG, GL_RG}; break;
	case ImageColorMode::MONOCHROME: format = {GL_RED, GL_RED}; break;
	default: return;
	}
	const uint8_t* pixdata = image.GetBlockBase();
	if (nullptr == pixdata) {
		_log->error("[TextureObject] Image is missing pixeldata");
		return;
	}
	GLint magfilter = GL_LINEAR;
	if (image.GetFilter() == PixelBuffer::NEAREST) {
		magfilter = GL_NEAREST;
	}
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
		glBindTexture(GL_TEXTURE_2D, this->texture_id);
		RenderSystem::ErrorCheck("Error binding texture", __LINE__, "TextureObject");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#ifndef __APPLE__
		// GL_TEXTURE_MAX_ANISOTROPY_EXT is not defined on osx
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
#endif
		RenderSystem::ErrorCheck("Error setting texture parameters", __LINE__, "TextureObject");
		_log->trace(
				"[TextureObject] Allocate {}x{} in {} from {}",
				image.Width(),
				image.Height(),
				graphics::GLSymbol::Get(format.to),
				graphics::GLSymbol::Get(format.from));
	}
	else {
		glBindTexture(GL_TEXTURE_2D, this->texture_id);
		RenderSystem::ErrorCheck("Error binding texture", __LINE__, "TextureObject");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format.to, image.Width(), image.Height(), 0, format.from, GL_UNSIGNED_BYTE, pixdata);
	RenderSystem::ErrorCheck("Error coping texture data to GPU", __LINE__, "TextureObject");
	glGenerateMipmap(GL_TEXTURE_2D);
	RenderSystem::ErrorCheck("GenerateMipmap failed", __LINE__, "TextureObject");
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::Generate(GLuint width, GLuint height, bool usealpha) {
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
		glBindTexture(GL_TEXTURE_2D, this->texture_id);
		RenderSystem::ErrorCheck("Error binding texture", __LINE__, "TextureObject");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#ifndef __APPLE__
		// GL_TEXTURE_MAX_ANISOTROPY_EXT is not defined on osx
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
#endif
		RenderSystem::ErrorCheck("Error setting texture parameters", __LINE__, "TextureObject");
	}
	else {
		glBindTexture(GL_TEXTURE_2D, this->texture_id);
		RenderSystem::ErrorCheck("Error binding texture", __LINE__, "TextureObject");
	}
	if (usealpha) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	RenderSystem::ErrorCheck("Error coping texture data to GPU", __LINE__, "TextureObject");
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::GenerateStencil(GLuint width, GLuint height) {
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::GenerateDepth(GLuint width, GLuint height, bool stencil) {
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if (this->compare) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);
	}
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	auto format = stencil ? GL_DEPTH_STENCIL : GL_DEPTH_COMPONENT;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::GenerateMultisample(GLuint width, GLuint height, GLuint samples) {
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->texture_id);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_FALSE);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void TextureObject::GenerateMultisampleStencil(GLuint width, GLuint height, GLuint samples) {
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->texture_id);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_STENCIL_INDEX, width, height, GL_FALSE);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void TextureObject::GenerateMultisampleDepth(GLuint width, GLuint height, GLuint samples, bool stencil) {
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->texture_id);
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	if (stencil) {
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH_STENCIL, width, height, GL_FALSE);
	}
	else {
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH_COMPONENT, width, height, GL_FALSE);
	}
	RenderSystem::ErrorCheck("", __LINE__, "TextureObject");
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}
} // namespace tec
