#include "MST.h"

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

#include "godot_cpp/classes/input_event_mouse_button.hpp"
#include "godot_cpp/classes/input_event_screen_touch.hpp"
#include "godot_cpp/classes/input_event_screen_drag.hpp"
#include "godot_cpp/classes/input_event_key.hpp"
#include "godot_cpp/classes/global_constants.hpp"

#include "godot_cpp/core/object.hpp"

void MST::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("initMS", "w", "h", "n", "avoid"), &MST::initMS, Vector2i{-1, -1});
	godot::ClassDB::bind_method(D_METHOD("reset"), &MST::reset);
	godot::ClassDB::bind_method(D_METHOD("get_unmarked_mine"), &MST::get_unmarked_mine);

	godot::ClassDB::bind_method(D_METHOD("set_quick_click", "option"), &MST::set_quick_click);
	godot::ClassDB::bind_method(D_METHOD("get_quick_click"), &MST::get_quick_click);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "quick_click"), "set_quick_click", "get_quick_click");

	godot::ClassDB::bind_method(D_METHOD("set_quick_mark", "option"), &MST::set_quick_mark);
	godot::ClassDB::bind_method(D_METHOD("get_quick_mark"), &MST::get_quick_mark);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "quick_mark"), "set_quick_mark", "get_quick_mark");

	godot::ClassDB::bind_method(D_METHOD("set_touch_mode", "option"), &MST::set_touch_mode);
	godot::ClassDB::bind_method(D_METHOD("get_touch_mode"), &MST::get_touch_mode);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "touch_mode"), "set_touch_mode", "get_touch_mode");

	godot::ClassDB::bind_method(D_METHOD("set_hold_mode", "option"), &MST::set_hold_mode);
	godot::ClassDB::bind_method(D_METHOD("get_hold_mode"), &MST::get_hold_mode);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hold_mode", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_hold_mode", "get_hold_mode");


	ADD_SIGNAL(MethodInfo("game_run"));
	ADD_SIGNAL(MethodInfo("game_stop"));
	ADD_SIGNAL(MethodInfo("game_reset"));
	ADD_SIGNAL(MethodInfo("game_win"));
	ADD_SIGNAL(MethodInfo("game_over"));
}

void MST::_ready(){
	TC = memnew(TimeCounter);
	this -> initMS(50, 50, 500);
}

//初始化，许多数据需要手动维护
int MST::initMS(int w, int h, int64_t n, Vector2i avoid){

	//先验证参数是否可用
	if(n >= static_cast<int64_t>(w)*h) return 1;    //too much Landmines
	else if(n < 0 or w <= 0 or h <= 0) return 2;    //must not be less than zero
	else if(w > 32767 or h > 32767) return 3;    //too large

	//通过后清空原有内容
	this -> mines.clear();
	this -> cell_statue.clear();
	this -> mapw = w;
	this -> maph = h;
	this -> mine_count = n;
	this -> mark_count = 0;
	this -> clear();
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			this -> set_cell({x, y}, 0, {11, 0});
			cell_statue[{x, y}] = close;
			//this -> mines[{x, y}] = 0;
		}
	}
	this -> closed_cell = static_cast<int64_t>(w) * h;

	//布雷
	std::vector<int>ids(static_cast<int64_t>(w) * h);
	std::iota(ids.begin(), ids.end(), 0);
	if(in_map(avoid)){
		std::swap(ids[avoid.x + avoid.y*w], ids.back());
		ids.pop_back();
	}
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::ranges::shuffle(ids, gen);
	for(int i=0; i<n; i++){
		int id = ids[i];
		int cx = id%w;
		int cy = id/w;
		this -> mines[{cx, cy}] = 9;
		for(int x=-1; x<=1; x++){
			for(int y=-1; y<=1; y++){
				if((x != 0 or y != 0) and this -> in_map({cx+x, cy+y}) and this -> mines[{cx + x, cy + y}] < 9){
					this -> mines[{cx + x, cy + y}]++;
				}
			}
		}
	}
	this -> game_statue = waiting;
	queue_redraw();
	emit_signal("game_reset");
	return 0;
}

