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
#include "GDSXML.h"

using namespace std;

string 
GDSXML::BeginToken( const string &inNode )
{
  return string( XML_TOKEN_LT + inNode + XML_TOKEN_GT ); 
}

string 
GDSXML::EndToken( const string &inNode ) 
{ 
  return string( XML_TOKEN_LTS + inNode + XML_TOKEN_GT ); 
}

void
ReplaceToken( string &xml, const string &tok, const string &rep  )
{
  size_t pos = xml.find( tok );
  while( pos != std::string::npos ) 
  {
    xml.replace( pos, tok.length(), rep);
    pos = xml.find( tok, pos + rep.size() );
  }
}

string
GDSXML::Escape( const string &inXML )
{
  string escapedXML = inXML;
  ReplaceToken( escapedXML, string( XML_TOKEN_AMP ),  string( XML_ESCAPE_AMP ) );
  ReplaceToken( escapedXML, string( XML_TOKEN_QUOT ), string( XML_ESCAPE_QUOT ) );
  ReplaceToken( escapedXML, string( XML_TOKEN_APOS ), string( XML_ESCAPE_APOS ) );
  ReplaceToken( escapedXML, string( XML_TOKEN_GT ),   string( XML_ESCAPE_GT ) );
  ReplaceToken( escapedXML, string( XML_TOKEN_LT ),   string( XML_ESCAPE_LT ) );
  return escapedXML;
}

string
GDSXML::Unescape( const string &inEscapedXML )
{
  string XML = inEscapedXML;
  ReplaceToken( XML, string( XML_ESCAPE_AMP ),  string( XML_TOKEN_AMP ) );
  ReplaceToken( XML, string( XML_ESCAPE_QUOT ), string( XML_TOKEN_QUOT ) );
  ReplaceToken( XML, string( XML_ESCAPE_APOS ), string( XML_TOKEN_APOS ) );
  ReplaceToken( XML, string( XML_ESCAPE_GT ),   string( XML_TOKEN_GT ) );
  ReplaceToken( XML, string( XML_ESCAPE_LT ),   string( XML_TOKEN_LT ) );
  return XML;
}

string
GDSXML::WrapSerialize( const string &inXML, bool wrapValue )
{
  string xmlOut;
  xmlOut += GDS_XML_DECLARATION;
  xmlOut += GDS_XML_DOCTYPE;
  xmlOut += GDS_XML_METAINFO_BEGIN;
  if( wrapValue ) xmlOut += GDS_XML_VALUE_NODE_BEGIN;
  xmlOut += inXML;
  if( wrapValue ) xmlOut += GDS_XML_VALUE_NODE_END;
  xmlOut += GDS_XML_METAINFO_END;
  return xmlOut;
}

string
GDSXML::SetupElement( const string &inNode, const string &inValue, bool newLine )
{ 
  string ret = BeginToken( inNode );
  ret += newLine ? "\n" : "";
  ret += inValue;
  ret += EndToken( inNode ) + "\n";
  return ret; 
}

string 
GDSXML::ExtractNode( const string &inXML, const string &inSearchNode )
{
  string startToken = BeginToken( inSearchNode );
  int startPos = inXML.find( startToken );
  if( startPos == string::npos ) return "";

  int endPos = inXML.find( EndToken( inSearchNode ) );
  if( endPos == string::npos ) return "";

  return string( inXML.begin() + startPos + startToken.size(), inXML.begin() + endPos );
}

string
GDSXML::ParseXML( const string &xml, const string &xml_node )
{
  size_t dummy = 0;
  return ParseXML( xml, xml_node, &dummy, dummy );
}

string
GDSXML::ParseXML( const string &xml, const string &xml_node, size_t* pos_finally, size_t pos_begin )
{
  string begin = BeginToken( xml_node );
  string end = EndToken( xml_node );
  
  pos_begin = xml.find( begin, pos_begin );
  size_t pos_end = xml.find( end, pos_begin + begin.size() );

  string ret( xml.begin() + pos_begin + begin.size(), xml.begin() + pos_end );
  *pos_finally = pos_end + end.size();

  return ret;
}

// TODO: This is really ugly and duplicates previous code.  
// This is copy-pasted from the sample code and could use some TLC.
static const std::string xml_stl_list_count( XML_STL_LIST_COUNT );
static const std::string token_count_begin( "<" + xml_stl_list_count + ">" );
static const size_t size_token_count_begin( token_count_begin.size() );
static const std::string token_count_end( "</" + xml_stl_list_count + ">\n" );
static const size_t size_token_count_end( token_count_end.size() );
static const std::string xml_stl_list_item_version( XML_STL_LIST_ITEM_VERSION );
static const std::string xml_stl_list_item( XML_STL_LIST_ITEM );
static const std::string token_item_begin( "<" + xml_stl_list_item + ">" );
static const size_t size_token_begin( token_item_begin.size() );
static const std::string token_item_end( "</" + xml_stl_list_item + ">\n" );
static const size_t size_token_end( token_item_end.size() );

vector< string >
GDSXML::XMLToVector( const string &inXML )
{
  size_t dummy = 0;
  return XMLToVector( inXML, &dummy, 0 );
}

vector< string > 
GDSXML::XMLToVector( const string &inXML, size_t *pos_finally, size_t pos_init )
{
  vector< string > ret;
  *pos_finally = 0;

  size_t pos_begin = inXML.find( token_count_begin, pos_init );
  if( pos_begin == string::npos ) return ret;

  size_t pos_end = inXML.find( token_count_end, pos_begin + size_token_count_begin );
  size_t count = atoi( string( inXML.begin() + pos_begin + size_token_count_begin, inXML.begin() + pos_end ).c_str() );

  pos_begin = inXML.find( xml_stl_list_item_version, pos_end + token_count_end.size() );
  if( pos_begin == string::npos ) return ret;

  pos_begin = inXML.find( token_item_begin, pos_begin + xml_stl_list_item_version.size() );
  pos_end = inXML.find( token_item_end, pos_begin + size_token_begin );
  
  size_t index = 0;
  for( size_t index = 0; index < count; index++ )
  {
    ret.push_back( string( inXML.begin() + pos_begin + size_token_begin, inXML.begin() + pos_end ) );
    pos_begin = inXML.find( token_item_begin, pos_end + size_token_end );
    pos_end = inXML.find( token_item_end, pos_begin + size_token_begin );
    if( pos_begin == string::npos ) break;
  }

  *pos_finally = pos_end + size_token_end;

  return ret;
}

std::string 
GDSXML::VectorToXML( const vector< string > &inVec, bool enable_item_version, string item_version)
{
  stringstream sstr_count;
  sstr_count << inVec.size();
  std::string xml = SetupElement( xml_stl_list_count, sstr_count.str() );

  if( enable_item_version ) 
    xml += SetupElement( xml_stl_list_item_version, item_version );

  for( vector< string >::const_iterator itr = inVec.begin(); itr != inVec.end(); ++itr )
      xml += SetupElement( xml_stl_list_item, *itr );

  return xml;
}




