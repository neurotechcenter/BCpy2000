/*************************************************************************
*  CONVERT.C
*
*      Transducer conversion functions for:
*         Thermocouples
*         Strain gauges
*         RTDs
*         Thermistors
*      Simply compile into an object code module and link with the
*      rest of your code.
*
*      See the file convert.h for prototypes.
*      See the NI-DAQ User Manual for descriptions.
*
**************************************************************************/

/* ---------------- *\
 * --- INCLUDES --- *
\* ---------------- */
#include <math.h>
#include "convert.h"


/* -------------------- *\
 * --- Thermocouple --- *
\* -------------------- */
/*************************************************************************
*  This section contains all the source code for performing thermocouple
*  buffer conversion routines. The Thermocouple_Buf_Convert function
*  converts a buffer of thermocouple voltages into a buffer of temperature
*  values taking into account the cold junction compensation. Cold junction
*  compensation is done by a more accurate method involving voltage
*  corrections to thermocouple buffer before being converted to the
*  equivalent temperature.
*
*  ADDING A NEW THERMOCOUPLE
*  -------------------------
*  1)  Define a constant for the thermocouple type in "convert.h".
*
*  2)  Use the polynomial data type to define the equation coeffecients
*      for all temperature ranges.
*
*  3)  Add the equation type for evaluating the polynomial in the function
*      "compute_equation".
*
*  4)  Modify the functions "temp_to_volt" and "volt_to_temp" to choose
*      the right polynomial.
*
**************************************************************************/

/*************************************************************************
*
*  Data Structures & Global Variables
*
*  polynomial : Data Structure that contains polynomial coeffecients used
*               by the thermocouple voltage and temperature conversion
*               functions.
*
*  Bpoly_VtoT[2] : Type B thermocouple polynomial for converting a thermocouple
*                  voltage to an equivalent temperature. The array subscript
*                  of two indicates that there are two equations that characterize
*                  B type thermocouples for the entire temperature range.
*
*  Bpoly_TtoV[2] : Type B thermocouple polynomial for converting a thermocouple
*                  temperature to an equivalent voltage. The array subscript
*                  of two indicates that there are two equations that characterize
*                  B type thermocouples for the entire temperature range.
*
**************************************************************************/

typedef struct
{
  int      eq_type;    /* Equation Type; for efficient computation */
  double   C[15];      /* polynomial coeffecients array */
  double   alpha[2];   /* exponential factor used by K_type thermocouples */

} polynomial;

/* function prototypes used only by the thermocouple conversion routines. */

int compute_equation ( polynomial *poly1, double input, double *output);
int volts_to_temp( int TCtype, double volts, double *temperature);
int temp_to_volts( int TCtype, double temperature, double *volts );
int change_temp_scale  ( int tempScale, double *temperature );

polynomial Bpoly_VtoT[2] =
{
  { 8,
    9.8423321E+1,   6.9971500E-1,     -8.4765304E-4,     1.005264E-6,
   -8.3345952E-10,  4.5508542E-13,    -1.5523037E-16,    2.9886750E-20,
   -2.4742860E-24,  0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 8,
    2.135071E+2,    2.8510504E-1,     -5.2742887E-5,     9.9160804E-9,
   -1.2965303E-12,  1.1195870E-16,    -6.0625199E-21,    1.8661696E-25,
   -2.4878585E-30,  0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                }

};

polynomial Epoly_VtoT[2] =
{
  { 8,
    0.0,            1.6977288E-2,     -4.3514970E-7,    -1.5859697E-10,
   -9.2502871E-14, -2.6084314E-17,    -4.1360199E-21,   -3.4034030E-25,
   -1.1564890E-29,  0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 9,
    0.0,            1.7057035E-2,     -2.3301759E-7,    6.5435585E-12,
   -7.3562749E-17, -1.7896001E-21,     8.4036165E-26,  -1.3735879E-30,
    1.0629823E-35, -3.2447087E-41,     0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                }
};

polynomial Jpoly_VtoT[3] =
{
  { 8,
    0.0,            1.9528268E-2,     -1.2286185E-6,    -1.0752178E-9,
   -5.9086933E-13, -1.7256713E-16,    -2.8131513E-20,   -2.3963370E-24,
   -8.3823321E-29,  0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 7,
    0.0,            1.978425E-2,    -2.001204E-7,     1.036969E-11,
   -2.549687E-16,   3.585153E-21,   -5.344285E-26,    5.099890E-31,
    0.0,            0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 5,
   -3.11358187E+3,  3.00543684E-1,  -9.94773230E-6,  1.70276630E-10,
   -1.43033468E-15, 4.73886084E-21,  0.0,            0.0,
    0.0,            0.0,             0.0,            0.0,
    0.0,            0.0,             0.0,            0.0,
    0.0                                                                }
};


polynomial Kpoly_VtoT[3] =
{
  { 8,
    0.0,            2.5173462E-2,     -1.1662878E-6,    -1.0833638E-9,
   -8.9773540E-13, -3.7342377E-16,    -8.6632643E-20,   -1.0450598E-23,
   -5.1920577E-28,  0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 9,
    0.0,            2.508355E-2,     7.860106E-8,    -2.503131E-10,
    8.315270E-14,  -1.228034E-17,    9.804036E-22,   -4.413030E-26,
    1.057734E-30,  -1.052755E-35,    0.0,             0.0,
    0.0,            0.0,             0.0,             0.0,
    0.0                                                                },

  { 6,
    -1.318058E+2,   4.830222E-2,     -1.646031E-6,    5.464731E-11,
    -9.650715E-16,  8.802193E-21,    -3.110810E-26,   0.0,
    0.0,            0.0,              0.0,            0.0,
    0.0,            0.0,              0.0,            0.0,
    0.0                                                                }
};

