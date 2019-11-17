// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <memory>
#include <string>

#include <al.h>
#include <alc.h>

#include "multiton.hpp"
#include "filesystem.hpp"

#define STB_VORBIS_HEADER_ONLY

// Rather than modifying the source to have these pragma just wrap the usages in the trillek code
#if defined(_MSC_VER)
	#pragma warning( push )
	#pragma warning( disable : 4244) // Conversion loss of data

	#pragma warning( push )
	#pragma warning( disable : 4457) // declaration hides function parameter

	#pragma warning( push )
	#pragma warning( disable : 4456) // declaration hides previous declaration

	#pragma warning( push )
	#pragma warning( disable : 4245) // signed / unsigned mismatch

	#pragma warning( push )
	#pragma warning( disable : 4701) // potentially uninitialized variable

	#include "resources/stb_vorbis.c"

	#pragma warning( pop ) 
	#pragma warning( pop ) 
	#pragma warning( pop ) 
	#pragma warning( pop ) 
	#pragma warning( pop )
#else
	#include "resources/stb_vorbis.c"
#endif

namespace tec {
	class VorbisStream {
	public:
		VorbisStream(std::size_t buffer_size = 32768);
		~VorbisStream();

		std::size_t BufferStream(ALint buffer);

		void Reset();

		/**
		 * \brief Creates a VorbisStream resource from the given filename.
		 *
		 * The only used initialization property is "filename".
		 * \param[in] const std::vector<Property>& properties The creation properties for the resource.
		 * \return std::shared_ptr<VorbisStream> The created VorbisStream resource.
		 */
		static std::shared_ptr<VorbisStream> Create(const FilePath& filename);
		
		const std::string GetName() const {
			return this->name;
		}
		void SetName(const std::string& _name) {
			this->name = _name;
		}
	private:
		std::string name;
		stb_vorbis* stream{ nullptr };
		stb_vorbis_info info;
		ALenum format;
		std::size_t buffer_size{ 0 };
		std::size_t totalSamplesLeft{ 0 };
		ALshort* sbuffer{ nullptr };
	};
}
