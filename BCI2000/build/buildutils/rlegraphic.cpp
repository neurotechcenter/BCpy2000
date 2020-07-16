////////////////////////////////////////////////////////////////////
// $Id: rlegraphic.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Author:      juergen.mellinger@uni-tuebingen.de
// Description: A program that converts graphics files into rle-
//   encoded C++ resource files.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <vcl.h>

int ToGray( int c )
{
  int gray = ( c & 0xff ) + ( ( c >> 8 ) & 0xff ) + ( ( c >> 16 ) & 0xff );
  return gray / 3;
}

int
main( int argc, char** argv )
{
  const char* usage = "Usage: rlegraphic <option> <filename>\n"
                      " Options are: --help Show this help\n";

  const char* inputFile = NULL;
  bool showUsage = argc < 2;
  for( int i = 1; i < argc; ++i )
  {
    if( string( "--help" ) == argv[ i ] )
      showUsage = true;
    else
      inputFile = argv[ i ];
  }
  if( showUsage )
  {
    cout << usage;
    return -1;
  }
  TPicture* pPicture = NULL;
  Graphics::TBitmap*  pBitmap = NULL;
  TCanvas*  pCanvas = NULL;
  try
  {
    pPicture = new TPicture;
    pPicture->LoadFromFile( inputFile );
    pBitmap = new Graphics::TBitmap;
    pBitmap->Width = pPicture->Width;
    pBitmap->Height = pPicture->Height;
    pCanvas = pBitmap->Canvas;
    pCanvas->Draw( 0, 0, pPicture->Graphic );
  }
  catch( ... )
  {
    cerr << "Could not load " << inputFile << " as a graphic file." << endl;
    return -1;
  }
  string fileName = ChangeFileExt( ExtractFileName( inputFile ), "" ).c_str();
  cout << hex << "namespace Resources {\n"
          " static struct\n"
          " { // run-length encoded bitmap data\n"
          "  unsigned char color;\n"
          "  unsigned short count;\n"
          " }\n"
          " " << fileName << "[] =\n"
          " {\n"
          "  {0,0x" << pBitmap->Width << "}, // width\n"
          "  {0,0x" << pBitmap->Height << "}, // height\n";
  int color = ToGray( pCanvas->Pixels[0][0] ),
      count = 0;
  for( int y = 0; y < pPicture->Height; ++y )
    for( int x = 0; x < pPicture->Width; ++x )
    {
      if( ToGray( pCanvas->Pixels[x][y] ) != color )
      {
        cout << "  {0x" << color << ",0x" << count << "},\n";
        color = ToGray( pCanvas->Pixels[x][y] );
        count = 0;
      }
      ++count;
    }
  if( count > 0 )
    cout << "  {0x" << color << ",0x" << count << "},\n";
  cout << "  {0x0,0x0} // terminating entry\n"
          " };\n"
          "} // namespace Resources\n";
  delete pBitmap;
  delete pPicture;
  return 0;
}