polynomial Npoly_VtoT[4] =
{
  { 9,
    0.0,            3.8436847E-2,      1.1010485E-6,     5.2229312E-9,
    7.2060525E-12,  5.8488586E-15,     2.7754916E-18,    7.7075166E-22,
    1.1582665E-25,  7.3138868E-30,     0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 7,
    0.0,            3.86896E-2,     -1.08267E-6,      4.70205E-11,
   -2.12169E-18,   -1.17272E-19,     5.39280E-24,    -7.98156E-29,
    0.0,            0.0,             0.0,             0.0,
    0.0,            0.0,             0.0,             0.0,
    0.0                                                                },
  { 5,
    1.972485E+1,   3.300943E-2,     -3.915159E-7,    9.855391E-12,
   -1.274371E-16,  7.767022E-22,     0.0,            0.0,
    0.0,            0.0,             0.0,            0.0,
    0.0,            0.0,             0.0,            0.0,
    0.0                                                                },
  { 8,
    0.0,            3.8783277E-2,     -1.1612344E-6,     6.9525655E-11,
   -3.0090077E-15,  8.8311584E-20,    -1.6213839E-24,    1.6693362E-29,
   -7.3117540E-35,  0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                }
};

polynomial Rpoly_VtoT[4] =
{
  { 10,
    0.0,            1.8891380E-1,     -9.3835290E-5,     1.3068619E-7,
   -2.2703580E-10,  3.5145659E-13,    -3.8953900E-16,    2.8239471E-19,
   -1.2607281E-22,  3.1353611E-26,    -3.3187769E-30,    0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 9,
    1.334584505E+1,  1.472644573E-1,  -1.844024844E-5,  4.031129726E-9,
   -6.249428360E-13, 6.468412046E-17, -4.458750426E-21, 1.994710149E-25,
   -5.313401790E-30, 6.481976217E-35,  0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0                                                                },
  { 5,
   -8.199599416E+1,  1.553962042E-1,  -8.342197663E-6,  4.279433549E-10,
   -1.191577910E-14, 1.492290091E-19,  0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0                                                                },
  { 4,
    3.406177836E+4, -7.023729171,      5.582903813E-4, -1.952394635E-8,
    2.560740231E-13, 0.0,              0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0                                                                }
};

polynomial Spoly_VtoT[4] =
{
  { 9,
    0.0,            1.84949460E-1,    -8.00504062E-5,    1.02237430E-7,
   -1.52248592E-10, 1.88821343E-13,   -1.59085941E-16,   8.23027880E-20,
   -2.34181944E-23, 2.79786260E-27,    0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 9,
    1.291507177E+1,  1.466298863E-1,  -1.534713402E-5,  3.145945973E-9,
   -4.163257839E-13, 3.187963771E-17, -1.291637500E-21, 2.183475087E-26,
   -1.447379511E-31, 8.211272125E-36,  0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 5,
   -8.087801117E+1,  1.621573104E-1,  -8.536869453E-6,  4.719686976E-10,
   -1.441693666E-14, 2.081618890E-19,  0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0                                                                },
  { 4,
    5.333875126E+4, -1.235892298E+1,   1.092657613E-3, -4.265693686E-8,
    6.247205420E-13, 0.0,              0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0,             0.0,              0.0,             0.0,
    0.0                                                                }
};

polynomial Tpoly_VtoT[2] =
{
  { 7,
    0.0,            2.5949192E-2,     -2.1316967E-7,     7.9018692E-10,
    4.2527777E-13,  1.3304473E-16,     2.0241446E-20,    1.2668171E-24,
    0.0,            0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                },
  { 6,
    0.0,            2.592800E-2,      -7.602961E-7,      4.637791E-11,
   -2.165394E-15,   6.048144E-20,     -7.293422E-25,     0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0,            0.0,               0.0,              0.0,
    0.0                                                                }
};

polynomial Bpoly_TtoV[2] =
{
  { 6,
    0.0,             -2.4650818346E-1,   5.9040421171E-3, -1.3257931636E-6,
    1.5668291901E-9, -1.6944529240E-12,  6.2990347094E-16, 0.0,
    0.0,              0.0,               0.0,              0.0,
    0.0,              0.0,               0.0,              0.0,
    0.0                                                                },
  { 8,
   -3.8938168621E+3,  2.8571747470E+1,  -8.4885104785E-2,  1.5785280164E-4,
   -1.6835344864E-7,  1.1109794013E-10, -4.4515431033E-14, 9.8975640821E-18,
   -9.3791330289E-22, 0.0,               0.0,              0.0,
    0.0,              0.0,               0.0,              0.0,
    0.0                                                                }
};

polynomial Epoly_TtoV[2] =
{
  { 13,
    0.0,               5.8665508708E+1,   4.5410977124E-2, -7.7998048686E-4,
   -2.5800160843E-5,  -5.9452583057E-7,  -9.3214058667E-9, -1.0287605534E-10,
   -8.0370123621E-13, -4.3979497391E-15, -1.6414776355E-17,-3.9673619516E-20,
   -5.5827328721E-23, -3.4657842013E-26,  0.0,              0.0,
    0.0                                                                      },
  { 10,
    0.0,               5.8665508710E+1,   4.5032275582E-2,   2.8908407212E-5,
   -3.3056896652E-7,   6.5024403270E-10, -1.9197495504E-13, -1.2536600497E-15,
    2.1489217569E-18, -1.4388041782E-21,  3.5960899481E-25,  0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                      }
};

polynomial Jpoly_TtoV[2] =
{
  { 8,
    0.0,               5.0381187815E+1,   3.0475836930E-2,  -8.5681065720E-5,
    1.3228195295E-7,  -1.7052958337E-10,  2.0948090697E-13, -1.2538395336E-16,
    1.5631725697E-20,  0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                       },
  { 5,
    2.9645625681E+5,  -1.4976127786E+3,   3.1787103924,     -3.1847686701E-3,
    1.5720819004E-6,  -3.0691369056E-10,  0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                       }
};

