// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008, 2009, 2010, 2011, 2017 The University of Utah
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

#include "SequenceFactory.h"

#include <cassert>
#include <iostream>
#include <string>
#include "LinearSequence.h"

// set data structure = no repetation of duplicate elements
// seqs_ = [...|...|...|...]  <------- a set data structure
//	     ^
//	     |
//	     |____(instance of Sequence class)
//
std::set<Sequence*> SequenceFactory::seqs_;

char SequenceFactory::current_sep_char_ = '_';
/*
   Use :
	generates current_sep_char_ and Sequence class object and inserts into the seqs_ set
	Sequence class instance can be of-
		1.DeltaMonitor class
		2.LinearSequence class
   Returns:
        The generated sequence class instance
 */
Sequence*
SequenceFactory::make_sequence()
{
	Sequence *seq = NULL;
	seq = new LinearSequence(LinearSequence::default_sep_char);
	current_sep_char_ = LinearSequence::default_sep_char;

	assert(seq);
	seqs_.insert(seq);
	return seq;
}
/*
   Use :
	deletes the dynamic memory allocated to the seqs_ set
 */

void
SequenceFactory::destroy_sequences()
{
	std::set<Sequence*>::iterator i;
	for(i = seqs_.begin(); i != seqs_.end(); ++i) {
		delete *i;
	}
	seqs_.clear();
}

