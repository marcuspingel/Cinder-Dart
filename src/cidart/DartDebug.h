// Copyright (c) 2013, Richard Eakin and the Dart project authors.
// Use of this source code (and the Dart itself) is governed by a
// BSD-style license that can be found in the LICENSE.txt file.

#pragma once

#include "cinder/app/App.h"

#define CIDART_LOG_VERBOSE 0

#if CIDART_LOG_VERBOSE
	#define LOG_V( stream )			{ std::cout << __PRETTY_FUNCTION__ << " |V| " << stream << std::endl; }
#else
	#define LOG_V( stream )
#endif

#define LOG_I( stream )				{ std::cout << __PRETTY_FUNCTION__ << " |I| " << stream << std::endl; }
#define LOG_E( stream )				{ std::cout << __PRETTY_FUNCTION__ << " |ERROR| " << stream << std::endl; }

#include <boost/assert.hpp>

#define CI_ASSERT(expr) BOOST_ASSERT(expr)

#include "dart_api.h"

#define CIDART_CHECK(result)						\
{													\
	if (Dart_IsError(result)) {						\
		LOG_E( Dart_GetError(result) );				\
		CI_ASSERT( 0 );								\
	}												\
}

#define CIDART_CHECK_RETURN(result)					\
{													\
	if (Dart_IsError(result)) {						\
		LOG_E( Dart_GetError(result) );				\
		return;										\
	}												\
}
