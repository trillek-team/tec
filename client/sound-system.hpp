#ifndef TRILLEK_CLIENT_SOUND_SYSTEM_HPP
#define TRILLEK_CLIENT_SOUND_SYSTEM_HPP

#include <memory>
#include <set>
#include <iostream>
#include <thread>
#include <utility>

#include <al.h>
#include <alc.h>
#include <spdlog/spdlog.h>

#include "multiton.hpp"
#include "entity.hpp"
#include "events.hpp"
#include "tec-types.hpp"
#include "command-queue.hpp"
#include "event-queue.hpp"

namespace tec {
	class VorbisStream;
	typedef Multiton<std::string, std::shared_ptr<VorbisStream>> SoundMap;

	enum class AUDIOSOURCE_STATE { PLAYING, PAUSED, STOPPED };

	extern std::unordered_map<std::string, std::function<void(std::string)>> file_factories;
	struct AudioSource {
		AudioSource(std::shared_ptr<VorbisStream> stream, bool auto_play) :
			vorbis_stream(std::move(stream)), source_state(auto_play ? AUDIOSOURCE_STATE::PLAYING : AUDIOSOURCE_STATE::PAUSED) {}
		AudioSource() = default;

		void Out(proto::Component* target) const {
			proto::AudioSource* comp = target->mutable_audio_source();
			comp->set_audio_name(this->audio_name);
			comp->set_looping(this->looping);
			comp->set_playing(this->source_state == AUDIOSOURCE_STATE::PLAYING);
			comp->set_volume(this->gain);
		}
		void In(const proto::Component& _source) {
			const proto::AudioSource& comp = _source.audio_source();
			if (comp.has_audio_name()) {
				this->audio_name = comp.audio_name();
				if (!SoundMap::Has(this->audio_name)) {
					std::string ext = this->audio_name.substr(
						this->audio_name.find_last_of('.') + 1);
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
				this->source_state = comp.playing() ? AUDIOSOURCE_STATE::PLAYING : AUDIOSOURCE_STATE::PAUSED;
			}
			if (comp.has_volume()) {
				this->gain = comp.volume();
			}
		}

		ALuint source{ 0 };
		ALuint buffer[2]{ 0,0 };
		bool looping{ false };
		std::shared_ptr<VorbisStream> vorbis_stream;
		AUDIOSOURCE_STATE source_state{ AUDIOSOURCE_STATE::PAUSED };
		int gain{ 100 }; // Volume
		std::string audio_name;
	};

	class SoundSystem : public CommandQueue<SoundSystem>,
		public EventQueue<EntityCreated>,
		public EventQueue<EntityDestroyed> {
	public:
		SoundSystem();

		void Update();

		void SetDelta(const double _delta) {
			this->delta = _delta;
		}

		void Stop() {
			this->running = false;
		}

		using EventQueue<EntityCreated>::On;
		using EventQueue<EntityDestroyed>::On;
		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityDestroyed> data);
	private:
		std::atomic_bool running{ true };
		double delta{ 0 };
		std::shared_ptr<spdlog::logger> _log;
		typedef Multiton<eid, AudioSource*> AudioSourceComponentMap;
		ALCdevice* device{ nullptr };
		ALCcontext* context{ nullptr };

		std::set<AudioSource*> queued_sources;
	};
}

#endif
