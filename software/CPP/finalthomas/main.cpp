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
    //Receiver myRX{RX_exercise, TX_exercise};
    //double P = myRX.power(); 
    ///std::cout << P << std::endl; 
    ///dessinerInterface(myRX.TX.position, myRX.position, myRX.E_receiver.reflection_walls, myRX.E_receiver.double_reflection_walls); 
    


    create_map(TX3_emettor); 
    calculate_powers(); 

    std::vector<double> project_powers1 = project_powers;
    project_powers.clear(); 
    project_receivers.clear(); 
    create_map(TX1_emettor1); 
    calculate_powers(); 

    std::vector<double> project_powers2 = project_powers;
    project_powers.clear();
    project_receivers.clear();
    create_map(TX1_emettor2);
    calculate_powers();

    for (int i = 0; i < project_powers1.size(); i++) {
        project_powers[i] = std::max(project_powers[i], project_powers1[i]); 
        project_powers[i] = std::max(project_powers[i], project_powers2[i]); 
    }
    //for (Receiver& my_receiver : project_receivers) {
      //  dessinerInterface(my_receiver.TX.position, my_receiver.position, my_receiver.E_receiver.reflection_walls, my_receiver.E_receiver.double_reflection_walls); 
    //}
    dessinerPuissances(project_powers); 

    return 0; 

}