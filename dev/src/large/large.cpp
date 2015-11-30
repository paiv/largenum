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

#include "large.h"

//============================================================================
//                                  constructors
//============================================================================

large::large() :
  _neg(false)
{
}

large::large( const large& v )
{
  _neg = v._neg;
  _val = v._val;
}

large::large( int i )
{
  if( i == 0 )
    _neg = false;
  else
  {
    _neg = i < 0;
    if( _neg )
      i = -i;
    _val.push_back(i);
  }
}

large::large( unsigned int i )
{
  _neg = false;
  if( i != 0 )
    _val.push_back(i);
}

large::large( bool neg, const vec_t& val )
{
  if( val.size() > 0 )
  {
    _neg = neg;
    _val = val;
  }
  else
  {
    _neg = false;
  }
}

//============================================================================
//                               operator methods
//============================================================================

large large::operator-() const
{
  return large( !_neg, _val );
}

large large::operator+( const large& v ) const
{
  if( v == 0 ) return *this;
  if( _neg != v._neg ) return ( *this - (-v) );

  large r = v;
  word_t k = 0, w;

  if( r._val.size() < _val.size() )
    r._val.resize( _val.size() );

  for( size_t i = 0; i < r._val.size(); i++ )
  {
    if( i < _val.size() )
    {
      w = r._val[i] + k;
      k = w < k;
      w += _val[i];
      k = k || w < _val[i];
    }
    else
    {
      if( k == 0 ) break;
      w = r._val[i] + k;
      k = w < k;
    }

    r._val[i] = w;
  }

  if( k > 0 )
    r._val.push_back(k);

  r.normalize();
  return r;
}

large large::operator-( const large& v ) const
{
  if( v == 0 ) return *this;
  if( _neg != v._neg )  return ( *this + (-v) );
  if( compare_abs(v) < 0 ) return -( v - *this );

  large r = *this;
  word_t k = 1, w;

  for( size_t i = 0; i < r._val.size(); i++ )
  {
    if( i < v._val.size() )
    {
      dword_t d = (dword_t) r._val[i] - v._val[i] + k + _MAXWORD;
      w = dword_lo(d);
      k = dword_hi(d);
    }
    else
    {
      if( k == 1 ) break;
      dword_t d = (dword_t) r._val[i] + k + _MAXWORD;
      w = dword_lo(d);
      k = dword_hi(d);
    }
    r._val[i] = w;
  }

  r.normalize();
  return r;
}

large large::operator*( const large& v ) const
{
  large r;
  word_t k;

  r._val.resize( _val.size() + v._val.size() );

  for( size_t i = 0; i < _val.size(); i++ )
  {
    k = 0;
    for( size_t j = 0; j < v._val.size(); j++ )
    {
      dword_t d = (dword_t) _val[i] * v._val[j] + r._val[i+j] + k;

      r._val[i+j] = dword_lo(d);
      k   = dword_hi(d);
    }
    r._val[i + v._val.size()] = k;
  }

  r._neg = _neg != v._neg;
  r.normalize();
  return r;
}

large large::operator/( const large& v ) const
{
  large r;
  return divide( v, r );
}

large large::operator%( const large& v ) const
{
  large r;
  divide( v, r );
  return r;
}

large large::operator^( int v ) const
{
  large r = *this;
  while( --v > 0 )
    r *= *this;
  return r;
}

large large::operator<<( size_t v ) const
{
  if( v == 0 ) return *this;
  if( _val.size() == 0 ) return 0;
  large r = *this;

  size_t k = v / ( sizeof(word_t) * 8 );
  r._val.resize( r._val.size() + k + 1 );
  if( k > 0 )
  {
    for( size_t i = r._val.size(); i > k; i-- )
      r._val[i-1] = r._val[i-1-k];
    for( size_t i = k; i > 0; i-- )
      r._val[i-1] = 0;
  }

  k = v % ( sizeof(word_t) * 8 );
  if( k > 0 )
  {
    word_t w, hi = 0;
    int  mb = sizeof(word_t) * 8 - k;

    for( size_t i = 0; i < r._val.size(); i++ )
    {
      w = r._val[i] >> mb;
      r._val[i] = ( r._val[i] << k ) + hi;
      hi = w;
    }
  }

  r.normalize();
  return r;
}

