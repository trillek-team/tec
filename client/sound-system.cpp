// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "sound-system.hpp"

#include "event-system.hpp"
#include "entity.hpp"
#include "components/velocity.hpp"
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

	SoundSystem::SoundSystem() {
		_log = spdlog::get("console_log");

		this->device = alcOpenDevice(NULL);// alCheckError();
		if (!this->device) {
			_log->warn("[Sound System] No OpenAL device selected.");
		}

		this->context = alcCreateContext(device, NULL);// alCheckError();
		if (!alcMakeContextCurrent(this->context)) {
			_log->warn("[Sound System] No OpenAL context created.");
		}
		alListener3f(AL_POSITION, 0, 0, 0); alCheckError();
		alListener3f(AL_VELOCITY, 0, 0, 0); alCheckError();
		//alListener3f(AL_DIRECTION, 0, 0,-1); alCheckError();
	}

	void SoundSystem::Update() {
		while (this->running) {
			if (this->delta < 0.0) {
				continue;
			}
			ProcessCommandQueue();
			EventQueue<EntityCreated>::ProcessEventQueue();

			for (auto itr = AudioSourceComponentMap::Begin(); itr != AudioSourceComponentMap::End(); ++itr) {
				if (this->queued_sources.find(itr->second) == this->queued_sources.end()) {
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
				if (state != AL_PLAYING && source->source_state == AUDIOSOURCE_STATE::PLAYING) {
					alSourcePlay(source->source); alCheckError();
				}
				else if (state == AL_PLAYING) {
					if (source->source_state == AUDIOSOURCE_STATE::PAUSED) {
						alSourcePause(source->source); alCheckError();
						alSourcei(source->source, AL_SOURCE_STATE, AL_PAUSED);
					}
					else if (source->source_state == AUDIOSOURCE_STATE::STOPPED) {
						alSourceStop(source->source); alCheckError();
						alSourcei(source->source, AL_SOURCE_STATE, AL_STOPPED);
					}
				}
			}
			this->delta = -1.0;
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	}

	void SoundSystem::On(std::shared_ptr<EntityCreated> data) {
		const proto::Entity& entity = data->entity;
		eid entity_id = entity.id();
		for (int i = 0; i < data->entity.components_size(); ++i) {
			const proto::Component& comp = data->entity.components(i);
			switch (comp.component_case()) {
				case proto::Component::kAudioSource:
				{
					AudioSource* audio_source = new AudioSource();
					audio_source->In(comp);
					AudioSourceComponentMap::Set(entity_id, audio_source);
				}
				break;
				case proto::Component::kRenderable:
				case proto::Component::kPointLight:
				case proto::Component::kDirectionalLight:
				case proto::Component::kPosition:
				case proto::Component::kOrientation:
				case proto::Component::kView:
				case proto::Component::kAnimation:
				case proto::Component::kScale:
				case proto::Component::kCollisionBody:
				case proto::Component::kVelocity:
				case proto::Component::kSpotLight:
				case proto::Component::kVoxelVolume:
				case proto::Component::kComputer:
				case proto::Component::kLuaScript:
				case proto::Component::COMPONENT_NOT_SET:
				break;
			}
		}
		if (AudioSourceComponentMap::Has(entity_id)) {
			AudioSource* source = AudioSourceComponentMap::Get(entity_id);
			if (!source->vorbis_stream) {
				return;
			}
			alGenSources(1, &source->source); alCheckError();

			alSourcef(source->source, AL_PITCH, 1); alCheckError();
			alSourcef(source->source, AL_GAIN, source->gain / 100.0f); alCheckError();
			if (Entity(entity_id).Has<Position>()) {
				const Position* pos = Entity(entity_id).Get<Position>();
				alSource3f(source->source, AL_POSITION, pos->value.x, pos->value.y, pos->value.z);
			}
			else {
				alSource3f(source->source, AL_POSITION, 0, 0, 0);
			}
			if (Entity(entity_id).Has<Velocity>()) {
				const Velocity* vel = Entity(entity_id).Get<Velocity>();
				alSource3f(source->source, AL_VELOCITY, vel->linear.x, vel->linear.y, vel->linear.z);
			}
			else {
				alSource3f(source->source, AL_VELOCITY, 0, 0, 0);
			}

			// Don't use for looping or it will loop over the same segment, not the overall stream, use AudioSource::looping instead.
			alSourcei(source->source, AL_LOOPING, AL_FALSE);

			alGenBuffers(2, &source->buffer[0]); alCheckError();

			source->vorbis_stream->BufferStream(source->buffer[0]);
			source->vorbis_stream->BufferStream(source->buffer[1]);

			alSourceQueueBuffers(source->source, 2, source->buffer); alCheckError();
			switch (source->source_state) {
				case AUDIOSOURCE_STATE::PLAYING:
					alSourcei(source->source, AL_SOURCE_STATE, AL_PLAYING);
					break;
				case AUDIOSOURCE_STATE::PAUSED:
					alSourcei(source->source, AL_SOURCE_STATE, AL_PAUSED);
					break;
				case AUDIOSOURCE_STATE::STOPPED:
					break;
			}
		}
	}
	void SoundSystem::On(std::shared_ptr<EntityDestroyed>) {

	}
}
