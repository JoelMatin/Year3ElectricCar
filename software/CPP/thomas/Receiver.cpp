#include "Receiver.h"


double Receiver::power() {
    std::complex<double> E_total = E_receiver.E_transmission() + E_receiver.E_reflection() +
		E_receiver.E_double_reflection(); 
	double power_receiver = 1 / (8 * TX.R) * std::norm(E_total) * pow(he, 2);
	return power_receiver;
}

