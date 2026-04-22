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

# Signal failure via TCL_ERROR instead of [exit $code].
#
# Calling [exit] from inside the script bypasses interp teardown:
# Tcl_Exit runs exit handlers and _exit()s the process while
# Tcl_FSEvalFileEx is still on the stack, so TclEvalEx's per-line
# Tcl_Obj cleanup never runs and the temps it built while parsing
# the [exit ...] line itself leak (visible under valgrind as
# TclSubstTokens / TEBCresume residue).
#
# Returning TCL_ERROR to Tcl_MainEx takes the clean path: the file
# evaluator unwinds normally, Tcl_MainEx sets its own exitCode to 1
# and evaluates [exit 1] from its epilogue — outside any active
# TclEvalEx frame, so the temps around its constructed "exit 1"
# command don't leak.
#
# Closing stderr before raising suppresses Tcl_MainEx's -errorinfo
# dump (the "while executing ... (file ... line ...)" trace it
# writes on TCL_ERROR); our own summary is already on stdout from
# tcltest, and the exit code is what meson test actually reads.
if {$failed} {
    puts stderr "[file tail [info script]]: $failed test(s) failed"
    close stderr
    error "test run failed"
}

