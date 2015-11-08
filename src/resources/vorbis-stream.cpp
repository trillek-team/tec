#include "resources/vorbis-stream.hpp"

#include "spdlog/spdlog.h"
#include "sound-system.hpp"

#undef STB_VORBIS_HEADER_ONLY
#include "resources/stb_vorbis.c"
namespace tec {

	VorbisStream::VorbisStream(std::size_t buffer_size) : buffer_size(buffer_size), stream(nullptr) {
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

		std::size_t  size = 0;
		int  num_read = 0;

		while (size < this->buffer_size) {
			num_read = stb_vorbis_get_samples_short_interleaved(this->stream,
				this->info.channels, this->sbuffer + size, this->buffer_size - size);
			if (num_read > 0) {
				size += num_read * this->info.channels;
			}
			else {
				break;
			}
		}


		if (size == 0) {
			return false;
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
		case VORBIS__no_error:
			return "No error";
		case VORBIS_need_more_data:
			return "Need more data";
		case VORBIS_invalid_api_mixing:
			return "Can't mix API modes";
		case VORBIS_outofmem:
			return "Out of memory!";
		case VORBIS_too_many_channels:
			return "Reached max number of channels";
		case VORBIS_file_open_failure:
			return "Can't open file";
		case VORBIS_seek_without_length:
			return "Unknow lenght of file";
		case VORBIS_unexpected_eof:
			return "File truncated or I/O error";
		case VORBIS_seek_invalid:
			return "Invalid seek. Corrupted file ?";
		default:
			return "Unknow Vorbis error";
		}
	}

	std::shared_ptr<VorbisStream> VorbisStream::Create(const FilePath& filename) {
		std::shared_ptr<VorbisStream> stream = std::make_shared<VorbisStream>();
		//stream->SetFileName(fname);
		stream->SetName(filename.SubpathFrom("assets").toGenericString());
		
		int error;
		// FIXME Better to pass a FILE handler and use the native fopen / fopen_w. Perhaps add a fopen to FileSystem ?
		// Als we not are doing path valid or file existence check
		stream->stream = stb_vorbis_open_filename(filename.toString().c_str(), &error, NULL);
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
		} else {
			spdlog::get("console_log")->warn("[Vorbis-Stream] Can't load file {} code {} : {}", filename.toString(), error, VorbisErrorToString(error));
			stream.reset();
		}
		return stream;
	}
}