//只重置状态，不重新布雷
void MST::reset(){
	cell_statue.clear();
	this -> clear();
	for(int y=0; y<this -> maph; y++){
		for(int x=0; x<this -> mapw; x++){
			this -> set_cell({x, y}, 0, {11, 0});
			cell_statue[{x, y}] = close;
			//this -> mines[{x, y}] = 0;
		}
	}
	this -> closed_cell = static_cast<int64_t>(this -> mapw) * this -> maph;
	this -> mark_count = 0;
	this -> game_statue = waiting;
	queue_redraw();
	emit_signal("game_reset");
}

//获取剩余雷的数量，具体是总数量-标记量
inline int64_t MST::get_unmarked_mine(){
	return this -> mine_count - this -> mark_count;
}

inline bool MST::in_map(Vector2i coords){
	return (coords.x >= 0 and coords.y >= 0 and coords.x < this -> mapw and coords.y < this -> maph);
}
//打开格子
void MST::open_cell(Vector2i pos){
	//pos出界、游戏结束、被打开和标记的情况
	if(this -> cell_statue[pos] == open or this -> cell_statue[pos] == mark or not in_map(pos) or game_statue == over or game_statue == win){
		return ;
	}

	//点开第一个格子
	if(this -> game_statue == waiting){
		//第一个点到雷重新生成地图并重新打开
		if(this -> mines[pos] == 9){
			initMS(this -> mapw, this -> maph, this -> mine_count, pos);
			open_cell(pos);
			return ;
		}
		this -> game_statue = running;
		//this -> game_run.emit();
		emit_signal("game_run");
	}

	//点到雷
	if(this -> mines[pos] == 9){
		this -> cell_statue[pos] = open;
		this -> set_cell(pos, 0, {10, 0});
		game_over(false);
	}

	//没点到雷
	else if(this -> mines[pos] >= 0 and this -> mines[pos] < 9){
		//是否已经打开、没开的数量，都需要手动维护
		this -> cell_statue[pos] = open;
		this -> closed_cell -= 1;
		this -> set_cell(pos, 0, {mines[pos], 0});
	}

	//空格子自动打开周围的（递推方式）
	if(this -> mines[pos] == 0){
		for(int y = -1; y <= 1; y++){
			for(int x = -1; x <= 1; x ++){
				Vector2i tgt = pos + Vector2i(x, y);
				if((x != 0 or y != 0) and cell_statue[tgt] == close){
					open_cell(tgt);
				}
			}
		}
	}

}

//统一方法标记地雷
void MST::mark_cell(Vector2i pos, bool statue){
	if(in_map(pos)){
		if(cell_statue[pos] == close and statue){
			cell_statue[pos] = mark;
			set_cell(pos, 0, {13, 0});
			this -> mark_count++;
		}
		else if(cell_statue[pos] == mark and not statue){
			cell_statue[pos] = close;
			set_cell(pos, 0, {11, 0});
			this -> mark_count--;
		}
	}
}

//游戏结束，输赢由外部判断（先判断了才会调用这条函数）
void MST::game_over(bool win){
	//this -> game_stop.emit();    //内部外部计时都先写上，万一能用上呢
	emit_signal("game_stop");
	for(int y=0; y<this -> maph; y++){
		for(int x=0; x<this -> mapw; x++){
			if(this -> cell_statue[{x, y}] == close) this -> set_cell({x, y}, 0, {this -> mines[{x, y}], 0});
			if(this -> cell_statue[{x, y}] == mark and this -> mines[{x, y}] < 9) this -> set_cell({x, y}, 0, {14, 0});
		}
	}
	if(win){
		this -> game_statue = this -> win;
		emit_signal("game_win");
	}
	else{
		this -> game_statue = over;
		emit_signal("game_over");
	}
}

