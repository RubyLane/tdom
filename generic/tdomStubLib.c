/*----------------------------------------------------------------------------
|   Copyright (c) 2007 Rolf Ade (rolf@pointsman.de)
+-----------------------------------------------------------------------------
|
|   Implements entry point, which has to be called by C coded extensions
|   to tDOM. Following http://wiki.tcl.tk/3358.
|
|   The contents of this file are subject to the Mozilla Public License
|   Version 2.0 (the "License"); you may not use this file except in
|   compliance with the License. You may obtain a copy of the License at
|   http://www.mozilla.org/MPL/
|
|   Software distributed under the License is distributed on an "AS IS"
|   basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
|   License for the specific language governing rights and limitations
|   under the License.
|
|   The Original Code is tDOM.
|
|   The Initial Developer of the Original Code is Jochen Loewer
|   Portions created by Jochen Loewer are Copyright (C) 1998, 1999
|   Jochen Loewer. All Rights Reserved.
|
|   Contributor(s):
|
|
|   written by Rolf Ade
|   August, 2007
|
\---------------------------------------------------------------------------*/

#undef USE_TCL_STUBS
#undef USE_EXA_STUBS
#define USE_TCL_STUBS 1
#define USE_EXA_STUBS 1

#include "tdom.h"

MODULE_SCOPE const TdomStubs *tdomStubsPtr;
const TdomStubs *tdomStubsPtr = NULL;

/*----------------------------------------------------------------------------
|   Tdom_InitStubs
|
\---------------------------------------------------------------------------*/

#undef TdomInitializeStubs
MODULE_SCOPE const char *
TdomInitializeStubs (
    Tcl_Interp *interp, 
    int exact
    )
{
    const char *actualVersion;
    TdomStubs *stubsPtr = NULL;

#if (TCL_MAJOR_VERSION == 8) && (TCL_MINOR_VERSION == 0)
    Tcl_SetResult(interp, "Too old Tcl version. Binary extensions "
                  "to tDOM are not possible, with a that outdated "
                  "Tcl version.", TCL_STATIC);
    return NULL;
#else
    actualVersion = tclStubsPtr->tcl_PkgRequireEx(interp, PACKAGE_NAME,
            PACKAGE_VERSION, exact, &stubsPtr);

    //fprintf(stderr, "stubs require of %s, got stubsPtr: %p\n", PACKAGE_NAME, stubsPtr);
    if (!actualVersion) {
        return NULL;
    }
    if (!stubsPtr) {
        Tcl_SetResult(interp, "This implementation of Tdom does not "
                      "support stubs", TCL_STATIC);
        return NULL;
    }
    
    tdomStubsPtr = stubsPtr;

    return actualVersion;
#endif
}
