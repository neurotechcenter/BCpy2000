#ifndef _MSV_IO_STREAM_H_
#define _MSV_IO_STREAM_H_
/**********************************************
* File: msvIOstream.h.
* Desc: Temporary fix for the new kind of iostream definitions
*		brought by your friendly Microsoft VC++ 6.0.
* Module: 
* Rev: 1999/02/14 : Hugo DesRosiers : Creation
***********************************************/


#if !defined(_USING_STD_NAMESPACE_)
#define _USING_STD_NAMESPACE_
#include <iostream>
#include <fstream>
#include <strstream>
#include <iosfwd>

using std::istream;
using std::ostream;
using std::fstream;
using std::ostream;
using std::strstream;
using std::istrstream;
using std::cout;
using std::cerr;
using std::endl;// libci.lib,libc.lib,libcp.lib

#endif


#endif		/* _MSV_IO_STREAM_H_ */
