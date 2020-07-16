////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: griffin.milsap@gmail.com
// Description: A set of low-overhead XML parsing functions for GDS
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
#ifndef GDS_XML_H
#define GDS_XML_H
#include <string>
#include <vector>
#include <utility>
#include <iostream>

//_____________________________________________________________________________
// XML definitions from boost
#define GDS_XML_DECLARATION "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n"
#define GDS_XML_DOCTYPE "<!DOCTYPE boost_serialization>\n"
#define GDS_XML_METAINFO_BEGIN "<boost_serialization signature=\"serialization::archive\" version=\"9\">\n"
#define GDS_XML_METAINFO_END "</boost_serialization>\n"

#define GDS_XML_VALUE_NODE_BEGIN "<value class_id=\"0\" tracking_level=\"0\" version=\"0\">\n"
#define GDS_XML_VALUE_NODE_END "</value>\n"
#define GDS_XML_VALUE_NODE "value"

//____________________________________________________________________________
// XML element definitions of a command message
#define GDS_XML_SESSION_ID_NODE "session_id_"
#define GDS_XML_COMMAND_NODE "command_"
#define GDS_XML_PAYLOAD_NODE "payload_"
#define GDS_XML_STATUS_NODE "status_"
#define GDS_XML_ERROR_NODE "error_"

//____________________________________________________________________________
// XML element definitions of measurement data information
#define GDS_XML_DATA_INFO_NUMBER_OF_SCANS_NODE "number_of_scans_"
#define GDS_XML_DATA_INFO_CHANNELS_PER_DEVICE_NODE "channels_per_device_"
#define GDS_XML_DATA_INFO_BUFFER_SIZE_IN_SAMPLES_NODE "buffer_size_in_samples_"

//____________________________________________________________________________
// XML element definitions of device information
#define GDS_XML_DEVICE_INFO_NAME "Name"
#define GDS_XML_DEVICE_INFO_TYPE "DeviceType"
#define GDS_XML_DEVICE_INFO_STATUS "InUse"

//_____________________________________________________________________________
// C++ STL to XML complients

// conversion to list (for example std::vector)
#define XML_STL_LIST_COUNT "count"
#define XML_STL_LIST_ITEM_VERSION "item_version"
#define XML_STL_LIST_ITEM "item"

// conversion to Device
#define XML_DEVICE_LIST_STATUS "InUse"
#define XML_DEVICE_LIST_NAME "Name"
#define XML_DEVICE_LIST_TYPE "DeviceType"

// conversion to std::pair
#define XML_STL_PAIR_FIRST "first"
#define XML_STL_PAIR_SECOND "second"

//____________________________________________________________________
// XML tokens and escape codes.
#define XML_TOKEN_QUOT "\""
#define XML_TOKEN_APOS "\'"
#define XML_TOKEN_LT "<"
#define XML_TOKEN_LTS "</"
#define XML_TOKEN_GT ">"
#define XML_TOKEN_AMP "&"

#define XML_ESCAPE_QUOT "&quot;"
#define XML_ESCAPE_APOS "&apos;"
#define XML_ESCAPE_LT "&lt;"
#define XML_ESCAPE_GT "&gt;"
#define XML_ESCAPE_AMP "&amp;"

namespace GDSXML
{
  std::string Escape( const std::string &inXML );
  std::string Unescape( const std::string &inEscapedXML );

  std::string BeginToken( const std::string &inNode );
  std::string EndToken( const std::string &inNode );

  std::string ExtractNode( const std::string &inXML, const std::string &inSearchNode );

  std::string WrapSerialize( const std::string &inXML, bool wrapValue = false );
  std::string SetupElement( const std::string &inNode, 
                          const std::string &inValue, 
                          bool newLine = false );

  template< class T >
  std::string SetupElement( const std::string &inNode, T inValue, bool newLine = false )
  {
    std::stringstream to_str;
    to_str << inValue;
    return SetupElement( inNode, to_str.str(), newLine );
  }

