#ifndef FLOW_TIMER_HEADER
#define FLOW_TIMER_HEADER

#include <bits/stdc++.h>

using namespace std;

namespace flow {
	/**
	 *  @brief  Class that can be used to time the execution of code.
	 */
	class Timer {
		public:
			chrono::_V2::system_clock::time_point starting_time;

			/**
			 *  @brief  Starts the timer.
			 */
			Timer()
			{
				restart();
			}

			/**
			 *  @brief  Restarts the timer.
			 */
			void restart()
			{
				starting_time = chrono::_V2::system_clock::now();
			}

			/**
			 *  @brief  Returns the number of duration_t elapsed.
			 *  @tparam  duration_t  The unit of time to measure the duration in.
			 *  Defaults to chrono::milliseconds.
			 */
			template <typename duration_t = chrono::milliseconds>
			size_t elapsed()
			{
				auto current_time = chrono::high_resolution_clock::now();

				return chrono::duration_cast<duration_t>(
					current_time - starting_time
				).count();
			}
	};
};

#endif