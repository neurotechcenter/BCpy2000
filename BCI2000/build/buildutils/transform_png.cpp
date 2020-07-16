////////////////////////////////////////////////////////////////////////////////
// $id$
// Author: mellinger@neurotechcenter.org
// Description: A utility that resamples .png bitmaps and applies color
//   transformations.
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <lodepng.h>
#include "ArithmeticExpression.h"
#include "Color.h"
#include "Debugging.h"

typedef std::vector<unsigned char> ImageData;

void ResampleImage( const ImageData& input, int w1, int h1, ImageData& output, int w2, int h2 )
{ // Linear interpolation resampling.
  if( w2 == w1 && h2 == h1 )
  {
    output = input;
    return;
  }
  // For downsampling, the result will be close to artifact-free because rectangular average has a sinc
  // spectrum with its first zero located at the new Nyquist frequency.
  // In case of upsampling, artifacts may be reduced by applying a [1,2,1]/4 fir filter prior to
  // resampling (which has a raised cosine spectrum, going smoothly to zero at the Nyquist frequency).

  // resample width
  int n11 = input.size() / 4, n21 = n11 * w2 / w1, n22 = n21 * h2 / h1;
  std::vector<float> tmp21( 4 * ( n21 + 1/*avoid right edge test*/), 0 );
  for( int x1 = 0; x1 < w1; ++x1 )
  {
    float x2Left = 1.f * x1 * w2 / w1,
          x2Right = 1.f * (x1+1) * w2 / w1,
          x2LeftFrac = ::fmodf( x2Left, 1.f ),
          x2RightFrac = ::fmodf( x2Right, 1.f );
    int x2LeftFloor = ::floor( x2Left ),
        x2RightFloor = ::floor( x2Right );
    for( int y1 = 0; y1 < h1; ++y1 )
    {
      for( int c = 0; c < 4; ++c )
      {
        float val = input[4*(x1+y1*w1)+c];
        tmp21[4*(x2LeftFloor+y1*w2)+c] -= val * x2LeftFrac;
        tmp21[4*(x2RightFloor+y1*w2)+c] += val * x2RightFrac;
        for( int x2 = x2LeftFloor; x2 < x2RightFloor; ++x2 )
          tmp21[4*(x2+y1*w2)+c] += val;
      }
    }
  }
  // resample height
  std::vector<float> tmp22( (n22 + w2/*avoid bottom edge test*/) * 4, 0 );
  for( int y1 = 0; y1 < h1; ++y1 )
  {
    float y2Top = 1.f * y1 * h2 / h1,
          y2Bot = 1.f * (y1+1) * h2 / h1,
          y2TopFrac = ::fmodf( y2Top, 1.f ),
          y2BotFrac = ::fmodf( y2Bot, 1.f );
    int y2TopFloor = ::floor( y2Top ),
        y2BotFloor = ::floor( y2Bot );
    for( int x2 = 0; x2 < w2; ++x2 )
    {
      for( int c = 0; c < 4; ++c )
      {
        float val = tmp21[4*(x2+y1*w2)+c];
        tmp22[4*(x2+y2TopFloor*w2)+c] -= val * y2TopFrac;
        tmp22[4*(x2+y2BotFloor*w2)+c]  += val * y2BotFrac;
        for( int y2 = y2TopFloor; y2 < y2BotFloor; ++y2 )
          tmp22[4*(x2+y2*w2)+c] += val;
      }
    }
  }
  // clamp and convert result
  output.resize( n22 * 4 );
  auto i = tmp22.begin();
  for( auto j = output.begin(); j != output.end(); ++j )
  {
    float f = *i++;
    f = std::max( 0.f, f );
    f = std::min( f, 255.f );
    *j = ::floor( f );
  }
}

