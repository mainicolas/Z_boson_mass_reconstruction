#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "muon.hpp"
#include <vector>

class detector{
    //-----------------déclaration des membres privés
    private:
        int Nmuon;
        double impulsion_std,angle_std;
        double bosonZ_mass;
        muon *tabmuon1;
        muon *tabmuon2;


    //-----------------déclaration des membres publiques
    public:
        detector();
        detector(int,double,double);
        detector(const detector &);
        ~detector();
        void print();

        //------------accesseurs
        int getNmuon() const {return Nmuon;}
        double getImpulsion_std() const {return impulsion_std;}
        double getAngle_std() const {return angle_std;}
        muon *getTabmuon1() const {return tabmuon1;}
        muon *getTabmuon2() const {return tabmuon2;}


        //------------mutateurs
        int setNmuon(int Nmuon_) {Nmuon=Nmuon_;}
        double setImpulsion_std(double impulsion_std_) {impulsion_std=impulsion_std_;}
        double setAngle_std(double angle_std_) {angle_std=angle_std_;}


        void setMuon(int i, const muon &);
        void tracker(int,double,double,double,std::vector<double>&);
        void mass(std::vector<double>&,double*,double*);

};

#endif