large large::operator>>( size_t v ) const
{
  size_t k = v / ( sizeof(word_t) * 8 );
  if( _val.size() <= k ) return 0;

  large r = *this;

  if( k > 0 )
  {
    for( size_t i = k; i < r._val.size(); i++ )
      r._val[i-k] = r._val[i];
    r._val.resize( r._val.size() - k );
  }

  k = v % ( sizeof(word_t) * 8 );
  if( k > 0 )
  {
    word_t w, lo = 0;
    int  mb = sizeof(word_t) * 8 - k;

    for( size_t i = r._val.size(); i > 0; i-- )
    {
      w = r._val[i-1] << mb;
      r._val[i-1] = ( r._val[i-1] >> k ) + lo;
      lo = w;
    }
  }

  r.normalize();
  return r;
}

large& large::operator=( const large& v )
{
  _neg = v._neg;
  _val = v._val;
  return *this;
}

large large::operator++(int)
{
  large r( *this );
  operator++();
  return r;
}

large large::operator--(int)
{
  large r( *this );
  operator--();
  return r;
}

size_t large::bitCount() const
{
  size_t n = _val.size() * sizeof(word_t) * 8;
  if( n == 0 ) return n;

  word_t w = hiword();
  for( word_t mask = 0x80000000; ( w & mask ) == 0; mask >>= 1 ) n--;
  return n;
}

//============================================================================
//                           auxiliary private methods
//============================================================================

void large::normalize()
{
  size_t i = _val.size();
  while( i > 0 && _val[i-1] == 0 ) i--;
  _val.erase( _val.begin() + i, _val.end() );
  if( _val.size() == 0 ) _neg = false;
}

int large::compare( const large& v ) const
{
  int r = _neg - v._neg;
  if( r > 0 ) return -1;
  if( r < 0 ) return 1;

  r = compare_abs(v);
  return _neg ? -r : r;
}

int large::compare_abs( const large& v ) const
{
  if( _val.size() > v._val.size() ) return 1;
  if( _val.size() < v._val.size() ) return -1;

  for( size_t i = _val.size(); i > 0; i-- )
  {
    if( _val[i-1] > v._val[i-1] ) return 1;
    if( _val[i-1] < v._val[i-1] ) return -1;
  }

  return 0;
}

large large::divide( const large& v, large& remainder ) const
{
  remainder = 0;
  int cmp = compare_abs(v);
  if( cmp == 0 ) return 1;
  if( cmp < 0 )
  {
    remainder = *this;
    return 0;
  }

  large q;
  word_t d = dword_lo( dword_t(0x100000000) / ( v.hiword(0) + 1) );
  large nu( false, _val );
  large nv( false, v._val );
  size_t n = nv._val.size();
  size_t m = nu._val.size() - n;
  nu *= d;
  nv *= d;

  for( size_t i = m; i >= 0; i-- )
  {
    dword_t d;
    d = nu.loword( n + i );
    d <<= 32;
    d += nu.loword( n + i - 1 );

    dword_t pq = d / nv.hiword(0);
    dword_t pr = d % nv.hiword(0);

    do
    {
      if( pq == 0x100000000 ||
        pq * nv.hiword(1) > ( pr << 32 ) + nu.loword( n + i - 2 ) )
      {
        pq--;
        pr += nv.hiword(0);
      }
      else
        break;
    }
    while( pr < 0x100000000 );

    if( nu.partial_sub( i, n + 1, nv * dword_lo(pq) ) < 0 )
    {
      pq--;
      nu.partial_sum( i, n, nv );
    }

    q._val.insert( q._val.begin(), dword_lo(pq) );

    if( i == 0 ) break;
  }

  remainder  = nu / d;
  remainder._neg = _neg;
  remainder.normalize();

  q._neg = _neg != v._neg;
  q.normalize();
  return q;
}

int large::partial_sub( size_t off, size_t len, const large& v )
{
  large u = get_partial( off, len );
  u -= v;
  replace_partial( off, len, u );
  return ( u < 0 ) ? -1 : 1;
}

int large::partial_sum( size_t off, size_t len, const large& v )
{
  large u = get_partial( off, len );
  u += v;
  replace_partial( off, len, u );
  return ( u < 0 ) ? -1 : 1;
}

large large::get_partial( size_t off, size_t len ) const
{
  large r;
  r._val.resize(len);
  for( size_t i = 0; i < len; i++ )
    r._val[i] = loword( off + i );
  r.normalize();
  return r;
}

void large::replace_partial( size_t off, size_t len, const large& v )
{
  for( size_t i = 0; i < len; i++ )
  {
    if( off + i >= _val.size() ) break;
    _val[off + i] = ( i < v._val.size() ) ? v._val[i] : 0;
  }
  normalize();
}

large::word_t large::loword( size_t off /* = 0 */ ) const
{
  if( _val.size() > off )
    return *( _val.begin() + off );
  return 0;
}

