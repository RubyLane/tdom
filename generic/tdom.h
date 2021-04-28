#ifndef _TDOM_H
#define _TDOM_H

#include <tcl.h>
#include <expat.h>

/*
 * If compiled against threaded Tcl core, we must take
 * some extra care about process-wide globals and the
 * way we name Tcl object accessor commands.
 */
#ifndef TCL_THREADS
  extern unsigned long domUniqueNodeNr;
  extern unsigned long domUniqueDocNr;
  extern Tcl_HashTable tdom_tagNames;
  extern Tcl_HashTable tdom_attrNames;
# define TDomNotThreaded(x) x
# define TDomThreaded(x)
#else
# define TDomNotThreaded(x)
# define TDomThreaded(x)    x
#endif /* TCL_THREADS */

/*--------------------------------------------------------------------------
|   DOMString
|
\-------------------------------------------------------------------------*/
typedef char* domString;   /* should 16-bit unicode character !!*/


/*--------------------------------------------------------------------------
|   domNodeType
|
\-------------------------------------------------------------------------*/
#if defined(_AIX)
#    define    ELEMENT_NODE                 1
#    define    ATTRIBUTE_NODE               2
#    define    TEXT_NODE                    3
#    define    CDATA_SECTION_NODE           4
#    define    ENTITY_REFERENCE_NODE        5
#    define    ENTITY_NODE                  6
#    define    PROCESSING_INSTRUCTION_NODE  7
#    define    COMMENT_NODE                 8
#    define    DOCUMENT_NODE                9
#    define    DOCUMENT_TYPE_NODE           10
#    define    DOCUMENT_FRAGMENT_NODE       11
#    define    NOTATION_NODE                12
#    define    ALL_NODES                    100

#    define    domNodeType                  int

#else

typedef enum {

    ELEMENT_NODE                = 1,
    ATTRIBUTE_NODE              = 2,
    TEXT_NODE                   = 3,
    CDATA_SECTION_NODE          = 4,
    ENTITY_REFERENCE_NODE       = 5,
    ENTITY_NODE                 = 6,
    PROCESSING_INSTRUCTION_NODE = 7,
    COMMENT_NODE                = 8,
    DOCUMENT_NODE               = 9,
    DOCUMENT_TYPE_NODE          = 10,
    DOCUMENT_FRAGMENT_NODE      = 11,
    NOTATION_NODE               = 12,
    ALL_NODES                   = 100
} domNodeType;

#endif

/*--------------------------------------------------------------------------
|   an index to the namespace records
|
\-------------------------------------------------------------------------*/
typedef unsigned int domNameSpaceIndex;



/*--------------------------------------------------------------------------
|   domException
|
\-------------------------------------------------------------------------*/
typedef enum {

    OK                          = 0,
    INDEX_SIZE_ERR              = 1,
    DOMSTRING_SIZE_ERR          = 2,
    HIERARCHY_REQUEST_ERR       = 3,
    WRONG_DOCUMENT_ERR          = 4,
    INVALID_CHARACTER_ERR       = 5,
    NO_DATA_ALLOWED_ERR         = 6,
    NO_MODIFICATION_ALLOWED_ERR = 7,
    NOT_FOUND_ERR               = 8,
    NOT_SUPPORTED_ERR           = 9,
    INUSE_ATTRIBUTE_ERR         = 10

} domException;


/*--------------------------------------------------------------------------
|   domDocInfo
|
\-------------------------------------------------------------------------*/
typedef struct domDocInfo {

    /* 'name' is always the name of the documentElement, no struct element
       needed for this */
    domString      publicId;
    domString      systemId;
    domString      internalSubset;
    /* Currently missing, according to DOM 2: 'entities' and 'notations'. */
    /* The following struct elements describes additional 'requested'
       facets of the document, following the xslt rec, section 16 */
    float          version;
    char          *encoding;
    int            omitXMLDeclaration;
    int            standalone;
    Tcl_HashTable *cdataSectionElements;
    domString      method;
    domString      mediaType;

} domDocInfo;

