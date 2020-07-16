/********************************* convert.h *******************************
*
*   Constant definitions used by the conversion functions
*
*   Function prototypes for the conversion functions
*
***************************************************************************/
#ifndef __convert_h__
#define __convert_h__

/* error codes */
#define  noErr                 0
#define  invalidTCtype        -1
#define  invalidTempScale     -2
#define  invalidBridgeConfig  -3
#define  invalidRTDconv       -4
#define  divideByZero         -5
#define  imaginaryNumber      -6

/* not deleted for compatibility with
   versions of NIDAQ 4.6.1 and before */
#define  E  1
#define  J  2
#define  K  3
#define  R  4
#define  S  5
#define  T  6

/* thermocouple types */
#define  B_TYPE_TC  7
#define  E_TYPE_TC  1
#define  J_TYPE_TC  2
#define  K_TYPE_TC  3
#define  N_TYPE_TC  8
#define  R_TYPE_TC  4
#define  S_TYPE_TC  5
#define  T_TYPE_TC  6


/* temperature scales */
#define  CELSIUS     1
#define  FAHRENHEIT  2
#define  KELVIN      3
#define  RANKINE     4

/* strain gauge bridge configurations */
#define  QTR_BRIDGE_I    1
#define  QTR_BRIDGE_II   2
#define  HALF_BRIDGE_I   3
#define  HALF_BRIDGE_II  4
#define  FULL_BRIDGE_I   5
#define  FULL_BRIDGE_II  6
#define  FULL_BRIDGE_III 7

/* RTD conversion type */
#define  DEFAULT_RTD_CONV  0

#define  USER_DEFINED      -1  /* can be used for user-defined conversion */

/* function prototypes */

int Thermocouple_Convert (int TCtype, double CJCtemp, int tempScale,
                     double TCvolts, double *TCtemp);
int Thermocouple_Buf_Convert (int TCtype, double CJCtemp, int tempScale,
                     unsigned long numPts, double TCbuffer[], double TCtemps[]);

int RTD_Convert (int convType, double Iex, double Ro, double A, double B,
                     int tempScale, double RTDvolts, double *RTDtemp);
int RTD_Buf_Convert (int convType, double Iex, double Ro, double A, double B,
                     int tempScale, unsigned long numPts, double RTDvolts[],
                     double RTDtemps[]);

int Strain_Convert (int bridgeConfig, double Vex, double GF, double v,
                     double Rg, double Rl, double Vinit, double strainVolts,
                     double *strainVal);
int Strain_Buf_Convert (int bridgeConfig, double Vex, double GF, double v,
                     double Rg, double Rl, double Vinit, unsigned long numPts,
                     double voltBuffer[], double strainVals[]);

int Thermistor_Convert (double R1, double Vref, double Volts, int TempScale,
                     double *Temperature);
int Thermistor_Buf_Convert (double R1, double Vref, unsigned long numPts,
                     double VoltBuf[], int TempScale, double Tempbuf[]);
int Thermistor_CrntSrc_Convert (double Iex, double Volts,
                     int TempScale, double *Temperature);
int Thermistor_CrntSrc_Buf_Convert (double Iex, unsigned long numPts,
                     double VoltBuf[], int TempScale, double TempBuf[]);

int Rt_to_Temp(double Rt, double *temp, int TempScale);

#endif /* __convert_h__ */