polynomial Kpoly_TtoV[2] =
{
  { 10,
    0.0,               3.9450128025E+1,   2.3622373598E-2,  -3.2858906784E-4,
   -4.9904828777E-6,  -6.7509059173E-8,  -5.7410327428E-10, -3.1088872894E-12,
   -1.0451609365E-14, -1.9889266878E-17, -1.6322697486E-20,  0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                      },
  { 100,
   -1.7600413686E+1,   3.8921204975E+1,   1.8558770032E-2,  -9.9457592874E-5,
    3.1840945719E-7,  -5.6072844889E-10,  5.6075059059E-13, -3.2020720003E-16,
    9.7151147152E-20, -1.2104721275E-23,  0.0,               0.0,
    0.0,               0.0,               0.0,               1.185976E+2,
   -1.183432E-4                                                              }

};

polynomial Npoly_TtoV[3] =
{
  { 8,
    0.0,               2.6159105962E+1,   1.0957484228E-2,  -9.3841111554E-5,
   -4.6412039759E-8,  -2.6303357716E-9,  -2.2653438003E-11, -7.6089300791E-14,
   -9.3419667835E-17,  0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                      },
  { 10,
    0.0,               2.5929394601E+1,   1.5710141880E-2,   4.3825627237E-5,
   -2.5261169794E-7,   6.4311819339E-10, -1.0063471519E-12,  9.9745338992E-16,
   -6.0863245607E-19,  2.0849229339E-22, -3.0682196151E-26,  0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                      }
};

polynomial Rpoly_TtoV[3] =
{
  { 9,
    0.0,               5.28961729765,     1.39166589782E-2, -2.38855693017E-5,
    3.56916001063E-8, -4.62347666298E-11, 5.00777441034E-14,-3.73105886191E-17,
    1.57716482367E-20,-2.81038625251E-24, 0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                      },
  { 5,
    2.95157925316E+3, -2.52061251332,     1.59564501865E-2, -7.64085947576E-6,
    2.05305291024E-9, -2.93359668173E-13, 0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                      },
  { 4,
    1.52232118209E+5, -2.68819888545E+2,  1.71280280471E-1, -3.45895706453E-5,
   -9.34633971046E-12, 0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                      }
};

polynomial Spoly_TtoV[3] =
{
  { 8,
    0.0,               5.40313308631,     1.25934289740E-2, -2.32477968689E-5,
    3.22028823036E-8, -3.31465196389E-11, 2.55744251786E-14,-1.25068871393E-17,
    2.71443176145E-21, 0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                       },
  { 4,
    1.32900444085E+3,  3.34509311344,     6.54805192818E-3, -1.64856259209E-6,
    1.29989605174E-11, 0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                       },
  { 4,
    1.46628232636E+5, -2.58430516752E+2,  1.63693574641E-1, -3.30439046987E-5,
   -9.43223690612E-12, 0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                       }

};

polynomial Tpoly_TtoV[2] =
{
  { 14,
    0.0,               3.8748106364E+1,   4.4194434347E-2,   1.1844323105E-4,
    2.0032973554E-5,   9.0138019559E-7,   2.2651156593E-8,   3.6071154205E-10,
    3.8493939883E-12,  2.8213521925E-14,  1.4251594779E-16,  4.8768662286E-19,
    1.0795539270E-21,  1.3945027062E-24,  7.9795153927E-28,  0.0,
    0.0                                                                       },
  { 8,
    0.0,               3.8748106364E+1,   3.3292227880E-2,   2.0618243404E-4,
   -2.1882256846E-6,   1.0996880928E-8,  -3.0815758772E-11,  4.5479135290E-14,
   -2.7512901673E-17,  0.0,               0.0,               0.0,
    0.0,               0.0,               0.0,               0.0,
    0.0                                                                       }
};

/*************************************************************************
*
*  Thermocouple_Convert
*
*      Uses the Thermocouple_Buf_Convert function to convert a thermocouple
*      voltage to temperature
*
**************************************************************************/
int Thermocouple_Convert (TCtype, CJCtemp, tempScale, TCvolts, TCtemp)
int TCtype;
double CJCtemp;
int tempScale;
double TCvolts, *TCtemp;
{
   *TCtemp = 0.0;
   return Thermocouple_Buf_Convert (TCtype, CJCtemp, tempScale, 1L, &TCvolts,
                                    TCtemp);
}
/*************************************************************************
*   compute_equation
*
*   FUNCTION:
*    Evaluates a polynomial equation.
*
*   INPUTS:
*    poly1  : Structure that holds the polynomial coeffecients and the
*             equation type to be evaluated.
*    input  : Input variable to be used to evaluate the polynomial.
*
*   OUTPUTS:
*    output : Contains the value of the evaluated polynomial. It is a
*             call by reference parameter.
*
**************************************************************************/

int compute_equation ( poly1, input, output)

  polynomial *poly1;
  double     input, *output;

