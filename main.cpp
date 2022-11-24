#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "detector.hpp"
#include "muon.hpp"
using namespace std;


int main(){
    //-------------------------------------- initialisation des variables

	/*##########################################
	->Les varibles sont ici initialisées à partir du fichier data.txt
	->Un vector est également définit ici pour récupérer les masses des bosons générés
	par la classe detector
	->Calcul de l'impulsion
	->Calcul de l'énergie avec la formule E² = m²c^4 + p²c², avec m=105,7.10^-3 GeV/c².
	##########################################*/

	ifstream ifile("data.txt");

	int N; //Nombre de muons
    double impulsion = sqrt((pow(91.187,2)-4*pow(105.66e-3,2))/4); //GeV/c
	double energy = sqrt(pow(105.7e-3,2)+pow(impulsion,2)); //GeV
	double imp_std,ang_std; //écart type pour l'impulsion et les angles
	double theta,phi; //radians, θ∈[0;π] φ∈[0;2π]
	double massZ_mean,massZ_std = 0; //masse moyenne et écart type du boson Z
	double acceptance; //acceptance du détecteur
	double biais;//biais selon l'angle θ
	vector<double>mass_z;

    ifile>>N>>acceptance>>biais>>imp_std>>ang_std;




    //-------------------------------------- génération des muons

	/*##########################################
	->Génération d'angle aléatoire:
		*pour phi : distribution uniforme entre 0 et 2π
		*pour theta : utilisation de la méthode des rejets selon la distribution de l'énoncé.
	->Création des tableaux de muons et antimuons avec ces angles
	##########################################*/

    srand(time(NULL));
    double random,thetax,thetay,distribution_theta;
	double range_y = 1.089; //maximum de la distribution de l'angle theta

    vector<double> tabtheta;
    vector<double> tabphi;
    int i = 0;
    int j=0;
    double q=0;
    while(i<N){
    	random = ((double)rand())/(double)RAND_MAX;
		thetax = random*M_PI;
		thetay = random*range_y;
    	distribution_theta = sin(thetax) + pow(cos(thetax),2)*sin(thetax);

        phi = random*(2*M_PI);
        if(thetax>((acceptance*M_PI)/180) && thetax<(((180-acceptance)*M_PI)/180) &&thetay<=distribution_theta){
            tabtheta.push_back(thetax);
            tabphi.push_back(phi);
            j++;
        }
      	if(thetay<=distribution_theta){
            q++;
            i++;
          }

    }
	cout<<"désintegrations totales:"<<q<<endl;
	cout<<"désintegrations détectées:"<<j<<endl<<endl;
    detector detec(j,imp_std,ang_std);

    for(int i=0;i<j;i++){
        muon tempmuon(impulsion,energy,tabtheta[i],tabphi[i]);
        detec.setMuon(i,tempmuon);
    }




    //-------------------------------------- boson Z

	/*########################################
	->Récupère les masses des bosons dans un vector à partir de la classe detector
	->Calcul de la moyenne et de l'écart type de la distribution de la masse
	->Incertitudes statistiques
	->Envoi d'intervalles de masses du boson Z dans un fichier texte
	########################################*/

    for(int i=0;i<j;i++){
        detec.tracker(i,biais,imp_std,ang_std,mass_z);
    }

	double*pointer_mass_mean =&massZ_mean;
	double*pointer_mass_std=&massZ_std;
	detec.mass(mass_z,pointer_mass_mean,pointer_mass_std);

	vector<double> events_number;
    vector<double> box;
    sort(mass_z.begin(),mass_z.end());
    for(float i=91;i<91.3;i+=0.005){
        int somme=0;
        for(int j=0;j<mass_z.size();j++){
            if(mass_z[j]<i && mass_z[j]>(i-0.005)){
                somme++;
            }
        }
        events_number.push_back(somme);
        box.push_back(i);
    }

	double max=0;
    for(int i=0;i<events_number.size();i++){
    	if(max<events_number[i]){
    		max = events_number[i];
    	}

    }
    for(int i=0; i<events_number.size();i++){
    	events_number[i]/=max;
    }

    ofstream _mass_("reconstruction.txt");
    for(int i=0;i<events_number.size();i++){
        _mass_<<box[i]<<" "<<events_number[i]<<endl;
    }



	//-------------------------------------- création d'un fichier gnuplot

	/*########################################
	->Trace les points issuent du fichier reconstruction.txt
	->Trace la gaussienne centrée sur la moyenne de la masse du boson Z avec l'écart-type associée
	->Trace la distribution de Breit-Wigner relativiste de largeur 2.5GeV
	########################################*/

	ofstream draw("draw.gnu");
	draw<<"set title \"Distribution de la masse invariante du boson Z ("<<detec.getNmuon()<<" désintégrations)\""<<endl;
	draw<<"set xlabel \"Masse du boson Z (GeV/c²)\""<<endl;
    draw<<"set grid"<<endl;
	draw<<"set xrange ["<<massZ_mean-30*massZ_std<<":"<<massZ_mean+30*massZ_std<<"]"<<endl;
    draw<<"set yrange [-0.2:1.2]"<<endl;

	draw<<"gaussian(x) = (exp(-(x-"<<massZ_mean<<")*(x-"<<massZ_mean<<")/(2*"<<massZ_std<<"*"<<massZ_std<<"))/("<<massZ_std<<"*sqrt(2*3.141593)))/(1/("<<massZ_std<<"*sqrt(2*3.141593)))"<<endl;
    draw<<"relativistic_breit_wigner(x)=(((14.0/22.0)*((2.5)*(2.5)*"<<massZ_mean<<"*"<<massZ_mean<<"))/(((x*x)-("<<massZ_mean<<"*"<<massZ_mean<<"))*((x*x)-("<<massZ_mean<<"*"<<massZ_mean<<"))+x*x*x*x*((2.5)*(2.5))/("<<massZ_mean<<"*"<<massZ_mean<<")))/(((14.0/22.0)*((2.5)*(2.5)*"<<massZ_mean<<"*"<<massZ_mean<<"))/((("<<massZ_mean<<"*"<<massZ_mean<<")-("<<massZ_mean<<"*"<<massZ_mean<<"))*(("<<massZ_mean<<"*"<<massZ_mean<<")-("<<massZ_mean<<"*"<<massZ_mean<<"))+"<<massZ_mean<<"*"<<massZ_mean<<"*"<<massZ_mean<<"*"<<massZ_mean<<"*((2.5)*(2.5))/("<<massZ_mean<<"*"<<massZ_mean<<")))"<<endl;

	draw<<"plot \"reconstruction.txt\" using 1:2 with points lt rgb \"#900C3F\""<<endl;
    draw<<"replot x lt rgb \"#FFFFFF\""<<endl;
	draw<<"replot gaussian(x) lt rgb \"#282828\""<<endl;
	draw<<"replot relativistic_breit_wigner(x) lt rgb \"#FFCC2D\""<<endl;
	draw<<"pause -1"<<endl;



	//-------------------------------------- commande système

	/*########################################
	->A mettre en commentaire en cas de problèmes
		alors entrer ->   gnuplot draw.gnu   <- dans le terminal
	########################################*/

	int syst = system("gnuplot draw.gnu");

    return 0;

}
