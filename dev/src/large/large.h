/*
 * Copyright (c) 2015 by Pavel Ivashkov, github.com/paiv
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//============================================================================
//  class large
//
//  Multiprecision signed integer class.
//   by : fuurin <fuurin@gmx.net>, 2004
//============================================================================

#pragma once

#include <string>
#include "pod_vector.h"

using std::string;


class large
{
public:
  large();
  large( const large& v );
  large( int i );
  large( unsigned int i );

  bool operator==( const large& v ) const  { return compare(v) == 0; }
  bool operator!=( const large& v ) const  { return compare(v) != 0; }
  bool operator>( const large& v ) const  { return compare(v) > 0;  }
  bool operator<( const large& v ) const  { return compare(v) < 0;  }
  bool operator>=( const large& v ) const  { return compare(v) >= 0; }
  bool operator<=( const large& v ) const  { return compare(v) <= 0; }

  large operator-() const;
  large operator+( const large& v ) const;
  large operator-( const large& v ) const;
  large operator*( const large& v ) const;
  large operator/( const large& v ) const;
  large operator%( const large& v ) const;
  large operator^( int v ) const;    // it's a power of v, not XOR
  large operator<<( size_t v ) const;
  large operator>>( size_t v ) const;

  large& operator=( const large& v );
  large& operator+=( const large& v )   { return *this = *this + v; }
  large& operator-=( const large& v )   { return *this = *this - v; }
  large& operator*=( const large& v )   { return *this = *this * v; }
  large& operator/=( const large& v )   { return *this = *this / v; }
  large& operator%=( const large& v )   { return *this = *this % v; }
  large& operator^=( int v )      { return *this = *this ^ v; }
  large& operator<<=( int v )     { return *this = *this << v; }
  large& operator>>=( int v )     { return *this = *this >> v; }
  large& operator++()       { return *this += 1;  }
  large& operator--()       { return *this -= 1;  }
  large operator++(int);
  large operator--(int);

  large modExp( const large& exponent, const large& modulus ) const;

  size_t bitCount() const;

  int   toInt() const;
  string  toString() const;
  string  toHex() const;
  size_t  toSignedBinary( unsigned char* bin, size_t size ) const;
  size_t  toUnsignedBinary( unsigned char* bin, size_t size ) const;

  static large fromString( const char* str );
  static large fromHex( const char* str );
  static large fromSignedBinary( const unsigned char* bin, size_t size );
  static large fromUnsignedBinary( const unsigned char* bin, size_t size );

private:
  typedef unsigned int  word_t;
  typedef unsigned __int64 dword_t;
  typedef pod_vector<word_t>  vec_t;
  const static word_t _MAXWORD = 0xFFFFFFFF;

  vec_t _val;
  bool _neg;

private:
  large( bool neg, const vec_t& val );

  void normalize();
  int  compare( const large& v ) const;
  int  compare_abs( const large& v ) const;
  large divide( const large& v, large& remainder ) const;

  int  partial_sub( size_t off, size_t len, const large& v );
  int  partial_sum( size_t off, size_t len, const large& v );
  large get_partial( size_t off, size_t len ) const;
  void replace_partial( size_t off, size_t len, const large& v );

  word_t loword( size_t off = 0 ) const;
  word_t hiword( size_t off = 0 ) const;
  word_t dword_hi( dword_t d ) const    { return (word_t)(d >> 32);   }
  word_t dword_lo( dword_t d ) const    { return (word_t)(d & 0xFFFFFFFF); }
};
