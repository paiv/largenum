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
//  class Rsa
//
//  An example of RSA implementation based on large integer class.
//   by : fuurin <fuurin@gmx.net>, 2004
//============================================================================

#pragma	once
#include "large.h"

namespace RSA
{

class Key
{
	friend	class Rsa;
public:
	Key()	{}

	Key( const char* modulus, const char* exponent )
	{
		_n		= large::fromString( modulus );
		_e		= large::fromString( exponent );
		_size	= ( _n.bitCount() + 7 ) / 8;
	}

private:
	size_t	_size;
	large	_n;
	large	_e;
};

class Rsa
{
public:
	void	setPublicKey( const Key& key )				{	_publicKey = key;		}
	void	setPrivateKey( const Key& key )				{	_privateKey = key;		}

	string	encrypt( const char* message ) const;
	string	decrypt( const char* message ) const;

private:
	Key		_privateKey;
	Key		_publicKey;
};

}	// namespace RSA
