# tdom.decls --
#
#	This file contains the declarations for all supported public
#	functions that are exported by the tDOM library via the stubs table.
#
# Copyright (c) 2002 Rolf Ade.

library tdom
interface tdom
#hooks {}

declare 0 generic {
    int TclExpatObjCmd (ClientData dummy, Tcl_Interp *interp, 
                        int objc, Tcl_Obj *const objv[])
}
declare 1 generic {
    int CheckExpatParserObj (Tcl_Interp *interp, Tcl_Obj *const nameObj)
}
declare 2 generic {
    int CHandlerSetInstall (Tcl_Interp *interp, Tcl_Obj *const expatObj,
                             CHandlerSet *handlerSet)
}
declare 3 generic {
    int CHandlerSetRemove (Tcl_Interp *interp, Tcl_Obj *const expatObj,
                            char *handlerSetName)
}
declare 4 generic {
    CHandlerSet * CHandlerSetCreate (char *name)
}
declare 5 generic {
    CHandlerSet * CHandlerSetGet (Tcl_Interp *interp, Tcl_Obj *const expatObj,
                                   char *handlerSetName)
}
declare 6 generic {
    void * CHandlerSetGetUserData (Tcl_Interp *interp, 
                                    Tcl_Obj *const expatObj,
                                    char *handlerSetName)
}
declare 7 generic {
    TclGenExpatInfo * GetExpatInfo (Tcl_Interp *interp,
                                     Tcl_Obj *const expatObj)
}
declare 8 generic {
    XML_Size XML_GetCurrentLineNumber(XML_Parser parser)
}
declare 9 generic {
    XML_Size XML_GetCurrentColumnNumber(XML_Parser parser)
}
declare 10 generic {
    XML_Index XML_GetCurrentByteIndex(XML_Parser parser)
}
declare 11 generic {
    int XML_GetCurrentByteCount(XML_Parser parser)
}
declare 12 generic {
    enum XML_Status XML_SetBase(XML_Parser parser, const XML_Char *base)
}
declare 13 generic {
    const XML_Char * XML_GetBase(XML_Parser parser)
}
declare 14 generic {
    int XML_GetSpecifiedAttributeCount(XML_Parser parser)
}
declare 15 generic {
    int XML_GetIdAttributeIndex(XML_Parser parser)
}
declare 16 generic {
    domNode * tcldom_getNodeFromName(Tcl_Interp *interp, char *nodeName, 
                                      char **errMsg)
}
declare 17 generic {
    domDocument * tcldom_getDocumentFromName (Tcl_Interp  *interp, 
                                               char *docName, char **errMsg)
}
declare 18 generic {
    SchemaData * tdomGetSchemadata (void)
}

declare 19 generic {
    void tcldom_createNodeObj(Tcl_Interp * interp, domNode *node, char *objCmdName)
}
declare 20 generic {
    domNode * domCloneNode(domNode *node, int deep)
}
declare 21 generic {
    domDocument * domCreateDoc(const char *baseURI, int storeLineColumn)
}
declare 22 generic {
    domDocument * domCreateDocument(const char *uri, char *documentElementTagName)
}
declare 23 generic {
    void domSetDocumentElement(domDocument *doc)
}
declare 24 generic {
    void domFreeDocument(domDocument *doc, domFreeCallback freeCB, void * clientData)
}
declare 25 generic {
    void domNop(void)
}



declare 26 generic {
    const char * domException2String(domException exception)
}

declare 27 generic {
    void domModuleInitialize(void)
}

declare 28 generic {
    domDocument * domReadDocument(
        XML_Parser parser,
        char *xml,
        int   length,
        int   ignoreWhiteSpaces,
        int   keepCDATA,
        int   storeLineColumn,
        int   ignoreXMLNS,
        int   feedbackAfter,
        Tcl_Obj *feedbackCmd,
        Tcl_Channel channel,
        const char *baseurl,
        Tcl_Obj *extResolver,
        int   useForeignDTD,
        int   paramEntityParsing,
#ifndef TDOM_NO_SCHEMA
        SchemaData *cdata,
#endif
        Tcl_Interp *interp,
        int  *status)
}


