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
//  class pod_vector
//
//  Vector class for POD types.
//   by : fuurin <fuurin@gmx.net>, 2004
//============================================================================

#pragma once

template< class T >
pod_vector<T>::pod_vector() :
  _size(0),
  _used(0),
  _data(NULL)
{
}

template< class T >
pod_vector<T>::pod_vector( const pod_vector& v ) :
  _size(0),
  _used(0),
  _data(NULL)
{
  copyfrom(v);
}

template< class T >
void pod_vector<T>::push_back( const T& v )
{
  if( _size == _used )
    reserve( _size + 1 );
  _data[_used++] = v;
}

template< class T >
void pod_vector<T>::insert( const_iterator where, const T& v )
{
  size_t pos = where - begin();

  if( _size == _used )
    reserve( _size + 1 );

  if( _used > 0 )
    memmove( &_data[pos+1], &_data[pos], ( _used - pos ) * sizeof(T) );

  _data[pos] = v;
  _used++;
}

template< class T >
void pod_vector<T>::erase( iterator first, iterator last )
{
  if( last == end() )
  {
    _used = first - begin();
    return;
  }

  size_t pos = first - begin();
  size_t n = end() - last;

  if( n > 0 )
    memmove( first, last, n * sizeof(T) );

  _used = pos + n;
}

template< class T >
void pod_vector<T>::clear()
{
  _size = 0;
  _used = 0;
  if( _data != NULL )
  {
    delete[] _data;
    _data = NULL;
  }
}

template< class T >
typename pod_vector<T>::reference pod_vector<T>::operator[]( size_t pos )
{
  if( _used < pos + 1 ) _used = pos + 1;
  return _data[pos];
}

template< class T >
void pod_vector<T>::reserve( size_t size )
{
  if( size > _size )
  {
    size = ( size + 7 ) & ~7; // round up to 8
    T* p = new T[size];
    if( _used > 0 )
      memcpy( p, _data, _used * sizeof(T) );

    delete[] _data;
    _data = p;
    _size = size;
  }
}

template< class T >
void pod_vector<T>::resize( size_t size )
{
  reserve( size );
  if( size > _used )
    memset( &_data[_used], 0, ( size - _used ) * sizeof(T) );
  _used = size;
}

template< class T >
bool pod_vector<T>::operator==( const pod_vector& v ) const
{
  return _used == v._used &&
    ( _used == 0 || memcmp( _data, v._data, _used * sizeof(T) ) == 0 );
}

template< class T >
void pod_vector<T>::copyfrom( const pod_vector& v )
{
  if( v._used == 0 )
  {
    clear();
    return;
  }

  reserve( v._used );
  _used = v._used;
  memcpy( _data, v._data, _used * sizeof(T) );
}
