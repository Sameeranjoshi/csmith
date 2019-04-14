// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008, 2010, 2011, 2015, 2017 The University of Utah
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

#include "DefaultRndNumGenerator.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Filter.h"
#include "SequenceFactory.h"
#include "Sequence.h"
#include "CGOptions.h"

DefaultRndNumGenerator *DefaultRndNumGenerator::impl_ = 0;

/*
 *	It is constructor which is used to create new generator.

	rand_depth_ = 0
	trace_string_ = ""
	Sequence pointer to seq_ = for ex. it can be LinearSequence 
 */
DefaultRndNumGenerator::DefaultRndNumGenerator(const unsigned long, Sequence *concrete_seq)
	: rand_depth_(0),
	  trace_string_(""),
	  seq_(concrete_seq)
{
	//Nothing to do
}

/*
 *	It destroys seqs_ = [...|...|...|...] which is set that stores instances of Sequence
 */
DefaultRndNumGenerator::~DefaultRndNumGenerator()
{
	SequenceFactory::destroy_sequences();
}

/*
 * Create singleton instance.
	impl_ is pointer holding generator
 */
DefaultRndNumGenerator*
DefaultRndNumGenerator::make_rndnum_generator(const unsigned long seed)
{
	if (impl_)
		return impl_;

	Sequence *seq = SequenceFactory::make_sequence();

	impl_ = new DefaultRndNumGenerator(seed, seq);
	assert(impl_);
	//impl_->seedrand(seed);

	return impl_;
}

/*
 * Return the sequence of random choices
   Use - It appends '_' for n-1 values of the map (seq_map_) and
        at end appends the last value
        ex. step 1 --> 1_2_3_
            step 2 --> 1_2_3_4 where 1,2,3,4 are the values in map

	and return "1_2_3_4"

   Data structure used - seq_map <int,int>	(in LinearSequence class)

 */
void
DefaultRndNumGenerator::get_sequence(std::string &sequence)
{
	std::ostringstream ss;
	seq_->get_sequence(ss);
	sequence = ss.str();
}

/*
	In Default Program Generator, it returns same string.
	In DFS Program Generator, it processes the string and then return
*/
std::string
DefaultRndNumGenerator::get_prefixed_name(const std::string &name)
{
	return name;
}
void
DefaultRndNumGenerator::add_number(int v, int bound, int k)
{
}

/*
 * Return a random number from  0..(n-1) with help of genrand() i.e. lrand48() - selects val from [0, 2^31)
        and implicilty assigns value to  trace_string_
DOUBT:	What is significane of *where string?
 */
unsigned int
DefaultRndNumGenerator::rnd_upto(const unsigned int n, const Filter *f, const std::string *where)
{
	static int g = 0;
	int h = g;
	if (h == 440)
		BREAK_NOP;   // for debugging
	unsigned int v = genrand() % n;
	unsigned INT64 local_depth = rand_depth_;
	rand_depth_++;
	//ofstream out("rnd.log", ios_base::app);
	//out << g++ << ": " << v << "(" << n << ")" << endl;

	if (f) {
		while (f->filter(v)) {
			// We could add numbers into sequence inside the previous filter.
			// If the previous filter failed, we need to roll back the rand_depth_ here.
			// This will also overwrite the value added in the map.
			rand_depth_ = local_depth+1;
			v = genrand() % n;
			/*out << g++ << ": " << v << "(" << n << ")" << endl;*/
		}
	}
	//out.close();
	if (where) {
	std::ostringstream ss;
		ss << *where << "->";
	trace_string_ += ss.str();
	}
	add_number(v, n, local_depth);
	return v;
}

/*
 * Return `true' p% of the time.
 */
bool
DefaultRndNumGenerator::rnd_flipcoin(const unsigned int p, const Filter *f, const std::string *)
{
	assert(p <= 100);
	unsigned INT64 local_depth = rand_depth_;
	rand_depth_++;
	if (f) {
		if (f->filter(0)) {
			add_number(1, 2, local_depth);
			return true;
		}
		else if (f->filter(1)) {
			add_number(0, 2, local_depth);
			return false;
		}
	}

	bool rv = (genrand() % 100) < p;
	if (rv) {
		add_number(1, 2, local_depth);
	}
	else {
		add_number(0, 2, local_depth);
	}
	return rv;
}

/*
	Use - It returns string "" appending "->" to it  from rnd_upto(). Check later, not sure about it
*/
std::string &
DefaultRndNumGenerator::trace_depth()
{
	return trace_string_;
}

/*
	Use - It calls lrand48() function which select a number from [0, 2^31) based on initial value.

	Note - srand48() does some complexation and assigns initial value to lrand48().

	Return - long between [0,2^31)
*/
unsigned long
DefaultRndNumGenerator::genrand(void)
{
	return AbsRndNumGenerator::genrand();
}

/*
        Use - It generates string of random digits in 2 ways using hex1 = "0123456789ABCDEF" as follows - 

              1. command line set is random - does computation on hex1 to generate random digit

              2. command line set is not random - calls abs to generate random digit of size num

        Input - Size of random digit.
                e.g. 2 = 1B
                     5 = 23ABC

        Return - string (e.g. 1B)
*/
std::string
DefaultRndNumGenerator::RandomHexDigits( int num )
{
	if (!CGOptions::is_random())
		return AbsRndNumGenerator::RandomHexDigits(num);

	std::string str;
	const char* hex1 = AbsRndNumGenerator::get_hex1();
	while (num--) {
		int x = genrand() % 16;
		str += hex1[x];
		seq_->add_number(x, 16, rand_depth_);
		rand_depth_++;
	}
	return str;
}

/*
	Use - It generates string of random digits in 2 ways using dec1 = "0123456789" as follows - 

	      1. command line set is random - does computation on dec1 to generate random digit

	      2. command line set is not random - calls abs to generate random digit of size num

	Input - Size of random digit.
		e.g. 2 = 23
		     5 = 32874

	Return - string (e.g. 23)
*/
std::string
DefaultRndNumGenerator::RandomDigits( int num )
{
	if (!CGOptions::is_random())
		return AbsRndNumGenerator::RandomDigits(num);

	std::string str;
	const char* dec1 = AbsRndNumGenerator:: get_dec1();
	while (num--) {
		int x = genrand() % 10;
		str += dec1[x];
		seq_->add_number(x, 10, rand_depth_);
		rand_depth_++;
	}
	return str;
}

