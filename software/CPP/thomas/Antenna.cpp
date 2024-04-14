#include "Antenna.h"
#include "Vectorials.h"


double Antenna::find_GTX3(Point RX) {
    double delta = -0.1;
    double phi_3dB = Pi / 6;
    Vectorials v_TX_RX = { RX.x - position.x , RX.y - position.y };
    v_TX_RX = { v_TX_RX.v1 / v_TX_RX.vector_norm(), v_TX_RX.v2 / v_TX_RX.vector_norm() };
    double phi = acos(v_TX_RX.v1);
    double GTXdB = 21.5836 - 12 * pow((phi - delta) / phi_3dB, 2);
    double GTX3 = pow(10, GTXdB / 10);
    return GTX3;
}

double Antenna::find_GTX1_2(Point RX) {
    return 1.7;
}