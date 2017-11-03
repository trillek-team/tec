// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "gbuffer.hpp"

namespace tec {
	GBuffer::GBuffer() : num_color_textures(0) {
		glGenFramebuffers(1, &this->frame_buffer_object);
	}
	void GBuffer::AddColorAttachments(unsigned short count,
		const unsigned int window_width, const unsigned int window_height) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
		if (count > GBUFFER_NUM_TEXTURES) {
			count = GBUFFER_NUM_TEXTURES;
		}
		this->num_color_textures = count;
		glGenTextures(this->num_color_textures, this->color_textures);
		glGenTextures(1, &this->final_texture);

		ResizeColorAttachments(window_width, window_height);
	}

	void GBuffer::ResizeColorAttachments(const unsigned int window_width, const unsigned int window_height) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
		for (unsigned int i = 0; i < this->num_color_textures; i++) {
			glBindTexture(GL_TEXTURE_2D, this->color_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window_width, window_height,
				0, GL_RGB, GL_FLOAT, nullptr);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
				GL_TEXTURE_2D, this->color_textures[i], 0);
		}
		glBindTexture(GL_TEXTURE_2D, this->final_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0,
			GL_RGB, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D,
			this->final_texture, 0);
		GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(this->num_color_textures, DrawBuffers);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void GBuffer::SetDepthAttachment(GBUFFER_DEPTH_TYPE type,
		const unsigned int width, const unsigned int height) {
		this->depth_type = type;
		glGenTextures(1, &this->depth_texture);
		ResizeDepthAttachment(width, height);
	}

	void GBuffer::ResizeDepthAttachment(const unsigned int width, const unsigned int height) {
		this->depth_width = width;
		this->depth_height = height;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
		glBindTexture(GL_TEXTURE_2D, this->depth_texture);
		if (this->depth_type == GBUFFER_DEPTH_TYPE_SHADOW) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, depth_height,
				depth_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
				this->depth_texture, 0);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, depth_width, depth_height,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				GL_TEXTURE_2D, this->depth_texture, 0);
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	bool GBuffer::CheckCompletion() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			return false;
		}
		return true;
	}

	void GBuffer::StartFrame() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GBuffer::ShadowPass() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
		glDrawBuffer(GL_NONE);
		glViewport(0, 0, this->depth_width, this->depth_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GBuffer::GeometyPass() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
		GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(this->num_color_textures, DrawBuffers);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
	}

	void GBuffer::StencilPass() {
		glDrawBuffer(GL_NONE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_STENCIL_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	}

	void GBuffer::LightPass(GLuint external_shadow_map_texture) {
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		for (unsigned int i = 0; i < this->num_color_textures; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, this->color_textures[i]);
		}
		glActiveTexture(GL_TEXTURE0 + this->num_color_textures);
		glBindTexture(GL_TEXTURE_2D, external_shadow_map_texture);
	}

	void GBuffer::FinalPass() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frame_buffer_object);
		glReadBuffer(GL_COLOR_ATTACHMENT4);
	}

	void GBuffer::BindForRendering() const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frame_buffer_object);
	}

	void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) const {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
	}
}