large::word_t large::hiword( size_t off /* = 0 */ ) const
{
  if( _val.size() > off )
    return *( _val.end() - 1 - off );
  return 0;
}


//============================================================================
//                                conversion methods
//============================================================================

int large::toInt() const
{
  int r = loword() & ( _MAXWORD >> 1 );
  return _neg ? -r : r;
}

large large::fromString( const char* str )
{
  large r;
  if( str != NULL )
  {
    bool neg = str[0] == '-';
    int  i = neg;

    if( str[i] == '0' && str[i+1] == 'x' )
    {
      r = fromHex( &str[i+2] );
    }
    else
    {
      for( ; str[i] != 0; i++ )
      {
        switch( str[i] )
        {
        default:
          goto wrongchar;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          r = r * 10 + ( str[i] - '0' );
        }
      }
      wrongchar: ;
    }

    if( neg ) r = -r;
  }
  return r;
}

string large::toString() const
{
  if( *this == 0 ) return "0";

  string s;
  large r( false, _val );

  for( ; r > 0; r /= 10 )
  {
    char c = ( r % 10 ).toInt();
    s.insert( s.begin(), '0' + c );
  }

  if( _neg ) s.insert( s.begin(), '-' );

  return s;
}

large large::fromHex( const char* str )
{
  large r;
  if( str != NULL )
  {
    for( int i = 0; str[i] != 0; i++ )
    {
      switch( str[i] )
      {
      default:
        goto wrongchar;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        r = r * 16 + ( str[i] - '0' );
        break;
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        r = r * 16 + ( str[i] - 'A' + 10 );
        break;
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        r = r * 16 + ( str[i] - 'a' + 10 );
        break;
      }
    }
    wrongchar: ;
  }
  return r;
}

string large::toHex() const
{
  if( *this == 0 ) return "00";

  string s;
  large r( false, _val );

  for( ; r > 0; r /= 16 )
  {
    char c = ( r % 16 ).toInt();
    if( c < 10 )
      s.insert( s.begin(), '0' + c );
    else
      s.insert( s.begin(), 'A' + c - 10 );
  }

  if( _neg ) s.insert( s.begin(), '-' );

  return s;
}

size_t large::toSignedBinary( unsigned char* bin, size_t size ) const
{
  // the binary format is:
  // bin[0] = sign, bin[1..n] = modulus (bigendian)

  if( bin == NULL || size < 2 ) return 0;

  bin[0] = _neg;
  return toUnsignedBinary( &bin[1], size - 1 ) + 1;
}

size_t large::toUnsignedBinary( unsigned char* bin, size_t size ) const
{
  // bin[0..n] = modulus (bigendian)

  if( bin == NULL || size < 1 ) return 0;

  size_t n = 0;
  for( size_t i = _val.size(); n < size && i > 0; i-- )
  {
    word_t w = _val[i-1];
    bin[n++] = w >> 24;
    if( n < size ) bin[n++] = w >> 16;
    if( n < size ) bin[n++] = w >> 8;
    if( n < size ) bin[n++] = w;
  }
  return n;
}

large large::fromSignedBinary( const unsigned char* bin, size_t size )
{
  large r;
  if( bin != NULL && size > 1 )
  {
    r = fromUnsignedBinary( &bin[1], size - 1 );
    r._neg = bin[0] != 0;
    r.normalize();
  }
  return r;
}

large large::fromUnsignedBinary( const unsigned char* bin, size_t size )
{
  large r;
  if( bin != NULL && size > 0 )
  {
    r._val.reserve( size / sizeof(word_t) + 1 );
    const size_t wn = sizeof(word_t);

    for( size_t i = size; i > 0; )
    {
      word_t w = 0;
      size_t off = (i < wn)? wn-i : 0;

      for( size_t j = off; j < wn; j++ )
      {
        w <<= 8;
        w += bin[ i+j-wn ];
      }

      i -= wn - off;
      r._val.push_back(w);
    }
    r.normalize();
  }
  return r;
}

//============================================================================
//                                extended arithmetics
//============================================================================
large large::modExp( const large& exponent, const large& modulus ) const
{
  large r  = 1;
  large base = *this;

  for( size_t i = 0; i < exponent._val.size(); i++ )
  {
    word_t le  = exponent._val[i];
    for( int k = 0; k < 32; k++, le /= 2 )
    {
      if( ( le & 1 ) > 0 )
        r = ( r * base ) % modulus;
      base = ( base ^ 2 ) % modulus;
    }
  }

  return r;
}
