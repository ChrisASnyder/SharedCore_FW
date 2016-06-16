#pragma once

#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif

#define SC_SUPPORTS_CRT_SECURITY