{

   switch ( poly1->eq_type )
   {

      case 4:
          *output =
            poly1->C[0] + input * (poly1->C[1] + input * (poly1->C[2] + input *
           (poly1->C[3] + input *  poly1->C[4])));
           break;

      case 5:
          *output =
            poly1->C[0] + input * (poly1->C[1] + input * (poly1->C[2] + input *
           (poly1->C[3] + input * (poly1->C[4] + input *  poly1->C[5]))));
           break;

      case 6:
          *output =
            poly1->C[0] + input * (poly1->C[1] + input * (poly1->C[2] + input *
           (poly1->C[3] + input * (poly1->C[4] + input * (poly1->C[5] + input *
            poly1->C[6])))));
            break;

      case 7:
          *output =
            poly1->C[0] + input * (poly1->C[1] + input * (poly1->C[2] + input *
           (poly1->C[3] + input * (poly1->C[4] + input * (poly1->C[5] + input *
           (poly1->C[6] + input *  poly1->C[7]))))));
           break;

      case 8:

          *output =
            poly1->C[0] + input * (poly1->C[1] + input * (poly1->C[2] + input *
           (poly1->C[3] + input * (poly1->C[4] + input * (poly1->C[5] + input *
           (poly1->C[6] + input * (poly1->C[7] + input *  poly1->C[8])))))));
           break;

      case 9:
          *output =
            poly1->C[0] + input * (poly1->C[1] + input * (poly1->C[2] + input *
           (poly1->C[3] + input * (poly1->C[4] + input * (poly1->C[5] + input *
           (poly1->C[6] + input * (poly1->C[7] + input * (poly1->C[8] + input *
            poly1->C[9]))))))));
           break;

      case 10:
          *output =
            poly1->C[0] + input * (poly1->C[1] + input * (poly1->C[2] + input *
           (poly1->C[3] + input * (poly1->C[4] + input * (poly1->C[5] + input *
           (poly1->C[6] + input * (poly1->C[7] + input * (poly1->C[8] + input *
           (poly1->C[9] + input * poly1->C[10])))))))));
           break;

      case 13:
          *output =
            poly1->C[0]  + input * (poly1->C[1]  + input * (poly1->C[2]  + input *
           (poly1->C[3]  + input * (poly1->C[4]  + input * (poly1->C[5]  + input *
           (poly1->C[6]  + input * (poly1->C[7]  + input * (poly1->C[8]  + input *
           (poly1->C[9]  + input * (poly1->C[10] + input * (poly1->C[11] + input *
           (poly1->C[12] + input *  poly1->C[13]))))))))))));
           break;

      case 14:
          *output =
            poly1->C[0]  + input * (poly1->C[1]  + input * (poly1->C[2]  + input *
           (poly1->C[3]  + input * (poly1->C[4]  + input * (poly1->C[5]  + input *
           (poly1->C[6]  + input * (poly1->C[7]  + input * (poly1->C[8]  + input *
           (poly1->C[9]  + input * (poly1->C[10] + input * (poly1->C[11] + input *
           (poly1->C[12] + input * (poly1->C[13] + input * poly1->C[14])))))))))))));
           break;

      case 100:

          *output =
            poly1->C[0]  + input * (poly1->C[1]  + input * (poly1->C[2]  + input *
           (poly1->C[3]  + input * (poly1->C[4]  + input * (poly1->C[5]  + input *
           (poly1->C[6]  + input * (poly1->C[7]  + input * (poly1->C[8]  + input *
           (poly1->C[9])))))))))
           +
           (double) ( (long double) poly1->alpha[0] *
                       powl( (long double) 2.718281828,
                             (long double) (poly1->alpha[1]*
                                           (input - 126.9686)*(input - 126.9686))));
            break;

      default :

          *output =
            poly1->C[0]  + input * (poly1->C[1]  + input * (poly1->C[2]  + input *
           (poly1->C[3]  + input * (poly1->C[4]  + input * (poly1->C[5]  + input *
           (poly1->C[6]  + input * (poly1->C[7]  + input * (poly1->C[8]  + input *
           (poly1->C[9]  + input * (poly1->C[10] + input * (poly1->C[11] + input *
           (poly1->C[12] + input * (poly1->C[13] + input * poly1->C[14] )))))))))))))
           +
           (double) ( (long double) poly1->alpha[0] *
                       powl( (long double) 2.718281828,
                       powl( (long double) (poly1->alpha[1]*(input - 126.9686)),
                             (long double) 2.0)));
           break;

    } /* end switch() */
    return 0;
}
/*************************************************************************
*   volts_to_temp
*
*    FUNCTION:
*    Converts a single thermocouple voltage value to a corresponding
*    temperature using the NIST standards. Several equations characterize
*    a thermocouple type corresponding to different temperature ranges.
*    The equations are mostly Nth order polynomials.
*
*    INPUTS:
*     TCtype       : The thermocouple type (constant defines in convert.h).
*     volts        : The thermocouple voltage in micro volts.
*
*    OUTPUTS:
*     temperature  : Call by reference parameter that returns the temperature.
*                    Returns an error if the thermocouple type is invalid.
*
*
**************************************************************************/

int volts_to_temp( TCtype, volts, temperature)


 int      TCtype;
 double   volts;
 double   *temperature;

