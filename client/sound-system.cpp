#include "sound-system.hpp"

#include "entity.hpp"
#include "physics-system.hpp" // TODO need to replace when Velocity component is extract.
#include "components/transforms.hpp"
#include "resources/vorbis-stream.hpp"

namespace tec {
	inline void alCheckError() {
		ALCenum error;
		error = alGetError();
		if (error != AL_NO_ERROR) {
			spdlog::get("console_log")->debug("[Sound System] {}", error);
		}
	}

	SoundSystem::SoundSystem() : running(true) {
		_log = spdlog::get("console_log");

		this->device = alcOpenDevice(NULL); alCheckError();
		if (!this->device) {
			_log->warn() << "[Sound System] No OpenAL device selected.";
		}

		this->context = alcCreateContext(device, NULL); alCheckError();
		if (!alcMakeContextCurrent(this->context)) {
			_log->warn() << "[Sound System] No OpenAL context created.";
		}
		alListener3f(AL_POSITION, 0, 0, 0);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		alListener3f(AL_ORIENTATION, 0, 0, -1);
	}

	void SoundSystem::Update() {
		while (this->running) {
			if (this->delta < 0.0) {
				continue;
			}
			ProcessCommandQueue();

			for (auto itr = AudioSourceComponentMap::Begin(); itr != AudioSourceComponentMap::End(); ++itr) {
				if (this->queued_sources.find(itr->second) == this->queued_sources.end()) {
					// Make sure the stream is valid before we attempt to buffer it.
					if (!itr->second->vorbis_stream) {
						continue;
					}
					alGenSources(1, &itr->second->source); alCheckError();

					alSourcef(itr->second->source, AL_PITCH, 1); alCheckError();
					alSourcef(itr->second->source, AL_GAIN, itr->second->gain / 100.0f); alCheckError();
					if (Entity(itr->first).Has<Position>()) {
						std::shared_ptr<Position> pos = Entity(itr->first).Get<Position>().lock();
						alSource3f(itr->second->source, AL_POSITION, pos->value.x, pos->value.y, pos->value.z);
					}
					else {
						alSource3f(itr->second->source, AL_POSITION, 0, 0, 0);
					}
					if (Entity(itr->first).Has<Velocity>()) {
						std::shared_ptr<Velocity> vel = Entity(itr->first).Get<Velocity>().lock();
						alSource3f(itr->second->source, AL_VELOCITY, vel->linear.x, vel->linear.y, vel->linear.z);
					}
					else {
						alSource3f(itr->second->source, AL_VELOCITY, 0, 0, 0);
					}

					// Don't use for looping or it will loop over the same segment, not the overall stream, use AudioSource::looping instead.
					alSourcei(itr->second->source, AL_LOOPING, AL_FALSE);

					alGenBuffers(2, &itr->second->buffer[0]); alCheckError();

					itr->second->vorbis_stream->BufferStream(itr->second->buffer[0]);
					itr->second->vorbis_stream->BufferStream(itr->second->buffer[1]);

					alSourceQueueBuffers(itr->second->source, 2, itr->second->buffer); alCheckError();
					switch (itr->second->source_state) {
						case PLAYING:
							alSourcei(itr->second->source, AL_SOURCE_STATE, AL_PLAYING);
							break;
						case PAUSED:
							alSourcei(itr->second->source, AL_SOURCE_STATE, AL_PAUSED);
							break;
					}
					this->queued_sources.insert(itr->second);
				}
			}

			for (auto source : this->queued_sources) {
				ALint processed = 0;

				alGetSourcei(source->source, AL_BUFFERS_PROCESSED, &processed);

				while (processed--) {
					ALuint buffer = 0;

					alSourceUnqueueBuffers(source->source, 1, &buffer); alCheckError();

					if (source->vorbis_stream->BufferStream(buffer) <= 0) {
						if (source->looping) {
							source->vorbis_stream->Reset();
							source->vorbis_stream->BufferStream(buffer);
						}
					}
					alSourceQueueBuffers(source->source, 1, &buffer); alCheckError();
				}
				ALint state;
				alGetSourcei(source->source, AL_SOURCE_STATE, &state); alCheckError();
				alSourcef(source->source, AL_GAIN, source->gain / 100.0f); alCheckError();

				// If there was a state change to play then start playing
				if (state != AL_PLAYING && source->source_state == PLAYING) {
					alSourcePlay(source->source); alCheckError();
				}
				else if (state == AL_PLAYING) {
					if (source->source_state == PAUSED) {
						alSourcePause(source->source); alCheckError();
						alSourcei(source->source, AL_SOURCE_STATE, AL_PAUSED);
					}
					else if (source->source_state == STOPPED) {
						alSourceStop(source->source); alCheckError();
						alSourcei(source->source, AL_SOURCE_STATE, AL_STOPPED);
					}
				}
			}
			this->delta = -1.0;
		}
	}
}
