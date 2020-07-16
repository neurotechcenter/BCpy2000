////////////////////////////////////////////////////////////////////////////////
// $Id: ThemeManager.h 3125 2011-02-21 02:08:27Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: A Singleton Theme class for texturing RenderObjects and managing
//   wave players for audio themes
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <vector>
#include <map>
#include <string>
#include <utility>
#include "WavePlayer.h"
#include "RenderObject.h"

class ThemeManager // Singleton
{
private:
	// The singleton instance
	static ThemeManager _instance;

	ThemeManager() {}
	~ThemeManager() {}
	ThemeManager( const ThemeManager & );             // intentionally undefined
	ThemeManager & operator=( const ThemeManager & ); // intentionally undefined

	// Private member variables
	std::vector< std::pair< RenderObject*, std::string > > mObjects;
	std::map< std::string, std::string > mTheme;
	std::map< std::string, WavePlayer* > mAudioTheme;

public:
	// Accessor
	static ThemeManager &Instance();

	// This just adds an object to the manager, it doesn't change anything about the object
	void AddObject( RenderObject* Obj, std::string Type );
	void ChangeObject( RenderObject* Obj, std::string Type );
	void AddTexture( std::string Type, std::string Texture );
	void AddAudioEvent( std::string Event, std::string File, float volume=1.0f );
	void FlushObjects() { mObjects.clear(); }
	void FlushTheme();
	void ApplyTheme( QGLWidget* widget );
	void AudioEvent( std::string Event );
	bool Contains( std::string key );
};

#endif // THEME_MANAGER_H