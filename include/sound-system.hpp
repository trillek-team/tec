#pragma once

#include <memory>
#include <set>
#include <iostream>
#include <al.h>
#include <alc.h>

#include "multiton.hpp"
#include "entity.hpp"
#include "types.hpp"
#include "command-queue.hpp"

namespace tec {
	class VorbisStream;

	enum AUDIOSOURCE_STATE { PLAYING, PAUSED, STOPPED };

	struct AudioSource {
		AudioSource(std::shared_ptr<VorbisStream> stream, bool auto_play) :
			vorbis_stream(stream), source_state(auto_play ? PLAYING : PAUSED) { }

		static ReflectionComponent Reflection(AudioSource* val) {
			ReflectionComponent refcomp;
			refcomp.properties["looping"] = std::to_string(val->looping);
			return std::move(refcomp);
		}

		ALuint source;
		ALuint buffer[2];

		bool looping;

		AUDIOSOURCE_STATE source_state;

		std::shared_ptr<VorbisStream> vorbis_stream;
	};

	class SoundSystem : public CommandQueue < SoundSystem > {
	public:
		SoundSystem();

		void Update(const double delta);
	private:
		typedef Multiton<eid, std::shared_ptr<AudioSource>> AudioSourceComponentMap;
		ALCdevice *device;
		ALCcontext *context;

		std::set<std::shared_ptr<AudioSource>> queued_sources;
	};
}
