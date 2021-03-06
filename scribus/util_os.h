/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _UTIL_OS_H
#define _UTIL_OS_H


#include "scribusapi.h"

bool SCRIBUS_API os_is_osx();
bool SCRIBUS_API os_is_win();
bool SCRIBUS_API os_is_win64();
bool SCRIBUS_API os_is_unix();

#endif
