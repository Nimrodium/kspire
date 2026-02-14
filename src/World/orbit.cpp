#include "orbit.h"


//Newton-Rhapson equation solver
double Orbit::solveEccentricAnomaly(double M, double ecc, double maxError = 1E-07)
{
    double adjust = 1.0;
    double guess = M + ecc * linalg::sin(M) + 0.5 * ecc * ecc * linalg::sin(2.0 * M);
    while (linalg::abs(adjust) > maxError)
    {
        double ans = guess - ecc * linalg::sin(guess);
        adjust = (M - ans) / (1.0 - ecc * linalg::cos(guess));
        guess += adjust;
    }
    return guess;
}

double Orbit::solveEccentricAnomalyHyp(double M, double ecc, double maxError = 1E-07)
{
		double adjust = 1.0;
		double guess = linalg::log(2.0 * M / ecc + 1.8);
		while (linalg::abs(adjust) > maxError)
		{
			adjust = (eccentricity * linalg::sinh(guess) - guess - M) / (eccentricity * linalg::cosh(guess) - 1.0);
			guess -= adjust;
		}
		return guess;
}        

void Orbit::calculate_state_from_keplers(double _UNIVERSAL_TIME) {
    universal_time = _UNIVERSAL_TIME;

    if (mu == 0) {printf("Mu undefined!\n"); return;}

    //Time along orbit after Epoch
    double ObT = universal_time - epoch;

    double radius;
    double eccentric_anomaly;
    double true_anomaly;
    //Calc MA, Ecc-A TA, and Radius.
    if (eccentricity < 1.0) { //SOLVE ELLIPTICAL / CIRCULAR
        
        if (eccentricity < 1E-05) { printf("ECC edge case\n");}
        //Calc MA
        mean_anomaly = ObT / period * 2.0 * pi;
        //Calc EA
        if (eccentricity < 1E-05) { //Near-circular orbits
            eccentric_anomaly = mean_anomaly;
            true_anomaly = mean_anomaly;
        } else {    //Noncircular orbits
            if (eccentricity < 0.9) {
                eccentric_anomaly = solveEccentricAnomaly(mean_anomaly,eccentricity);
            } else {
                printf("ECC TOO HIGHH!!! SOLVE EXTREME CASE WITH OTHER FUNC\n");
            }
                //Calc TA
                true_anomaly = linalg::acos(
                (linalg::cos(eccentric_anomaly) - eccentricity)
                /(1.0 - eccentricity * linalg::cos(eccentric_anomaly))
            );
        }
        if (ObT > period / 2.0) {
            true_anomaly = pi * 2 - true_anomaly;
        }
        radius = semi_major_axis * (1.0 - eccentricity * eccentricity)
        / (1.0 + eccentricity * linalg::cos(true_anomaly));

    } else if (eccentricity > 1.0) { //SOLVE HYPERBOLIC
        if (eccentricity == 1.0)    //Turn parabolic orbit edge case into hyperbolic
            eccentricity += 1E-10;
        mean_anomaly = 2.0 * pi * linalg::abs(ObT) / period;
        eccentric_anomaly = solveEccentricAnomalyHyp(mean_anomaly,eccentricity);
        true_anomaly = linalg::atan2(linalg::sqrt(eccentricity * eccentricity - 1.0) * linalg::sinh(eccentric_anomaly), eccentricity - linalg::cosh(eccentric_anomaly));
        if (ObT < 2.0) {
            true_anomaly = pi * 2 - true_anomaly;
        }
        radius = (0.0 - semi_major_axis) * (eccentricity * eccentricity - 1.0) / (1.0 + eccentricity * linalg::cos(true_anomaly));
    }  

    //Now convert to perifocal plane
    //Describes a 2D plane along the plane of the orbit
    //POS
    linalg::vec<double, 3> r_pf;
    r_pf.x = radius * cos(true_anomaly);
    r_pf.y = radius * sin(true_anomaly);
    r_pf.z = 0.0;

    //VEL

    double h = sqrt(mu * semi_major_axis * (1 - eccentricity*eccentricity));

    linalg::vec<double, 3> v_pf;
    v_pf.x = -mu / h * sin(true_anomaly);
    v_pf.y =  mu / h * (eccentricity + cos(true_anomaly));
    v_pf.z =  0.0;


    //Rotation matrix
    linalg::mat<double,3,3> RM; 

    //Precompute terms
    double cos_o = linalg::cos(long_ascending_node);
    double sin_o = linalg::sin(long_ascending_node);
    double cos_i = linalg::cos(inclination);
    double sin_i = linalg::sin(inclination);
    double cos_w = linalg::cos(argument_of_periapsis);
    double sin_w = linalg::sin(argument_of_periapsis);

    RM[0][0] = cos_o*cos_w-sin_o*cos_i*sin_w;
    RM[0][1] = -cos_o*sin_w-sin_o*cos_i*cos_w;
    RM[0][2] = sin_o*sin_i;

    RM[1][0] = sin_o*cos_w+cos_o*cos_i*sin_w;
    RM[1][1] = -sin_o*sin_w-cos_o*cos_i*cos_w;
    RM[1][2] = -cos_o*sin_i;

    RM[2][0] = sin_i*sin_w;
    RM[2][1] = sin_i*cos_w;
    RM[2][2] = cos_i;
    
    //Matrix multiplication
    POS = linalg::mul(RM,r_pf);
    VEL = linalg::mul(RM,v_pf);
}

//https://orbital-mechanics.space/classical-orbital-elements/orbital-elements-and-the-state-vector.html
//Calculate on-rails orbital elements given keplarian elements
void Orbit::physics_to_rails(linalg::vec<double,3> POS, linalg::vec<double,3> VEL,double epoch) {
    //Y is up

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
    mean_anomaly_at_epoch = epoch;

    //STO LAN
    long_ascending_node = Omega;

    //STO A_P
    argument_of_periapsis = omega;


}