{
  polynomial *poly1;

  /* convert voltage into micro volts */

  volts = volts * 1.0E+6;

  switch ( TCtype)
  {

     case B_TYPE_TC:

        if ( volts <= 2431 )           /* TempRange -50 to 250 ; ErrRange   0.03 to -0.02 */
           poly1 = &Bpoly_VtoT[0];
        else                           /* TempRange 700 to 1820 ; ErrRange 0.02 to -0.01 */
           poly1 = &Bpoly_VtoT[1];
        break ;


     case J_TYPE_TC:
        if ( volts <= 0 )              /* TempRange -210 to 0 ; ErrRange 0.03 to -0.05 */
           poly1 = &Jpoly_VtoT[0];
        else if ( volts <= 42919 )     /* TempRange 0 to 760 ; ErrRange 0.04 to -0.04 */
           poly1 = &Jpoly_VtoT[1];
        else                           /* TempRange 760 to 1200 ; ErrRange 0.03 to -0.04 */
           poly1 = &Jpoly_VtoT[2];
        break ;


     case K_TYPE_TC:

        if ( volts <= 0 )              /* TempRange -200 to 0 ; ErrRange   0.04 to -0.02 */
           poly1 = &Kpoly_VtoT[0];
        else if ( volts <= 20644 )     /* TempRange 0 to 500 ; ErrRange 0.04 to -0.05 */
           poly1 = &Kpoly_VtoT[1];
        else                           /* TempRange 500 to 1372 ; ErrRange  0.06 to -0.05 */
           poly1 = &Kpoly_VtoT[2];
        break ;

     case N_TYPE_TC:

        if ( volts <= 0 )              /* TempRange -200 to 0 ; ErrRange   0.03 to -0.02 */
           poly1 = &Npoly_VtoT[0];
        else if ( volts <= 20613 )     /* TempRange 0 to 600 ; ErrRange 0.03 to -0.02 */
           poly1 = &Npoly_VtoT[1];
        else                           /* TempRange 600 to 1300 ; ErrRange  0.02 to -0.04 */
           poly1 = &Npoly_VtoT[2];
        break ;

     case T_TYPE_TC:

        if ( volts <= 0 )              /* TempRange -200 to 0 ; ErrRange 0.04 to -0.02 */
           poly1 = &Tpoly_VtoT[0];
        else                           /* TempRange 0 to 400 ; ErrRange  0.03 to -0.03 */
           poly1 = &Tpoly_VtoT[1];
        break ;

     case E_TYPE_TC:

        if ( volts <= 0 )              /* TempRange -200 to 0 ; ErrRange 0.03 to -0.01 */
           poly1 = &Epoly_VtoT[0];
        else                           /* TempRange 0 to 1000 ; ErrRange  0.02 to -0.02 */
           poly1 = &Epoly_VtoT[1];
        break ;

     case R_TYPE_TC:

        if ( volts <= 1923 )           /* TempRange -50 to 250 ; ErrRange   0.02 to -0.02 */
           poly1 = &Rpoly_VtoT[0];
        else if ( volts <= 13228 )     /* TempRange 250 to 1200 ; ErrRange 0.005 to -0.005 */
           poly1 = &Rpoly_VtoT[1];
        else if ( volts <= 19739 )     /* TempRange 1064 to 1664.5 ; ErrRange  0.001 to -0.0005 */
           poly1 = &Rpoly_VtoT[2];
        else                           /* TempRange 1664.5 to 1768.1 ; ErrRange 0.002 to -0.001 */
           poly1 = &Rpoly_VtoT[3];
        break ;

     case S_TYPE_TC:

        if ( volts <= 1874 )           /* TempRange -50 to 250 ; ErrRange   0.02 to -0.02 */
           poly1 = &Spoly_VtoT[0];
        else if ( volts <= 11950 )     /* TempRange 250 to 1200 ; ErrRange 0.005 to -0.005 */
            poly1 = &Spoly_VtoT[1];
        else if ( volts <= 17536 )     /* TempRange 1064 to 1664.5 ; ErrRange  0.001 to -0.0005 */
           poly1 = &Spoly_VtoT[2];
        else                           /* TempRange 1664.5 to 1768.1 ; ErrRange 0.002 to -0.002 */
           poly1 = &Spoly_VtoT[3];
        break ;

     default:
        return invalidTCtype;

  } /* end of switch() */

  compute_equation ( poly1, volts, temperature);

  return noErr;
}

/*************************************************************************
*   temp_to_volts
*
*    FUNCTION:
*    Converts a thermocouple temperature to an equivalent thermocouple voltage
*    using the NIST standards. Several equations characterize
*    a thermocouple type corresponding to different temperature ranges.
*    The equations are mostly Nth order polynomials.
*
*    INPUTS:
*     TCtype       : The thermocouple type (constant defines in convert.h).
*     temperature  : The temperature to be converted in degree Celsius.
*
*    OUTPUTS:
*     volts        : Call by reference parameter that returns the equivalent
*                    voltage. Returns an error if the thermocouple type is invalid.
*
*
**************************************************************************/

int temp_to_volts( TCtype, temperature, volts )


 int      TCtype;
 double   temperature;
 double   *volts;

{
  polynomial *poly1;

  switch ( TCtype)
  {
     case B_TYPE_TC:
        if ( temperature <= 630.615 )     /* TempRange 0 to 630.615 */
           poly1 = &Bpoly_TtoV[0];
        else                               /* TempRange 630.615 to 1820 */
           poly1 = &Bpoly_TtoV[1];
        break ;

     case J_TYPE_TC:
        if ( temperature <= 760 )         /* TempRange -210 to 760 */
           poly1 = &Jpoly_TtoV[0];
        else                              /* TempRange 760 to 1200 */
           poly1 = &Jpoly_TtoV[1];
        break ;

     case K_TYPE_TC:

        if ( temperature <= 0 )           /* TempRange -270 to 0 */
           poly1 = &Kpoly_TtoV[0];
        else                              /* TempRange 0 to 1372 */
           poly1 = &Kpoly_TtoV[1];
       break ;

     case N_TYPE_TC:

        if ( temperature <= 0 )           /* TempRange -270 to 0 */
           poly1 = &Npoly_TtoV[0];
        else                              /* TempRange 0 to 1300 */
           poly1 = &Npoly_TtoV[1];
       break ;

     case T_TYPE_TC:

        if ( temperature <= 0 )           /* TempRange -200 to 0 */
           poly1 = &Tpoly_TtoV[0];
        else                              /* TempRange 0 to 400 */
           poly1 = &Tpoly_TtoV[1];
        break ;

     case E_TYPE_TC:

        if ( temperature <= 0 )           /* TempRange -200 to 0 */
           poly1 = &Epoly_TtoV[0];
        else                              /* TempRange 0 to 1000 */
           poly1 = &Epoly_TtoV[1];
        break ;

     case R_TYPE_TC:

        if ( temperature <= 1064.18 )     /* TempRange -50 to 1064.18 */
           poly1 = &Rpoly_TtoV[0];
        else if ( temperature <= 1664.5 ) /* TempRange 1064.18 to 1664.5 */
           poly1 = &Rpoly_TtoV[1];
        else                              /* TempRange 1664.5 to 1768.1*/
           poly1 = &Rpoly_TtoV[2];
        break ;

     case S_TYPE_TC:

        if ( temperature <= 1064.18 )     /* TempRange -50 to 1064.18 */
           poly1 = &Spoly_TtoV[0];
        else if ( temperature <= 1664.5 ) /* TempRange 1064.18 to 1664.5 */
           poly1 = &Spoly_TtoV[1];
        else                              /* TempRange 1664.5 to 1768.1*/
           poly1 = &Spoly_TtoV[2];
        break ;

      default:
        return invalidTCtype;

  } /* end of switch() */

  compute_equation ( poly1, temperature, volts);

  /* return voltage in volts */
  *volts = *volts / 1.0E+6 ;

  return noErr;
}


