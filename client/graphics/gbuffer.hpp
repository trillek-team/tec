#pragma once

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

// Based off http://ogldev.atspace.co.uk/www/tutorial35/tutorial35.html

namespace tec {
class GBuffer {
public:
	enum class TEXTURE_TYPE : int {
		POSITION = 0,
		DIFFUSE,
		NORMAL,
		EMISSION,
		TEXTURE_TYPE_LAST // leave this last
	};
	enum class DEPTH_TYPE : int { DEPTH = (int)TEXTURE_TYPE::TEXTURE_TYPE_LAST, STENCIL };

	GBuffer() {}
	~GBuffer() {}
	void AddColorAttachments(const unsigned int window_width, const unsigned int window_height);
	void ResizeColorAttachments(const unsigned int window_width, const unsigned int window_height);
	void SetDepthAttachment(DEPTH_TYPE type, const unsigned int width, const unsigned int height);
	void ResizeDepthAttachment(const unsigned int width, const unsigned int height);
	bool CheckCompletion() const;
	void StartFrame() const;

	void BeginGeometryPass() const;
	static void EndGeometryPass();

	static void StencilPass();

	void BeginLightPass();

	void BeginDirLightPass();

	void BeginPointLightPass();
	void EndPointLightPass();

	void FinalPass() const;

	void BindForWriting() const;
	void BindForRendering() const;

	static void SetReadBuffer(TEXTURE_TYPE TextureType);

	GLuint GetDepthTexture() const { return this->depth_texture; }

	GLuint GetColorTexture(unsigned short index) const {
		if (index < this->num_color_textures) {
			return this->color_textures[index];
		}
		return 0;
	}

private:
	GLuint frame_buffer_object{0};
	GLuint color_textures[(unsigned)TEXTURE_TYPE::TEXTURE_TYPE_LAST]{0, 0, 0, 0};
	GLuint depth_texture{0};
	GLuint final_texture{0};
	static const unsigned short num_color_textures{(unsigned)TEXTURE_TYPE::TEXTURE_TYPE_LAST};
	unsigned int depth_width{0}, depth_height{0};
	DEPTH_TYPE depth_type{DEPTH_TYPE::DEPTH};
};
} // namespace tec
