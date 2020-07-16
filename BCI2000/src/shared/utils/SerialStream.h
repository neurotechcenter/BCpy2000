////////////////////////////////////////////////////////////////////////////////
// $Id: SerialStream.h 5699 2017-08-21 18:22:33Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A std::iostream interface for blocking serial communication.
//              serialstream: A std::iostream interface to the data stream on a
//                serial line. Will wait for flush or eof before sending data.
//                Send/receive is blocking; one can use rdbuf()->in_avail()
//                to check for data.
//              serialbuf: A helper class that does the actual send/receive
//                calls.
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
#ifndef SERIAL_STREAM_H
#define SERIAL_STREAM_H

#include <iostream>

class serialbuf : public std::streambuf
{
    enum
    {
      buf_size = 512,
    };

  public:
    enum
    {
      infiniteTimeout = -1,
      defaultTimeout = 500, // ms
    };

  public:
    serialbuf();
    virtual ~serialbuf();
    void set_timeout( int t );
    int  get_timeout() const        { return m_timeout; }
    bool is_open() const;
    serialbuf* open( const char* device );
    serialbuf* close();

  protected:
    virtual std::streamsize showmanyc();           // Called from streambuf::in_avail().

  protected:
    virtual std::ios::int_type underflow();        // Called from read operations if empty.
    virtual std::ios::int_type overflow( int c );  // Called if write buffer is filled.
    virtual int sync();                            // Called from iostream::flush().

  protected:
    void* m_handle;
    int   m_timeout;
};

class serialstream : public std::iostream
{
  public:
    serialstream();
    explicit serialstream( const char* device );
    virtual ~serialstream()         {}
    bool is_open() const            { return buf.is_open(); }
    void open( const char* device );
    void close();

  private:
    serialbuf buf;
};

#endif // SERIAL_STREAM_H
