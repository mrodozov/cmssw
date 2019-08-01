#include "BFit.h"
#include <cstring>

using namespace std;
using namespace magfieldparam;

//_______________________________________________________________________________

#ifdef BFit_PW
const double BFit::Z_nom[4] = {-2.37615687260664e-2, -1.86400109250045e-2, -1.80502358070104e-2, -1.60470955291956e-2};

const double BFit::B_nom[4] = {2.02156567013928,  //Nominal foeld values
                               3.51622117206486,
                               3.81143026675623,
                               4.01242188708911};

const double BFit::C_nom[4][16] = {{1.0,
                                    -3.61278802720839e-3,
                                    6.36561393690475e-6,
                                    8.32541914664693e-5,
                                    -2.42108313492765e-6,
                                    -1.87295909297299e-5,
                                    3.06832709074461e-7,
                                    1.91827319271226e-6,
                                    -2.15392717311725e-8,
                                    -1.25266203359502e-7,
                                    3.87507522135914e-10,
                                    4.85518568040635e-9,
                                    4.42080729840719e-11,
                                    -8.83065447433858e-11,
                                    -2.41380148377896e-12,
                                    0.0},
                                   {1.0,
                                    -5.04020236643808e-3,
                                    2.03224205921125e-6,
                                    6.79444854179620e-5,
                                    -1.98082200052911e-6,
                                    -1.93324798138490e-5,
                                    3.15120940544812e-7,
                                    1.82623212354924e-6,
                                    -3.30483297560429e-8,
                                    -1.13251951654739e-7,
                                    1.96974144659278e-9,
                                    4.25153392971594e-9,
                                    -6.12986034064675e-11,
                                    -7.59031334826116e-11,
                                    6.40295019219590e-13,
                                    0.0},
                                   {1.0,
                                    -5.23012318846739e-3,
                                    8.80302231241395e-7,
                                    6.51341641212249e-5,
                                    -1.68564063895995e-6,
                                    -1.93693613146655e-5,
                                    2.58178734098114e-7,
                                    1.81311192824207e-6,
                                    -2.79301520182866e-8,
                                    -1.11679980224632e-7,
                                    1.72615649164433e-9,
                                    4.17328869038146e-9,
                                    -5.72514160410955e-11,
                                    -7.41998111228714e-11,
                                    7.30938527053447e-13,
                                    0.0},
                                   {1.0,
                                    -5.34172971309074e-3,
                                    2.48943649506081e-7,
                                    6.23054033447814e-5,
                                    -1.60390978074464e-6,
                                    -1.92618217244767e-5,
                                    2.42461261622770e-7,
                                    1.78772142159379e-6,
                                    -2.61432416866515e-8,
                                    -1.09159464672341e-7,
                                    1.62705377496138e-9,
                                    4.02967933726133e-9,
                                    -5.48168162195020e-11,
                                    -7.00249566028285e-11,
                                    8.22254619144001e-13,
                                    0.0}};
#else
const double BFit::dZ_0 = -2.62328760352034e-2;
const double BFit::dZ_2 = 5.94363870284212e-4;

const double BFit::C_0[16] = {1.0,
                              -2.52864632909442e-3,
                              8.76365790071351e-6,
                              9.19077286315044e-5,
                              -2.49284256023752e-6,
                              -1.80143891826520e-5,
                              2.29295162454016e-7,
                              1.96139195659245e-6,
                              -3.47342625923464e-9,
                              -1.32147627969588e-7,
                              -1.50735830442900e-9,
                              5.17724172101696e-9,
                              1.54539960459831e-10,
                              -9.30914368388717e-11,
                              -5.20466591966397e-12,
                              0.0};

const double BFit::C_2[16] = {0.0,
                              -2.96314154618866e-4,
                              -6.04246295125223e-7,
                              -2.22393436573694e-6,
                              2.84133631738674e-9,
                              -2.07090716476209e-7,
                              2.55850963123821e-8,
                              -1.06689136150163e-8,
                              -5.48842256680751e-9,
                              1.78987539969165e-9,
                              5.57809366992069e-10,
                              -8.25055601520632e-11,
                              -3.18509299957904e-11,
                              1.11714602344300e-12,
                              7.90102331886296e-13,
                              0.0};

const double BFit::C_4[16] = {0.0,
                              7.57194953855834e-6,
                              4.48169046115052e-9,
                              2.49606093449927e-8,
                              3.42264285146368e-9,
                              7.95338846845187e-9,
                              -1.57711106312732e-9,
                              1.02715424120585e-11,
                              2.57261485255293e-10,
                              -2.41682937761163e-11,
                              -2.27894837943020e-11,
                              7.98570801347331e-13,
                              1.17889573705870e-12,
                              1.64571374852252e-14,
                              -2.60212133934707e-14,
                              0.0};
#endif
//_______________________________________________________________________________
BFit::BFit() {
  dZ = 0.;
  memset(C, 0, 16 * sizeof(double));
  rz_poly *P_base = new rz_poly(16);  //Potential basis

  Bz_base = new rz_poly(P_base->Diff(1));  //Bz basis
  Bz_base->SetOFF(1);                      //Switch off linear term

  Br_base = new rz_poly(P_base->Diff(0));  //Br basis is shifted, so
  Br_base->SetOFF(0);                      //"0" term is ignored

  delete P_base;
}

//_______________________________________________________________________________
void BFit::SetField(double B) {
  //Set nominal field [Tesla]
  //
  unsigned int jj;

#ifdef BFit_PW
  unsigned int kk = 1;
  double w_0, w_1;
  if (B <= B_nom[0]) {
    dZ = Z_nom[0];
    for (jj = 0; jj < 16; ++jj) {
      C[jj] = B * C_nom[0][jj];
    }
  } else if (B >= B_nom[3]) {
    dZ = Z_nom[3];
    for (jj = 0; jj < 16; ++jj) {
      C[jj] = B * C_nom[3][jj];
    }
  } else {
    while (B_nom[kk] < B)
      ++kk;
    w_1 = (B - B_nom[kk - 1]) / (B_nom[kk] - B_nom[kk - 1]);
    w_0 = 1.0 - w_1;
    dZ = Z_nom[kk - 1] * w_0 + Z_nom[kk] * w_1;
    for (jj = 0; jj < 16; ++jj) {
      C[jj] = B * (C_nom[kk - 1][jj] * w_0 + C_nom[kk][jj] * w_1);
    }
  }
#else
  double B2 = B * B;
  dZ = dZ_0 + dZ_2 * B2;
  for (jj = 0; jj < 16; ++jj) {
    C[jj] = B * ((C_4[jj] * B2 + C_2[jj]) * B2 + C_0[jj]);
  }
#endif
}

//_______________________________________________________________________________
void BFit::GetField(double r, double z, double phi, double &Br, double &Bz, double &Bphi) const {
  //Get field components in the point (r,z,phi). Always return Bphi=0.
  //Parameters phi and Bphi introduced in order to keep interface
  //compatible with future trully 3D version
  //
  double zc = z + dZ;

  Bz = Bz_base->GetSVal(r, zc, C);
  Br = Br_base->GetSVal(r, zc, C + 1);
  Bphi = 0.;
}
