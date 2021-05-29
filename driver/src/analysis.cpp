#include "main.h"


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


// int **init_sample(int len)
// {
// 	int **sample = 0;
// 	if (!(sample = (int **)malloc(len * sizeof(int *))))
// 		Serial.println("Malloc in sample failed");
// 	int i = 0;
// 	while (i < len)
// 	{
// 		if (!(sample[i] = (int *)malloc(2 * sizeof(int))))
// 			Serial.println("Malloc in sample failed");
// 		sample[i][0] = 0;
// 		sample[i][1] = 0;
// 	}
// }
