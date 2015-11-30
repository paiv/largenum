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

#pragma	once

template< class T >
class pod_vector
{
public:
	typedef	const T&	const_reference;
	typedef	const T*	const_iterator;
	typedef	T&	reference;
	typedef	T*	iterator;

public:
	pod_vector();
	~pod_vector()											{	clear();				}
	pod_vector( const pod_vector& v );
	pod_vector&	operator=( const pod_vector& v )			{	copyfrom(v); return *this;	}

	void	push_back( const T& v );
	void	insert( const_iterator where, const T& v );
	void	erase( iterator first, iterator last );

	const_reference		operator[]( size_t pos ) const		{	return	_data[pos];		}
	const_iterator		begin() const						{	return	&_data[0];		}
	const_iterator		end() const							{	return	&_data[_used];	}
	reference			operator[]( size_t pos );
	iterator			begin()								{	return	&_data[0];		}
	iterator			end()								{	return	&_data[_used];	}

	size_t	size() const									{	return	_used;		}
	void	reserve( size_t size );
	void	resize( size_t size );

	bool	operator==( const pod_vector& v ) const;

protected:
	T*		_data;
	size_t	_size;
	size_t	_used;

protected:
	void	clear();
	void	copyfrom( const pod_vector& v );
};

#include "pod_vector.inl"
