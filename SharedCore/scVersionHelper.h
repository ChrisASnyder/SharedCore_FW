
#ifndef __SCVERSION_HELPER_H
#define __SCVERSION_HELPER_H

#include "scTypes.h"

#define MAX_LABEL_SIZE 32

#pragma pack(1)


/// <summary>
/// This structure is used to store the version information for the firmware binary data. The const variable
/// is declared in the code file. A utility application is used to generate the new version number by parsing
/// and updating the version macros that are used to build the structure.
/// </summary>
typedef struct
{
	uint16_t		_Major;
	uint16_t		_Minor;
	uint32_t		_Build;
	const char*		_pLabel;
} AppVersionData_t;
#pragma pack()

extern const AppVersionData_t Application_Version;

#endif // !defined(__SCVERSION_HELPER_H)
