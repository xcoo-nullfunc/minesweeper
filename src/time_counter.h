#pragma once

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

#include <chrono>

using namespace godot;
using namespace std::chrono_literals;

class TimeCounter : public RefCounted{
	GDCLASS(TimeCounter, RefCounted)

	protected:
		static void _bind_methods();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;
		double fix_time = 0.0f;

		enum time_counter_status : std::uint8_t{
			waiting,
			running,
			paused
		};

		time_counter_status statue = waiting;

	public:
		TimeCounter() = default;
		~TimeCounter() override = default;

		void time_fix_add(double time);
		/*[[nodiscard]]*/double get_fix_time();
		void start();
		void pause();
		void reset(double initial_time = 0);

		/*[[nodiscard]]*/double get_time();
		/*[[nodiscard]]*/String get_statue();



};
