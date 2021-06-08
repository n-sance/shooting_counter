#include "main.h"

std::deque<int> deque_acc(DEQUE_SIZE, 0);
std::deque<int> deque_piez(DEQUE_SIZE, 0);
ShootDetector shoot_detector(ACCEL_THRESHOLD, PIEZO_THRESHOLD, DEADZONE_SAMPLES);

static void update_queues(const sensors_data_sample_t &measurement) {
    deque_acc.push_back(measurement.acc);
    deque_piez.push_back(measurement.piezo);
    deque_acc.pop_front();
    deque_piez.pop_front();
}

	ShootDetector::ShootDetector(int acc_thr, int piezo_thr, int timeout)
	{
		acc_threshold = acc_thr;
		piezo_threshold = piezo_thr;
		potential_shoot = false;
		acc_thr_exceeded = false;
		piezo_thr_exceeded = false;
		shoots = 0;
		timeout = 3;
		timeout_cycles_left = 0;
	}

	void ShootDetector::potential_shoot_check(std::deque<int>& a, std::deque<int>& p)
	{
		if (!timeout_cycles_left)
		{
			acc_thr_exceeded |= a.back() > acc_threshold ? true : false;
			piezo_thr_exceeded |= p.back() > piezo_threshold ? true : false;
			if (acc_thr_exceeded || piezo_thr_exceeded)
				potential_shoot = true;
			if (acc_thr_exceeded && piezo_thr_exceeded)
			{
				shoots++;
				acc_thr_exceeded = false;
				piezo_thr_exceeded = false;
				potential_shoot = false;
				timeout_cycles_left = timeout;
			}
		}
		else
			timeout_cycles_left--;
	}


int calculate_shoots()
{
	sensors_data_sample_t measurement = get_data_from_sensors();
	update_queues(measurement);

    shoot_detector.potential_shoot_check(deque_acc, deque_piez);

	//Log
    Serial.print("Shoots: "); Serial.print(shoot_detector.shoots);
	return shoot_detector.shoots;
}
