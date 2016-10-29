// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#ifndef TRILLEK_CLIENT_SOUND_SYSTEM_HPP
#define TRILLEK_CLIENT_SOUND_SYSTEM_HPP

#include <memory>
#include <set>
#include <iostream>
#include <al.h>
#include <alc.h>

#include "spdlog/spdlog.h"
#include "multiton.hpp"
#include "entity.hpp"
#include "events.hpp"
#include "types.hpp"
#include "command-queue.hpp"
#include "event-queue.hpp"

namespace tec {
	class VorbisStream;
	typedef Multiton<std::string, std::shared_ptr<VorbisStream>> SoundMap;

	enum AUDIOSOURCE_STATE { PLAYING, PAUSED, STOPPED };

	extern std::map<std::string, std::function<void(std::string)>> file_factories;
	struct AudioSource {
		AudioSource(std::shared_ptr<VorbisStream> stream, bool auto_play) :
			vorbis_stream(stream), source_state(auto_play ? PLAYING : PAUSED),
			gain(100) { }
		AudioSource() : vorbis_stream(nullptr), source_state(PAUSED), gain(100)
		{ }

		void Out(proto::Component* target) {
			proto::AudioSource* comp = target->mutable_audio_source();
			comp->set_audio_name(this->audio_name);
			comp->set_looping(this->looping);
			comp->set_playing((this->source_state == PLAYING) ? true : false);
			comp->set_volume(this->gain);
		}
		void In(const proto::Component& source) {
			const proto::AudioSource& comp = source.audio_source();
			if (comp.has_audio_name()) {
				this->audio_name = comp.audio_name();
				if (!SoundMap::Has(this->audio_name)) {
					std::string ext = this->audio_name.substr(
						this->audio_name.find_last_of(".") + 1);
					if (file_factories.find(ext) != file_factories.end()) {
						file_factories[ext](this->audio_name);
					}
				}
				this->vorbis_stream = SoundMap::Get(this->audio_name);
			}
			if (comp.has_looping()) {
				this->looping = comp.looping();
			}
			if (comp.has_playing()) {
				this->source_state = comp.playing() ? PLAYING : PAUSED;
			}
			if (comp.has_volume()) {
				this->gain = comp.volume();
			}
		}

		ALuint source;
		ALuint buffer[2];
		bool looping;
		std::shared_ptr<VorbisStream> vorbis_stream;
		AUDIOSOURCE_STATE source_state;
		int gain; // Volume
		std::string audio_name;
	};

	class SoundSystem : public CommandQueue < SoundSystem >,
		public EventQueue<EntityCreated>, public EventQueue<EntityDestroyed> {
	public:
		SoundSystem();

		void Update();

		void SetDelta(const double delta) {
			this->delta = delta;
		}

		void Stop() {
			this->running = false;
		}
		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityDestroyed> data);
	private:
		std::atomic<bool> running;
		double delta;
		std::shared_ptr<spdlog::logger> _log;
		typedef Multiton<eid, AudioSource*> AudioSourceComponentMap;
		ALCdevice *device;
		ALCcontext *context;

		std::set<AudioSource*> queued_sources;
	};
}

#endif