/*--------------------------------------------------------------------------
|   domDocument
|
\-------------------------------------------------------------------------*/
typedef unsigned int domDocFlags;
typedef struct domDocument {

    domNodeType       nodeType  : 8;
    domDocFlags       nodeFlags : 8;
    domNameSpaceIndex dummy     : 16;
    unsigned long     documentNumber;
    struct domNode   *documentElement;
    struct domNode   *fragments;
#ifdef TCL_THREADS
    struct domNode   *deletedNodes;
#endif
    struct domNS    **namespaces;
    int               nsptr;
    int               nslen;
    char            **prefixNSMappings; /* Stores doc global prefix ns
                                           mappings for resolving of
                                           prefixes in seletNodes expr */
#ifdef TCL_THREADS
    unsigned int      nodeCounter;
#endif
    struct domNode   *rootNode;
    Tcl_HashTable    *ids;
    Tcl_HashTable    *unparsedEntities;
    Tcl_HashTable    *baseURIs;
    Tcl_HashTable    *xpathCache;
    char             *extResolver;
    domDocInfo       *doctype;
    TDomThreaded (
        Tcl_HashTable tdom_tagNames;   /* Names of tags found in doc */
        Tcl_HashTable tdom_attrNames;  /* Names of tag attributes */
        unsigned int  refCount;        /* # of object commands attached */
        struct _domlock *lock;          /* Lock for this document */
    )
} domDocument;


/*--------------------------------------------------------------------------
|  domLock
|
\-------------------------------------------------------------------------*/
#ifdef TCL_THREADS
typedef struct _domlock {
    domDocument* doc;           /* The DOM document to be locked */
    int numrd;                  /* # of readers waiting for lock */
    int numwr;                  /* # of writers waiting for lock */
    int lrcnt;                  /* Lock ref count, > 0: # of shared
                                 * readers, -1: exclusive writer */
    Tcl_Mutex mutex;            /* Mutex for serializing access */
    Tcl_Condition rcond;        /* Condition var for reader locks */
    Tcl_Condition wcond;        /* Condition var for writer locks */
    struct _domlock *next;       /* Next doc lock in global list */
} domlock;

#endif


/*--------------------------------------------------------------------------
|   namespace
|
\-------------------------------------------------------------------------*/
typedef struct domNS {

   char   *uri;
   char   *prefix;
   int     index;

} domNS;


/*---------------------------------------------------------------------------
|   type domActiveNS
|
\--------------------------------------------------------------------------*/
typedef struct _domActiveNS {

    int    depth;
    domNS *namespace;

} domActiveNS;


/*--------------------------------------------------------------------------
|   domLineColumn
|
\-------------------------------------------------------------------------*/
typedef struct domLineColumn {

    long  line;
    long  column;

} domLineColumn;


/*--------------------------------------------------------------------------
|   domNode
|
\-------------------------------------------------------------------------*/
typedef unsigned int domNodeFlags;
typedef struct domNode {

    domNodeType         nodeType  : 8;
    domNodeFlags        nodeFlags : 8;
#ifdef TDOM_LESS_NS
    domNameSpaceIndex   namespace : 8;
    unsigned int        info      : 8;
#else
    unsigned int        dummy     : 8;
    unsigned int        info      : 8;
#endif
    unsigned int        nodeNumber;
    domDocument        *ownerDocument;
    struct domNode     *parentNode;
    struct domNode     *previousSibling;
    struct domNode     *nextSibling;

    domString           nodeName;  /* now the element node specific fields */
#ifndef TDOM_LESS_NS
    domNameSpaceIndex   namespace;
#endif
    struct domNode     *firstChild;
    struct domNode     *lastChild;
    struct domAttrNode *firstAttr;

} domNode;

/*--------------------------------------------------------------------------
|   domDeleteInfo
|
\-------------------------------------------------------------------------*/

typedef struct domDeleteInfo {
    domDocument * document;
    domNode     * node;
    Tcl_Interp  * interp;
    char        * traceVarName;
} domDeleteInfo;


/*--------------------------------------------------------------------------
|   domTextNode
|
\-------------------------------------------------------------------------*/
typedef struct domTextNode {

    domNodeType         nodeType  : 8;
    domNodeFlags        nodeFlags : 8;
#ifdef TDOM_LESS_NS
    domNameSpaceIndex   namespace : 8;
    unsigned int        info      : 8;
#else
    unsigned int        dummy     : 8;
    unsigned int        info      : 8;
#endif
    unsigned int        nodeNumber;
    domDocument        *ownerDocument;
    struct domNode     *parentNode;
    struct domNode     *previousSibling;
    struct domNode     *nextSibling;

    domString           nodeValue;   /* now the text node specific fields */
    int                 valueLength;

} domTextNode;


/*--------------------------------------------------------------------------
|   domProcessingInstructionNode
|
\-------------------------------------------------------------------------*/
typedef struct domProcessingInstructionNode {

    domNodeType         nodeType  : 8;
    domNodeFlags        nodeFlags : 8;
#ifdef TDOM_LESS_NS
    domNameSpaceIndex   namespace : 8;
    unsigned int        info      : 8;
#else
    unsigned int        dummy     : 8;
    unsigned int        info      : 8;
#endif
    unsigned int        nodeNumber;
    domDocument        *ownerDocument;
    struct domNode     *parentNode;
    struct domNode     *previousSibling;
    struct domNode     *nextSibling;

    domString           targetValue;   /* now the pi specific fields */
    int                 targetLength;
#ifndef TDOM_LESS_NS
    domNameSpaceIndex   namespace;
#endif
    domString           dataValue;
    int                 dataLength;

} domProcessingInstructionNode;


