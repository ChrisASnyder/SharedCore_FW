
#include "scVersionHelper.h"

// This variable must be defined in the main application. The build script will auto
// update the variable.
extern const AppVersionData_t Application_Version ;


/*

Create a file in your main project version.c and add the following code. This will
then be modified by the versioning application to create the version data based on
the rules.

#ifdef _DEBUG
    #define REV_LABEL "---"
#else
    #define REV_LABEL "---"
#endif	

const char g_pVersionLabel []    PROGMEM = REV_LABEL;

const AppVersionData_t Application_Version =
{
    1,
    0,
    228,
    g_pVersionLabel
};


*/