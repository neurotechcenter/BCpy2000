////////////////////////////////////////////////////////////////////////////////
// $Id: ThemeManager.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: A Singleton Theme class for texturing RenderObjects and managing
//   wave players for audio themes
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "ThemeManager.h"
#include "FileUtils.h"
#include "BCIStream.h"

using namespace std;

ThemeManager ThemeManager::_instance;

ThemeManager &ThemeManager::Instance()
{
  return _instance;
}

// Loop through all the objects and apply the theme
void ThemeManager::ApplyTheme( QGLWidget* widget )
{
	// Put together a list of texture handles - one for each texture we need.
	vector< int > tIds;
	for( map< string, string >::iterator itr = mTheme.begin(); itr != mTheme.end(); itr++ )
	{
		QImage im;
		if( !im.load( QString( FileUtils::AbsolutePath( itr->second ).c_str() ) ) )
			bcierr << "Could not load texture: " << FileUtils::AbsolutePath( itr->second ) << endl;

		// TODO:  debug build seems to crash here on preflight
		tIds.push_back( widget->bindTexture( im, GL_TEXTURE_2D ) );
	}

	// Associate appropriate texture handles with the objects
	for( unsigned int i = 0; i < mObjects.size(); i++ )
	{
		unsigned int tIdx = 0;
		map< string, string >::iterator itr;
		for( itr = mTheme.begin(); itr != mTheme.end(); itr++, tIdx++ )
			if( itr->first == mObjects[i].second ) break;
		if( itr == mTheme.end() )
		{
			bcierr << "Could not find a texture for RenderObject of type " << mObjects[i].second << endl;
			break;
		}
		mObjects[i].first->SetTexture( tIds[tIdx] );
	}
}

void ThemeManager::AddTexture( string Type, string Texture )
{
	mTheme[ Type ] = Texture;
}

void ThemeManager::AddAudioEvent( string Event, string File, float volume )
{
	// Create a new wave player for this event and add it to the map
	WavePlayer* wp = new WavePlayer();
	if( wp->ErrorState() != WavePlayer::noError )
		bcierr << "There was an issue creating a waveplayer object." << endl;
	wp->SetFile( FileUtils::AbsolutePath( File ) );
	if( wp->ErrorState() != WavePlayer::noError )
		bcierr << "Could not open file: " << FileUtils::AbsolutePath( File ) << endl;
	wp->SetVolume(volume);
	mAudioTheme[ Event ] = wp;
}

bool ThemeManager::Contains( string key )
{
	return mTheme.find( key ) != mTheme.end();
}

void ThemeManager::AddObject( RenderObject* Obj, string Type )
{
	pair< RenderObject*, string > entry;
	entry.first = Obj; entry.second = Type;
	mObjects.push_back( entry );
}

void ThemeManager::ChangeObject( RenderObject* Obj, string Type )
{
	for( vector< pair< RenderObject*, string > >::iterator itr = mObjects.begin(); itr != mObjects.end(); ++itr )
		if ( (*itr).first == Obj ) { (*itr).second = Type; return; }
	AddObject(Obj, Type);
}

void ThemeManager::FlushTheme()
{
	// Clear out the theme
	mTheme.clear();

	// Deallocate all wave players
	map< string, WavePlayer* >::iterator itr;
	for( itr = mAudioTheme.begin(); itr != mAudioTheme.end(); itr++ )
	{
		delete itr->second;
		itr->second = NULL;
	}

	// Clear out the audio theme
	mAudioTheme.clear();
}

void ThemeManager::AudioEvent( string Event )
{
	// Play the requested audio event
	if( mAudioTheme.find( Event ) == mAudioTheme.end() )
	{
		bcierr << "There is no audio event defined for event: " << Event << endl;
		return;
	}
	mAudioTheme[ Event ]->Play();

	// Do some error checking
	if( mAudioTheme[ Event ]->ErrorState() != WavePlayer::noError )
		bcierr << "There was an error with playing the audio for event: " << Event << endl;
}
