#include "texture-object.hpp"

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
			Load(locked_ptr->GetBlockBase(), locked_ptr->Width(), locked_ptr->Height());
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
	}
}

void TextureObject::Load(const PixelBuffer& image) {
	auto _log = spdlog::get("console_log");
	auto err = glGetError();
	if (err) {
		return;
	}

	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	GLenum gformat;
	switch (image.GetFormat()) {
	case ImageColorMode::COLOR_RGBA: gformat = GL_RGBA; break;
	case ImageColorMode::COLOR_RGB: gformat = GL_RGB; break;
	case ImageColorMode::MONOCHROME_A: gformat = GL_RG; break;
	case ImageColorMode::MONOCHROME: gformat = GL_RED; break;
	default: return;
	}
	const uint8_t* pixdata = image.GetBlockBase();
	if (nullptr == pixdata) {
		_log->error("[Texture-Object] Missing pixeldata");
		return;
	}
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	err = glGetError();
	if (err) {
		_log->trace("[Texture-Object] Error binding texture");
		return;
	}
	GLint magfilter = GL_LINEAR; // TODO Add a get/set magfilter and add code to generate mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

#ifndef __APPLE__
	glTexParameterf(
			GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f); // GL_TEXTURE_MAX_ANISOTROPY_EXT is not defined on osx
#endif

	err = glGetError();
	if (err) {
		_log->trace("[Texture-Object] Error setting texture filters");
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, gformat, image.Width(), image.Height(), 0, gformat, GL_UNSIGNED_BYTE, pixdata);
	err = glGetError();
	if (err) {
		_log->trace("[Texture-Object] Error coping texture data to GPU");
		return;
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::Load(const uint8_t* image, GLuint width, GLuint height) {
	auto _log = spdlog::get("console_log");
	auto err = glGetError();
	if (err) {
		return;
	}
	if (!this->texture_id) {
		return;
	}

	GLenum gformat;
	switch (this->source_ptr.lock()->GetFormat()) {
	case ImageColorMode::COLOR_RGBA: gformat = GL_RGBA; break;
	case ImageColorMode::COLOR_RGB: gformat = GL_RGB; break;
	case ImageColorMode::MONOCHROME_A: gformat = GL_RG; break;
	case ImageColorMode::MONOCHROME: gformat = GL_RED; break;
	default: return;
	}
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	err = glGetError();
	if (err) {
		_log->trace("[Texture-Object] Error binding texture");
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, gformat, width, height, 0, gformat, GL_UNSIGNED_BYTE, image);
	err = glGetError();
	if (err) {
		_log->trace("[Texture-Object] Error coping texture data to GPU");
		return;
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::Generate(GLuint width, GLuint height, bool usealpha) {
	auto _log = spdlog::get("console_log");
	auto err = glGetError();
	if (err) {
		return;
	}
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	err = glGetError();
	if (err) {
		_log->trace("[Texture-Object] Error binding texture");
		return;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

#ifndef __APPLE__
	glTexParameterf(
			GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f); // GL_TEXTURE_MAX_ANISOTROPY_EXT is not defined on osx
#endif

	err = glGetError();
	if (err) {
		_log->trace("[Texture-Object] Error setting texture filters");
		return;
	}
	if (usealpha) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	err = glGetError();
	if (err) {
		_log->trace("[Texture-Object] Error writing texture data on GPU");
		return;
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::GenerateStencil(GLuint width, GLuint height) {
	auto err = glGetError();
	if (err) {
		return;
	}
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	err = glGetError();
	if (err) {
		return;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	err = glGetError();
	if (err) {
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);
	err = glGetError();
	if (err) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::GenerateDepth(GLuint width, GLuint height, bool stencil) {
	if (glGetError()) {
		return;
	}
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	if (glGetError()) {
		return;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if (this->compare) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);
	}
	if (glGetError()) {
		return;
	}
	auto format = stencil ? GL_DEPTH_STENCIL : GL_DEPTH_COMPONENT;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
	if (glGetError()) {
		return;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::GenerateMultisample(GLuint width, GLuint height, GLuint samples) {
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->texture_id);
	auto err = glGetError();
	if (err) {
		return;
	}
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_FALSE);
	err = glGetError();
	if (err) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void TextureObject::GenerateMultisampleStencil(GLuint width, GLuint height, GLuint samples) {
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->texture_id);
	auto err = glGetError();
	if (err) {
		return;
	}
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_STENCIL_INDEX, width, height, GL_FALSE);
	err = glGetError();
	if (err) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void TextureObject::GenerateMultisampleDepth(GLuint width, GLuint height, GLuint samples, bool stencil) {
	if (!this->texture_id) {
		glGenTextures(1, &this->texture_id);
	}
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->texture_id);
	auto err = glGetError();
	if (err) {
		return;
	}
	if (stencil) {
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH_STENCIL, width, height, GL_FALSE);
	}
	else {
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH_COMPONENT, width, height, GL_FALSE);
	}
	err = glGetError();
	if (err) {
		return;
	}
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}
} // namespace tec
