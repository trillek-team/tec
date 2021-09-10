#include "vorbis-stream.hpp"

#include <spdlog/spdlog.h>

#include "multiton.hpp"

#undef STB_VORBIS_HEADER_ONLY

// Rather than modifying the source to have these pragma just wrap the usages in the trillek code
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4244) // Conversion loss of data

#pragma warning(push)
#pragma warning(disable : 4457) // declaration hides function parameter

#pragma warning(push)
#pragma warning(disable : 4456) // declaration hides previous declaration

#pragma warning(push)
#pragma warning(disable : 4245) // signed / unsigned mismatch

#pragma warning(push)
#pragma warning(disable : 4701) // potentially uninitialized variable

#include "stb_vorbis.c"

#pragma warning(pop)
#pragma warning(pop)
#pragma warning(pop)
#pragma warning(pop)
#pragma warning(pop)
#else
#include "stb_vorbis.c"
#endif

namespace tec {
typedef Multiton<std::string, std::shared_ptr<VorbisStream>> SoundMap;
VorbisStream::VorbisStream(unsigned buffer_size) : buffer_size(buffer_size) {
	this->sbuffer = new ALshort[this->buffer_size];
}

VorbisStream::~VorbisStream() {
	delete[] this->sbuffer;
	delete this->stream;
}

std::size_t VorbisStream::BufferStream(ALint buffer) {
	if (!this->stream) {
		return 0;
	}
	std::size_t size = 0;
	int num_read = 0;

	while (size < this->buffer_size) {
		num_read = stb_vorbis_get_samples_short_interleaved(
				this->stream, this->info.channels, this->sbuffer + size, this->buffer_size - size);
		if (num_read > 0) {
			size += num_read * this->info.channels;
		}
		else {
			break;
		}
	}

	if (size == 0) {
		return 0;
	}

	alBufferData(buffer, this->format, this->sbuffer, size * sizeof(ALshort), this->info.sample_rate);
	this->totalSamplesLeft -= size;

	return size;
}

void VorbisStream::Reset() {
	stb_vorbis_seek_start(this->stream);
	this->totalSamplesLeft = stb_vorbis_stream_length_in_samples(this->stream) * this->info.channels;
}

inline std::string VorbisErrorToString(int error) {
	switch (error) {
	case VORBIS__no_error: return "No error";
	case VORBIS_need_more_data: return "Need more data";
	case VORBIS_invalid_api_mixing: return "Can't mix API modes";
	case VORBIS_outofmem: return "Out of memory!";
	case VORBIS_too_many_channels: return "Reached max number of channels";
	case VORBIS_file_open_failure: return "Can't open file";
	case VORBIS_seek_without_length: return "Unknown length of file";
	case VORBIS_unexpected_eof: return "File truncated or I/O error";
	case VORBIS_seek_invalid: return "Invalid seek. Corrupted file ?";
	default: return "Unknown Vorbis error";
	}
}

std::shared_ptr<VorbisStream> VorbisStream::Create(const Path& filename) {
	auto _log = spdlog::get("console_log");
	std::shared_ptr<VorbisStream> stream = std::make_shared<VorbisStream>();
	stream->SetName(filename.Relative().toString());

	int error;
	std::unique_ptr<FILE> file;
	try {
		file = filename.OpenFile();
	}
	catch (PathException& e) {
		_log->warn("[Vorbis-Stream] Can't open file {}", filename);
		stream.reset();
		return stream;
	}
	stream->stream = stb_vorbis_open_file(file.release(), TRUE, &error, nullptr);
	if (stream->stream) {
		stream->info = stb_vorbis_get_info(stream->stream);
		if (stream->info.channels == 2) {
			stream->format = AL_FORMAT_STEREO16;
		}
		else {
			stream->format = AL_FORMAT_MONO16;
		}
		stream->totalSamplesLeft = stb_vorbis_stream_length_in_samples(stream->stream) * stream->info.channels;
		SoundMap::Set(stream->GetName(), stream);
	}
	else {
		_log->warn("[Vorbis-Stream] Can't load file {} code {} : {}", filename, error, VorbisErrorToString(error));
		stream.reset();
	}
	return stream;
}
} // namespace tec
