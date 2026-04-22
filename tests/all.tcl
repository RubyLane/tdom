# all.tcl --
#
# This file contains a top-level script to run all of the Tcl
# tests.  Execute it by invoking "tclsh all.test".
#
# Copyright (c) 1998-1999 by Scriptics Corporation.
# All rights reserved.
#
# RCS: @(#) $Id$
# 

source [file join [file dir [info script]] loadtdom.tcl]

singleProcess 1
if {$tcl_version < 8.5} {
    # We still support 8.4 to some degree
    testsDirectory [file dirname [info script]]
} else {
    configure {*}$argv -testdir [file dirname [info script]]
}
set failed [runAllTests]
# See http://mini.net/tcl/3248 for an explanation.
rename ::exit ::_real_exit
proc exit args {}
::_real_exit [expr {$failed ? 1 : 0}]

