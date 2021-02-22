#pragma once

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <memory>
#include <string>

#include "multiton.hpp"

#include "resources/pixel-buffer.hpp"

namespace tec {
class TextureObject;
typedef Multiton<std::string, std::shared_ptr<TextureObject>> TextureMap;

class TextureObject final {
public:
	TextureObject(): texture_id(0), compare(false) {}
	~TextureObject();

	/**
	*\brief new texture instance from image
	*/
	TextureObject(const PixelBuffer&);

	/**
	* \brief new texture instance from an image pointer
	*/
	TextureObject(std::weak_ptr<PixelBuffer>);

	// no copying (although it could be done)
	TextureObject(const TextureObject&) = delete;
	TextureObject& operator=(const TextureObject&) = delete;

	// Move textures
	TextureObject(TextureObject&&);
	TextureObject& operator=(TextureObject&&);

	/**
	* \brief delete the underlaying GL texture
	*/
	void Destroy();

	/**
	* \brief get the ID of the texture
	* note: this method is not const, since GL can modify the texture with the ID
	* \return GLuint the GL texture ID
	*/
	GLuint GetID() { return this->texture_id; }

	/**
	* \return true if the texture was created dynamic
	*/
	bool IsDynamic() { return !this->source_ptr.expired(); }

	/**
	* Called by the RenderSystem to update dynamic textures
	*/
	void Update();

	void SetCompare(bool c) { this->compare = c; }

	/**
	* \brief create a texture from an image
	*/
	void Load(const PixelBuffer&);

	/**
	* \brief create a texture from raw image data
	*/
	void Load(const uint8_t*, GLuint width, GLuint height);

	/**
	* \brief create a blank texture RGB or RGBA format
	*/
	void Generate(GLuint width, GLuint height, bool usealpha);

	/**
	* \brief create a blank depth texture with or without stencil
	*/
	void GenerateDepth(GLuint width, GLuint height, bool stencil);

	/**
	* \brief create a blank stencil texture
	*/
	void GenerateStencil(GLuint width, GLuint height);

	/**
	* \brief create a blank multi-sample texture RGB or RGBA format
	*/
	void GenerateMultisample(GLuint width, GLuint height, GLuint samples);

	/**
	* \brief create a blank multi-sample depth texture with or without stencil
	*/
	void GenerateMultisampleDepth(GLuint width, GLuint height, GLuint samples, bool stencil);

	/**
	* \brief create a blank multi-sample stencil texture
	*/
	void GenerateMultisampleStencil(GLuint width, GLuint height, GLuint samples);

	bool operator==(const TextureObject& other) { return this->texture_id == other.texture_id; }

protected:
	GLuint texture_id{0};
	bool compare{0};
	std::weak_ptr<PixelBuffer> source_ptr;
};

} // namespace tec
