#include "graphics/gbuffer.hpp"

namespace tec {
	bool GBuffer::Init(unsigned int window_width, unsigned int window_height) {
		// Create the FBO
		glGenFramebuffers(1, &this->frame_buffer_object);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);

		// Create the gbuffer textures
		glGenTextures(GBUFFER_NUM_TEXTURES, this->textures);
		glGenTextures(1, &this->depth_texture);

		for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
			glBindTexture(GL_TEXTURE_2D, this->textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window_width, window_height, 0, GL_RGB, GL_FLOAT, nullptr);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->textures[i], 0);
		}

		// depth
		glBindTexture(GL_TEXTURE_2D, this->depth_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
			nullptr);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth_texture, 0);

		GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE) {
			return false;
		}

		// restore default FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return true;
	}

	bool GBuffer::Resize(unsigned int window_width, unsigned int window_height) {
		// Create the FBO
		if (this->frame_buffer_object) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);

			for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
				if (this->textures[i]) {
					glBindTexture(GL_TEXTURE_2D, this->textures[i]);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window_width, window_height, 0, GL_RGB, GL_FLOAT, nullptr);
					glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->textures[i], 0);
				}
			}

			// depth
			if (this->depth_texture) {
				glBindTexture(GL_TEXTURE_2D, this->depth_texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
					nullptr);
				glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth_texture, 0);
			}

			GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
			glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE) {
			return false;
		}

		// restore default FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return true;
	}

	void GBuffer::BindForWriting() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
	}

	void GBuffer::BindForRendering() const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frame_buffer_object);
	}

	void GBuffer::BindForReading() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, this->textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
		}
	}

	void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) const {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
	}
}
