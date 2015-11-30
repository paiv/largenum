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


#include "stdafx.h"
#include "TestMath.h"
#include "large.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MathTestCase );

void MathTestCase::setUp()
{
}

void MathTestCase::testStr()
{
	const char	sa[]	=
		"a2ba40ee07e3b2bd2f02ce227f36a195024486e49c19cb41bbbdfbba98b22b0e"
		"577c2eeaffa20d883a76e65e394c69d4b3c05a1e8fadda27edb2a42bc000fe88"
		"8b9b32c22d15add0cd76b3e7936e19955b220dd17d4ea904b1ec102b2e4de775"
		"1222aa99151024c7cb41cc5ea21d00eeb41f7c800834d2c6e06bce3bce7ea9a5";
	const char	sb[]	=
		"1142711739573838416547593099003995904539601664546729315709986206"
		"7563668954002551393202351277151441009251035352607468725381647258"
		"1386512404723160802610417749684340444785376595696649176884149771"
		"7300629102344525781130432940163084200315122801341316406534835444"
		"91686103567479056477782535675663257592116025533311397";
	const char	sc[]	=
		"0x"
		"A2BA40EE07E3B2BD2F02CE227F36A195024486E49C19CB41BBBDFBBA98B22B0E"
		"577C2EEAFFA20D883A76E65E394C69D4B3C05A1E8FADDA27EDB2A42BC000FE88"
		"8B9B32C22D15ADD0CD76B3E7936E19955B220DD17D4EA904B1EC102B2E4DE775"
		"1222AA99151024C7CB41CC5EA21D00EEB41F7C800834D2C6E06BCE3BCE7EA9A5";

	large	a	= large::fromHex(sa);
	large	b	= large::fromString(sb);
	large	c	= large::fromString(sc);
	CPPUNIT_ASSERT( a == b );
	CPPUNIT_ASSERT( b == c );

	unsigned char	ac[]	= { 0xCF, 0xE0, 0xEF, 0xE0, 0x20, 0x00, 0x01, 0x08,
								0xDA, 0x34, 0x46, 0xAA, 0x51, 0x29, 0x02, 0x00 };
	unsigned char	bc[1024];
	size_t	n;
	string	s;

	a	= large::fromUnsignedBinary( ac, sizeof(ac)/sizeof(ac[0]) );
	CPPUNIT_ASSERT( a == large::fromString( "276318134876696588224268584557570753024" ) );

	n	= a.toUnsignedBinary( bc, sizeof(bc)/sizeof(bc[0]) );
	CPPUNIT_ASSERT( n == sizeof(ac)/sizeof(ac[0]) );
	CPPUNIT_ASSERT( memcmp( ac, bc, n ) == 0 );

	s	= a.toHex();
	b	= large::fromHex( s.c_str() );
	n	= b.toUnsignedBinary( bc, sizeof(bc)/sizeof(bc[0]) );
	CPPUNIT_ASSERT( n == sizeof(ac)/sizeof(ac[0]) );
	CPPUNIT_ASSERT( memcmp( ac, bc, n ) == 0 );
}

void MathTestCase::testAdd()
{
	large	a;
	large	b(0);
	CPPUNIT_ASSERT( a + b == 0 );
	CPPUNIT_ASSERT( a - b == 0 );
	CPPUNIT_ASSERT( (large)-1 < 0 );

	string		sa, sb, sc;
	sa	= a.toString();
	sb	= b.toString();
	CPPUNIT_ASSERT( sa == "0" );
	CPPUNIT_ASSERT( sa == sb );

	a += 1;
	CPPUNIT_ASSERT( a == 1 );
	CPPUNIT_ASSERT( a == b + 1 );
	CPPUNIT_ASSERT( a - 1 == b );
	CPPUNIT_ASSERT( a - 2 == -1 );
	CPPUNIT_ASSERT( a + 0 == a );
	CPPUNIT_ASSERT( a - 0 == a );
	sa	= a.toString();
	sb	= (b + 1).toString();
	sc	= (a - 2).toString();
	CPPUNIT_ASSERT( sa == "1" );
	CPPUNIT_ASSERT( sa == sb );
	CPPUNIT_ASSERT( sc == "-1" );

	a	= 0xFFFFFFFF;
	b	= a;
	a	+= b;
	CPPUNIT_ASSERT( a - 0xFFFFFFFF == b );
	sa	= a.toString();
	sb	= (b + 0xFFFFFFFF).toString();
	CPPUNIT_ASSERT( sa == "8589934590" );
	CPPUNIT_ASSERT( sa == sb );

	unsigned char	val[]	= { 0, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
								0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC };
	large	c	= large::fromSignedBinary( val, sizeof(val)/sizeof(val[0]) );
	sc	= c.toString();
	CPPUNIT_ASSERT( sc == "5846006549323611672814739330865132078623730171900" );
	CPPUNIT_ASSERT( a + c  == b + 0xFFFFFFFF + c );
	sa	= ( a + c ).toString();
	sb	= ( b + 0xFFFFFFFF + c ).toString();
	CPPUNIT_ASSERT( sa == "5846006549323611672814739330865132078632320106490" );
	CPPUNIT_ASSERT( sa == sb );

	a	= 0xFFFFFFFF;
	b	= a;
	a	+= b;
	b	= a + b;
	sb	= b.toString();
	CPPUNIT_ASSERT( sb == "12884901885" );
	CPPUNIT_ASSERT( b == a + 0xFFFFFFFF );
	CPPUNIT_ASSERT( (large)0xFFFFFFFF - b == -a );
}

