// Copyright (c) 1998-2021
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL$
// $Id$
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Geert-Jan Giezeman, Andreas Fabri

#ifndef CGAL_DISTANCE_3_WEIGHTED_POINT_3_WEIGHTED_POINT_3_H
#define CGAL_DISTANCE_3_WEIGHTED_POINT_3_WEIGHTED_POINT_3_H

#include <CGAL/squared_distance_Point_3_Point_3.h>

#include <CGAL/Point_3.h>

namespace CGAL {

template <class K>
inline
typename K::FT
squared_distance(const Weighted_point_3<K>& wpt1,
                 const Weighted_point_3<K>& wpt2)
{
  return internal::squared_distance(wpt1.point(), wpt2.point(), K());
}

} // namespace CGAL

#endif // CGAL_DISTANCE_3_WEIGHTED_POINT_3_WEIGHTED_POINT_3_H
