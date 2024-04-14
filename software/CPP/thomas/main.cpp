#include <iostream>
#include "interface_graphique.h"

int main() {
     

    /*Field E = {{TX_exercise.position, RX_exercise}, TX_exercise};
    std::vector<Point> sym_points{}; 
    std::vector<Point> refl_points{}; 
    std::vector<Wall> mesReflex = E.get_reflection_walls(refl_points, sym_points);
    std::complex<double> E_Reflex = E.E_reflection(); 
    std::cout << E_Reflex << std::endl;*/

     ///code de vérif de 8.1
    /*Receiver myRX{RX_exercise, TX_exercise};
    double P = myRX.power(); 
    std::cout << P << std::endl; 
    dessinerInterface(myRX.TX.position, myRX.position, myRX.E_receiver.reflection_walls, myRX.E_receiver.double_reflection_walls); 
    */


    create_map(TX3_emettor); 
    calculate_powers(); 
    //for (Receiver& my_receiver : project_receivers) {
      //  dessinerInterface(my_receiver.TX.position, my_receiver.position, my_receiver.E_receiver.reflection_walls, my_receiver.E_receiver.double_reflection_walls); 
    //}
    for (const double P : project_powers) {
        std::cout << P << std::endl; 
    }
    dessinerPuissances(TX3_emettor.position, project_powers); 

    return 0; 

}