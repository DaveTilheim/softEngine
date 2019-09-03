#ifndef __TIMELINE_HPP__
#define __TIMELINE_HPP__
#include "Entity.hpp"
#include <ctime>
#include <chrono>

using namespace std::chrono;

namespace se
{
	class Timeline
	{
	private:
		steady_clock::time_point beg;
		steady_clock::time_point current;
		void updateTime();
		float getPassedTime();
		bool terminated;
	public:
		float wait;
		std::function<void(Entity *)> lambda;
		std::function<void()> endfunc;
		Entity *target;
		bool end;
		bool endCall = false;
		Timeline(float, std::function<void(Entity *)> lambda, Entity *, bool=false);
		Timeline(float, std::function<void(Entity *)> lambda, bool=false);
		~Timeline();
		void update();
		bool isTerminated();
		void reset();
		void setEndfunc(std::function<void()> endfunc);
	};

	
}


#endif