/*--------------------------------------------------------------------------
|   domAttrNode
|
\-------------------------------------------------------------------------*/
typedef unsigned int domAttrFlags;
typedef struct domAttrNode {

    domNodeType         nodeType  : 8;
    domAttrFlags        nodeFlags : 8;
#ifdef TDOM_LESS_NS
    domNameSpaceIndex   namespace : 8;
    unsigned int        info      : 8;
#else
    unsigned int        dummy     : 8;
    unsigned int        info      : 8;
    domNameSpaceIndex   namespace;
#endif
    domString           nodeName;
    domString           nodeValue;
    int                 valueLength;
    struct domNode     *parentNode;
    struct domAttrNode *nextSibling;

} domAttrNode;

/*--------------------------------------------------------------------------
|   domAddCallback
|
\-------------------------------------------------------------------------*/
typedef int  (*domAddCallback)  (domNode * node, void * clientData);
typedef void (*domFreeCallback) (domNode * node, void * clientData);

struct TclGenExpatInfo;

typedef void (*CHandlerSet_userDataReset)(Tcl_Interp *interp, void *userData);
typedef void (*CHandlerSet_userDataFree)(Tcl_Interp *interp, void *userData);
typedef void (*CHandlerSet_parserReset)(XML_Parser parser, void *userData);
/*
typedef void (*CHandlerSet_initParse)(const struct TclGenExpatInfo *expat,
                                      void *userData);
*/
typedef void (*CHandlerSet_initParse)(Tcl_Interp *interp, void *userData);

typedef struct CHandlerSet {
    struct CHandlerSet *nextHandlerSet;
    char *name;                     /* refname of the handler set */
    int ignoreWhiteCDATAs;          /* ignore 'white' CDATA sections */

    void *userData;                 /* Handler set specific Data Structure;
                                       the C handler set extension has to
                                       malloc the needed structure in his
                                       init func and has to provide a
                                       cleanup func (to free it). */

    CHandlerSet_userDataReset        resetProc;
    CHandlerSet_userDataFree         freeProc;
    CHandlerSet_parserReset          parserResetProc;
    CHandlerSet_initParse            initParseProc;

    /* C func for element start */
    XML_StartElementHandler          elementstartcommand;
    /* C func for element end */
    XML_EndElementHandler            elementendcommand;
    /* C func for character data */
    XML_CharacterDataHandler         datacommand;
    /* C func for namespace decl start */
    XML_StartNamespaceDeclHandler    startnsdeclcommand;
    /* C func for namespace decl end */
    XML_EndNamespaceDeclHandler      endnsdeclcommand;
    /* C func for processing instruction */
    XML_ProcessingInstructionHandler picommand;
    /* C func for default data */
    XML_DefaultHandler               defaultcommand;
    /* C func for unparsed entity declaration */
    XML_NotationDeclHandler          notationcommand;
    /* C func for external entity */
    XML_ExternalEntityRefHandler     externalentitycommand;
    /* C func for unknown encoding */
    XML_UnknownEncodingHandler       unknownencodingcommand;
    /* C func for comments */
    XML_CommentHandler               commentCommand;
    /* C func for "not standalone" docs */
    XML_NotStandaloneHandler         notStandaloneCommand;
    /* C func for CDATA section start */
    XML_StartCdataSectionHandler     startCdataSectionCommand;
    /* C func for CDATA section end */
    XML_EndCdataSectionHandler       endCdataSectionCommand;
    /* C func for <!ELEMENT decl's */
    XML_ElementDeclHandler           elementDeclCommand;
    /* C func for <!ATTLIST decl's */
    XML_AttlistDeclHandler           attlistDeclCommand;
    /* C func for <!DOCTYPE decl's */
    XML_StartDoctypeDeclHandler      startDoctypeDeclCommand;
    /* C func for <!DOCTYPE decl ends */
    XML_EndDoctypeDeclHandler        endDoctypeDeclCommand;
    /* C func for <?XML decl's */
    XML_XmlDeclHandler               xmlDeclCommand;
    /* C func for <!ENTITY decls's */
    XML_EntityDeclHandler            entityDeclCommand;
} CHandlerSet;

