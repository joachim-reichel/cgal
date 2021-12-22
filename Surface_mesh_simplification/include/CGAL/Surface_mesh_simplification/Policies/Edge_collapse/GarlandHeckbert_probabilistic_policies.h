// Copyright (c) 2019  GeometryFactory (France). All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Baskin Burak Senbaslar,
//                 Mael Rouxel-Labbé,
//                 Julian Komaromy

#ifndef CGAL_SURFACE_MESH_SIMPLIFICATION_POLICIES_GARLANDHECKBERT_PROBABILISTIC_POLICIES_H
#define CGAL_SURFACE_MESH_SIMPLIFICATION_POLICIES_GARLANDHECKBERT_PROBABILISTIC_POLICIES_H

#include <CGAL/license/Surface_mesh_simplification.h>

#include <CGAL/Surface_mesh_simplification/internal/Common.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/internal/GarlandHeckbert_policy_base.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/internal/GarlandHeckbert_functions.h>

#include <CGAL/Default.h>
#include <CGAL/property_map.h>

#include <tuple>
#include <utility>

namespace CGAL {
namespace Surface_mesh_simplification {

template <typename TriangleMesh, typename GeomTraits, typename FaceVarianceMap>
class Probabilistic_plane_quadric_calculator
{
  typedef typename boost::graph_traits<TriangleMesh>::halfedge_descriptor      halfedge_descriptor;
  typedef typename boost::graph_traits<TriangleMesh>::face_descriptor          face_descriptor;

  typedef typename GeomTraits::FT                                              FT;

  typedef Constant_property_map<face_descriptor, std::pair<FT, FT> >           Default_FVM;
  typedef typename Default::Get<FaceVarianceMap, Default_FVM>::type            Face_variance_map;

  typedef typename internal::GarlandHeckbert_matrix_types<GeomTraits>::Mat_4   Mat_4;
  typedef typename internal::GarlandHeckbert_matrix_types<GeomTraits>::Col_4   Col_4;

private:
  // @fixme check the magic values

  // magic number determined by some testing
  static constexpr FT default_variance_unit = 0.05;

  // magic number - for most use cases, there is no input variance, so it makes sense to
  // set the positional variance to a smaller value than the normal variance
  static constexpr FT position_variance_factor = 0.1;

  Face_variance_map m_face_variance_map;

public:
  Probabilistic_plane_quadric_calculator() { CGAL_assertion(false); } // compilation only

  template <typename FVM>
  Probabilistic_plane_quadric_calculator(const FVM fvm)
      : m_face_variance_map(fvm)
  { }

  Probabilistic_plane_quadric_calculator(TriangleMesh& tmesh,
                                         typename boost::enable_if<std::is_same<Face_variance_map, Default_FVM> >::type* = nullptr)
  {
    // try to initialize the face variance map using the estimated variance
    // parameters are constants defined for this class
    m_face_variance_map = Default_FVM { internal::estimate_variances(tmesh, GeomTraits(),
                                                                     default_variance_unit,
                                                                     position_variance_factor) };
  }

public:
  template <typename VertexPointMap>
  Mat_4 construct_quadric_from_edge(const halfedge_descriptor he,
                                    const TriangleMesh& tmesh,
                                    const VertexPointMap vpm,
                                    const GeomTraits& gt) const
  {
    typedef typename GeomTraits::FT                                            FT;
    typedef typename GeomTraits::Point_3                                       Point_3;
    typedef typename GeomTraits::Vector_3                                      Vector_3;

    const Vector_3 normal = internal::construct_edge_normal(he, tmesh, vpm, gt);
    const Point_3 p = get(vpm, source(he, tmesh));

    FT n_variance, p_variance;
    std::tie(n_variance, p_variance) = get(m_face_variance_map, face(he, tmesh));

    return internal::construct_prob_plane_quadric_from_normal(normal, p, gt, n_variance, p_variance);
  }

  template <typename VertexPointMap>
  Mat_4 construct_quadric_from_face(const face_descriptor f,
                                    const TriangleMesh& tmesh,
                                    const VertexPointMap vpm,
                                    const GeomTraits& gt) const
  {
    typedef typename GeomTraits::FT                                            FT;
    typedef typename GeomTraits::Point_3                                       Point_3;
    typedef typename GeomTraits::Vector_3                                      Vector_3;

    const Vector_3 normal = internal::construct_unit_normal_from_face(f, tmesh, vpm, gt);
    const Point_3 p = get(vpm, source(halfedge(f, tmesh), tmesh));

    FT n_variance, p_variance;
    std::tie(n_variance, p_variance) = get(m_face_variance_map, f);

    return internal::construct_prob_plane_quadric_from_normal(normal, p, gt, n_variance, p_variance);
  }

  Col_4 construct_optimal_point(const Mat_4& quadric,
                                const Col_4& /*p0*/,
                                const Col_4& /*p1*/) const
  {
    // @fixme check this
    return internal::construct_optimal_point_invertible<GeomTraits>(quadric);
  }
};

// Implements probabilistic plane quadrics,
// optionally takes a face variance map giving a per-face variance
template<typename TriangleMesh,
         typename GeomTraits,
         typename FaceVarianceMap = CGAL::Default>
class GarlandHeckbert_probabilistic_policies
  : public internal::GarlandHeckbert_placement_base<
             Probabilistic_plane_quadric_calculator<TriangleMesh, GeomTraits, FaceVarianceMap>,
             TriangleMesh, GeomTraits>,
    public internal::GarlandHeckbert_cost_base<
             Probabilistic_plane_quadric_calculator<TriangleMesh, GeomTraits, FaceVarianceMap>,
             TriangleMesh, GeomTraits>
{
public:
  typedef Probabilistic_plane_quadric_calculator<
            TriangleMesh, GeomTraits, FaceVarianceMap>                         Quadric_calculator;

private:
  typedef internal::GarlandHeckbert_placement_base<
            Quadric_calculator, TriangleMesh, GeomTraits>                      Placement_base;

  typedef internal::GarlandHeckbert_cost_base<
            Quadric_calculator, TriangleMesh, GeomTraits>                      Cost_base;

  // Diamond base
  typedef internal::GarlandHeckbert_quadrics_storage<
            Quadric_calculator, TriangleMesh, GeomTraits>                      Quadrics_storage;

  typedef GarlandHeckbert_probabilistic_policies<TriangleMesh, GeomTraits>     Self;

public:
  typedef Self                                                                 Get_cost;
  typedef Self                                                                 Get_placement;

  typedef typename GeomTraits::FT                                              FT;

public:
  // Only available if the quadric calculator is using the default (constant) variance property map
  GarlandHeckbert_probabilistic_policies(TriangleMesh& tmesh,
                                         const FT dm = FT(100))
    : Quadrics_storage(tmesh, Quadric_calculator(tmesh)), Placement_base(), Cost_base(dm)
  { }

  template <typename FVM>
  GarlandHeckbert_probabilistic_policies(TriangleMesh& tmesh,
                                         const FT dm,
                                         const FVM fvm)
    : Quadrics_storage(tmesh, Quadric_calculator(fvm)), Placement_base(), Cost_base(dm)
  { }

public:
  const Get_cost& get_cost() const { return *this; }
  const Get_placement& get_placement() const { return *this; }

  using Cost_base::operator();
  using Placement_base::operator();
};

} // namespace Surface_mesh_simplification
} // namespace CGAL

#endif // CGAL_SURFACE_MESH_SIMPLIFICATION_POLICIES_GARLANDHECKBERT_PROBABILISTIC_POLICIES_H