  std::string ParseXML( const std::string &xml,
                        const std::string &xml_node );
  std::string ParseXML( const std::string &xml, 
                        const std::string &xml_node, 
                        size_t* pos_finally, 
                        size_t pos_begin = 0 );

  std::vector< std::string > XMLToVector( const std::string &inXML );
  std::vector< std::string > XMLToVector( const std::string &inXML, size_t *pos_finally, size_t pos_init = 0 );

  std::string VectorToXML( const std::vector< std::string > &inVec, bool enable_item_version = true, std::string item_version = "0" );





  class GDSXMLItem
  {
   public:
    operator std::string () const { return ToXML(); }
    friend std::ostream& operator<< ( std::ostream &os, const GDSXMLItem &item ) { os << item.ToXML(); return os; }
    virtual std::string MetaInfo() const { return ""; }

   protected:
    virtual std::string ToXML() const { return ""; }
  };

  template< typename T1, typename T2 >
  class GDSPair : public GDSXMLItem
  {
   public:
    GDSPair( std::pair< T1, T2 > *pPair ) : mpPair( pPair ) { }

   protected:
    virtual std::string ToXML() const
    {
      string xml;
      xml += GDSValue< string >( "first", mpPair->first, mpPair->first.MetaInfo() );
      xml += GDSValue< string >( "second", mpPair->second, mpPair->second.MetaInfo() );
      return xml;
    }

   private:
    std::pair< T1, T2 > mpPair;
  };

  template< typename T >
  class GDSVector : public GDSXMLItem
  {
   public:
    GDSVector( const std::vector< T > *pVector, std::string item_version = "0" ) 
      : mpList( pVector ), mItemVersion( item_version ) { }

   protected:
    virtual std::string ToXML() const
    {
      std::stringstream xml;
      xml << GDSValue< size_t >( "count", mpList->size() ) << "\n";

      if( mItemVersion != "" ) 
        xml << GDSValue< std::string >( "item_version", mItemVersion ) << "\n";

      for( vector< T >::const_iterator itr = mpList->cbegin(); itr != mpList->cend(); ++itr )
        xml << GDSValue< std::string >( "item", *itr, itr == mpList->cbegin() ? itr->MetaInfo() : "" ) << "\n";

      return xml.str();
    }

   private:
    std::string mItemVersion;
    const std::vector< T > *mpList;
  };

  template < typename T >
  class GDSValue : public GDSXMLItem
  {
   public:
    // FIXME: Set a default value -- node = ""
    GDSValue( std::string node, T value, std::string metaInfo = "" ) : 
      mNode( node ), mValue( value ), mMetaInfo( metaInfo ) { }
    T& operator=( T arg ) { return mValue = arg; }
    T& operator()() { return mValue; }

    std::string BeginTag( bool closed = false ) const 
    { 
      std::string xml = mNode;
      xml += ( MetaInfo() != "" ) ? " " + MetaInfo() : "";
      return "<" + xml + ( ( closed ) ? " /" : "" ) + ">";
    }
    std::string EndTag() const { return std::string( "</" + mNode + ">" ); }

    virtual std::string MetaInfo() const { return mMetaInfo; }

   protected:
    virtual std::string ToXML() const 
    {
      std::string value = GetValue();
      if( mNode == "" ) return value;
      bool multiLine = value.find( "\n" ) != string::npos;

      std::stringstream xml;
      xml << BeginTag();
      if( multiLine ) xml << "\n";
      xml << value << EndTag();

      return xml.str();
    }

   private:
    std::string GetValue() const
    {
      std::stringstream ss;
      ss << mValue;
      return ss.str();
    }

    std::string mNode;
    std::string mMetaInfo;
    T mValue;
  };

  //template <> std::string GDSValue< GDSXMLItem& >::GetValue() const { return mValue; }

};
#endif // GDS_XML_H