/*************************************************************************
*
*  change_temp_scale
*
*
*    FUNCTION:
*    Converts a temperature in degree's centigrade into another valid
*    scale. The allowed units of temperature are KELVIN, FAHRENHEIT and RANKINE.
*
*
*    INPUTS:
*    TempScale   : The units to which the temperature is to be converted.
*    temperature : Temperature in degree's Celsius.
*
*
*    OUTPUTS:
*    temperature : The temperature in the new units.
*                  Returns an error if an invalid Temp_Scale is specified.
*
*
**************************************************************************/

int change_temp_scale  ( tempScale, temperature )

 int tempScale;
 double *temperature;  /* temperature input in degree's centigrade */

{

  switch ( tempScale )
  {

          case CELSIUS:
              *temperature = *temperature;
              break;
          case KELVIN:
              *temperature = *temperature + 273.15;
              break;
           case FAHRENHEIT:
              *temperature = *temperature * 1.8 + 32.0;
              break;
           case RANKINE:
              *temperature = *temperature * 1.8 + 491.67;
              break;
           default:
              return invalidTempScale;

  } /* end of switch() */

  return noErr;
}

/*************************************************************************
*
*  Thermocouple_Buf_Convert
*
*    Converts a buffer of thermocouple voltages to temperatures
*    using the formula appropriate for the given thermocouple type.
*
*      TCtype - the thermocouple type (constant defines in convert.h)
*     CJCtemp - temperature (in Celsius) to be used for cold-junction
*               compensation
*   tempScale - code for temperature scale (constant defines in convert.h)
*      numPts - number of points to convert
*    TCbuffer - buffer of thermocouple voltages in volts
*     TCtemps - return buffer of temperatures in Celsius
*
**************************************************************************/
int Thermocouple_Buf_Convert (TCtype, CJCtemp, tempScale, numPts, TCbuffer,
                              TCtemps)
int TCtype;
double CJCtemp;
int tempScale;
unsigned long numPts;
double TCbuffer[], TCtemps[];
{
   int err = noErr;  /* variable to keep track of error conditions */
   unsigned long i;
   double CJCvolts;

   /*
    * Convert the cold junction temperature into an thermocouple equivalent
    * voltage using the NIST standard.
    */

   err = temp_to_volts (TCtype, CJCtemp, &CJCvolts);
   if ( err == invalidTCtype ) return invalidTCtype;

   for ( i = 0; i < numPts ; i++)
   {
      err = volts_to_temp (TCtype, TCbuffer[i]+ CJCvolts, &TCtemps[i]);
      if ( err == invalidTCtype ) break;

      err = change_temp_scale (tempScale, &TCtemps[i]);
      if ( err == invalidTempScale ) break;

    } /* end of for (i =0; ...) */

    return err;
}


/* -------------------- *\
 * --- Strain Gauge --- *
\* -------------------- */
/*************************************************************************
*
*  Strain_Convert
*
*      Uses the Strain_Buf_Convert function to convert a strain
*      gauge voltage to strain.
*
**************************************************************************/
int Strain_Convert (bridgeConfig, Vex, GF, v, Rg, Rl, Vinit, strainVolts,
                    strainVal)
int bridgeConfig;
double Vex, GF, v, Rg, Rl, Vinit, strainVolts, *strainVal;
{
   *strainVal = 0.0;
   return Strain_Buf_Convert (bridgeConfig, Vex, GF, v, Rg, Rl, Vinit, 1L,
                              &strainVolts, strainVal);
}

/*************************************************************************
*
*  Strain_Buf_Convert
*
*    Converts a buffer of strain gauge voltages to strain values using
*    the formula appropriate for the given bridge configuration.
*
*    bridgeConfig - code indicating the strain gauge bridge configuration
*                   used (constant defines given in convert.h)
*             Vex - excitation voltage used
*              GF - gauge factor
*               v - Poisson's ratio (if applicable)
*              Rg - strain gauge nominal value
*              Rl - lead resistance
*           Vinit - initial unstrained voltage of the strain gauge after
*                   mounting in the configuration
*          numPts - number of data points to be converted
*      voltBuffer - buffer of strain gauge voltages to be converted
*      strainVals - return buffer of strain values
*
**************************************************************************/
int Strain_Buf_Convert (bridgeConfig, Vex, GF, v, Rg, Rl, Vinit, numPts,
                           voltBuffer, strainVals)
