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
		enum class GBUFFER_TEXTURE_TYPE : int {
			GBUFFER_TEXTURE_TYPE_POSITION = 0,
			GBUFFER_TEXTURE_TYPE_DIFFUSE,
			GBUFFER_TEXTURE_TYPE_NORMAL,
		};
		enum class GBUFFER_DEPTH_TYPE {
			GBUFFER_DEPTH_TYPE_STENCIL
		};

		GBuffer();
		~GBuffer() {}
		void AddColorAttachments(const unsigned int window_width, const unsigned int window_height);
		void ResizeColorAttachments(const unsigned int window_width, const unsigned int window_height);
		void SetDepthAttachment(GBUFFER_DEPTH_TYPE type,
								const unsigned int width, const unsigned int height);
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

		static void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) ;

		GLuint GetDepthTexture() const {
			return this->depth_texture;
		}

		GLuint GetColorTexture(unsigned short index) const {
			if (index < this->num_color_textures) {
				return this->color_textures[index];
			}
			return 0;
		}
	private:
		GLuint frame_buffer_object{ 0 };
		GLuint color_textures[sizeof(GBUFFER_TEXTURE_TYPE) - 1]{ 0,0,0 };
		GLuint depth_texture{ 0 };
		GLuint final_texture{ 0 };
		unsigned short num_color_textures{ sizeof(GBUFFER_TEXTURE_TYPE) - 1 };
		unsigned int depth_width{ 0 }, depth_height{ 0 };
		GBUFFER_DEPTH_TYPE depth_type{ GBUFFER_DEPTH_TYPE::GBUFFER_DEPTH_TYPE_STENCIL };
	};
}
