// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

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
		enum GBUFFER_DEPTH_TYPE {
			GBUFFER_DEPTH_TYPE_SHADOW,
			GBUFFER_DEPTH_TYPE_STENCIL
		};

		GBuffer();
		~GBuffer() { }
		void AddColorAttachments(unsigned short count,
			const unsigned int window_width, const unsigned int window_height);
		void ResizeColorAttachments(const unsigned int window_width, const unsigned int window_height);
		void SetDepthAttachment(GBUFFER_DEPTH_TYPE type,
			const unsigned int width, const unsigned int height);
		void ResizeDepthAttachment(const unsigned int width, const unsigned int height);
		bool CheckCompletion() const;
		void StartFrame();
		void ShadowPass();
		void GeometyPass();
		void StencilPass();
		void LightPass(GLuint external_shadow_map_texture);
		void FinalPass();
		void BindForRendering() const;
		void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) const;

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
		GLuint frame_buffer_object;
		GLuint color_textures[GBUFFER_NUM_TEXTURES];
		GLuint depth_texture;
		GLuint final_texture;
		unsigned short num_color_textures;
		unsigned int depth_width, depth_height;
		GBUFFER_DEPTH_TYPE depth_type;
	};
}
