/////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Hot patching/detouring of functions using a trampoline
//   technique.
//
//   We overwrite the beginning of the original function with a jump to
//   our own code, which may then choose to handle the function call, forward
//   it to the original function, or a combination of the two.
//   To be able to call the original function, we save the original code bytes
//   to a different position in memory, followed with a jump to the unaltered
//   part of the code.
//   This "trampoline" approach is considered most robust but requires use of
//   a disassembler library in order to determine instruction boundaries, and
//   to detect relative jump instructions within the code that is moved to the
//   trampoline.
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
////////////////////////////////////////////////////////////////////
#include "FunctionHook.h"

#include <udis86.h>
#include <cstdint>
#include <Windows.h>

namespace {

template<class T> T Abs( T t ) { return t < 0 ? -t : t; }
struct MemoryAddress
{
  union { void* p; uint8_t* u8; uint16_t* u16; uint32_t* u32; size_t n; };
  MemoryAddress(void* p_) { p = p_; }
  MemoryAddress(size_t n_) { n = n_; }
  MemoryAddress(int n_) { n = n_; }
  bool operator<(const MemoryAddress& other) { return n < other.n; }
  MemoryAddress& operator+=(ptrdiff_t d) { n += d; return *this; }
  MemoryAddress& operator-=(ptrdiff_t d) { n -= d; return *this; }
  ptrdiff_t operator-(const MemoryAddress& other) const { return n - other.n; }
  MemoryAddress operator+(ptrdiff_t d) const { return MemoryAddress(*this) += d; }
  operator void*() { return p; }
  operator const void*() const { return p; }
};

class Disassembler
{
public:
  Disassembler(int bits = 8 * sizeof(void*));
  int ProcessInstruction(MemoryAddress& ioInstructionPointer, int maxBytes = MAXINT32);
  ud_mnemonic_code Instruction() const;