declare 29 generic {
    void domFreeNode(
        domNode *node, 
        domFreeCallback freeCB, 
        void *clientData, 
        int dontfree)
}

declare 30 generic {
    domTextNode * domNewTextNode(
        domDocument *doc,
        const char  *value,
        int          length,
        domNodeType  nodeType)
}

declare 31 generic {
    domNode * domNewElementNode(domDocument *doc, const char *tagName)
}
		
declare 32 generic {
    domNode * domNewElementNodeNS(domDocument *doc, const char *tagName, const char *uri)
}

declare 33 generic {
    domProcessingInstructionNode * domNewProcessingInstructionNode(
                                  domDocument *doc,
                                  const char  *targetValue,
                                  int          targetLength,
                                  const char  *dataValue,
                                  int          dataLength)
}

declare 34 generic {
    domAttrNode * domSetAttribute(domNode *node, const char *restrict attributeName, const char *restrict attributeValue)
}

declare 35 generic {
    domAttrNode * domSetAttributeNS(
        domNode *node, const char *attributeName,
        const char *attributeValue,
        const char *uri,
        int   createNSIfNeeded)
}
declare 36 generic {
    domAttrNode * domGetAttributeNodeNS(domNode *node, const char *uri, const char *localname)
}

declare 37 generic {
    int domRemoveAttribute(domNode *node, const char *attributeName)
}
declare 38 generic {
    int domRemoveAttributeNS(domNode *node, const char *uri, const char *localName)
}
declare 39 generic {
    domNode * domPreviousSibling(domNode *attr)
}
declare 40 generic {
    domException domDeleteNode(domNode *node, domFreeCallback freeCB, void *clientData)
}
declare 41 generic {
    domException domRemoveChild(domNode *node, domNode *childToRemove)
}
declare 42 generic {
    domException domAppendChild(domNode *node, domNode *childToAppend)
}
declare 43 generic {
    domException domInsertBefore(domNode *node, domNode *childToInsert, domNode *refChild)
}
declare 44 generic {
    domException domReplaceChild(domNode *node, domNode *newChild, domNode *oldChild)
}
declare 45 generic {
    domException domSetNodeValue(domNode *node, const char *nodeValue, int valueLen)
}

declare 46 generic {
    domTextNode * domAppendNewTextNode(
        domNode *parent,
        const char *value,
        int length,
        domNodeType nodeType,
        int disableOutputEscaping)
}
declare 47 generic {
    domNode * domAppendNewElementNode(domNode *parent, const char *tagName, const char *uri)
}
declare 48 generic {
    domNode * domAppendLiteralNode(domNode *parent, domNode *node)
}
declare 49 generic {
    domNS * domAddNSToNode(domNode *node, domNS *nsToAdd)
}
declare 50 generic {
    const char * domNamespacePrefix(domNode *node)
}
declare 51 generic {
    const char * domNamespaceURI(domNode *node)
}
declare 52 generic {
    const char * domGetLocalName(const char *nodeName)
}
declare 53 generic {
    int domSplitQName(const char *name, char *prefix, const char **localName)
}
declare 54 generic {
    domNS * domLookupNamespace(domDocument *doc, const char *prefix, const char *namespaceURI)
}
declare 55 generic {
    domNS * domLookupPrefix(domNode *node, const char *prefix)
}
declare 56 generic {
    int domIsNamespaceInScope(domActiveNS *NSstack, int NSstackPos, const char *prefix, const char *namespaceURI)
}
declare 57 generic {
    const char * domLookupPrefixWithMappings(domNode *node, const char *prefix, char **prefixMappings)
}
declare 58 generic {
    domNS * domLookupURI(domNode *node, char *uri)
}
declare 59 generic {
    domNS * domGetNamespaceByIndex(domDocument *doc, int nsIndex)
}
declare 60 generic {
    domNS * domNewNamespace(domDocument *doc, const char *prefix, const char *namespaceURI)
}
declare 61 generic {
    int domGetLineColumn(domNode *node, long *line, long *column)
}