int
main( int argc, char* argv[] )
{
  MemoryDebugging::Init init_; // avoid stalled debug builds when memory debugging is enabled
  bool usage = false, rgb = false;
  float wfactor = 1, hfactor = 1;
  int width2 = 0, height2 = 0;
  std::string input, output;
  std::vector<ArithmeticExpression> init, process;

  for( int i = 1; i < argc; ++i )
  {
    std::string arg = argv[i];
    if( arg.find( "--init=" ) == 0 )
      init.push_back( arg.substr( 7 ) );
    else if( arg.find( "--process=" ) == 0 )
      process.push_back( arg.substr( 10 ) );
    else if( arg.find( "--width=" ) == 0 )
      width2 = ::atoi( arg.substr( 8 ).c_str() );
    else if( arg.find( "--height=" ) == 0 )
      height2 = ::atoi( arg.substr( 9 ).c_str() );
    else if( arg.find( "--width*=" ) == 0 )
      wfactor = ::atof( arg.substr( 9 ).c_str() );
    else if( arg.find( "--height*=" ) == 0 )
      hfactor = ::atof( arg.substr( 10 ).c_str() );
    else if( arg == "--rgb" )
      rgb = true;
    else if( arg == "--hsv" )
      rgb = false;
    else if( arg == "--no-input" )
      input = arg;
    else if( input.empty() )
      input = arg;
    else if( output.empty() )
      output = arg;
    else
    {
      std::cerr << "Unexpected argument: " << arg << std::endl;
      usage = true;
    }
  }
  
  if( input == "--no-input" && (width2 < 1 || height2 < 1) )
  {
    std::cerr << "With --no-input, --width and --height must be given as well." << std::endl;
    usage = true;
  }
  if( input.empty() || output.empty() || usage )
  {
    std::cerr << "Usage: transform_png [options] (--no-input|<input file>) <output file>\n"
              << " Options are: --width=<width> --height=<height>\n"
              << "  --init=<expression> --process=<expression>\n"
              << "  (--rgb|--hsv) selects color space\n"
              << std::endl;
    return -1;
  }

  ImageData image1, image2;
  unsigned width1, height1;
  if( input == "--no-input" )
  {
    width1 = width2;
    height1 = height2;
    image1.resize( width1 * height1 * 4, 0 );
  }
  else
  {
    unsigned error = lodepng::decode( image1, width1, height1, input, LCT_RGBA, 8 );
    if( error )
    {
      std::cerr << "Error reading from " << input << ":\n" << lodepng_error_text( error ) << " (" << error << ")" << std::endl;
      return error;
    }
  }
  if( width2 == 0 )
    width2 = width1;
  if( height2 == 0 )
    height2 = height1;
  width2 *= wfactor;
  height2 *= hfactor;
  ResampleImage( image1, width1, height1, image2, width2, height2 );

  ArithmeticExpression::VariableContainer vars, consts = ArithmeticExpression::Constants;
  consts["Width"] = width2, consts["Height"] = height2;
  for( auto& expr : init )
    usage = !expr.Compile( ArithmeticExpression::Context( vars, consts ) ) || usage;
  double* X = &vars["X"], *Y = &vars["Y"], *A = &vars["A"],
    *R = nullptr, *G = nullptr, *B = nullptr,
    *H = nullptr, *S = nullptr, *V = nullptr;
  if( rgb )
    R = &vars["R"], G = &vars["G"], B = &vars["B"];
  else
    H = &vars["H"], S = &vars["S"], V = &vars["V"];
  for( auto& expr : process )
    usage = !expr.Compile( ArithmeticExpression::Context( vars, consts ) ) || usage;
  if( usage )
    return -2;

  for( auto& expr : init )
    expr.Evaluate();
  for( auto& expr : process )
    for( int x = 0; x < width2; ++x )
      for( int y = 0; y < height2; ++y )
      {
        *X = x, *Y = y;
        unsigned char* p = &image2[4*(x+width2*y)];
        int r = *p++, g = *p++, b = *p++;
        *A = *p++ / 255.;
        p -= 4;
        if( rgb )
          *R = r / 255., *G = g / 255., *B = b / 255.;
        else
          RGBColor( r, g, b ).ToHsv360( *H, *S, *V );
        expr.Evaluate();
        if( rgb )
        {
          double c[] = { *R, *G, *B, *A };
          for( auto f : c )
          {
            f = std::max( 0., std::min( 1., f ) );
            *p++ = ::floor( f * 255. );
          }
        }
        else
        {
          *S = std::max( 0., std::min( 1., *S ) );
          *V = std::max( 0., std::min( 1., *V ) );
          RGBColor c = RGBColor::FromHsv360( *H, *S, *V );
          *p++ = c.R(), *p++ = c.G(), *p++ = c.B();
          *A = std::max( 0., std::min( 1., *A ) );
          *p++ = ::floor( *A * 255. );
        }
      }

  unsigned error = lodepng::encode( output, image2, width2, height2, LCT_RGBA, 8 );
  if( error )
  {
    std::cerr << "Output error " << error << ": " << lodepng_error_text( error ) << std::endl;
    return error;
  }
  return 0;
}

