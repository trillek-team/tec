#include "resources/vorbis-stream.hpp"

#undef STB_VORBIS_HEADER_ONLY
#include "resources/stb_vorbis.c"
namespace tec {

	VorbisStream::VorbisStream(size_t buffer_size) : buffer_size(buffer_size), stream(nullptr) {
		this->sbuffer = new ALshort[this->buffer_size];
	}

	VorbisStream::~VorbisStream() {
		delete[] this->sbuffer;
		delete this->stream;
	}

	size_t VorbisStream::BufferStream(ALint buffer) {
		if (!this->stream) {
			return 0;
		}

		size_t  size = 0;
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

	std::shared_ptr<VorbisStream> VorbisStream::Create(std::string filename) {
		std::shared_ptr<VorbisStream> stream = std::make_shared<VorbisStream>();
		int error;
		stream->stream = stb_vorbis_open_filename(filename.c_str(), &error, NULL);
		if (stream->stream) {
			stream->info = stb_vorbis_get_info(stream->stream);
			if (stream->info.channels == 2) {
				stream->format = AL_FORMAT_STEREO16;
			}
			else {
				stream->format = AL_FORMAT_MONO16;
			}
			stream->totalSamplesLeft = stb_vorbis_stream_length_in_samples(stream->stream) * stream->info.channels;
		}
		else {
			stream.reset();
		}
		return stream;
	}
}