  struct Operand : ud_operand_t
  {
    int encoding_bits;
    ud_lval* mutable_lval;
  };
  const Operand& Operand(size_t) const;
  size_t NumOperands() const;

private:
  ud_t mUd;
  mutable struct Operand mOperands[4];
  mutable int mNumOperands;
  static const struct Operand sNullOperand;
};
const struct Disassembler::Operand Disassembler::sNullOperand;

Disassembler::Disassembler(int bits)
: mNumOperands(-1)
{
  ::ud_init(&mUd);
  ::ud_set_mode(&mUd, bits);
  ::ud_set_syntax(&mUd, nullptr);
}

int
Disassembler::ProcessInstruction(MemoryAddress& ip, int maxBytes)
{
  ::ud_set_input_buffer(&mUd, ip.u8, maxBytes);
  ::ud_set_pc(&mUd, ip.n);
  int result = ::ud_disassemble(&mUd);
  if(result)
    mNumOperands = -1;
  ip += result;
  return result;
}

ud_mnemonic_code
Disassembler::Instruction() const
{
  return ::ud_insn_mnemonic(&mUd);
}

const struct Disassembler::Operand&
Disassembler::Operand(size_t i) const
{
  if(i >= NumOperands())
    return sNullOperand;
  return mOperands[i];
}

size_t
Disassembler::NumOperands() const
{
  if (mNumOperands < 0)
  {
    mNumOperands = 0;
    ::memset(mOperands, 0, sizeof(mOperands));
    for(const ud_operand_t* p = ::ud_insn_opr(&mUd, mNumOperands); p; p = ::ud_insn_opr(&mUd, ++mNumOperands))
    {
      struct Operand& op = mOperands[mNumOperands];
      static_cast<ud_operand_t&>(op) = *p;
      switch (p->type)
      {
      case UD_OP_MEM:
        op.encoding_bits = p->offset;
        break;
      case UD_OP_PTR:
      case UD_OP_IMM:
      case UD_OP_JIMM:
        op.encoding_bits = p->size;
        break;
      case UD_OP_CONST:
      case UD_OP_REG:
        op.encoding_bits = 0;
        break;
      }
    }
    int d = 0;
    for(struct Operand* p = mOperands + mNumOperands - 1; p >= mOperands; --p)
    { // avoid assumptions about prefix and instruction
      // length by determining distance to the instruction's end
      d += p->encoding_bits;
      union { int64_t n; ud_lval* p; } loc = { ::ud_insn_off(&mUd) + ::ud_insn_len(&mUd) - d / 8 };
      p->mutable_lval = loc.p;
    }
  }
  return mNumOperands;
}

bool Relocate(const MemoryAddress& begin, const MemoryAddress& end, int64_t displacement)
{
  Disassembler d;
  MemoryAddress ip = begin;
  while(d.ProcessInstruction(ip) && ip.n <= end.n)
  {
    for(int i = 0; i < d.NumOperands(); ++i)
    {
      const struct Disassembler::Operand& op = d.Operand(i);
      bool fixup = false;
      switch (op.type)
      {
      case UD_OP_JIMM:
        fixup = true;
        break;
      case UD_OP_MEM:
        fixup = (op.base == UD_R_RIP);
        break;
      }
      if (fixup)
      {
        int check = displacement;
        switch (op.encoding_bits)
        {
        case 8:
          op.mutable_lval->sbyte += displacement;
          check = op.mutable_lval->sbyte - op.lval.sbyte;
          break;
        case 16:
          op.mutable_lval->sword += displacement;
          check = op.mutable_lval->sword - op.lval.sword;
          break;
        case 32:
          op.mutable_lval->sdword += displacement;
          check = op.mutable_lval->sdword - op.lval.sdword;
          break;
        case 64:
          op.mutable_lval->sqword += displacement;
          break;
        default:
          return false;
        }
        if (check != displacement) // displacement too large for operand
          return false;
      }
    }
  }
  return true;
}

bool FindInstructionEnd(const MemoryAddress& begin, MemoryAddress& end)
{
  Disassembler d;
  MemoryAddress ip = begin;
  while(ip < end && d.ProcessInstruction(ip))
    ;
  if(ip < end)
    return false;
  end = ip;
  return true;
}

class TemporaryProtection
{
public:
  TemporaryProtection(void* addr, size_t length, int protection)
    : mAddr(addr), mLength(length), mProtect(protection)
  {
    ::VirtualProtect(mAddr, mLength, mProtect, &mProtect);
  }
  ~TemporaryProtection()
  {
    ::VirtualProtect(mAddr, mLength, mProtect, &mProtect);
    ::FlushInstructionCache(::GetCurrentProcess(), mAddr, mLength);
  }
private:
  void* mAddr;
  size_t mLength;
  DWORD mProtect;
};

const uint32_t fInUse = 0x80000000;
static char* sBase, *sBegin, *sEnd;
enum { acquire, release };
static void TrampolineLock( int action )
{
  static HANDLE sMutex;
  static int16_t sMutexInit;

  if (action == acquire)
  {
    if (::InterlockedIncrement16(&sMutexInit) == 1)
    {
      sMutex = ::CreateMutexW(nullptr, TRUE, nullptr);
      ::InterlockedIncrement16(&sMutexInit);
    }
    else
    {
      while (sMutexInit < 2)
        ;
      ::WaitForSingleObject(sMutex, INFINITE);
    }
  }
  else if (action == release)
  {
    ::ReleaseMutex(sMutex);
  }
}

template<class T> T AlignTo(int alignment, T t) { return ((t + alignment - 1) / alignment) * alignment; }

bool CloseEnough(const MemoryAddress& addr1, const MemoryAddress& addr2)
{
  if(sizeof(void*) <= 4)
    return true; // any address distance fits into a 32 bit offset
  return Abs(addr1 - addr2) < MAXINT32 / 2; // use only half range to account for jump relocations
}

MemoryAddress AllocateTrampoline( MemoryAddress inCloseTo, size_t inNumBytes )
{
  TrampolineLock(acquire);
  int allocBytes = inNumBytes + sizeof(fInUse);
  static SYSTEM_INFO sys = { 0 };
  if( sys.dwAllocationGranularity == 0 )
    ::GetSystemInfo( &sys );
  if( sEnd - sBegin < allocBytes || !CloseEnough(sBegin, inCloseTo) )
  {
    MEMORY_BASIC_INFORMATION info;
    MemoryAddress desired = inCloseTo;
    desired -= MAXINT32 / 2;
    if( desired > inCloseTo )
      desired = sys.lpMinimumApplicationAddress;
    sBase = nullptr;
    bool ok = true;
    while( ok && !sBase )
    {
      desired.n = AlignTo(sys.dwAllocationGranularity, desired.n);
      ok = ::VirtualQuery( desired, &info, sizeof(info) );
      while( ok && info.State != MEM_FREE )
      {
        desired = info.BaseAddress;
        desired += info.RegionSize;
        ok = CloseEnough(desired, inCloseTo);
        ok = ok && ::VirtualQuery( desired, &info, sizeof(info) );
      }
      if( ok )
      {
        sBase = (char*)::VirtualAlloc( desired, sys.dwAllocationGranularity, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
        if( !sBase )
          desired += info.RegionSize;
      }
    }
    sBegin = sBase;
    sEnd = sBegin;
    if( ok )
      sEnd += sys.dwAllocationGranularity;
  }
  MemoryAddress result = 0;
  if( sEnd - sBegin >= allocBytes )
  {
    result = sBegin;
    sBegin += allocBytes;
    DWORD protect = PAGE_READWRITE;
    ::VirtualProtect( result, allocBytes, protect, &protect );
    *result.u32 = allocBytes | fInUse;
    result += sizeof(fInUse);
  }
  TrampolineLock(release);
  return result;
}

void
ReleaseTrampoline(void* p)
{
  TrampolineLock(acquire);
  {
    MemoryAddress addr(p);
    addr -= sizeof(fInUse);
    TemporaryProtection protect(addr, sizeof(fInUse), PAGE_EXECUTE_READWRITE);
    *addr.u32 &= ~fInUse;
  }
  MEMORY_BASIC_INFORMATION info;
  if (::VirtualQuery(p, &info, sizeof(info)))
  {
    MemoryAddress a = info.AllocationBase;
    while(*a.u32 && !(*a.u32 & fInUse))
      a += *a.u32;
    if (!*a.u32)
    {
      if(sBase == info.AllocationBase)
      {
        sBase = 0;
        sBegin = 0;
        sEnd = 0;
      }
      ::VirtualFree(info.AllocationBase, 0, MEM_RELEASE);
    }
  }
  TrampolineLock(release);
}

static const uint8_t sReljmp8[] =  { 0xEB, 0xfe }; // infinite loop
static const uint8_t sReljmp32[] = { 0xE9, 0xfb, 0xff, 0xff, 0xff }; // infinite loop
static const uint8_t sAbsjmp64[] =
{ 0x68, 0, 0, 0, 0, // push imm32
  0xc7, 0x44, 0x24, 0x04, 0, 0, 0, 0, // mov [rsp+4], imm32
  0xc3, // ret
};
static void SetAbsjmp64Target(uint8_t* jmpAddr, uint64_t targetAddr)
{
  union { uint8_t* u8; uint32_t* u32; } loc = { jmpAddr };
  loc.u8 += 1;
  *loc.u32 = targetAddr;
  loc.u8 += 8;
  *loc.u32 = targetAddr >> 32;
}

void
Hotpatch(MemoryAddress& addr, const MemoryAddress& patch, const MemoryAddress& patchEnd)
{
  // While the patch is being applied, code in concurrent threads might see
  // inconsistent code, and crash.
  // This is avoided by atomically putting an infinite loop reljmp8 of size 2
  // at the entry point in order to keep any code from progressing into the code
  // bytes being patched.
  // Once the patch has been applied, this infinite loop is replaced with the final
  // two bytes of code, again in an atomic fashion. 

  size_t length = patchEnd - patch;
  if(length < 2)
    length = 2;

  TemporaryProtection protect(addr, length, PAGE_EXECUTE_READWRITE);
  union { const uint8_t* u8; const uint16_t* u16; }
  loop = { sReljmp8 }, final_ = { patch.u8 };
  *addr.u16 = *loop.u16;
  ::FlushInstructionCache(::GetCurrentProcess(), addr, 2);
  if(patch)
  {
    ::memcpy(addr + 2, patch + 2, patchEnd - (patch + 2));
    *addr.u16 = *final_.u16;
    ::FlushInstructionCache(::GetCurrentProcess(), addr, length);
  }
}

} // namespace

namespace FunctionHook {

void* StripJumps(void* inAddress)
{
  // If the code at addr is a pointer-based jump, we resolve the pointer
  // and continue at the pointer's target address.
  MemoryAddress stripped = inAddress;
  Disassembler d;
  bool done = false;
  while (!done)
  {
    done = true;
    MemoryAddress ip = stripped;
    if (d.ProcessInstruction(ip) && d.Instruction() == UD_Ijmp)
    {
      const struct Disassembler::Operand& op = d.Operand(0);
      if (op.type == UD_OP_MEM)
      {
        ptrdiff_t offset = 0;
        switch (op.offset)
        {
        case 8:
          offset = op.lval.sbyte;
          break;
        case 16:
          offset = op.lval.sword;
          break;
        case 32:
          offset = op.lval.sdword;
          break;
        case 64:
          offset = op.lval.sqword;
          break;
        default:
          return false;
        }
        if (op.scale)
          offset *= op.scale;
        switch (op.base)
        {
        case UD_R_RIP:
          offset += ip.n;
          break;
        case UD_NONE:
          break;
        default:
          return stripped; // unexpected
        }
        union { ptrdiff_t n; void** p; } ptr = { offset };
        done = (stripped == *ptr.p);
        stripped = *ptr.p;
      }
    }
  }
  return stripped;
}

bool Apply(void* inFunctionHooked, void* inHookFunction, void** outTrampolineToUnhookedFunction)
{
  MemoryAddress functionHooked = inFunctionHooked, hook = inHookFunction;

  // The hooked function's first bytes of code will be overwritten with a reljmp32.
  // The offset to the first untouched instruction ("FUI"), and the number of instructions
  // affected are variable, but a typical scenario will look like this:
  // | original instruction | original instruction | original instruction | ...
  // | reljmp32_1                | untouched bytes | FUI                  | ...
  //
  // Affected instruction(s) will be relocated to a "trampoline," followed with another reljmp32 that
  // jumps back to the original function's FUI.
  // This way, the trampoline forms an entry point for calling the unhooked function:
  // | original instruction | original instruction | reljmp32_2 -> continue at FUI above
  //
  // In a 64bit environment, a 32bit relative jump will not cover all possible address
  // distances between hook function, and original function. Replacing the 32bit jump with
  // a (rather long) 64bit jump is not an option because it might be larger than the function
  // itself, resulting in damage to unrelated code or data.
  // Instead, it is made sure that trampoline memory is allocated within a 32bit range to
  // the original function. Then, if the hook function is outside the 32bit range, an absjmp64
  // to the hook function is added to the trampoline, and used a target for reljmp32_1. 
  //
  // So the full trampoline layout is as follows (length_3 may be zero):
  // | relocated instructions | reljmp32_2 | absjmp64     |
  // | length_1               | length_2   | length_3     |
  // | trampoline_1                        | trampoline_2 |
  bool needAbsjmp64 = false;
  if (sizeof(void*) > 4)
  {
    ptrdiff_t displacement = functionHooked - hook;
    needAbsjmp64 = (displacement > 0 && int32_t(displacement + 16) != displacement);
    needAbsjmp64 = needAbsjmp64 || (displacement < 0 && int32_t(displacement - 16) != displacement);
  }
  MemoryAddress instructionsEnd = functionHooked.n + sizeof(sReljmp32);
  if(!FindInstructionEnd(functionHooked, instructionsEnd ))
    return false;
  size_t length_1 = instructionsEnd.n - functionHooked.n,
         length_2 = sizeof(sReljmp32),
         length_3 = needAbsjmp64 ? sizeof(sAbsjmp64) : 0;
  MemoryAddress trampolineBegin = AllocateTrampoline( functionHooked, length_1 + length_2 + length_3 );
  if( !trampolineBegin )
    return false;
  // Set up trampoline_1:
  MemoryAddress trampolineEnd = trampolineBegin.n + length_1 + length_2;
  ::memcpy(trampolineBegin, functionHooked, length_1);
  ::memcpy(trampolineBegin + length_1, sReljmp32, length_2);
  // reljmp32_2 is initialized to jump back to itself, so it will jump to desired offset after
  // relocation.
  if(!Relocate(trampolineBegin, trampolineEnd, functionHooked - trampolineBegin))
    return false;
  uint8_t reljmp32_1[sizeof(sReljmp32)];
  ::memcpy(reljmp32_1, sReljmp32, sizeof(reljmp32_1));
  if(needAbsjmp64)
  {
    // Set up trampoline_2:
    MemoryAddress trampoline2Begin = trampolineEnd;
    trampolineEnd += length_3;
    ::memcpy(trampoline2Begin, sAbsjmp64, sizeof(sAbsjmp64));
    SetAbsjmp64Target(trampoline2Begin.u8, hook.n);
    if (!Relocate(reljmp32_1, reljmp32_1 + sizeof(reljmp32_1), trampoline2Begin - functionHooked))
      return false;
  }
  else if (!Relocate(reljmp32_1, reljmp32_1 + sizeof(reljmp32_1), hook - functionHooked))
  {
    return false;
  }
  // Finish trampoline:
  DWORD protection = PAGE_EXECUTE;
  if( !::VirtualProtect( trampolineBegin, trampolineEnd - trampolineBegin, protection, &protection ) )
    return false;
  // Trampoline has been set up, now let's overwrite the original function's entry
  // with reljmp32_1.
  Hotpatch(functionHooked, reljmp32_1, reljmp32_1 + sizeof(reljmp32_1));
  *outTrampolineToUnhookedFunction = trampolineBegin;
  return true;
}

bool
Unapply(void* inFunctionHooked, void* inTrampolineToUnhookedFunction)
{
  MemoryAddress functionHooked(inFunctionHooked),
                trampoline(inTrampolineToUnhookedFunction);
  TemporaryProtection protect(trampoline, 32, PAGE_EXECUTE_READWRITE);
  MemoryAddress instructionsEnd = trampoline + sizeof(sReljmp32);
  if (!FindInstructionEnd(trampoline, instructionsEnd))
    return false;
  Hotpatch(functionHooked, nullptr, nullptr);
  if (Relocate(trampoline, instructionsEnd, trampoline - functionHooked))
    Hotpatch(functionHooked, trampoline, instructionsEnd);
  ReleaseTrampoline(trampoline);
  return true;
}

} // namespace FunctionHook