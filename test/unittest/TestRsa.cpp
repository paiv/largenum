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
#include "rsa.h"

void MathTestCase::testRsa()
{
	const char	message[]	=
		"Папа у Васи силён в математике.";

	// 128 bit
	{
		const char	modulus[]			= "240678700919950932854873583899826809119";
		const char	publicExponent[]	= "65537";
		const char	privateExponent[]	= "215078334346672052913003310680037266401";

		RSA::Rsa	rsa1, rsa2;
		RSA::Key	publicKey( modulus, publicExponent );
		RSA::Key	privateKey( modulus, privateExponent );

		rsa1.setPublicKey( publicKey );
		rsa2.setPrivateKey( privateKey );

		string	sc	= rsa1.encrypt( message );
		string	sm	= rsa2.decrypt( sc.c_str() );
		CPPUNIT_ASSERT( sm == message );
	}

	// 1024 bit
	{
		const char	modulus[]	=
			"0x"
			"a8b3b284af8eb50b387034a860f146c4919f318763cd6c5598c8ae4811a1e0ab"
			"c4c7e0b082d693a5e7fced675cf4668512772c0cbc64a742c6c630f533c8cc72"
			"f62ae833c40bf25842e984bb78bdbf97c0107d55bdb662f5c4e0fab9845cb514"
			"8ef7392dd3aaff93ae1e6b667bb3d4247616d4f5ba10d4cfd226de88d39f16fb";
		const char	publicExponent[]	=
			"0x010001";
		const char	privateExponent[]	=
			"0x"
			"53339cfdb79fc8466a655c7316aca85c55fd8f6dd898fdaf119517ef4f52e8fd"
			"8e258df93fee180fa0e4ab29693cd83b152a553d4ac4d1812b8b9fa5af0e7f55"
			"fe7304df41570926f3311f15c4d65a732c483116ee3d3d2d0af3549ad9bf7cbf"
			"b78ad884f84d5beb04724dc7369b31def37d0cf539e9cfcdd3de653729ead5d1";

		RSA::Rsa	rsa1, rsa2;
		RSA::Key	publicKey( modulus, publicExponent );
		RSA::Key	privateKey( modulus, privateExponent );

		rsa1.setPublicKey( publicKey );
		rsa2.setPrivateKey( privateKey );

		string	sc	= rsa1.encrypt( message );
		string	sm	= rsa2.decrypt( sc.c_str() );
		CPPUNIT_ASSERT( sm == message );
	}
}
