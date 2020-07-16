////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Creates and initializes a QApplication object.
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
////////////////////////////////////////////////////////////////////////////////
#include "QtMain.h"

#if USE_QT
#include <QApplication>
#include <QIcon>
#include <QPixmap>
#include "Resources.h"

#if HAVE_LIB_TINY
# include "FileUtils.h"
# include "ExceptionCatcher.h"
#endif

#if _WIN32
# include <Windows.h>
#endif

struct QtApplication::Private
{
  Private( int&, char** );
  int Run();
  QApplication mApp;
};

QtApplication::Private::Private( int& argc, char** argv )
: mApp( argc, argv )
{
  mApp.setOrganizationName( PROJECT_NAME );
  mApp.setOrganizationDomain( PROJECT_DOMAIN );
#if HAVE_LIB_TINY
  mApp.setApplicationName( QString::fromLocal8Bit( FileUtils::ApplicationTitle().c_str() ) );
#endif

  QIcon icon;
  QPixmap img;

#if _WIN32
  HRSRC h = ::FindResourceA( 0, MAKEINTRESOURCE(1), RT_GROUP_ICON );
  HGLOBAL h2 = h ? ::LoadResource( 0, h ) : 0;
  void* pIcons = h2 ? ::LockResource( h2 ) : nullptr;
  if( pIcons )
  {
    int id = ::LookupIconIdFromDirectoryEx( static_cast<BYTE*>( pIcons ), TRUE, 256, 256, LR_DEFAULTCOLOR );
    if( id )
    {
      HRSRC h = ::FindResourceA( 0, MAKEINTRESOURCE(id), RT_ICON );
      HGLOBAL h2 = h ? ::LoadResource( 0, h ) : 0;
      void* pIcon = h2 ? ::LockResource( h2 ) : nullptr;
      if( pIcon && img.loadFromData( static_cast<const uchar*>( pIcon ), ::SizeofResource( 0, h ) ) )
        icon.addPixmap( img.scaledToWidth( 256, Qt::SmoothTransformation ) );
    }
  }
#else

#define LOAD_ICON(x) \
  if( img.loadFromData( reinterpret_cast<const uchar*>( Resources::AppIcon##x.data ), \
    Resources::AppIcon##x.length, Resources::AppIcon##x.type ) ) icon.addPixmap( img );

#ifdef IS_RESOURCE_AppIcon256
# define SCALE_ICON(x) \
  icon.addPixmap( img.scaledToWidth( (x), Qt::SmoothTransformation ) );
#else
# define SCALE_ICON(x)
#endif

#ifdef IS_RESOURCE_AppIcon256
  LOAD_ICON( 256 );
#endif

#ifdef IS_RESOURCE_AppIcon64
  LOAD_ICON( 64 );
#else
  SCALE_ICON( 64 );
#endif

#ifdef IS_RESOURCE_AppIcon48
  LOAD_ICON( 48 );
#else
  SCALE_ICON( 48 );
#endif

#ifdef IS_RESOURCE_AppIcon32
  LOAD_ICON( 32 );
#else
  SCALE_ICON( 32 );
#endif

#ifdef IS_RESOURCE_AppIcon16
  LOAD_ICON( 16 );
#else
  SCALE_ICON( 16 );
#endif

#endif // _WIN32

  mApp.setWindowIcon( icon );
}

#undef LOAD_ICON
#undef SCALE_ICON

QtApplication::QtApplication( int& argc, char** argv )
: p( nullptr )
{
  // Unless linked statically, Qt does runtime loading of its platform support DLL from
  // a "platforms" directory inside one of its QCoreApplication::libraryPaths().
  // In order to detect deployment issues early we force Qt to only search our own
  // library path, relative to the executable file that contains the code for QApplication.
  auto paths = QCoreApplication::libraryPaths();
  for( const auto& path : paths )
    QCoreApplication::removeLibraryPath( path );
#if _WIN32
  DWORD flags = GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS;
  HMODULE hModule = NULL;
  BOOL ok = ::GetModuleHandleExW( flags, LPWSTR(&QApplication::exec), &hModule );
  wchar_t name[MAX_PATH + 3];
  if( ok )
    ok = ::GetModuleFileNameW( hModule, name, sizeof(name)/sizeof(*name) );
  Assert( ok );
  if( ok )
  {
    wchar_t* p = name + ::wcslen( name );
    while( p > name && *(p-1) != L'\\' && *(p-1) != L'/' )
      --p;
    ::wcscpy( p, L"qtplugins" );
    QCoreApplication::addLibraryPath( QString::fromWCharArray( name ) );
  }
#endif
  QCoreApplication::addLibraryPath((FileUtils::ExecutablePath() + "/qtplugins").c_str());
  QApplication::setStyle("fusion"); // avoid issues with native vs nonnative controls on Windows 10
  p = new Private( argc, argv );
}

QtApplication::~QtApplication()
{
  delete p;
}

int
QtApplication::Run()
{
#if HAVE_LIB_TINY
  FunctionCall<int()> call( &QApplication::exec );
  bool finished = ExceptionCatcher()
    .SetMessage( "Aborting " + FileUtils::ApplicationTitle() )
    .Run( call );
  return finished ? call.Result() : -1;
#else
  return QApplication::exec();
#endif
}

#endif // USE_QT