int bridgeConfig;
double Vex, GF, v, Rg, Rl, Vinit;
unsigned long numPts;
double voltBuffer[], strainVals[];
{
   int i;
   double Vr, temp1, temp2;

   /* default excitation voltage is 3.333V */
   if (Vex == 0.0) Vex = 3.333;

   if (GF == 0) return divideByZero;

   /*-------------------------------*\
   |  for all bridge configurations: |
   |     Vr = (V - Vinit) / Vex      |
   \*-------------------------------*/
   switch (bridgeConfig)
   {
      case QTR_BRIDGE_I:
      case QTR_BRIDGE_II:
         /*-------------------------------------------*\
         |  strain = -4Vr / [GF(1+2Vr)] * (1 + Rl/Rg)  |
         \*-------------------------------------------*/
         if (Rg == 0) return divideByZero;
         temp1 = -4.0 / GF * (1.0 + Rl/Rg);
         for (i = 0; i < numPts; i++)
         {
            Vr = (voltBuffer[i] - Vinit) / Vex;
            temp2 = 1.0 + 2.0 * Vr;
            if (temp2 == 0) return divideByZero;
            strainVals[i] = Vr / temp2 * temp1;
         }
         break;

      case HALF_BRIDGE_I:
         /*----------------------------------------------------*\
         |  strain = -4Vr / [GF[(1+v)-2Vr(v-1)]] * (1 + Rl/Rg)  |
         \*----------------------------------------------------*/
         if (Rg == 0) return divideByZero;
         temp1 = -4.0 / GF * (1.0 + Rl/Rg);
         for (i = 0; i < numPts; i++)
         {
            Vr = (voltBuffer[i] - Vinit) / Vex;
            temp2 = (1.0 + v) - 2.0 * Vr * (v - 1.0);
            if (temp2 == 0) return divideByZero;
            strainVals[i] = Vr / temp2 * temp1;
         }
         break;

      case HALF_BRIDGE_II:
         /*----------------------------------*\
         |  strain = -2Vr / GF * (1 + Rl/Rg)  |
         \*----------------------------------*/
         if (Rg == 0) return divideByZero;
         temp1 = -2.0 / GF * (1.0 + Rl/Rg);
         for (i = 0; i < numPts; i++)
         {
            Vr = (voltBuffer[i] - Vinit) / Vex;
            strainVals[i] = Vr * temp1;
         }
         break;

      case FULL_BRIDGE_I:
         /*-------------------*\
         |  strain = -Vr / GF  |
         \*-------------------*/
         for (i = 0; i < numPts; i++)
         {
            Vr = (voltBuffer[i] - Vinit) / Vex;
            strainVals[i] = -1.0 * Vr / GF;
         }
         break;

      case FULL_BRIDGE_II:
         /*-------------------------*\
         |  strain = -2Vr / GF(v+1)  |
         \*-------------------------*/
         if (v == -1.0) return divideByZero;
         temp1 = -2.0 / GF * (1.0 + v);
         for (i = 0; i < numPts; i++)
         {
            Vr = (voltBuffer[i] - Vinit) / Vex;
            strainVals[i] = Vr * temp1;
         }
         break;

      case FULL_BRIDGE_III:
         /*-------------------------------------*\
         |  strain = -2Vr / [GF[(v+1)-Vr(v-1)]]  |
         \*-------------------------------------*/
         temp1 = -2.0 / GF;
         for (i = 0; i < numPts; i++)
         {
            Vr = (voltBuffer[i] - Vinit) / Vex;
            temp2 = (v + 1.0) - Vr * (v - 1.0);
            if (temp2 == 0) return divideByZero;
            strainVals[i] = Vr / temp2 * temp1;
         }
         break;

      case USER_DEFINED:
         /*
         put your user defined formula here:
         for (i = 0; i < numPts; i++)
         {
            strainVals[i] = ..... ;
         }
         */
         break;

      default:
         return invalidBridgeConfig;
   }
   return noErr;
}

/* ----------- *\
 * --- RTD --- *
\* ----------- */

/*************************************************************************
*
*  RTD_Convert
*
*      Uses the RTD_Buf_Convert function to convert a voltage
*      to RTD resistance and then to a temperature
*
**************************************************************************/
int RTD_Convert (convType, Iex, Ro, A, B, tempScale, RTDvolts, RTDtemp)
int convType;
double Iex, Ro, A, B;
int tempScale;
double RTDvolts, *RTDtemp;
{
   *RTDtemp = 0.0;
   return RTD_Buf_Convert (convType, Iex, Ro, A, B, tempScale, 1L, &RTDvolts, RTDtemp);
}


/*************************************************************************
*
*  RTD_Buf_Convert
*
*      Uses either the formula given below or a user defined formula
*      to convert a buffer of RTD voltages to resistances and then
*      to temperatures.
*
*      A, B, and C are coefficients for the Callendar-Van Dusen
*      equation for platinum RTD's that gives RTD resistance as a
*      function of temperature:
*
*          Rt = Ro[1 + At + Bt^2 + C(t-100)t^3]
*          where
*          Ro = RTD resistance at 0C
*          A = alpha(1 + delta/100)
*          B = -alpha*delta/100^2
*          C = -alpha*beta/100^4
*
*      For temperatures above 0C, beta = 0; so the equation reduces to a
*      quadratic formula for which we have found the appropriate root:
*      (so this function is only for temperatures above 0C)
*
*          Rt = Ro[1 + At + Bt^2]
*
*      All the RTD documentation we have found either gives values for
*      A and B, or for alpha and delta, in which case A and B can be
*      calculated from the equations given above.  The user must be careful
*      that the A and B that are passed to the function do not already
*      incorporate Ro.
*
*      The most common RTDs are 100 ohm platinum RTDs that follow the
*      European Curve (DIN 43760) or the American Curve.  The A and B
*      coefficients for those two types are given below:
*
*      European (DIN 43760):
*      A = 3.90802e-03, B = -5.80195e-07  (alpha = 0.00385, delta = 1.492)
*
*      American:
*      A = 3.9784e-03, B = -5.8408e-07  (alpha = 0.00392, delta = 1.492)
*
*      The four wire measurement technique should be used for best
*      results.  In this techique, the RTD resistance can be calculated very
*      easily from the measured voltage and the excitation current:
*
*          Rt = RTDvolts / Iex
*
*      Function parameters:
*      convType - conversion to be used (constant defines given in convert.h)
*           Iex - excitation current used
*            Ro - RTD resistance in ohms at 0C
*         A & B - coefficients to the Callendar Van-Dusen equation above
*     tempScale - temperature scale to use (constant defines in convert.h)
*        numPts - number of data points to convert
*    voltBuffer - buffer of voltages to convert
*      RTDtemps - return buffer of temperatures
*
**************************************************************************/
int RTD_Buf_Convert (convType, Iex, Ro, A, B, tempScale, numPts, voltBuffer, RTDtemps)
int convType;
double Iex, Ro, A, B;
int tempScale;
unsigned long numPts;
double voltBuffer[], RTDtemps[];
{
   int i;
   double a, b, c, q, Rt, argu;

   /* default excitation current is 150 microAmps */
   if (Iex == 0.0) Iex = 150e-6;

   switch (convType)
   {
      case DEFAULT_RTD_CONV:
         /*----------------------------------------------------------*\
         | a, b, and c are the coefficients for the ax^2 + bx + c = 0 |
         | form of the equation:  Rt = Ro[1 + At + Bt^2]              |
         \*----------------------------------------------------------*/
         a = Ro * B;
         b = Ro * A;
         for (i = 0; i < numPts; i++)
         {
            /* convert voltage to RTD resistance */
            Rt = voltBuffer[i] / Iex;
            c = Ro - Rt;
            /* method taken from Numerical Recipes; there is another root
               (q/a), but it always comes out an unreasonable number */
			   argu = b * b - 4.0 * a * c;
            if (argu < 0)
               return imaginaryNumber;
            q = -0.5 * (b + sqrt(argu));
            switch (tempScale) {
               case CELSIUS:
                  RTDtemps[i] = (c / q);
                  break;
               case KELVIN:
                  RTDtemps[i] = (c / q) + 273.15;
                  break;
               case FAHRENHEIT:
                  RTDtemps[i] = (c / q) * 1.8 + 32.0;
                  break;
               case RANKINE:
                  RTDtemps[i] = (c / q) * 1.8 + 491.67;
                  break;
               default:
                  return invalidTempScale;
            }
         }
         break;

      case USER_DEFINED:
         /*
         put your user defined formula here:
         for (i = 0; i < numPts; i++)
         {
            RTDtemps[i] = ..... ;
         }
         */
         break;

      default:
         return invalidRTDconv;

   }  /* end switch */

   return noErr;
}



