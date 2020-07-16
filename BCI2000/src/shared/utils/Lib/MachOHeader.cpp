////////////////////////////////////////////////////////////////////////////////
// $Id$
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
#include "MachOHeader.h"

#include <mach-o/loader.h>
#include <mach-o/swap.h>
#include <sys/mman.h>

Tiny::MachOHeader::MachOHeader( void* inHeaderAddress )
: mpAddress( static_cast<char*>(inHeaderAddress) ),
  mpHeader( static_cast<mach_header*>(inHeaderAddress) ),
  mBits( 0 ),
  mNeedSwap(false)
{
    if(mpHeader) switch(mpHeader->magic)
    {
        case MH_CIGAM_64:
            mNeedSwap = true;
        case MH_MAGIC_64:
            mBits = 64;
            break;
        case MH_CIGAM:
            mNeedSwap = true;
        case MH_MAGIC:
            mBits = 32;
            break;
    }
}
    
Tiny::MachOHeader::~MachOHeader()
{
}

load_command*
Tiny::MachOHeader::Command( int kind, int idx )
{
    if(!mpHeader)
        return nullptr;
    if(idx<0)
        return nullptr;
    if(mNeedSwap)
        return nullptr;

    union { char* c; load_command* lc; } p = { mpAddress };
    switch(mBits)
    {
        case 32:
            p.c += sizeof(mach_header);
            break;
        case 64:
            p.c += sizeof(mach_header_64);
            break;
    }
    int i = 0, count = 0;
    while( i++ < mpHeader->ncmds )
    {
        if(p.lc->cmd == kind && count++ == idx)
            return p.lc;
        p.c += p.lc->cmdsize;
    }
    return nullptr;
}
 

Tiny::MachOFile::MachOFile( MachOHeader& h, bool isLoadedImage )
: mpHeader(&h), mIsLoadedImage(isLoadedImage)
{
}
    
const char*
Tiny::MachOFile::ExportLibraryName()
{
    if(mIsLoadedImage)
        return nullptr;

    union { load_command* lc; dylib_command* d; char* c; } p = { mpHeader->Command(LC_ID_DYLIB) };
    if( p.d )
        return p.c + p.d->dylib.name.offset;
    return nullptr;
}
    
template<class T, int C>
static T* SegmentFromSection( MachOHeader* pHeader, int section )
{
    if( section > 0 )
    {
        union { load_command* lc; T* d; } segment;
        for( int k = 0; (segment.lc = pHeader->Command(C, k)); ++k )
            if((section -= segment.d->nsects) <= 0)
                return segment.d;
    }
    return nullptr;
}

template<class U, class T, int C>
static bool NextExport( MachOHeader* pHeader, MachOFile::SymbolInfo& ioInfo )
{
    union { load_command* lc; symtab_command* st; } p = { pHeader->Command(LC_SYMTAB) };
    if(!p.st)
        return false;
    T* pSegment = nullptr;
    union { void** p; int* i; } pIdx = { &ioInfo.opaque };
    for( ; !pSegment && *pIdx.i < p.st->nsyms; ++*pIdx.i )
    {
        union { const void* p; const U* n; } entry = { pHeader->BaseAddress() + p.st->symoff };
        entry.n += *pIdx.i;
        pSegment = SegmentFromSection<T, C>( pHeader, entry.n->n_sect );
        if(pSegment)
        {
            ioInfo.name = pHeader->BaseAddress() + p.st->stroff + entry.n->n_un.n_strx;
            ioInfo.offset = entry.n->n_value;
            ioInfo.data = !(pSegment->initprot & PROT_EXEC);
        }
    }
    if(pSegment)
        ++*pIdx.i;
    return pSegment;
}
    
bool
Tiny::MachOFile::NextExport( MachOFile::SymbolInfo& ioInfo )
{
    if(mIsLoadedImage)
        return nullptr;
    
    switch(mpHeader->Bits())
    {
        case 32:
            return ::NextExport<struct nlist, segment_command, LC_SEGMENT>( mpHeader, ioInfo );
        case 64:
            return ::NextExport<struct nlist_64, segment_command_64, LC_SEGMENT_64>( mpHeader, ioInfo );
    }
    return false;
}

