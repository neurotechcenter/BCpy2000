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
#ifndef __CHANNEL_ROUTER_H
#define __CHANNEL_ROUTER_H

#include<string>
#include<map>


class TypedChannel 
{
    public:
        TypedChannel()
        {
            m_type_set = false;
            m_idx_set = false;
        };

		TypedChannel(const TypedChannel& o)
		{
            this->m_type = o.m_type;
            this->m_idx = o.m_idx;
            this->m_type_set = o.m_type_set;
            this->m_idx_set = o.m_idx_set;
		}

        TypedChannel(std::string in_t, unsigned int in_idx)
        {
            m_type = in_t;
            m_idx = in_idx;
            m_type_set = true;
            m_idx_set = true;
        };

        TypedChannel& operator= (const TypedChannel& o)
        { 
            this->m_type = o.m_type;
            this->m_idx = o.m_idx;
            this->m_type_set = o.m_type_set;
            this->m_idx_set = o.m_idx_set;
            return *this;
        };
        TypedChannel& operator= (const TypedChannel* o)
        { 
            this->m_type = o->m_type;
            this->m_idx = o->m_idx;
            this->m_type_set = o->m_type_set;
            this->m_idx_set = o->m_idx_set;
            return *this;
        };

        int setIndex(unsigned int in_idx)
        {
            unsigned int t_old_idx = m_idx;
            m_idx = in_idx;
            m_idx_set = true;
            return t_old_idx;
        };
        int getIndex() const
        { return m_idx; };
        
        void setType(std::string in_t)
        {
            m_type = in_t;
            m_type_set = true;
        };
        const std::string & getType() const
        {
            return m_type;
        };
        bool isValid()
        { return m_type_set && m_idx_set; };


    private:
        std::string         m_type;
        unsigned int        m_idx;
        bool                m_type_set;
        bool                m_idx_set;
    
};

bool operator==(const TypedChannel& in_channel_a, const TypedChannel& in_channel_b);

class ChannelRouter
{
    public: 
        ChannelRouter (unsigned  int in_generic_channel_count);
        ~ChannelRouter();

        bool addChannelType( const std::string& in_type, unsigned int in_count );

        bool route(unsigned int in_generic_idx, const TypedChannel* in_typed_ch);
        bool route(unsigned int in_generic_idx, const std::string& in_type, const unsigned int in_typed_idx );

        const int typedToGeneric ( const std::string& in_type, const unsigned int in_idx) const;
		const int typedToGeneric ( const TypedChannel& in_typed_channel ) const;
		TypedChannel const * const genericToTyped ( const unsigned int in_generic_idx ) const;

		unsigned int get_generic_channel_count()
		{
			return m_generic_channel_count;
		};
		int get_typed_channel_count(const std::string& in_type)
		{
			if(m_channel_types.find(in_type) != m_channel_types.end())
				return m_channel_types[in_type];
			else
				return -1;
		};

    private: 
        unsigned int m_generic_channel_count;
        std::map<std::string, unsigned int> m_channel_types;
        TypedChannel *m_channel_mapping;



};

#endif
