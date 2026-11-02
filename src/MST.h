#pragma once

#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/tile_map_layer.hpp"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/variant/signal.hpp"
#include "godot_cpp/variant/string.hpp"

#include "time_counter.h"

#include <map>
#include <cstdint>

using namespace godot;
using namespace std::chrono_literals;

class MST : public TileMapLayer {
	GDCLASS(MST, TileMapLayer)

	protected:
		static void _bind_methods();

	private:
		enum cell_status : std::uint8_t {
			btm = 0,    //beyond the map
			close = 1,
			open = 2,
			mark = 3
		};

		enum game_status : std::uint8_t {
			waiting,
			running,
			win,
			over
		};

		std::map<Vector2i, int> mines;
		std::map<Vector2i, cell_status> cell_statue;
		std::map<String, String> options;

		Vector2i target{-1, -1};
		game_status game_statue = waiting;
		int64_t closed_cell = 0, mark_count = 0;
		int mapw = 0, maph = 0;
		int64_t mine_count = 0;

		void open_cell(Vector2i pos);
		void mark_cell(Vector2i pos, bool statue = true);
		void game_over(bool win);
		inline bool in_map(Vector2i coords);

		bool quick_click = false;
		bool quick_mark = false;    //vary lazy

		//false : open_mode | true : mark_mode
		bool touch_mode = false, hold_mode=true;
		bool moved = false;
		Ref<TimeCounter> TC;
		double hold_time = 0.25, hold_timeout = 3;    //区分：(0, hold_time] -> touch; 9=(hold_time, hold_timeout] -> hold;
	public:
		MST() = default;
		~MST() override = default;

		void _ready() override;

		//args : wigth, height, number os landmines, avoid generate map on the site
		int initMS(int w, int h, int64_t n, Vector2i avoid = {-1, -1});  //generate landmines
		void reset();  //not generate landmines
		/*[[nodiscard]]*/inline int64_t get_unmarked_mine();

		void set_quick_click(const bool option);
		/*[[nodiscard]]*/ bool get_quick_click();
		void set_quick_mark(const bool option);
		/*[[nodiscard]]*/ bool get_quick_mark();
		void set_touch_mode(const bool option);
		/*[[nodiscard]]*/ bool get_touch_mode();
		void set_hold_mode(const bool option);
		/*[[nodiscard]]*/ bool get_hold_mode();

		void _unhandled_input(const Ref<InputEvent> &p_event) override;
		void _draw() override;
		void _exit_tree() override;
};
