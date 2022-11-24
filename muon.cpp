//méthodes de la classe muon (muon.hpp)
#include "muon.hpp"
#include <iostream>


muon::muon(){} //constructeur par défaut

muon::muon(double impulsion_,double energy_,double theta_,double phi_){ //constructeur
    impulsion = impulsion_;
    energy = energy_;
    theta = theta_;
    phi = phi_;
}

muon::muon(const muon & newMuon){ //constructeur de recopie
    impulsion = newMuon.getImpulsion();
    energy = newMuon.getEnergy();
    theta = newMuon.getTheta();
    phi = newMuon.getPhi();
}

muon::~muon(){} //destructeur

void muon::print(){ //méthode affichage
    std::cout<<"impulsion:"<<impulsion<<" "<<"energy:"<<energy<<" "<<"theta:"<<theta<<" "<<"phi:"<<phi<<std::endl;
}
