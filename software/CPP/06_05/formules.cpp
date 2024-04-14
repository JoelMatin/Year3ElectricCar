#include "formule.h"

using namespace std::complex_literals;

float distance(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

float produitScalaire(std::vector<float> v1, std::vector<float> v2) {
    float produitScalaire = v1[0] * v2[0] + v1[1] * v2[1];
    return produitScalaire;
}

float calculNorme(std::vector<float> unVecteur) {
    return sqrt(produitScalaire(unVecteur, unVecteur));
}

void vecteurNorme(std::vector<float>& unVecteur) {
    float norme = calculNorme(unVecteur);
    unVecteur = { unVecteur[0] / norme, unVecteur[1] / norme };
}

std::vector<float> additionerVecteur(std::vector<float> v1, std::vector<float> v2) {
    std::vector<float> v3 = { v1[0] + v2[0], v1[1] + v2[1] };
    return v3;
}

std::vector<float> soustraireVecteur(std::vector<float> v1, std::vector<float> v2) {
    std::vector<float> v3 = { v1[0] - v2[0], v1[1] - v2[1] };
    return v3;
}

std::vector<float> multiplierVecteur(std::vector<float> v, float scalaire) {
    std::vector<float> kv = { v[0] * scalaire, v[1] * scalaire };
    return kv;
}





/////////////////////////////////////////////////


//calcul de gamma_m = alpha + ibeta
std::complex<float> calcul_gm(float epsilon, float sigma, float omega) {
    float alpha = omega * sqrt(mu0 * epsilon / 2) * sqrt(sqrt(1 + (float)pow(sigma / (omega * epsilon), 2)) - 1.F);
    float beta = omega * sqrt(mu0 * epsilon / 2) * sqrt(sqrt(1 + (float)pow(sigma / (omega * epsilon), 2)) + 1.F);
    return alpha + 1if* beta;
}

std::complex<float> calcul_zm(emetteur Source, mur Mur) {
    float omega = Source.freq * 2 * M_PI;
    std::complex<float> epsilonTilde = Mur.permittivite - 1if* Mur.conductivite / omega;
    std::complex<float> Zm = sqrt(mu0 / epsilonTilde);
    return Zm;
}

// Fonction verifie //
point calculPtReflexion(point PSource, point Recepteur, mur Mur) {
    std::vector<float> n = {-( Mur.p2.y - Mur.p1.y),Mur.p2.x - Mur.p1.x };
    vecteurNorme(n);

    //std::vector<float> n = { 1, 0 };
    std::vector<float> vecteurMur = { Mur.p2.x - Mur.p1.x, Mur.p2.y - Mur.p1.y };
    vecteurNorme(vecteurMur);                 //norme le vecteur directeur du mur

    std::vector<float> re = { PSource.x - Mur.p1.x, PSource.y - Mur.p1.y };

    float ren = produitScalaire(re, n);       //produit scalaire entre re et n
    std::vector<float> ri = soustraireVecteur(re, multiplierVecteur(n, 2 * ren));

    std::vector<float> rx = { Recepteur.x - Mur.p1.x, Recepteur.y - Mur.p1.y };
    std::vector<float> d = soustraireVecteur(rx, ri);

    float t = (d[1] * ri[0] - d[0] * ri[1]) / (vecteurMur[0] * d[1] - vecteurMur[1] * d[0]);

    point PtReflexion = { Mur.p1.x + t * vecteurMur[0], Mur.p1.y + t * vecteurMur[1] };
    //std::cout << "(" << PtReflexion.x << ", " << PtReflexion.y << ")" << std::endl;
    return PtReflexion;
}


//trouve le symtrique d'un point par rapport  un mr
point trouverSym(point pt, mur Mur) {
    std::vector<float> n = {- (Mur.p2.y - Mur.p1.y),Mur.p2.x - Mur.p1.x };
    vecteurNorme(n);                    //norme le vecteur normale pour les calculs
    std::vector<float> vecteurMur = { Mur.p2.x - Mur.p1.x, Mur.p2.y - Mur.p1.y };
    vecteurNorme(vecteurMur);

    std::vector<float> re = { pt.x - Mur.p1.x, pt.y - Mur.p1.y };
    float ren = produitScalaire(re, n);       //produit scalaire entre re et n
    std::vector<float> ri = soustraireVecteur(re, multiplierVecteur(n, 2 * ren));
    //std::cout << ri[0] << ", " << ri[1] << std::endl;
    point I = { Mur.p1.x + ri[0], Mur.p1.y + ri[1] };
    return I;
}

//calcul les paramtres trigonomtriques de transmission dans le mur
void calculTrigoReflex(mur MurReflex, point Recepteur, point pSource, point sym, float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s) {
    std::vector<float> d{ Recepteur.x - sym.x, Recepteur.y - sym.y };  //vecteur entre TX et RX
    std::vector<float> n;                        //vecteurn normal au mr
    if (MurReflex.p1.x == MurReflex.p2.x) {                  //s'il est vertical
        n = { 1, 0 };

    }
    else if (MurReflex.p1.y == MurReflex.p2.y) {              //s'il est horizontal
        n = { 0, 1 };
    }
    else {                                       //s'il est oblique
        n = { MurReflex.p1.y - MurReflex.p2.y, MurReflex.p1.x - MurReflex.p2.x };
    }
    vecteurNorme(n);
    vecteurNorme(d);
    cosOi = abs(produitScalaire(d, n));
    sinOi = sqrt(1 - pow(cosOi, 2));
    sinOt = sqrt(epsilon0 / MurReflex.permittivite) * sinOi;
    cosOt = sqrt(1 - pow(sinOt, 2));
    s = MurReflex.epaisseur / cosOt;
}


void calculTrigoTrans(mur MurTrans, point Cible, point pSource, float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s) {
    std::vector<float> d{ Cible.x - pSource.x, Cible.y - pSource.y };  //vecteur entre TX et RX
    std::vector<float> n;                        //vecteurn normal au mr
    if (MurTrans.p1.x == MurTrans.p2.x) {                  //s'il est vertical
        n = { 1, 0 };

    }
    else if (MurTrans.p1.y == MurTrans.p2.y) {              //s'il est horizontal
        n = { 0, 1 };
    }
    else {                                       //s'il est oblique
        n = { MurTrans.p1.y - MurTrans.p2.y, MurTrans.p1.x - MurTrans.p2.x };
    }
    vecteurNorme(n);
    vecteurNorme(d);
    cosOi = abs(produitScalaire(d, n));
    sinOi = sqrt(1 - pow(cosOi, 2));
    sinOt = sqrt(epsilon0 / MurTrans.permittivite) * sinOi;
    cosOt = sqrt(1 - pow(sinOt, 2));
    s = MurTrans.epaisseur / cosOt;
}


//calcul le coefficient de transmission d'une polarisation perpendiculaire
std::complex<float> coeffTrans(mur Mur, point Recepteur, emetteur Source, point pSource, std::complex<float> Zm, std::complex<float> gm) {
    float cosOi, cosOt, sinOi, sinOt, s;
    calculTrigoTrans(Mur, Recepteur, pSource, cosOi, cosOt, sinOi, sinOt, s);

    float omega = 2 * M_PI * Source.freq;
    float beta = omega / c;

    std::complex<float> Rp = (Zm * cosOi - Z0 * cosOt) / (Zm * cosOi + Z0 * cosOt);
    std::complex<float> Tm = ((1.F - (std::complex<float>)pow(Rp, 2)) * exp(-gm * s)) / (1.0F - (std::complex<float>)pow(Rp, 2) * exp(-2.F * gm * s) * exp(beta * 1if * 2.F * s * sinOt * sinOi));
    return Tm;
}

//calcule le coefficient cummule de plusieurs obstacles
std::complex<float> coeffTransTotal(emetteur Source, point Cible, point pSource, float omega, std::vector<mur> MursTrans) {
    std::complex<float> Tm = 1;
    for (const mur& murTrans : MursTrans) {
        std::complex<float> zmT = calcul_zm(Source, murTrans);
        std::complex<float> gmT = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
        Tm *= coeffTrans(murTrans, Cible, Source, pSource, zmT, gmT);
    }
    return Tm;
}

//calcul le coefficient de rflexion d'une polarisation perpendiculaire
std::complex<float> coeffReflex(mur Mur, point Recepteur, emetteur Source, point pSource, std::complex<float> Zm, std::complex<float> gm, point sym) {
    float cosOi, cosOt, sinOi, sinOt, s;
    calculTrigoReflex(Mur, Recepteur, pSource, sym, cosOi, cosOt, sinOi, sinOt, s);
    float omega = 2 * M_PI * Source.freq;
    float beta = omega / c;

    std::complex<float> Rp = (Zm * cosOi - Z0 * cosOt) / (Zm * cosOi + Z0 * cosOt);
    std::complex<float> Rm = Rp - (1.F - Rp * Rp) *
        Rp * (exp(-2.F * gm * s) * exp(beta * 1if * 2.F * s * sinOt * sinOi)) /
        (1.F - Rp * Rp * exp(-2.F * gm * s) * exp(beta * 1if * 2.F * s * sinOt * sinOi));
    //std::cout << "Coefficient de Rflexion: " << Rm << std::endl;
    return Rm;
}

//calcule la puissance de l'metteur dans la direction du rcepteur en ca
float calculGTX(emetteur Source, point Recepteur) {
    return 1.7; 
}


//verifie si p1 p2 p3 sont enumeres dans le sens trigonomtrique
bool sensTrigo(point p1, point p2, point p3) {
    return (p3.y - p1.y) * (p2.x - p1.x) > (p2.y - p1.y) * (p3.x - p1.x);
}

float calculPente(point p1, point p2) {
    return (p2.y - p1.y) / (p2.x - p1.x);
}

//vrifie s'il y a intersection entre segments p1p2 et p3p4: si le sens d'enumeration (trigo ou anti
// trigo) de p1, p2, p3 n'est pas celui de p1, p2, p4 ou que celui de p1, p3, p4 est different de celui de p2, p3, p4 alors
//pour chaque segment, le deuxime point se trouve de l'autre cote du deuxieme segment donc intersection. Un point
//d'un segment ne peut pas tre sur l'autre sinon le sens d'enumeration est le mme et il n'y a pas intersection
bool segmentsIntersect(point p1, point p2, point p3, point p4) {
    if ((calculPente(p3, p4) == calculPente(p3, p1)) || (calculPente(p3, p4) == calculPente(p3, p2))) {
        return false;
    }
    else {
        return ((sensTrigo(p1, p2, p3) != sensTrigo(p1, p2, p4)) && (sensTrigo(p1, p3, p4) != sensTrigo(p2, p3, p4)));
    }
}
