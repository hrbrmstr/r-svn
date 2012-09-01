/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 2002-2012	The R Core Team.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, a copy is available at
 *  http://www.r-project.org/Licenses/
 *
 */
/*
 *  This file replaces the previously used ROUTINES file and is used to
 *  explicitly register native routines that are located in the R
 *  executable (e.g. R.bin, Rgui.exe) but which are intended to be
 *  accessible to S code via .C(), .Fortran(), .Call(), .External().
 *  The approach we use here is the regular registration mechanism that
 *  packages can use to explicitly list the symbols to be exported.
 *  For .C() and .Call() routines, we give the number of arguments
 *  expected.
 *  For .C() routines, we also specify the types of the arguments.
 *  For .Fortran() and .External() routines, we specify only the name
 *  and symbol.

 *  To add an entry, first determine by which interface the routine will
 *  be accessed:
 *   .C, .Call, .External or .Fortran
 *  Then add an entry to
 *    cMethods, callMethods, externalMethods, or fortranMethods
 *  respectively
 *
 *  DTL 14-Dec-2002
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Defn.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Applic.h>
#include <R_ext/Linpack.h>


/*  These get the declarations of some routines refernced here but
    not explicitly declared.    This is necessary when we link with
    a C++ compiler because the linkage changes as the declarations
    are (currently) within extern "C" blocks.
*/
#include <R_ext/Callbacks.h>
#include <Rdynpriv.h>

#include "basedecl.h"

/* FIXME: bincode is no longer used in R, but is still used by
   packages mixOmics spam .  Remove after R 2.15.2.
*/
void bincode (double *x, int *n, double *breaks, int *nb,
	      int *code, int *right, int *include_border, int *naok);

static R_NativePrimitiveArgType bincode_t[] = {REALSXP, INTSXP, REALSXP, INTSXP, INTSXP, LGLSXP, LGLSXP, LGLSXP};


#define CDEF(name)  {#name, (DL_FUNC) &name, sizeof(name ## _t)/sizeof(name ## _t[0]), name ##_t}

static R_CMethodDef cMethods [] = {
    CDEF(bincode), // remove after R 2.15.2

    {NULL, NULL, 0}
};


#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static R_CallMethodDef callMethods [] = {
    /* Top-level task callbacks */
    CALLDEF(R_getTaskCallbackNames, 0),
    CALLDEF(R_removeTaskCallback, 1),
    CALLDEF(R_addTaskCallback, 4),

    /* Methods related routines. */
    CALLDEF(R_traceOnOff, 1),
    CALLDEF(R_setS4Object, 3),

    CALLDEF(R_getbcprofcounts, 0),
    CALLDEF(R_startbcprof, 0),
    CALLDEF(R_stopbcprof, 0),

    {NULL, NULL, 0}
};


#define FDEF(name, n)  {#name, (DL_FUNC) &F77_SYMBOL(name), n, NULL}
static R_FortranMethodDef fortranMethods[] = {
    FDEF(ch2inv, 5),
    FDEF(chol, 5),
    FDEF(cg, 13),
    FDEF(ch, 12),
    FDEF(rg, 10),
    FDEF(rs, 9),
    /* Linpack etc */
    FDEF(dchdc, 7),
    FDEF(dqrcf, 8),
    FDEF(dqrdc2, 9),
    FDEF(dqrls, -1), // for historical reasons, 13 args
    FDEF(dqrqty, 7),
    FDEF(dqrqy, 7),
    FDEF(dqrrsd, 7),
    FDEF(dqrxb, 7),
    FDEF(dsvdc, 13),
    FDEF(dtrco, 6),
    {NULL, NULL, 0}
};


void attribute_hidden
R_init_base(DllInfo *dll)
{
    R_registerRoutines(dll, cMethods, callMethods, fortranMethods, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
