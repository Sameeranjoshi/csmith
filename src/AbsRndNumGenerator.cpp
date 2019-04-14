// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008, 2009, 2010, 2011, 2015, 2017 The University of Utah
// All rights reserved.
//
// This file is part of `csmith', a random generator of C programs.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "AbsRndNumGenerator.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "DefaultRndNumGenerator.h"
#include "DFSRndNumGenerator.h"

using namespace std;
//     These are linux kernel functions:
//     srand48() = function is initialization function(it initializes the initial value of seed)
//      which  should  be  called  before  using lrand48()
//     lrand48() = functions return nonnegative long integers
//      uniformly distributed over the interval [0, 2^31).
#ifndef HAVE_LRAND48
extern "C" {
	extern void srand48(long seed);
	extern long lrand48(void);
}
#endif

const char *AbsRndNumGenerator::hex1 = "0123456789ABCDEF";

const char *AbsRndNumGenerator::dec1 = "0123456789";

AbsRndNumGenerator::AbsRndNumGenerator()
{
	//Nothing to do
}

AbsRndNumGenerator::~AbsRndNumGenerator()
{
	//Nothing to do
}

/*
 * Factory method to create random number generators.
	SELECTS THE TYPE OF GENERATOR BASED ON PARAMTER PASSED TO IT
	GENERATOR TYPES CAN BE:
	1.Default
	2. DFS
	3.SimpleDelta
 */
AbsRndNumGenerator*
AbsRndNumGenerator::make_rndnum_generator(RNDNUM_GENERATOR impl, const unsigned long seed)
{
	AbsRndNumGenerator *rImpl = 0;
	//calls srand48() before calling the drand48() as its an initialization function
	AbsRndNumGenerator::seedrand(seed);
	switch (impl) {
		case rDefaultRndNumGenerator:
			rImpl = DefaultRndNumGenerator::make_rndnum_generator(seed);
			break;
		case rDFSRndNumGenerator:
			rImpl = DFSRndNumGenerator::make_rndnum_generator();
			break;
		default:
			assert(!"unknown random generator");
			break;
	}

	return rImpl;
}
/*
   Use :
	calls srand48() before calling the lrand48()
   Parameters:
	seed value

   Returns:
 *
 */
void
AbsRndNumGenerator::seedrand(const unsigned long seed )
{
#ifdef HAVE_SRAND48_DETERMINISTIC
	// OpenBSD requires a special call to activate the standard,
	// deterministic behavior of `lrand48'.
	srand48_deterministic(seed);
#else
	srand48(seed);
#endif
}

/*
 * Return random shuffled integers in set [0...n]
 * Note: deprecated.
 */
#if 0
unsigned int*
AbsRndNumGenerator::rnd_shuffle(unsigned int n)
{
	unsigned int* ary = new unsigned int[n];
	unsigned int i = 0;
	unsigned int tmp;

	for (i = 0; i < n; i++) {
		ary[i] = i;
	}

	for (i = 0; i < n; i++) {
		unsigned int j = rnd_upto(i);
		tmp = ary[j];
		ary[j] = ary[i];
		ary[i] = tmp;
	}
	return ary;
}
#endif

/*
   Use :
	calls lrand48() and finally generates a random number
   Parameters:
	initially srand48() needs to be called

   Returns:
        random number
 *
 */

unsigned long
AbsRndNumGenerator::genrand(void)
{
	return lrand48();
}
/*
   Use :
	 loop(from  0 to  num of times)
		From hex1 array randomly chooses an element and appends to the str
   Parameters:
        num = size of the random number user wants
	ex num = 5 ----> 39A92
	ex num = 2 ----> 2D

   Returns:
        a newly created random number from hex1
 *
 */

std::string
AbsRndNumGenerator::RandomHexDigits( int num )
{
	std::string str;
	while ( num-- )
	{
		str += hex1[genrand()%16];
	}

	return str;
}
/*
   Use :
	 loop(from  0 to  num of times)
		From dec1 array randomly chooses an element and appends to the str
   Parameters:
        num = size of the random number user wants
	ex num = 5 ----> 39492 
	ex num = 2 ----> 25

   Returns:
        a newly created random number from dec1
 *
 */

std::string
AbsRndNumGenerator::RandomDigits( int num )
{
	std::string str;
	while ( num-- )
	{
		str += dec1[genrand()%10];
	}

	return str;
}
//returns the hex1 string declared at top
const char *
AbsRndNumGenerator::get_hex1()
{
	return AbsRndNumGenerator::hex1;
}
//returns the dec1 string declared at top
const char *
AbsRndNumGenerator::get_dec1()
{
	return AbsRndNumGenerator::dec1;
}

