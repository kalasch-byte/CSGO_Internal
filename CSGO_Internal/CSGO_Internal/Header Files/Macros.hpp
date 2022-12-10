#pragma once

#include "Includes.hpp"

#define DEALLOC( x ) \
{ \
	if (x != nullptr) \
	    delete x; \
} \

#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(size) BYTE MACRO_CONCAT(_pad, __COUNTER__)[size];

#define LERP( variable, state, percent_float, min_alpha, max_alpha ) \
{ \
	variable = std::clamp( variable + ( state ? (int)( 1.f / percent_float * g_csgo.m_globals->m_frametime * 255 ) \
											  : -(int)( 1.f / percent_float * g_csgo.m_globals->m_frametime * 255 ) ), min_alpha, max_alpha ); \
} \

#define SLEEP(ms) \
{ \
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));	\
}