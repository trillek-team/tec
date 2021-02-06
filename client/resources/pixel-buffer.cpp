#include "pixel-buffer.hpp"

#include <cstring>
#include <fstream>

#include <spdlog/spdlog.h>

namespace tec {
	PixelBuffer::PixelBuffer(PixelBuffer && rv) noexcept {
		writelock.lock();
		rv.writelock.lock();
		imagewidth = rv.imagewidth;
		imageheight = rv.imageheight;
		bufferpitch = rv.bufferpitch;
		imagebitdepth = rv.imagebitdepth;
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
		imagebitdepth = 0;
		imagemode = ImageColorMode::MONOCHROME;
		image_x = 0;
		image_y = 0;
		dirty = false;
		Create(width, height, bitspersample, mode);
	}

	PixelBuffer & PixelBuffer::operator=(PixelBuffer && rv) noexcept {
		writelock.lock();
		rv.writelock.lock();
		imagewidth = rv.imagewidth;
		imageheight = rv.imageheight;
		bufferpitch = rv.bufferpitch;
		imagebitdepth = rv.imagebitdepth;
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
		std::fprintf(stderr, "P6\n%d\n%d\n%d\n", imagewidth, imageheight, (1u << imagebitdepth) - 1);
		if (!blockptr) return;
		uint8_t * p = blockptr.get();
		switch (imagemode) {
			case ImageColorMode::COLOR_RGB:
				for (uint32_t i = 0; i < bufferpitch * imageheight; i++) {
					std::fputc(p[i], stderr);
				}
				break;
			case ImageColorMode::COLOR_RGBA:
				for (uint32_t b = 0, i = 0; i < bufferpitch * imageheight; i++, b++) {
					if (b == 4) b = 0;
					if (b < 3) std::fputc(p[i], stderr); // only output RGB
				}
				break;
			case ImageColorMode::MONOCHROME:
			case ImageColorMode::MONOCHROME_A:
			case ImageColorMode::UNKNOWN_MODE:
				break;
		}
	}
	void PixelBuffer::PPMDebug(const char * ofile) {
		// output a PPM image to stderr as a debug feature
		std::fstream file(ofile, std::ios::out | std::ios::binary);
		if (!file.is_open()) {
			return;
		}
		file << "P6\n" << imagewidth << '\n' << imageheight << '\n';
		file << ((1u << imagebitdepth) - 1) << '\n';
		if (!blockptr) return;
		uint8_t * p = blockptr.get();
		switch (imagemode) {
			case ImageColorMode::COLOR_RGB:
				file.write((char*)p, bufferpitch * imageheight);
				break;
			case ImageColorMode::COLOR_RGBA:
				for (uint32_t i = 0; i < bufferpitch * imageheight; i += 4) {
					file.write((char*)(p + i), 3); // only output RGB
				}
				break;
			case ImageColorMode::MONOCHROME:
			case ImageColorMode::MONOCHROME_A:
			case ImageColorMode::UNKNOWN_MODE:
				break;

		}
	}

	bool PixelBuffer::Create(uint32_t width, uint32_t height, uint32_t bitspersample, ImageColorMode mode) {
		uint8_t pixelsize;
		switch (mode) {
			case ImageColorMode::COLOR_RGB:
				pixelsize = 3;
				break;
			case ImageColorMode::COLOR_RGBA:
				pixelsize = 4;
				break;
			case ImageColorMode::MONOCHROME:
				pixelsize = 1;
				break;
			case ImageColorMode::MONOCHROME_A:
				pixelsize = 2;
				break;
			default:
				return false;
		}
		writelock.lock();
		imagepixelsize = pixelsize;
		imagewidth = width;
		imageheight = height;
		imagebitdepth = bitspersample;
		imagemode = mode;
		image_x = 0;
		image_y = 0;
		dirty = true;
		bufferpitch = (bitspersample * imagepixelsize);
		bufferpitch = width * ((bufferpitch >> 3u) + ((bufferpitch & 0x7u) ? 1 : 0));

		blockptr = std::unique_ptr<uint8_t[]>(new uint8_t[bufferpitch * height]);
		if (!blockptr) {
			writelock.unlock();
			return false;
		}
		std::memset(blockptr.get(), 0, bufferpitch * height);
		writelock.unlock();
		return true;
	}

	std::shared_ptr<PixelBuffer> PixelBuffer::Create(const std::string& name, const FilePath& filename) {
		auto pbuf = std::make_shared<PixelBuffer>();
		PixelBufferMap::Set(name, pbuf);
		if (!filename.empty()) {
			pbuf->Load(filename);
		}
		return pbuf;
	}

	bool PixelBuffer::IsDirty() const {
		return dirty;
	}
	void PixelBuffer::Invalidate() {
		dirty = true;
	}
	void PixelBuffer::Validate() {
		dirty = false;
	}

	const uint8_t * PixelBuffer::GetBlockBase() const {
		if (blockptr) {
			return blockptr.get();
		}
		return nullptr;
	}

#define STB_IMAGE_IMPLEMENTATION
#include "resources/stb_image.h"

	bool PixelBuffer::Load(const FilePath& filename) {
		int num_components;
		unsigned char *data;
		// FIXME Better to pass a FILE handler and use the native fopen / fopen_w. Perhaps add a fopen to FileSystem ?
		// Also we not are doing path valid or file existence check
		data = stbi_load(filename.toString().c_str(), &this->imagewidth, &this->imageheight, &num_components, 0);
		if (data) {
			switch (num_components) {
				case 3:
					this->imagemode = ImageColorMode::COLOR_RGB;
					break;
				case 4:
					this->imagemode = ImageColorMode::COLOR_RGBA;
					break;
				case 1:
					this->imagemode = ImageColorMode::MONOCHROME;
					break;
				case 2:
					this->imagemode = ImageColorMode::MONOCHROME_A;
					break;
				default:
					this->imagemode = ImageColorMode::UNKNOWN_MODE;
					break;
			}
			this->imagepixelsize = num_components > 0 ? static_cast<unsigned int>(num_components) : 0;
			this->imagebitdepth = 8;
			this->image_x = 0;
			this->image_y = 0;
			this->dirty = true;
			this->bufferpitch = (8 * imagepixelsize);
			this->bufferpitch = this->imagewidth * ((this->bufferpitch >> 3u) +
				((this->bufferpitch & 0x7u) ? 1 : 0));
			this->writelock.lock();
			this->blockptr.reset(data);
			this->writelock.unlock();

			spdlog::get("console_log")->trace("[Pixel-Buffer] Loaded image {}", filename.FileName());
			return true;
		}

		spdlog::get("console_log")->warn("[Pixel-Buffer] Can't load image {}", filename.toString());
		return false;
	}

}
