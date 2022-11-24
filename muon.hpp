#ifndef MUON_HPP
#define MUON_HPP


class muon{
    //-----------------déclaration des membres privés
    private :
        double impulsion,energy;
        double theta,phi;

    //-----------------déclaration des membres publiques
    public :
        muon(); //constructeur par défaut
        muon(double,double,double,double); //constructeur
        muon(const muon &); //constructeur de recopie
        ~muon(); //destructeur
        void print();

        //------------accesseurs
        double getImpulsion() const {return impulsion;}
        double getEnergy() const {return energy;}
        double getTheta() const {return theta;}
        double getPhi() const {return phi;}

        //------------mutateurs
        void setImpulsion(double impulsion_) {impulsion=impulsion_;}
        void setEnergy(double energy_) {energy=energy_;}
        void setTheta(double theta_) {theta=theta_;}
        void setPhi(double phi_) {phi=phi_;}

};

#endif
