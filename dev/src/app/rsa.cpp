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

#include <memory>
#include "rsa.h"

namespace RSA
{

string Rsa::encrypt( const char* message ) const
{
  string  so;
  // RSAES-PKCS1-V1_5-ENCRYPT

  size_t  k  = _publicKey._size;
  unsigned char*  buf  = new unsigned char[k];
  std::auto_ptr<unsigned char>  guardBuf(buf);

  for( size_t i = 0; i < strlen(message); )
  {
    size_t  mLen  = strlen( &message[i] );
    if( mLen > k - 11 )  mLen  = k - 11;

    buf[0]  = 0;
    buf[1]  = 2;
    memset( &buf[2], -1, k - mLen - 3 );  // FIXME: must be random
    buf[ k - mLen - 1 ]  = 0;
    memcpy( &buf[k-mLen], &message[i], mLen  );
    i  += mLen;

    large  EM  = large::fromUnsignedBinary( buf, k );
    large  C  = EM.modExp( _publicKey._e, _publicKey._n );

    so  += C.toHex();
  }

  return  so;
}

string Rsa::decrypt( const char* message ) const
{
  string  so;
  // RSAES-PKCS1-V1_5-DECRYPT

  size_t  k  = _privateKey._size;
  unsigned char*  buf  = new unsigned char[k*2+1];
  std::auto_ptr<unsigned char>  guardBuf(buf);

  for( size_t i = 0; i < strlen(message); )
  {
    size_t  mLen  = strlen( &message[i] );
    if( mLen > k*2 )  mLen  = k*2;
    else
      if( mLen != k*2 )  return  "decrypton error";

    memcpy( buf, &message[i], mLen );
    buf[mLen]  = 0;
    i  += mLen;

    large  C  = large::fromHex( (const char*) &buf[0] );
    large  EM  = C.modExp( _privateKey._e, _privateKey._n );
    size_t  n  = EM.toUnsignedBinary( buf, k );
    buf[n]  = 0;

    if( n < 11 || buf[0] != 0 || buf[1] != 2 )  return "decrypton error";
    size_t  j  = 2;
    for( ; j < n && buf[j] != 0; j++ );
    if( j < 10 || j >= n )  return "decrypton error";

    so  += (char*) &buf[++j];
  }

  return  so;
}

}  // namespace RSA
