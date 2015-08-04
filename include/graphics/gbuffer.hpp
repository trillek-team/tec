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
		enum GBUFFER_TEXTURE_TYPE {
			GBUFFER_TEXTURE_TYPE_POSITION,
			GBUFFER_TEXTURE_TYPE_DIFFUSE,
			GBUFFER_TEXTURE_TYPE_NORMAL,
			GBUFFER_TEXTURE_TYPE_TEXCOORD,
			GBUFFER_NUM_TEXTURES
		};

		GBuffer() { }

		~GBuffer() { }

		bool Init(unsigned int window_width, unsigned int window_height);

		bool Resize(unsigned int window_width, unsigned int window_height);

		void StartFrame();

		void BindForGeomPass();

		void BindForStencilPass();

		void BindForShadowPass();

		void BindForLightPass();

		void BindForFinalPass();

		void BindForRendering() const;

		void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) const;
	private:
		GLuint frame_buffer_object;
		GLuint textures[GBUFFER_NUM_TEXTURES];
		GLuint depth_texture;
		GLuint final_texture;
		GLuint shadow_map_texture;
		GLuint shadow_frame_buffer_object;
	};
}
