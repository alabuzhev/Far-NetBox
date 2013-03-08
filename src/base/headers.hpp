#pragma once

/*
headers.hpp

Стандартные заголовки
*/
/*
Copyright © 1996 Eugene Roshal
Copyright © 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "nbafx.h"

#include <new>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cwchar>
#include <ctime>
#include <cmath>
#include <limits>
#include <cfloat>

#include <process.h>
#include <search.h>
#include <share.h>

#undef _W32API_OLD

#ifdef _MSC_VER
# include <sdkddkver.h>
# if _WIN32_WINNT < 0x0501
#  error Windows SDK v7.0 (or higher) required
# endif
#endif //_MSC_VER

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#define WIN32_NO_STATUS //exclude ntstatus.h macros from winnt.h
#include <windows.h>
#undef WIN32_NO_STATUS
#include <winioctl.h>
#include <mmsystem.h>
#include <winspool.h>
#include <setupapi.h>
#include <aclapi.h>
#include <sddl.h>
#include <dbt.h>
#include <lm.h>
#define SECURITY_WIN32
#include <security.h>
#define PSAPI_VERSION 1
#include <psapi.h>
#include <shlobj.h>
#include <shellapi.h>

#ifdef _MSC_VER
// # include <shobjidl.h>
# include <winternl.h>
# include <cfgmgr32.h>
# include <ntddscsi.h>
# include <virtdisk.h>
# include <RestartManager.h>
#endif // _MSC_VER

#ifdef USE_DLMALLOC
#include <dlmalloc/malloc-2.8.6.h>
#endif

//---------------------------------------------------------------------------

#ifdef USE_DLMALLOC
#define nb_malloc(size) dlmalloc(size)
#define nb_calloc(count,size) dlcalloc(count,size)
#define nb_realloc(ptr,size) dlrealloc(ptr,size)
#define nb_free(ptr) dlfree(ptr)
#else
#define nb_malloc(size) ::malloc(size)
#define nb_calloc(count,size) ::calloc(count,size)
#define nb_realloc(ptr,size) ::realloc(ptr,size)
#define nb_free(ptr) ::free(ptr)
#endif

#if defined(__cplusplus)
inline void * operator_new(size_t size)
{
	void * p = nb_malloc(size);
	/*if (!p)
	{
		static std::bad_alloc badalloc;
		throw badalloc;
	}*/
	return p;
}

inline void operator_delete(void * p)
{
	nb_free(p);
}
#endif

#ifdef USE_DLMALLOC
/// custom memory allocation
#define DEF_CUSTOM_MEM_ALLOCATION_IMPL            \
	public:                                         \
 	void * operator new(size_t size)                \
	{                                               \
		return operator_new(size);                    \
	}                                               \
	void operator delete(void * p, size_t size)     \
	{                                               \
		(void)(size);                                 \
		operator_delete(p);                    \
	}                                               \
 	void * operator new[](size_t size)              \
	{                                               \
		return operator_new(size);                    \
	}                                               \
	void operator delete[](void * p, size_t size)   \
	{                                               \
		(void)(size);                                 \
		operator_delete(p);                    \
	}                                               \
 	void * operator new(size_t size, void * p)      \
	{                                               \
		(void)(size);                                 \
		return p;                                     \
	}                                               \
	void operator delete(void * p, void *)          \
	{                                               \
		(void)(p);                                    \
	}                                               \
 	void * operator new[](size_t size, void * p)    \
	{                                               \
		(void)(size);                                 \
		return p;                                     \
	}                                               \
	void operator delete[](void * p, void *)        \
	{                                               \
		(void)(p);                                    \
	}

#ifdef _DEBUG
#define CUSTOM_MEM_ALLOCATION_IMPL DEF_CUSTOM_MEM_ALLOCATION_IMPL \
 	void * operator new(size_t size, const char * /*lpszFileName*/, int /*nLine*/) \
	{\
		return operator_new(size);\
	}\
 	void* operator new[](size_t size, const char * /*lpszFileName*/, int /*nLine*/)\
	{\
		return operator_new(size);\
	}\
	void operator delete(void* p, const char * /*lpszFileName*/, int /*nLine*/)\
	{\
		operator_delete(p);\
	}\
	void operator delete[](void* p, const char * /*lpszFileName*/, int /*nLine*/)\
	{\
		operator_delete(p);\
	}
#else
#define CUSTOM_MEM_ALLOCATION_IMPL DEF_CUSTOM_MEM_ALLOCATION_IMPL
#endif

#else
#define CUSTOM_MEM_ALLOCATION_IMPL 
#endif

//---------------------------------------------------------------------------

// winnls.h
#ifndef NORM_STOP_ON_NULL
#define NORM_STOP_ON_NULL 0x10000000
#endif

#ifndef True
#define True true
#endif
#ifndef False
#define False false
#endif
#ifndef Integer
typedef intptr_t Integer;
#endif
#ifndef Int64
typedef __int64 Int64;
#endif
#ifndef Boolean
typedef bool Boolean;
#endif
#ifndef Word
typedef WORD Word;
#endif

#ifndef HIDESBASE
#define HIDESBASE 
#endif

#define NullToEmpty(s) (s?s:L"")

template <class T>
inline const T&Min(const T &a, const T &b) { return a<b?a:b; }

template <class T>
inline const T&Max(const T &a, const T &b) { return a>b?a:b; }

