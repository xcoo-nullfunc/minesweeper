#include "time_counter.h"

void TimeCounter::_bind_methods(){
	//godot::ClassDB::bind_method(D_METHOD("time_fix_add", "time"), &TimeCounter::time_fix_add);
	godot::ClassDB::bind_method(D_METHOD("start"), &TimeCounter::start);
	godot::ClassDB::bind_method(D_METHOD("reset", "initial_time"), &TimeCounter::reset, 0);
	godot::ClassDB::bind_method(D_METHOD("get_time"), &TimeCounter::get_time);
	godot::ClassDB::bind_method(D_METHOD("get_fix_time"), &TimeCounter::get_fix_time);
	godot::ClassDB::bind_method(D_METHOD("pause"), &TimeCounter::pause);
	godot::ClassDB::bind_method(D_METHOD("get_statue"), &TimeCounter::get_statue);
}

void TimeCounter::time_fix_add(double time){
	this -> fix_time += time;
}

void TimeCounter::start(){
	if(this -> statue == waiting){
		this -> start_time = std::chrono::high_resolution_clock::now();
		this -> statue = running;
	}
	else if(this -> statue == paused) {
		this -> fix_time += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - this -> end_time).count();
	}

}

void TimeCounter::pause(){
	this -> statue = paused;
	this -> end_time = std::chrono::high_resolution_clock::now();
}

double TimeCounter::get_fix_time(){
	return this -> fix_time;
}

void TimeCounter::reset(double initial_time){
	this -> statue = waiting;
	this -> fix_time = initial_time;
}

double TimeCounter::get_time(){
	if(this -> statue == waiting)return this -> fix_time;
	else if(this -> statue == running)return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - this -> start_time).count() + this -> fix_time;
	else if(this -> statue == paused)return std::chrono::duration<double>(this -> end_time - this -> start_time).count() + this -> fix_time;
	return -1;   //unknow statue
}

String TimeCounter::get_statue(){
	if(this -> statue == waiting) return "waiting";
	else if(this -> statue == running) return "running";
	else if(this -> statue == paused) return "paused";
	else return "unknow statue";

}