/*----------------------------------------------------------------------------
|   The structure below is used to refer to an event handler set
|   of Tcl scripts.
\---------------------------------------------------------------------------*/

typedef struct TclHandlerSet {
    struct TclHandlerSet *nextHandlerSet;
    char *name;                     /* refname of the handler set */
    int status;                     /* handler set status */
    int continueCount;		    /* reference count for continue */
    int ignoreWhiteCDATAs;          /* ignore 'white' CDATA sections */

    Tcl_Obj *elementstartcommand;      /* Script for element start */
    Tcl_ObjCmdProc *elementstartObjProc;
    ClientData      elementstartclientData;
    Tcl_Obj *elementendcommand;        /* Script for element end */
    Tcl_ObjCmdProc *elementendObjProc;
    ClientData      elementendclientData;
    Tcl_Obj *datacommand;	       /* Script for character data */
    Tcl_ObjCmdProc *datacommandObjProc;
    ClientData      datacommandclientData;
    Tcl_Obj *startnsdeclcommand;       /* Script for namespace decl start */
    Tcl_Obj *endnsdeclcommand;         /* Script for namespace decl end */
    Tcl_Obj *picommand;		       /* Script for processing instruction */
    Tcl_Obj *defaultcommand;	       /* Script for default data */
    Tcl_Obj *notationcommand;	       /* Script for notation declaration */
    Tcl_Obj *externalentitycommand;    /* Script for external entity */
    Tcl_Obj *unknownencodingcommand;   /* Script for unknown encoding */
    Tcl_Obj *commentCommand;           /* Script for comments */
    Tcl_Obj *notStandaloneCommand;     /* Script for "not standalone" docs */
    Tcl_Obj *startCdataSectionCommand; /* Script for CDATA section start */
    Tcl_Obj *endCdataSectionCommand;   /* Script for CDATA section end */
    Tcl_Obj *elementDeclCommand;       /* Script for <!ELEMENT decl's */
    Tcl_Obj *attlistDeclCommand;       /* Script for <!ATTLIST decl's */
    Tcl_Obj *startDoctypeDeclCommand;  /* Script for <!DOCTYPE decl's */
    Tcl_Obj *endDoctypeDeclCommand;    /* Script for <!DOCTYPE decl ends */
    Tcl_Obj *xmlDeclCommand;           /* Script for <?XML decl's */
    Tcl_Obj *entityDeclCommand;        /* Script for <!ENTITY decl's */
} TclHandlerSet;

typedef struct expatElemContent {
    XML_Content             *content;
    struct expatElemContent *next;
} ExpatElemContent;

#include "domxpath.h"
#include "schema.h"

typedef struct TclGenExpatInfo {
    XML_Parser  parser;		/* The expat parser structure */
    Tcl_Interp *interp;		/* Interpreter for this instance */
    Tcl_Obj    *name;		/* name of this instance */
    int final;			/* input data complete? */
    int needWSCheck;            /* Any handler set has ignoreWhiteCDATAs==1? */
    int status;			/* application status */
    Tcl_Obj *result;		/* application return result */
    const char *context;        /* reference to the context pointer */
    Tcl_Obj *cdata;             /* Accumulates character data */
    ExpatElemContent *eContents;/* The reported XML_Contents as linked list */
    int ns_mode;                /* namespace mode */
    Tcl_Obj *baseURI;
    int finished;
    int parsingState;           /* 0 == freshly (re-)initialized
                                   1 == initParserProcs called
                                   2 == parsing an input chunk */
    XML_Char nsSeparator;       
    int paramentityparsing;     
    int noexpand;
    int useForeignDTD;
    const char *currentmarkup;  /* Used to transfer data for method */
    int currentmarkuplen;       /* currentmarkup */
#ifndef TDOM_NO_SCHEMA
    SchemaData *sdata;          /* Validation / Schema data */
#endif

    TclHandlerSet *firstTclHandlerSet;
    CHandlerSet *firstCHandlerSet;
} TclGenExpatInfo;

#ifdef BUILD_tdom
# undef TCL_STORAGE_CLASS
# define TCL_STORAGE_CLASS DLLEXPORT
#endif

#ifdef USE_TDOM_STUBS
EXTERN CONST char* TdomInitializeStubs _ANSI_ARGS_((Tcl_Interp* interp, int exact));
#    define Tdom_InitStubs(interp, version, exact) TdomInitializeStubs((interp), (exact))
#else
#    define Tdom_InitStubs(interp, version, exact) Tcl_PkgRequire((interp), "tdom", (version), (exact))
#endif

#include "tdomDecls.h"

EXTERN int Tdom_Init _ANSI_ARGS_((Tcl_Interp* interp));

#endif
