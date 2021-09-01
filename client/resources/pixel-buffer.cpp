#include "pixel-buffer.hpp"

#include <cstring>
#include <fstream>

#include <spdlog/spdlog.h>

namespace tec {

static uint32_t BytesForBitSize(uint32_t bits) { return ((bits / 8) + ((bits & 0x7u) ? 1 : 0)); }

PixelBuffer::PixelBuffer(PixelBuffer&& rv) noexcept {
	writelock.lock();
	rv.writelock.lock();
	imagewidth = rv.imagewidth;
	imageheight = rv.imageheight;
	bufferpitch = rv.bufferpitch;
	channelbitdepth = rv.channelbitdepth;
	imagepixelsize = rv.imagepixelsize;
	imagemode = rv.imagemode;
	image_x = rv.image_x;
	image_y = rv.image_y;
	dirty = rv.dirty;
	blockptr = std::move(rv.blockptr);
	rv.imageheight = 0;
	rv.imagewidth = 0;
	rv.bufferpitch = 0;
	rv.writelock.unlock();
	writelock.unlock();
}
PixelBuffer::PixelBuffer(uint32_t width, uint32_t height, uint32_t bitspersample, ImageColorMode mode) {
	imagepixelsize = 0;
	imagewidth = 0;
	imageheight = 0;
	channelbitdepth = 0;
	imagemode = ImageColorMode::MONOCHROME;
	image_x = 0;
	image_y = 0;
	dirty = false;
	Create(width, height, bitspersample, mode);
}

PixelBuffer& PixelBuffer::operator=(PixelBuffer&& rv) noexcept {
	writelock.lock();
	rv.writelock.lock();
	imagewidth = rv.imagewidth;
	imageheight = rv.imageheight;
	bufferpitch = rv.bufferpitch;
	channelbitdepth = rv.channelbitdepth;
	imagepixelsize = rv.imagepixelsize;
	imagemode = rv.imagemode;
	image_x = rv.image_x;
	image_y = rv.image_y;
	dirty = rv.dirty;
	blockptr = std::move(rv.blockptr);
	rv.imageheight = 0;
	rv.imagewidth = 0;
	rv.bufferpitch = 0;
	rv.writelock.unlock();
	writelock.unlock();
	return *this;
}

void PixelBuffer::PPMDebug() {
	// output a PPM image to stderr as a debug feature
	std::fprintf(stderr, "P6\n%d\n%d\n%d\n", imagewidth, imageheight, (1u << channelbitdepth) - 1);
	if (!blockptr)
		return;
	uint8_t* p = blockptr.get();
	switch (imagemode) {
	case ImageColorMode::COLOR_RGB:
	case ImageColorMode::GAMMA_RGB:
		for (uint32_t i = 0; i < bufferpitch * imageheight; i++) {
			std::fputc(p[i], stderr);
		}
		break;
	case ImageColorMode::COLOR_RGBA:
	case ImageColorMode::GAMMA_RGBA:
		for (uint32_t b = 0, i = 0; i < bufferpitch * imageheight; i++, b++) {
			if (b == 4)
				b = 0;
			if (b < 3)
				std::fputc(p[i], stderr); // only output RGB
		}
		break;
	default: break;
	}
}
void PixelBuffer::PPMDebug(const char* ofile) {
	// output a PPM image to file
	std::fstream file(ofile, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		return;
	}
	file << "P6\n" << imagewidth << '\n' << imageheight << '\n';
	file << ((1u << channelbitdepth) - 1) << '\n';
	if (!blockptr)
		return;
	uint8_t* p = blockptr.get();
	switch (imagemode) {
	case ImageColorMode::COLOR_RGB:
	case ImageColorMode::GAMMA_RGB: file.write((char*)p, bufferpitch * imageheight); break;
	case ImageColorMode::COLOR_RGBA:
	case ImageColorMode::GAMMA_RGBA:
		for (uint32_t i = 0; i < bufferpitch * imageheight; i += 4) {
			file.write((char*)(p + i), 3); // only output RGB
		}
		break;
	default: break;
	}
}

bool PixelBuffer::Create(uint32_t width, uint32_t height, uint32_t bitsperchannel, ImageColorMode mode) {
	uint32_t channels;
	switch (mode) {
	case ImageColorMode::COLOR_RGB:
	case ImageColorMode::GAMMA_RGB: channels = 3; break;
	case ImageColorMode::COLOR_RGBA:
	case ImageColorMode::GAMMA_RGBA: channels = 4; break;
	case ImageColorMode::MONOCHROME: channels = 1; break;
	case ImageColorMode::MONOCHROME_A: channels = 2; break;
	default: return false;
	}
	writelock.lock();
	imagepixelsize = BytesForBitSize(bitsperchannel * channels);
	imagewidth = width;
	imageheight = height;
	channelbitdepth = bitsperchannel;
	imagemode = mode;
	image_x = 0;
	image_y = 0;
	dirty = true;
	bufferpitch = width * imagepixelsize;

	blockptr = std::unique_ptr<uint8_t[]>(new uint8_t[bufferpitch * height]);
	if (!blockptr) {
		writelock.unlock();
		return false;
	}
	std::memset(blockptr.get(), 0, bufferpitch * height);
	writelock.unlock();
	return true;
}

std::shared_ptr<PixelBuffer> PixelBuffer::Create(const std::string name, const FilePath& filename, bool gamma_space) {
	auto pbuf = std::make_shared<PixelBuffer>();
	PixelBufferMap::Set(name, pbuf);
	if (!filename.empty()) {
		pbuf->Load(filename, gamma_space);
	}
	return pbuf;
}

bool PixelBuffer::IsDirty() const { return dirty; }
void PixelBuffer::Invalidate() { dirty = true; }
void PixelBuffer::Validate() { dirty = false; }

const uint8_t* PixelBuffer::GetBlockBase() const {
	if (blockptr) {
		return blockptr.get();
	}
	return nullptr;
}

#define STB_IMAGE_IMPLEMENTATION
#include "resources/stb_image.h"

bool PixelBuffer::Load(const FilePath& filename, bool gamma_space) {
	int num_components;
	unsigned char* data;
	// FIXME Better to pass a FILE handler and use the native fopen / fopen_w. Perhaps add a
	// fopen to FileSystem ? Also we not are doing path valid or file existence check
	data = stbi_load(filename.toString().c_str(), &this->imagewidth, &this->imageheight, &num_components, 0);
	if (data) {
		uint32_t channels = std::max(num_components, 0);
		switch (channels) {
		case 3: this->imagemode = gamma_space ? ImageColorMode::GAMMA_RGB : ImageColorMode::COLOR_RGB; break;
		case 4: this->imagemode = gamma_space ? ImageColorMode::GAMMA_RGBA : ImageColorMode::COLOR_RGBA; break;
		case 1: this->imagemode = ImageColorMode::MONOCHROME; break;
		case 2: this->imagemode = ImageColorMode::MONOCHROME_A; break;
		default: this->imagemode = ImageColorMode::UNKNOWN_MODE; break;
		}
		this->channelbitdepth = 8;
		this->image_x = 0;
		this->image_y = 0;
		this->dirty = true;
		uint32_t bits_per_pixel = (this->channelbitdepth * channels);
		this->imagepixelsize = BytesForBitSize(bits_per_pixel);
		this->bufferpitch = this->imagewidth * this->imagepixelsize;
		this->writelock.lock();
		this->blockptr.reset(data);
		this->writelock.unlock();

		spdlog::get("console_log")->debug("[Pixel-Buffer] Loaded image {}", filename.FileName());
		return true;
	}

	spdlog::get("console_log")->warn("[Pixel-Buffer] Can't load image {}", filename.toString());
	return false;
}

} // namespace tec
