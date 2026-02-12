#include "orbit.h"


void Orbit::sim() {
    if (mu == 0) {
        printf("Mu undefined!\n");
    }
}

//https://orbital-mechanics.space/classical-orbital-elements/orbital-elements-and-the-state-vector.html
//Calculate on-rails orbital elements given keplarian elements
void Orbit::physics_to_rails(linalg::vec<double,3> POS, linalg::vec<double,3> VEL,double epoch) {
    //Y is up
    double pi = 3.14159265;
    if (mu == 0 ) { printf("E 134814: MU NOT SET!!!!\n");}
    //Dont use linalg::normalize() to find length.

    double POS_mag = linalg::length(POS);
    double VEL_mag = linalg::length(VEL);
    double v_r = linalg::dot(POS, VEL) / POS_mag; //radial velocity
    //double v_p = linalg::sqrt((VEL_mag*VEL_mag) - (v_r*v_r));   //azimuthal velocity
    
    //Angular momentum (h, h_mag)
    linalg::vec<double, 3> h = linalg::cross(POS,VEL);
    double h_mag = linalg::length(h);

    //Inclination
    double i = acos(linalg::clamp(h[1] / h_mag, -1.0, 1.0));

    //Right Ascension @ Ascending Node
    //N=^K x h
    linalg::vec<double,3> K = {0,1,0};

    //Node line (n, n_mag)
    linalg::vec<double,3> n = linalg::cross(K, h);
    double n_mag = linalg::length(n);

    //LAN (Omega)
    double Omega = acos(linalg::clamp(n[0] / n_mag, -1.0, 1.0));
    if (n[2] < 0)
    Omega = 2*pi - Omega;


    //Eccentricity (e, e_mag)
    linalg::vec<double,3> e = (linalg::cross(VEL, h) / mu) - (POS / POS_mag);
    double e_mag = linalg::length(e);


    //Argument of periapsis (Lowercase omega)
    double omega = acos(linalg::clamp(dot(n, e) / (n_mag * e_mag), -1.0, 1.0));
    if (dot(cross(n, e), h) < 0)
        omega = 2*pi - omega;
    
    //True anomaly
    double ta = acos(linalg::clamp(dot(e, POS) / (e_mag * POS_mag), -1.0, 1.0));
    if (v_r < 0)
        ta = 2*pi - ta;


    double specific_energy = (VEL_mag*VEL_mag)/2.0 - (mu / POS_mag);
    double a = -mu / (2.0 * specific_energy);
    double p = (h_mag * h_mag) / mu;
    //STO ECC
    eccentricity = e_mag;

    //STO SmA
    if (eccentricity < 1.0) { //CAnnot be hyperbonlic
        semi_minor_axis = a * sqrt(1 - eccentricity*eccentricity);
    } else {
        semi_minor_axis = NAN;
    }
    //STO SMA
    semi_major_axis = a;

    //STO SLR
    semi_latus_rectum = p;

    //STO Epoch
    this->epoch = epoch;
    
    //STO Period
    if (eccentricity < 1.0) {
        period = 2*pi * sqrt((a*a*a)/mu);
    } else {
        period = NAN;
    }

    //STO Orbital speed
    orbital_speed = sqrt(mu * (2.0/POS_mag - 1.0/a)); //Derived from orbital elems

    //STO INC
    inclination = i;

    //STO MA
    double E = 2.0 * linalg::atan( linalg::tan(ta/2.0) *
        linalg::sqrt((1.0 - eccentricity)/(1.0 + eccentricity)));
        mean_anomaly = E - eccentricity * linalg::sin(E);

    //STO MA-E
    //?????????????????

    //STO LAN
    long_ascending_node = Omega;

    //STO A_P
    argument_of_periapsis = omega;


}
