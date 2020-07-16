#ifndef _VHT_HAPTIC_EFFECT_
#define _VHT_HAPTIC_EFFECT_

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

#define VHS_INFINITE_TIME   1e40       // gravitational force and strong force ratio

//: A haptic effect parameter descriptor.
// Store values for the seven parameters of the haptic effect equation.  The
// equation is:
// a*( b+cos(2PI*omega*t)*(c+d*exp(-kappa*(t-beta)^2)))
// vhtHapticEffect also configures the effect parameters to generate the follofing 
// preset effects
//    - pulse
//    - jolt
//    - sinWave
//    - sinWaveAM
//!vhtmodule: Device
class VH_DEVICE_DLL_EXPORT vhtHapticEffect
{
  public:
    double  a, b, c, d, omega, kappa, beta, period, duration;

  public:
    vhtHapticEffect(void);
    //: Constructor.
    void pulse(double height, double width, double delay, double T, int cnt = 1);
    //: pulse configures the effect parameters to generate a Gaussian pulse 
    //!param: height - Pulse height [0..1].
    //!param: width - Pulse width (s).
    //!param: delay - Pulse delay (s).
    //!param: T - Period (s).
    //!param: cnt - Number of counts.
    // with center at delay.
    // The pulse is repited with period T. 
    // If p = 0 pulse generates a constant effect of value = height
    // cnt is the number of pulses to be generated
    // height goes from 0 (no force) to 1 (maximun force)
    // The maximun height is 1 coresponding to maximun force.
    // width, delay and T (period) are specified in seconds.
    // width is delta-time corresponding to points at height/2 (gaussian width)


    void jolt(double amplitude, double width, int cnt = 1);
    //: single pulse at t = 0
    //!param: amplitude - Jolt amplitude [0..1].
    //!param: width - Jolt width.
    //!param: cnt - Number of counts.

    void sinWave(double amplitude, double frequency, double durat = VHS_INFINITE_TIME );
    //: sineWave generate effect parameters for a simple sin function
    //!param: amplitude - Wave amplitude [0..1].
    //!param: frequency - Frequency of wave (Hz).
    //!param: durat - Effect duration (s).
    // amplitud goes from 0 (no force) to 1 (maximun force)
    // frequency is in Hz

    void sinWaveAM(double amplitude, double width, double T, double frequency, double durat = VHS_INFINITE_TIME);
    //: sinWave generates a sin vawe modulated by a gaussian.
    //!param: amplitude - Wave amplitude [0..1].
    //!param: frequency - Frequency of wave (Hz).
    //!param: T - Duration.
    //!param: durat - Effect duration (s).
    // the effect repeates with a period T
    // the units are the same as in pulse and sinWave.

};

#endif