template <class T>
inline const T Round(const T &a, const T &b) { return a/b+(a%b*2>b?1:0); }

inline void* ToPtr(intptr_t T){ return reinterpret_cast<void*>(T); }

template<typename T>
inline void ClearStruct(T& s) { memset(&s, 0, sizeof(s)); }

template<typename T>
inline void ClearStruct(T* s) { T dont_instantiate_this_template_with_pointers = s; }

template<typename T, size_t N>
inline void ClearArray(T (&a)[N]) { memset(a, 0, sizeof(a[0])*N); }

#define SIGN_UNICODE    0xFEFF
#define SIGN_REVERSEBOM 0xFFFE
#define SIGN_UTF8       0xBFBBEF

#ifdef __GNUC__
# if _GCC_VER < GCC_VER_(4,6,1)
#  define nullptr NULL
#  include "lang.hpp"
# else
   enum LNGID:int;
# endif
#endif

#if defined(_MSC_VER) && _MSC_VER>1600
enum LNGID:int;
#endif

#if defined(_MSC_VER) && _MSC_VER<1600
#define nullptr NULL
#endif

template <typename T>
bool CheckNullOrStructSize(const T* s) {return !s || (s->StructSize >= sizeof(T));}
template <typename T>
bool CheckStructSize(const T* s) {return s && (s->StructSize >= sizeof(T));}

#ifdef _DEBUG
#define SELF_TEST(code) \
	namespace { \
		struct SelfTest { \
			SelfTest() { \
				code; \
			} \
		} _SelfTest; \
	}
#else
#define SELF_TEST(code)
#endif

//---------------------------------------------------------------------------

template <typename T> class custom_nballocator_t;

template <> class custom_nballocator_t<void>
{
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    // reference to void members are impossible.
    typedef void value_type;
    template <class U> 
        struct rebind { typedef custom_nballocator_t<U> other; };
};    

namespace alloc {
  inline void destruct(char *){}
  inline void destruct(wchar_t*){}
  template <typename T> 
  inline void destruct(T * t){t->~T();}
} // namespace
namespace nballoc {
template <typename T>
class custom_nballocator_t
{
public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T value_type;

  template <class U> 
  struct rebind { typedef custom_nballocator_t<U> other; };
  custom_nballocator_t()
  {}
  pointer address(reference x) const { return &x; }
  const_pointer address(const_reference x) const { return &x; }
  pointer allocate(size_type size, custom_nballocator_t<void>::const_pointer hint = 0)
  {
    return static_cast<pointer>(nb_malloc(size * sizeof(T)));
  }
  //for Dinkumware:
  /*char *_Charalloc(size_type n)
  {
    return static_cast<char*>(nb_malloc(n));
  }*/
  // end Dinkumware

  template <class U> custom_nballocator_t(const custom_nballocator_t<U>&)
  {}
  void deallocate(pointer p, size_type n)
  {
    nb_free(p);
  }
  void deallocate(void *p, size_type n)
  {
    nb_free(p);
  }
  size_type max_size() const throw()
  {
    return size_t(-1) / sizeof(value_type);
  }
  void construct(pointer p, const T& val)
  {
    new(static_cast<void*>(p)) T(val);
  }
  void construct(pointer p)
  {
    new(static_cast<void*>(p)) T();
  }
  void destroy(pointer p)
  {
    alloc::destruct(p);
    // nb_free(p);
    // p->~T();
  }
  // static void dump(){mem_.dump();};
private:
};

template <typename T, typename U>
inline bool operator==(const custom_nballocator_t<T>&, const custom_nballocator_t<U>&)
{
  return true;
}

template <typename T, typename U>
inline bool operator!=(const custom_nballocator_t<T>&, const custom_nballocator_t<U>&)
{
  return false;
}

} // namespace nballoc

template <class T> 
struct custom_nballocator_t
{
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T value_type;

  template <class U> struct rebind { typedef custom_nballocator_t<U> other; };
  custom_nballocator_t() throw() {}
  custom_nballocator_t(const custom_nballocator_t&) throw() {}

  template <class U> custom_nballocator_t(const custom_nballocator_t<U>&) throw(){}

  ~custom_nballocator_t() throw() {}

  pointer address(reference x) const { return &x; }
  const_pointer address(const_reference x) const { return &x; }

  pointer allocate(size_type s, void const * = 0)
  {
    if (0 == s)
      return NULL;
    pointer temp = (pointer)nb_calloc(s, sizeof(T)); 
    if (temp == NULL)
      throw std::bad_alloc();
    return temp;
  }

  void deallocate(pointer p, size_type)
  {
    nb_free(p);
  }

  size_type max_size() const throw()
  {
    // return std::numeric_limits<size_t>::max() / sizeof(T); 
    return size_t(-1) / sizeof(T); 
  }

  void construct(pointer p, const T& val)
  {
    new((void *)p) T(val);
  }

  void destroy(pointer p)
  {
    // p->~T();
    alloc::destruct(p);
  }
};

template <typename T, typename U>
inline bool operator==(const custom_nballocator_t<T>&, const custom_nballocator_t<U>&)
{
  return false;
}

template <typename T, typename U>
inline bool operator!=(const custom_nballocator_t<T>&, const custom_nballocator_t<U>&)
{
  return true;
}

//---------------------------------------------------------------------------

#include "UnicodeString.hpp"
#include "local.hpp"