void MathTestCase::testShift()
{
	large	a(0);
	CPPUNIT_ASSERT( a << 0 == a );
	CPPUNIT_ASSERT( a >> 0 == a );
	CPPUNIT_ASSERT( a << 3 == 0 );
	CPPUNIT_ASSERT( a >> 5 == 0 );

	large	b( 0x3FFFFFFC );
	large	c	= large::fromString("0x3FFFFFFC00");
	CPPUNIT_ASSERT( b << 0 == b );
	CPPUNIT_ASSERT( b << 2 == 0xFFFFFFF0 );
	CPPUNIT_ASSERT( b << 8 == c );
	CPPUNIT_ASSERT( c >> 2 == large::fromString("0xFFFFFFF00") );
	CPPUNIT_ASSERT( c >> 8 == b );
}

void MathTestCase::testMul()
{
	large	a(0);
	large	b(0);
	CPPUNIT_ASSERT( a * b == 0 );
	CPPUNIT_ASSERT( a * 10 == 0 );

	a	= 1;
	b	= 1;
	CPPUNIT_ASSERT( a * b == 1 );
	CPPUNIT_ASSERT( b * a == 1 );
	CPPUNIT_ASSERT( a * 10 == 10 );

	b	= -2;
	CPPUNIT_ASSERT( a * b == -2 );
	CPPUNIT_ASSERT( b * a == -2 );

	a	= 0xFFFFFFFF;
	a	*= b;
	CPPUNIT_ASSERT( b * 0xFFFFFFFF == a );
	CPPUNIT_ASSERT( a / b == 0xFFFFFFFF );

	a	= 10;
	b	= 1;
	try
	{
		b	= a / 0;
		CPPUNIT_FAIL( "division by zero should raise exception" );
	}
	catch(...)
	{
	}
	CPPUNIT_ASSERT( b == 1 );

	a	= 0xFFFFFFFF;
	b	= a + 1;
	large	c	= a;
	c	= c * b + a;
	c	= c * b + a;
	c	= c * b + a;
	c	= c * b + a;
	string	sc	= c.toString();
	CPPUNIT_ASSERT( sc == "1461501637330902918203684832716283019655932542975" );

	large	t	= c;
	c	= a;
	c	= c * b + a;
	c	= c * b + a;
	c	= c * b + a;
	CPPUNIT_ASSERT( ( t - a ) / b == c );
}

void MathTestCase::testMod()
{
	large	a(2);
	large	b(3);
	large	c(3), r;
	CPPUNIT_ASSERT( a % b == 2 );
	CPPUNIT_ASSERT( b % c == 0 );

	a	-= 4;
	CPPUNIT_ASSERT( a % b == a );

	a	= large::fromString( "1461501637330902918203684832716283019655932542975" );
	b	= large::fromString( "79823759843" );
	c	= a % b;
	CPPUNIT_ASSERT( c.toString() == "15341857946" );
	CPPUNIT_ASSERT( ( a - c ) % b == 0 );

	a	= large::fromString( "146150163733" );
	b	= large::fromString( "7785350250401" );
	c	= ( a ^ 15 ) % b;
	CPPUNIT_ASSERT( c.toString() == "1728887337866" );
	CPPUNIT_ASSERT( a.modExp( 15, b ) == c );

	a	= large::fromString( "1098570193587091823740912384709182355" );
	b	= large::fromString( "8966" );
	c	= large::fromString( "2304973180982139048123491872364819723" );
	r	= large::fromString( "1687519362357607938552469522927673660" );
	CPPUNIT_ASSERT( a.modExp( b, c ) == r );
}
