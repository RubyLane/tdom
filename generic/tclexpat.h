#include <expat.h>

/*--------------------------------------------------------------------------
|   Function prototypes
|
\-------------------------------------------------------------------------*/

#if defined(_MSC_VER) || defined(BUILD_tdom) || defined(__MINGW32__) 
#  undef TCL_STORAGE_CLASS
#  define TCL_STORAGE_CLASS DLLEXPORT
#endif

Tcl_ObjCmdProc TclExpatObjCmd;

int CheckExpatParserObj (Tcl_Interp *interp,
 			Tcl_Obj *const nameObj);
int CHandlerSetInstall (Tcl_Interp *interp,
 		       Tcl_Obj *const expatObj,
 		       CHandlerSet *handlerSet);
int CHandlerSetRemove (Tcl_Interp *interp,
 		      Tcl_Obj *const expatObj,
 		      char *handlerSetName);
CHandlerSet * CHandlerSetCreate (char *name);
CHandlerSet * CHandlerSetGet (Tcl_Interp *interp,
                              Tcl_Obj *const expatObj,
 			     char *handlerSetName);
void * CHandlerSetGetUserData (Tcl_Interp *interp,
                               Tcl_Obj *const expatObj,
 			      char *handlerSetName);

TclGenExpatInfo * GetExpatInfo (Tcl_Interp *interp,
				       Tcl_Obj *const expatObj);