void MST::_unhandled_input(const Ref<InputEvent> &p_event){
	const InputEventMouseButton *InputMouseButton = Object::cast_to<InputEventMouseButton>(p_event.ptr());
	const InputEventKey *InputKey = Object::cast_to<InputEventKey>(p_event.ptr());
	const InputEventScreenTouch *InputScreenTouch = Object::cast_to<InputEventScreenTouch>(p_event.ptr());
	const InputEventScreenDrag *InputScreenDrag = Object::cast_to<InputEventScreenDrag>(p_event.ptr());

	//只接受非模拟的鼠标操作和第一个触屏操作
	if((InputMouseButton != nullptr and InputMouseButton -> get_device() >= 0) or (InputScreenTouch != nullptr and InputScreenTouch -> get_index() == 0)){
		if(this -> game_statue == over or this -> game_statue == win) return ;
		//Vector2i coords = this -> get_cell_coords(InputMouseButton -> get_global_position());
		Vector2i coords{-1, -1};

		coords = this -> local_to_map(get_local_mouse_position());    //先试试共用此转换
		cell_status &current_statue = cell_statue[coords];

		double touch_time = -1;    //触摸操作松开后再触发，防止误触(防止意图滑动屏幕时操作)，所以触屏操作中quick_click不生效
								   //后续靠这个区分操作类型，==-1鼠标操作，==0触摸按下，>0触摸松开
		if(InputScreenTouch != nullptr and InputScreenTouch -> get_index() == 0){    //计index0按住的时间
			if(InputScreenTouch -> is_pressed()){
				touch_time = 0;
				this -> TC -> reset();
				this -> TC -> start();
				this -> moved = false;
			}
			else if(InputScreenTouch -> is_released()){
				this -> TC -> pause();
				touch_time = this -> TC -> get_time();
			}
		}


		//标记格子
		if((InputMouseButton != nullptr and InputMouseButton->is_pressed() and InputMouseButton -> get_button_index() == godot::MOUSE_BUTTON_RIGHT)
				or (touch_time > 0 and touch_time <= this -> hold_time and this -> touch_mode and not moved)
				or (touch_time > this -> hold_time and touch_time <= this -> hold_timeout and this -> hold_mode and not moved)){
			if(this -> game_statue == waiting){
				this -> game_statue = running;
				emit_signal("game_run");
			}
			if(current_statue == close){
				mark_cell(coords, true);
			}
			else if(current_statue == mark){
				mark_cell(coords, false);
			}
		}

		//打开格子
		else if((InputMouseButton != nullptr and InputMouseButton -> get_button_index() == godot::MOUSE_BUTTON_LEFT and InputMouseButton -> is_pressed()) or touch_time == 0){
			this -> target = coords;
			if(cell_statue[target] == close and in_map(target)){
				//只在鼠标操作生效
				if(this -> quick_click and InputMouseButton != nullptr){
					open_cell(target);
					target = {-1, -1};
				}
				else this -> set_cell(target, 0, {12, 0});
			}
		}
		else if((InputMouseButton != nullptr and InputMouseButton -> get_button_index() == godot::MOUSE_BUTTON_LEFT and InputMouseButton -> is_released() and not quick_click) or touch_time > 0){
			//松开左键再开格子
			if(cell_statue[target] == close and in_map(target)) this -> set_cell(target, 0, {11, 0});
			if((InputMouseButton != nullptr and coords == this -> target and cell_statue[target] == close)
					or (((touch_time > 0 and touch_time <= this -> hold_time and not this -> touch_mode and not moved)
					or (touch_time > this -> hold_time and touch_time <= this -> hold_timeout and not this -> hold_mode and not moved)) and cell_statue[target] == close)){
				open_cell(coords);
				target = {-1, -1};
			}
		}

		//这个更多的是对周围格子的作用
		if(cell_statue[target] == open and in_map(target) and ((InputMouseButton != nullptr and InputMouseButton -> is_pressed()) or touch_time == 0)){
			//尝试开周围格子
			int mine = 0, marks = 0, closed = 0;
			//先做统计
			for(int x = -1; x <= 1; x++){
				for(int y = -1; y <= 1; y++){
					if(not in_map(target + Vector2i(x, y))) continue;
					if(mines[target + Vector2i(x, y)] == 9) mine++;
					if(cell_statue[target + Vector2i(x, y)] == mark) marks++;
					if(cell_statue[target + Vector2i(x, y)] == close) closed++;
				}
			}
			//雷数==标记数 -> 开格子，否则只做提示
			if(mine == marks){
				for(int x = -1; x <= 1; x++){
					for(int y = -1; y <= 1; y++){
						if(not in_map(target + Vector2i(x, y))) continue;
						if(cell_statue[target + Vector2i(x, y)] == close){
							open_cell(target + Vector2i(x, y));
						}
					}
				}
			}
			//数字=未开格子数 -> 标记格子
			else if(closed+marks == mines[target] and this -> quick_mark){
				for(int x = -1; x <= 1; x++){
					for(int y = -1; y <= 1; y++){
						if(not in_map(target + Vector2i(x, y))) continue;
						if(cell_statue[target + Vector2i(x, y)] == close){
							mark_cell(target + Vector2i(x, y));
						}
					}
				}
			}
			//提示周围的未开格子
			else {
				for(int x = -1; x <= 1; x++){
					for(int y = -1; y <= 1; y++){
						if(cell_statue[target + Vector2i(x, y)] == close){
							if(not in_map(target + Vector2i(x, y))) continue;
							this -> set_cell(target + Vector2i(x, y), 0, {12, 0});
						}
					}
				}
			}
		}
		else if(cell_statue[target] == open and ((InputMouseButton != nullptr and InputMouseButton -> is_released()) or touch_time > 0)){
			for(int x = -1; x <= 1; x++){
				for(int y = -1; y <= 1; y++){
					if(not in_map(target + Vector2i(x, y))) continue;
					if(cell_statue[target + Vector2i(x, y)] == close){
						this -> set_cell(target+Vector2i(x, y), 0, {11, 0});
					}
				}
			}
			target = {-1, -1};

		}

		if(this -> closed_cell == this -> mine_count){
			game_over(true);
		}
		else if(this -> closed_cell < this -> mine_count){
			print_line("ERROR mine_count less than closed_cell");
		}

	}
	//处理键盘输入
	else if(InputKey != nullptr){
		/*
		 * 结束时按R重新开始，SHIFT+R重新布雷（重新初始化）
		 * 运行时需要额外按住CTRL
		 */
		if(InputKey -> is_pressed()){
			if(InputKey -> get_keycode() == godot::KEY_R){
				if(this -> game_statue == over or this -> game_statue == win or Input::get_singleton() -> is_key_pressed(godot::KEY_CTRL)){
					if(Input::get_singleton() -> is_key_pressed(godot::KEY_SHIFT)){
						this -> initMS(this -> mapw, this -> maph, this -> mine_count);
					}
					else {
						this -> reset();
					}
				}
			}
		}
	}

	else if(InputScreenDrag != nullptr){
		this -> moved = true;    //滑动只移动相机，不进行操作
	}

}

void MST::_draw(){
	draw_rect(Rect2(-5, -5, static_cast<real_t>(this -> mapw * 40 + 10), static_cast<float>(this -> maph * 40 + 10)), Color(0, 0, 0, 255), false, 10);
}

void MST::_exit_tree(){
	this -> clear();
}

void MST::set_quick_click(const bool option){
	this -> quick_click = option;
}
bool MST::get_quick_click(){
	return this -> quick_click;
}

void MST::set_quick_mark(const bool option){
	this -> quick_mark = option;
}
bool MST::get_quick_mark(){
	return this -> quick_mark;
}

void MST::set_touch_mode(const bool option){
	this -> touch_mode = option;
}
bool MST::get_touch_mode(){
	return this -> touch_mode;
}

void MST::set_hold_mode(const bool option){
	this -> hold_mode = option;
}
bool MST::get_hold_mode(){
	return this -> hold_mode;
}


