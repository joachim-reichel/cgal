// ======================================================================
//
// Copyright (c) 1999 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
// 
// release       : 
// release_date  : 
// 
// file          : number_type_basic.h
// package       : Number_types
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Stefan Schirra
//
//
// coordinator   : MPI, Saarbruecken
// ======================================================================
 

#ifndef CGAL_NUMBER_TYPE_BASIC_H
#define CGAL_NUMBER_TYPE_BASIC_H

#define CGAL_NTS CGAL::NTS::

#if ((__GNUC__ == 2) && (__GNUC_MINOR__ == 95))
#include <cmath>
#endif

// CGAL uses std::min and std::max
// (see ISO C++ 25.3.7, page 562),
// if feasible

#include <algorithm>

CGAL_BEGIN_NAMESPACE

#ifndef CGAL_CFG_USING_USING_BUG

using std::min;
using std::max;

#else

template <class NT>
inline
NT
// const NT&
min(const NT& x, const NT& y)
{ return (y < x) ? y : x; }

template <class NT>
inline
NT
// const NT&
max(const NT& x, const NT& y)
{ return (x < y) ? y : x; }

#endif // CGAL_CFG_USING_USING_BUG

CGAL_END_NAMESPACE

#include <CGAL/number_utils.h>
#include <CGAL/number_utils_classes.h>
#include <CGAL/double.h>
#include <CGAL/float.h>
#include <CGAL/int.h>
#include <CGAL/Interval_arithmetic.h>

#endif // CGAL_NUMBER_TYPE_BASIC_H