declare 62 generic {
    int domXPointerChild(
        domNode * node,
        int all,
        int instance,
        domNodeType type,
        char *element,
        char *attrName,
        char *attrValue,
        int attrLen,
        domAddCallback addCallback,
        void * clientData)
}

declare 63 generic {
    int domXPointerDescendant(
        domNode * node,
        int all,
        int instance,
        int * i,
        domNodeType type,
        char *element,
        char *attrName,
        char *attrValue,
        int attrLen,
        domAddCallback addCallback,
        void * clientData)
}

declare 64 generic {
    int domXPointerAncestor(
        domNode * node,
        int all,
        int instance,
        int * i,
        domNodeType type,
        char *element,
        char *attrName,
        char *attrValue,
        int attrLen,
        domAddCallback addCallback,
        void * clientData)
}

declare 65 generic {
    int domXPointerXSibling(
        domNode * node,
        int forward_mode,
        int all,
        int instance,
        domNodeType type,
        char *element,
        char *attrName,
        char *attrValue,
        int attrLen,
        domAddCallback addCallback,
        void * clientData)
}

declare 66 generic {
    const char * findBaseURI(domNode *node)
}

declare 67 generic {
    void tcldom_tolower(const char *str, char *str_out, int  len)
}
declare 68 generic {
    int domIsNAME(const char *name)
}
declare 69 generic {
    int domIsPINAME(const char *name)
}
declare 70 generic {
    int domIsQNAME(const char *name)
}
declare 71 generic {
    int domIsNCNAME(const char *name)
}
declare 72 generic {
    int domIsChar(const char *str)
}
declare 73 generic {
    char * domClearString(char *str, int *haveToFree)
}
declare 74 generic {
    int domIsBMPChar(const char *str)
}
declare 75 generic {
    int domIsComment(const char *str)
}
declare 76 generic {
    int domIsCDATA(const char *str)
}
declare 77 generic {
    int domIsPIValue(const char *str)
}
declare 78 generic {
    void domCopyTo(domNode *node, domNode *parent, int copyNS)
}
declare 79 generic {
    void domCopyNS(domNode *from, domNode *to)
}
declare 80 generic {
    domAttrNode * domCreateXMLNamespaceNode(domNode *parent)
}
declare 81 generic {
    void domRenumberTree(domNode *node)
}
declare 82 generic {
    int domPrecedes(domNode *node, domNode *other)
}
declare 83 generic {
    void domNormalize(domNode *node, int forXPath, domFreeCallback freeCB, void *clientData)
}
declare 84 generic {
    domException domAppendData(domTextNode *node, const char *value, int length, int disableOutputEscaping)
}
declare 85 generic {
    void domLocksLock(domlock *dl, int how)
}
declare 86 generic {
    void domLocksUnlock(domlock *dl)
}
declare 87 generic {
    void domLocksAttach(domDocument *doc)
}
declare 88 generic {
    void domLocksDetach(domDocument *doc)
}
declare 89 generic {
    void domLocksFinalize(ClientData dummy)
}

#---------------------------------------------------------------------------
#   coercion routines for calling from C++
#
#---------------------------------------------------------------------------
declare 90 generic {
    domAttrNode * coerceToAttrNode( domNode *n )
}
declare 91 generic {
    domTextNode * coerceToTextNode( domNode *n )
}
declare 92 generic {
    domProcessingInstructionNode * coerceToProcessingInstructionNode( domNode *n )
}


declare 93 generic {
    domAttrNode * domSetAttributeEx(
        domNode *node,
        const char *restrict attributeName,  int nameLength,
        const char *restrict attributeValue, int valueLength)
}
