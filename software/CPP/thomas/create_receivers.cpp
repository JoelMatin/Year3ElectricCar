#include "create_receivers.h"

std::vector<Receiver> project_receivers{}; 
std::vector<double> project_powers; 

void create_map(Antenna TX) {
	for (int m = 0; m < nb_receivers_x; m++) {
		for (int n = 0; n < nb_receivers_y; n++) {
			const Receiver my_receiver{ Point{ m * receiver_area + receiver_area / 2 , n * receiver_area + receiver_area / 2 }, TX };
			project_receivers.push_back(my_receiver); 
		}
	}
}


void calculate_powers() {
	for (int m = 0; m < project_receivers.size(); m++) {
		double local_power = project_receivers[m].power(); 
		project_powers.push_back(local_power); 
	}
}
