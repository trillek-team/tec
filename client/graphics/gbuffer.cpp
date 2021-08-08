#include "gbuffer.hpp"

#include <spdlog/spdlog.h>

namespace tec {
void GBuffer::AddColorAttachments(const unsigned int window_width, const unsigned int window_height) {
	if (this->frame_buffer_object == 0) {
		glGenFramebuffers(1, &this->frame_buffer_object);
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
	glGenTextures(this->num_color_textures, this->color_textures);
	glGenTextures(1, &this->final_texture);

	ResizeColorAttachments(window_width, window_height);
}

void GBuffer::ResizeColorAttachments(const unsigned int window_width, const unsigned int window_height) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
	for (unsigned int i = 0; i < this->num_color_textures; i++) {
		glBindTexture(GL_TEXTURE_2D, this->color_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->color_textures[i], 0);
	}

	glBindTexture(GL_TEXTURE_2D, this->final_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGB, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, this->final_texture, 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::SetDepthAttachment(GBUFFER_DEPTH_TYPE type, const unsigned int width, const unsigned int height) {
	this->depth_type = type;
	glGenTextures(1, &this->depth_texture);
	ResizeDepthAttachment(width, height);
}

void GBuffer::ResizeDepthAttachment(const unsigned int width, const unsigned int height) {
	this->depth_width = width;
	this->depth_height = height;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object);
	// FIXME: minor thing: this used to create a depth+stencil format
	// however it was doing so using *Renderbuffers* not textures
	// GL_DEPTH32F_STENCIL8 isn't supported on textures (or rarely)
	// so maybe a separate STENCIL8 texture or renderbuffer could be added
	// also note: the stencil pass isn't entirely required, it's just a possible optimization.
	glBindTexture(GL_TEXTURE_2D, this->depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0,  GL_DEPTH_COMPONENT, GL_BYTE, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth_texture, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

bool GBuffer::CheckCompletion() const {
	this->BindForWriting();
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return status == GL_FRAMEBUFFER_COMPLETE;
}

void GBuffer::StartFrame() const {
	this->BindForWriting();
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::BeginGeometryPass() const {
	this->BindForWriting();
	GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(this->num_color_textures, DrawBuffers);

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
}

void GBuffer::EndGeometryPass() { glDepthMask(GL_FALSE); }

void GBuffer::StencilPass() {
	glDrawBuffer(GL_NONE);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 0, 0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
}

void GBuffer::BeginLightPass() {
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	for (unsigned int i = 0; i < this->num_color_textures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->color_textures[i]);
	}
}

void GBuffer::BeginDirLightPass() { glStencilFunc(GL_ALWAYS, 0, 0); }

void GBuffer::BeginPointLightPass() {}

void GBuffer::EndPointLightPass() {
	// glDisable(GL_BLEND);
}

void GBuffer::FinalPass() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	this->BindForRendering();
	glReadBuffer(GL_COLOR_ATTACHMENT4);
}

void GBuffer::BindForWriting() const { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_object); }

void GBuffer::BindForRendering() const { glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frame_buffer_object); }

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + static_cast<int>(TextureType));
}
} // namespace tec