/* ------------------ *\
 * --- Thermistor --- *
\* ------------------ */
/*************************************************************************
*  Thermistor_Convert
*
*      Uses the Thermistor_Buf_Convert function to convert a voltage
*      to thermistor resistance and then to a temperature.
*
**************************************************************************/
int Thermistor_Convert (R1, Vref, Volts, TempScale, Temperature)
double R1, Vref, Volts, *Temperature;
int TempScale;
{
   *Temperature = 0.0;
   return Thermistor_Buf_Convert (R1, Vref, 1L, &Volts, TempScale, Temperature);
}

/*************************************************************************
*  Thermistor_Buf_Convert
*
*      Uses the formula given below
*      to convert a buffer of Thermistor voltages to resistances and then
*      calls the function Rt_to_Temp to convert them to temperatures. Use
*      this function when the thermistor is connected to a voltage
*      reference (Vref) with a series resistor (R1). Volts is the voltage
*      across the thermistor.
*
*          Rt = R1 * (Volts / (Vref- Volts))
*          where
*          Rt = Themistor resistance in ohms
*
**************************************************************************/
int Thermistor_Buf_Convert (R1, Vref, numPts, VoltBuf, TempScale, TempBuf)
unsigned long numPts;
double R1, Vref, VoltBuf[], TempBuf[];
int TempScale;
{
   unsigned long i;
   double Rt;
   int err;

   for (i = 0L; i < numPts; i++)
   {
      if (Vref == VoltBuf[i]) return divideByZero;
      Rt = R1 * (VoltBuf[i] / (Vref - VoltBuf[i]));

      err = Rt_to_Temp(Rt, &TempBuf[i], TempScale);
      if (err)
	      return err;
   }
   return noErr;
}
/*************************************************************************
 *  Thermistor_CrntSrc_Convert
 *
 *      Uses the Thermistor_CrntSrc_Buf_Convert function to convert a voltage
 *      to thermistor resistance and then to a temperature. This version is
 *      used for current excited thermistor
 *
 **************************************************************************/
int Thermistor_CrntSrc_Convert (double Iex, double Volts,
				int TempScale, double *Temperature)
{
  *Temperature = 0.0;
  return Thermistor_CrntSrc_Buf_Convert (Iex, 1L, &Volts, TempScale, Temperature);
}

/*************************************************************************
 *  Thermistor_CrntSrc_Buf_Convert
 *
 *      Uses the formula given below
 *      to convert a buffer of Thermistor voltages to resistances and then
 *      calls the function Rt_to_Temp to convert them to temperatures. Use
 *      this function when the thermistor is connected to a current source
 *      of Iex Amps. Volts is the voltage across the thermistor.
 *
 *          Rt = Volts / Current
 *          where
 *          Rt = Themistor resistance in ohms
 *
 **************************************************************************/
int Thermistor_CrntSrc_Buf_Convert (double Iex, unsigned long numPts,
				    double VoltBuf[], int TempScale, double TempBuf[])
{
  unsigned long i;
  double Rt;
  int err;

  for (i = 0L; i < numPts; i++)
    {
      if (Iex == 0.0) return divideByZero;
      Rt = VoltBuf[i] / Iex;

      err = Rt_to_Temp(Rt, &TempBuf[i], TempScale);
      if (err)
	{
	  return err;
	}

    }

  return noErr;
}

/*************************************************************************
 *  Rt_to_Temp
 *
 *      Uses the formula given below
 *      to convert a thermistor resistance to temperature in Kelvin
 *      then converts this temparature to the required scale.
 *
 *      a,b,c are coefficients taken from a precision thermistor handbook
 *
 *          a = 1.295361e-3
 *          b = 2.343159e-4
 *          c = 1.018703e-7
 *
 *      1/Tk = a + b*ln(Rt) + c*ln(Rt)^3
 *
 *      Tk is the temperature reading in Kelvin
 *
 **************************************************************************/
int Rt_to_Temp(double Rt, double *temp, int TempScale)
{
  double logRt, logRtCubed;
  double a, b, c;

  a = 1.295361e-3 ;
  b = 2.343159e-4 ;
  c = 1.018703e-7 ;

  if (Rt <= 0.0) return imaginaryNumber;
  logRt = log (Rt);
  logRtCubed = logRt * logRt * logRt;
  *temp = 1 / (a + b * logRt + c * logRtCubed);

  /* Convert the temperature(s) from degrees Kelvin to ... */
  switch (TempScale)
    {
    case CELSIUS:
      *temp -= 273.15;
      break;
    case KELVIN:
      break;
    case FAHRENHEIT:
      *temp = 1.8 * (*temp) - 459.67;
      break;
    case RANKINE:
      (*temp) *= 1.8;
      break;
    default:
      return invalidTempScale;
    }

  return noErr;
}
