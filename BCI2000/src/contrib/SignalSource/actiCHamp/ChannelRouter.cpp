////////////////////////////////////////////////////////////////////$
// Authors: Paul Ignatenko <paul dot ignatenko at gmail dot com
// Description: Library for managing and routing channels to different
// locations
//
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
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////

#include"ChannelRouter.h"
#include<string>
#include<map>
#include<algorithm>
#include<cstring>

using namespace std;

bool operator==(const TypedChannel& in_channel_a, const TypedChannel& in_channel_b) 
{
    if(in_channel_a.getType() == in_channel_b.getType())
    {
        if(in_channel_a.getIndex() == in_channel_b.getIndex())
        {
            return true;
        }
    }
    return false;
};


/**
 * Construct a channel router
 *
 * Construct a channel router given a count of routed channels
 *
 * @param in_channel_count total count of routed channels
 *
 * @return a ChannelRouter object
 */

ChannelRouter::ChannelRouter(unsigned int in_generic_channel_count)
{
    m_generic_channel_count = in_generic_channel_count;
    m_channel_mapping = new TypedChannel[m_generic_channel_count];
}


ChannelRouter::~ChannelRouter()
{
    delete[] m_channel_mapping;
	m_channel_mapping = 0;
}

/**
 * Add a type of channel
 *
 * Add a type of channel given a channel type and a total count of those
 * channels
 *
 * @param in_type  the type of channel given
 * @param in_channel_count the total type of channels in that catagory
 *
 * @return true if successful false if in_type already exists
 */
bool ChannelRouter::addChannelType( const std::string& in_type, const unsigned int in_channel_count )
{   
    map<string, unsigned int>::iterator it;

    it = m_channel_types.find(in_type);
    if( it == m_channel_types.end() )
    {
        m_channel_types[in_type] = in_channel_count;
        return true;
    }
    return false;
}

//TODO: Safety check in_idx and in_typed_channel.getIndex() so it's not out of
//range.
//TODO: Safety check channel type.

/**
 * map a generic channel to a TypedChannel
 *
 * map a generic channel to a TypedChannel type given the generic channel index and a
 * TypedChannel
 * 
 *
 * @param in_generic_idx    the generic channel to map a channel to
 * @param in_typed_ch       the TypedChannel to map a generic channel to
 *
 * @return returns true if successful, false otherwise
 */

bool ChannelRouter::route(const unsigned int in_generic_idx, const TypedChannel* in_typed_ch)
{
    if(in_generic_idx < m_generic_channel_count)
    {
        m_channel_mapping[in_generic_idx] = in_typed_ch;
        return true;
    }
    return false;

}



/**
 * map a generic channel to a TypedChannel
 *
 * map a generic channel to a TypedChannel given the generic channel index, a
 * channel type and a channel index
 * 
 *
 * @param in_generic_idx    the generic channel to map a channel to
 * @param in_type           the type of the TypedChannel to map to
 * @param in_typed_idx      the channel idx of the TypedChannel to map to
 *
 * @return returns true if successful, false otherwise
 */

bool ChannelRouter::route(const unsigned int in_generic_idx, const std::string& in_type, const unsigned int in_typed_idx )
{
    if(in_generic_idx < m_generic_channel_count)
    {
        m_channel_mapping[in_generic_idx].setType(in_type);
        m_channel_mapping[in_generic_idx].setIndex(in_typed_idx);
        return true;
    }
    
    return false;

}

/**
 * gets generic channel index given typed type and index
 *
 * gets generic channel index given typed type and index
 *
 * @param in_type the type of typed channel
 * @param in_idx the index of typed channel
 *
 * @return generic index if found; -1 if not
 */



const int ChannelRouter::typedToGeneric ( const std::string& in_type, const unsigned int in_idx) const
{
    for(unsigned int i = 0; i < m_generic_channel_count; ++i)
    {
        if( m_channel_mapping[i] == TypedChannel(in_type, in_idx))
        {
            return i;
        }
    }
    return -1;

}
/**
 * gets generic channel index given TypedChannel
 *
 * gets generic channel index given TypedChannel
 *
 * @param in_typed_channel the TypedChannel to find an index for
 *
 * @return generic index if found; -1 if not
 */

const int ChannelRouter::typedToGeneric ( const TypedChannel& in_typed_channel ) const
{
    for(unsigned int i = 0; i < m_generic_channel_count; ++i)
    {
        if( m_channel_mapping[i] == in_typed_channel )
        {
            return i;
        }
    }
    return -1;

}

/**
 * gets typed channel given generic channel index
 *
 * gets typed channel given generic channel index
 *
 * @param in_generic_idx the generic index to find a TypedChannel for
 *
 * @return returns the TypedChannel if found or null
 */

TypedChannel const * const ChannelRouter::genericToTyped ( const unsigned int in_generic_idx ) const
{
    if(in_generic_idx < m_generic_channel_count)
    {
        return &m_channel_mapping[in_generic_idx];
    }
    return NULL;
}
