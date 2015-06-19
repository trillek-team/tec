#pragma once

#include <memory>
#include <string>
#include <al.h>
#include <alc.h>

#include "multiton.hpp"

#define STB_VORBIS_HEADER_ONLY
#include "resources/stb_vorbis.c"

namespace tec {
	class VorbisStream {
	public:
		VorbisStream(size_t buffer_size = 32768);
		~VorbisStream();

		size_t BufferStream(ALint buffer);

		void Reset();

		/**
		 * \brief Creates a VorbisStream resource from the given filename.
		 *
		 * The only used initialization property is "filename".
		 * \param[in] const std::vector<Property>& properties The creation properties for the resource.
		 * \return std::shared_ptr<MD5Mesh> The created MD5Mesh resource.
		 */
		static std::shared_ptr<VorbisStream> Create(std::string filename);
	private:
		stb_vorbis* stream;
		stb_vorbis_info info;
		ALenum format;
		size_t buffer_size;
		size_t totalSamplesLeft;
		ALshort* sbuffer;
	};
}
