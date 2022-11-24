//méthodes de la classe detector (detector.hpp)
#include "detector.hpp"

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

detector::detector(){} //contructeur par défaut

detector::detector(int Nmuon_,double impulsion_std_,double angle_std_){ //constructeur
    Nmuon = Nmuon_;
    impulsion_std = impulsion_std_;
    angle_std = angle_std_;
    tabmuon1 = new muon[Nmuon_];
    tabmuon2 = new muon[Nmuon_];
}

detector::detector(const detector & newDetector){ //constructeur de recopie
    Nmuon = newDetector.getNmuon();
    impulsion_std = newDetector.getImpulsion_std();
    angle_std = newDetector.getAngle_std();
    tabmuon1 = new muon[Nmuon];
    tabmuon2 = new muon[Nmuon];
}

detector::~detector(){ //destructeur
    delete[] tabmuon1;
    delete[] tabmuon2;
}

void detector::setMuon(int i, const muon & newMuon){
    tabmuon1[i] = muon(newMuon);
	tabmuon2[i] = muon(newMuon);

    tabmuon2[i].setTheta(tabmuon1[i].getTheta()+M_PI);
}

void detector::print(){ //méthode affiche
    cout<<"muon 1 : "<<endl;
    for(int i=0; i<Nmuon; i++){
        tabmuon1[i].print();
    }

    cout<<"muon 2 : "<<endl;
    for(int i=0; i<Nmuon; i++){
        tabmuon2[i].print();
    }
}

void detector::tracker(int i,double biais, double impulsion_std, double angle_std, vector<double>&mass_z){
	typedef chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();

	myclock::duration d = myclock::now() - beginning;
	unsigned seed2 = d.count();

	//minstd_rand0 hihi (seed2);
    default_random_engine generator(seed2);

    normal_distribution<double> impulsion_distribution(tabmuon1[i].getImpulsion(),impulsion_std);
    tabmuon1[i].setImpulsion(impulsion_distribution(generator));
    tabmuon2[i].setImpulsion(impulsion_distribution(generator));

    normal_distribution<double> theta_muon1_distribution(tabmuon1[i].getTheta()*biais,angle_std);
    tabmuon1[i].setTheta(theta_muon1_distribution(generator));

    normal_distribution<double> phi_muon1_distribution(tabmuon1[i].getPhi(),angle_std);
    tabmuon1[i].setPhi(phi_muon1_distribution(generator));

    normal_distribution<double> theta_muon2_distribution(tabmuon2[i].getTheta()*biais,angle_std);
    tabmuon2[i].setTheta(theta_muon2_distribution(generator));

    normal_distribution<double> phi_muon2_distribution(tabmuon2[i].getPhi(),angle_std);
    tabmuon2[i].setPhi(phi_muon2_distribution(generator));


    double energy_part = pow((tabmuon1[i].getEnergy()+tabmuon2[i].getEnergy()),2);

    double x = (tabmuon1[i].getImpulsion()*sin(tabmuon1[i].getTheta())*cos(tabmuon1[i].getPhi())) * (tabmuon2[i].getImpulsion()*sin(tabmuon2[i].getTheta())*cos(tabmuon2[i].getPhi()));
    double y = (tabmuon1[i].getImpulsion()*sin(tabmuon1[i].getTheta())*sin(tabmuon1[i].getPhi())) * (tabmuon2[i].getImpulsion()*sin(tabmuon2[i].getTheta())*sin(tabmuon2[i].getPhi()));
    double z = (tabmuon1[i].getImpulsion()*cos(tabmuon1[i].getTheta())) * (tabmuon2[i].getImpulsion()*cos(tabmuon2[i].getTheta()));

    bosonZ_mass = sqrt(energy_part - (2*(x+y+z)+pow(45.5934,2)+pow(45.5934,2)));

	mass_z.push_back(bosonZ_mass);
}

void detector::mass(vector<double>&mass_z,double*mass_mean, double*mass_std){
	*mass_mean = (accumulate(mass_z.begin(),mass_z.end(),0.))/Nmuon;

    for(int i=0;i<Nmuon;i++){
        *mass_std += pow((mass_z[i]-*mass_mean),2);
    }
    *mass_std = sqrt(*mass_std/Nmuon);

	double statistical_incert = *mass_std/sqrt(Nmuon-1);

    cout<<"moyenne masse :"<<*mass_mean<<endl;
    cout<<"écart-type :"<<*mass_std<<endl;
	cout<<"incertitute statistique :"<<statistical_incert<<endl;

}
