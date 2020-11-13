// Copyright (c) 2019 GeometryFactory SARL (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s)     : Simon Giraudot

#ifndef CGAL_KINETIC_SHAPE_RECONSTRUCTION_3_H
#define CGAL_KINETIC_SHAPE_RECONSTRUCTION_3_H

// #include <CGAL/license/Kinetic_shape_reconstruction.h>

// CGAL includes.
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

// Internal includes.
#include <CGAL/KSR/utils.h>
#include <CGAL/KSR/debug.h>
#include <CGAL/KSR_3/Event.h>
#include <CGAL/KSR_3/Event_queue.h>
#include <CGAL/KSR_3/Data_structure.h>
#include <CGAL/KSR_3/Polygon_splitter.h>

namespace CGAL {

template<typename GeomTraits>
class Kinetic_shape_reconstruction_3 {

public:
  using Kernel = GeomTraits;

private:
  using FT          = typename Kernel::FT;
  using Point_2     = typename Kernel::Point_2;
  using Point_3     = typename Kernel::Point_3;
  using Segment_2   = typename Kernel::Segment_2;
  using Segment_3   = typename Kernel::Segment_3;
  using Vector_2    = typename Kernel::Vector_2;
  using Transform_3 = typename Kernel::Aff_transformation_3;

  using Data = KSR_3::Data_structure<Kernel>;

  using PVertex = typename Data::PVertex;
  using PEdge   = typename Data::PEdge;
  using PFace   = typename Data::PFace;

  using IVertex = typename Data::IVertex;
  using IEdge   = typename Data::IEdge;

  using Event       = KSR_3::Event<Data>;
  using Event_queue = KSR_3::Event_queue<Data>;

  using Bbox_3 = CGAL::Bbox_3;

  // using EK = CGAL::Exact_predicates_exact_constructions_kernel;
  using Polygon_splitter = KSR_3::Polygon_splitter<Data, Kernel>;

private:
  Data m_data;
  Event_queue m_queue;
  FT m_min_time;
  FT m_max_time;
  const bool m_verbose;

public:
  Kinetic_shape_reconstruction_3(const bool verbose = true) :
  m_min_time(-FT(1)),
  m_max_time(-FT(1)),
  m_verbose(verbose)
  { }

  template<
  typename InputRange,
  typename PolygonMap>
  const bool partition(
    const InputRange& input_range,
    const PolygonMap polygon_map,
    const unsigned int k = 1,
    const FT enlarge_bbox_ratio = FT(11) / FT(10),
    const bool reorient = false) {

    if (m_verbose) std::cout.precision(20);
    if (input_range.size() == 0) {
      CGAL_warning_msg(input_range.size() != 0,
      "WARNING: YOUR INPUT IS EMPTY. RETURN WITH NO CHANGE!");
      return false;
    }

    if (k == 0) {
      CGAL_warning_msg(k != 0,
      "WARNING: YOU SET K TO 0. THE VALID VALUES ARE {1,2,...}. RETURN WITH NO CHANGE!");
      return false;
    }

    if (enlarge_bbox_ratio < FT(1)) {
      CGAL_warning_msg(enlarge_bbox_ratio >= FT(1),
      "WARNING: YOU SET ENLARGE_BBOX_RATIO < 1. THE VALID RANGE IS [1, +INF). RETURN WITH NO CHANGE!");
      return false;
    }

    if (m_verbose) {
      std::cout << std::endl << "--- INITIALIZING KSR:" << std::endl;
    }

    FT time_step;
    std::array<Point_3, 8> bbox;
    create_bounding_box(
      input_range, polygon_map, enlarge_bbox_ratio, reorient, bbox, time_step);
    if (m_verbose) {
      std::cout << "* precomputed time_step: " << time_step << std::endl;
    }

    std::vector< std::vector<Point_3> > bbox_faces;
    bounding_box_to_polygons(bbox, bbox_faces);
    add_polygons(input_range, polygon_map, bbox_faces);

    if (m_verbose) {
      std::cout << "* intersecting input polygons ...";
      KSR_3::dump(m_data, "init");
      // KSR_3::dump_segmented_edges(m_data, "init");
    }

    check_integrity();
    make_polygons_intersection_free();
    check_integrity();
    set_k_intersections(k);

    if (m_verbose) {
      KSR_3::dump(m_data, "intersected");
      // KSR_3::dump_segmented_edges(m_data, "intersected");
      std::cout << " done" << std::endl;
    }

    // for (KSR::size_t i = 6; i < m_data.number_of_support_planes(); ++i) {
    //   const auto& sp = m_data.support_plane(i);
    //   std::cout << "plane index: " << i << std::endl;
    //   std::cout << "plane: " <<
    //   sp.plane().a() << ", " <<
    //   sp.plane().b() << ", " <<
    //   sp.plane().c() << ", " <<
    //   sp.plane().d() << std::endl;
    // }

    m_data.check_bbox();
    std::cout << std::endl << "POLYGON SPLITTER SUCCESS!" << std::endl << std::endl;
    exit(EXIT_SUCCESS);

    if (m_verbose) {
      std::cout << std::endl << "--- RUNNING THE QUEUE:" << std::endl;
      std::cout << "propagation started ..." << std::endl;
    }
    std::size_t num_iterations = 0;
    m_min_time = FT(0);
    m_max_time = time_step;
    CGAL_assertion(m_min_time >= FT(0) && m_max_time >= m_min_time);
    while (initialize_queue()) {

      run(k);
      m_min_time = m_max_time;
      m_max_time += time_step;
      check_integrity();
      ++num_iterations;

      // if (m_verbose) {
      //   std::cout << ".";
      //   if (num_iterations == 50) {
      //     std::cout << std::endl;
      //   }
      // }

      // if (num_iterations > 100) {
      //   CGAL_assertion_msg(false, "WHY SO MANY ITERATIONS?");
      // }
    }
    if (m_verbose) {
      std::cout << "... propagation finished" << std::endl;
    }

    if (m_verbose) {
      std::cout << std::endl << "--- FINALIZING KSR:" << std::endl;
      std::cout << "* checking final mesh integrity ...";
    }
    check_integrity();
    if (m_verbose) {
      dump(m_data, "iter_1000-final-result");
      std::cout << " done" << std::endl;
    }

    // m_data.create_polyhedrons();
    return true;
  }

  template<typename OutputIterator>
  OutputIterator output_partition_edges_to_segment_soup(
    OutputIterator edges) const {

    CGAL_assertion_msg(false, "TODO: IMPLEMENT OUTPUT PARTITION EDGES!");
    return edges;
  }

  template<typename VertexOutputIterator, typename FaceOutputIterator>
  void output_partition_faces_to_polygon_soup(
    VertexOutputIterator vertices, FaceOutputIterator faces,
    const bool with_bbox = false) const {

    CGAL_assertion_msg(false, "TODO: IMPLEMENT OUTPUT PARTITION FACES!");
  }

  template<typename PolyhedronOutputIterator>
  PolyhedronOutputIterator output_partition_polyhedrons(
    PolyhedronOutputIterator polyhedrons) const {

    CGAL_assertion_msg(false, "TODO: IMPLEMENT OUTPUT PARTITION POLYHEDRONS!");
    return polyhedrons;
  }

  template<typename InputRange, typename PointMap, typename VectorMap>
  void reconstruct(
    const InputRange& input_range,
    const PointMap point_map,
    const VectorMap normal_map) {

    CGAL_assertion_msg(false, "TODO: ADD RECONSTRUCTION!");
  }

private:
  template<
  typename InputRange,
  typename PolygonMap>
  void create_bounding_box(
    const InputRange& input_range,
    const PolygonMap polygon_map,
    const FT enlarge_bbox_ratio,
    const bool reorient,
    std::array<Point_3, 8>& bbox,
    FT& time_step) const {

    if (reorient)
      initialize_optimal_box(input_range, polygon_map, bbox);
    else
      initialize_axis_aligned_box(input_range, polygon_map, bbox);

    CGAL_assertion(bbox.size() == 8);
    time_step  = KSR::distance(bbox.front(), bbox.back());
    time_step /= FT(50);

    enlarge_bounding_box(enlarge_bbox_ratio, bbox);

    const auto& minp = bbox.front();
    const auto& maxp = bbox.back();
    if (m_verbose) {
      std::cout << "* bounding box minp: " <<
      minp.x() << "\t, " << minp.y() << "\t, " << minp.z() << std::endl;
    }
    if (m_verbose) {
      std::cout << "* bounding box maxp: " <<
      maxp.x() << "\t, " << maxp.y() << "\t\t, " << maxp.z() << std::endl;
    }
  }

  template<
  typename InputRange,
  typename PolygonMap>
  void initialize_optimal_box(
    const InputRange& input_range,
    const PolygonMap polygon_map,
    std::array<Point_3, 8>& bbox) const {

    CGAL_assertion_msg(false, "TODO: IMPLEMENT THE ORIENTED OPTIMAL BBOX!");
  }

  template<
  typename InputRange,
  typename PolygonMap>
  void initialize_axis_aligned_box(
    const InputRange& input_range,
    const PolygonMap polygon_map,
    std::array<Point_3, 8>& bbox) const {

    Bbox_3 box;
    for (const auto& item : input_range) {
      const auto& polygon = get(polygon_map, item);
      box += CGAL::bbox_3(polygon.begin(), polygon.end());
    }

    // The order of faces corresponds to the standard order from here:
    // https://doc.cgal.org/latest/BGL/group__PkgBGLHelperFct.html#gad9df350e98780f0c213046d8a257358e
    bbox = {
      Point_3(box.xmin(), box.ymin(), box.zmin()),
      Point_3(box.xmax(), box.ymin(), box.zmin()),
      Point_3(box.xmax(), box.ymax(), box.zmin()),
      Point_3(box.xmin(), box.ymax(), box.zmin()),
      Point_3(box.xmin(), box.ymax(), box.zmax()),
      Point_3(box.xmin(), box.ymin(), box.zmax()),
      Point_3(box.xmax(), box.ymin(), box.zmax()),
      Point_3(box.xmax(), box.ymax(), box.zmax()) };
  }

  void enlarge_bounding_box(
    const FT enlarge_bbox_ratio,
    std::array<Point_3, 8>& bbox) const {

    CGAL_assertion_msg(
      enlarge_bbox_ratio > FT(1), "TODO: HANDLE THE CASE ENLARGE_BBOX_RATIO = FT(1)");
    const auto a = CGAL::centroid(bbox.begin(), bbox.end());
    Transform_3 scale(CGAL::Scaling(), enlarge_bbox_ratio);
    for (auto& point : bbox)
      point = scale.transform(point);

    const auto b = CGAL::centroid(bbox.begin(), bbox.end());
    Transform_3 translate(CGAL::Translation(), a - b);
    for (auto& point : bbox)
      point = translate.transform(point);
  }

  void bounding_box_to_polygons(
    const std::array<Point_3, 8>& bbox,
    std::vector< std::vector<Point_3> >& bbox_faces) const {

    bbox_faces.clear();
    bbox_faces.reserve(6);

    bbox_faces.push_back({bbox[0], bbox[1], bbox[2], bbox[3]});
    bbox_faces.push_back({bbox[0], bbox[1], bbox[6], bbox[5]});
    bbox_faces.push_back({bbox[1], bbox[2], bbox[7], bbox[6]});
    bbox_faces.push_back({bbox[2], bbox[3], bbox[4], bbox[7]});
    bbox_faces.push_back({bbox[3], bbox[0], bbox[5], bbox[4]});
    bbox_faces.push_back({bbox[5], bbox[6], bbox[7], bbox[4]});
    CGAL_assertion(bbox_faces.size() == 6);

    // Simon's bbox. The faces are different.
    // const FT xmin = bbox[0].x();
    // const FT ymin = bbox[0].y();
    // const FT zmin = bbox[0].z();
    // const FT xmax = bbox[7].x();
    // const FT ymax = bbox[7].y();
    // const FT zmax = bbox[7].z();
    // const std::vector<Point_3> sbbox = {
    //   Point_3(xmin, ymin, zmin),
    //   Point_3(xmin, ymin, zmax),
    //   Point_3(xmin, ymax, zmin),
    //   Point_3(xmin, ymax, zmax),
    //   Point_3(xmax, ymin, zmin),
    //   Point_3(xmax, ymin, zmax),
    //   Point_3(xmax, ymax, zmin),
    //   Point_3(xmax, ymax, zmax) };

    // bbox_faces.push_back({sbbox[0], sbbox[1], sbbox[3], sbbox[2]});
    // bbox_faces.push_back({sbbox[4], sbbox[5], sbbox[7], sbbox[6]});
    // bbox_faces.push_back({sbbox[0], sbbox[1], sbbox[5], sbbox[4]});
    // bbox_faces.push_back({sbbox[2], sbbox[3], sbbox[7], sbbox[6]});
    // bbox_faces.push_back({sbbox[1], sbbox[5], sbbox[7], sbbox[3]});
    // bbox_faces.push_back({sbbox[0], sbbox[4], sbbox[6], sbbox[2]});
    // CGAL_assertion(bbox_faces.size() == 6);
  }

  template<
  typename InputRange,
  typename PolygonMap>
  void add_polygons(
    const InputRange& input_range,
    const PolygonMap polygon_map,
    const std::vector< std::vector<Point_3> >& bbox_faces) {

    m_data.reserve(input_range.size());
    add_bbox_faces(bbox_faces);
    add_input_polygons(input_range, polygon_map);
  }

  void add_bbox_faces(
    const std::vector< std::vector<Point_3> >& bbox_faces) {

    for (const auto& bbox_face : bbox_faces)
      m_data.add_bbox_polygon(bbox_face);

    CGAL_assertion(m_data.number_of_support_planes() == 6);
    CGAL_assertion(m_data.ivertices().size() == 8);
    CGAL_assertion(m_data.iedges().size() == 12);

    if (m_verbose) {
      std::cout << "* added bbox faces: " << bbox_faces.size() << std::endl;
    }
  }

  template<
  typename InputRange,
  typename PolygonMap>
  void add_input_polygons(
    const InputRange& input_range,
    const PolygonMap polygon_map) {

    KSR::size_t input_index = 0;
    for (const auto& item : input_range) {
      const auto& polygon = get(polygon_map, item);
      m_data.add_input_polygon(polygon, input_index);
      ++input_index;
    }
    CGAL_assertion(m_data.number_of_support_planes() > 6);
    if (m_verbose) {
      std::cout << "* added input polygons: " <<  input_range.size() << std::endl;
    }
  }

  const bool check_integrity() const {

    for (KSR::size_t i = 0; i < m_data.number_of_support_planes(); ++i) {
      if (!m_data.is_mesh_valid(i)) {
        if (m_verbose) {
          const std::string msg = "ERROR: mesh " + std::to_string(i) + " is not valid!";
          CGAL_assertion_msg(false, msg.c_str());
        }
        return false;
      }

      for (const auto& iedge : m_data.iedges(i)) {
        const auto& iplanes = m_data.intersected_planes(iedge);
        if (iplanes.find(i) == iplanes.end()) {
          if (m_verbose) {
            const std::string msg = "ERROR: support_plane " + std::to_string(i) +
            " is intersected by " + m_data.str(iedge) +
            " but it claims it does not intersect it!";
            CGAL_assertion_msg(false, msg.c_str());
          }
          return false;
        }
      }
    }

    for (const auto iedge : m_data.iedges()) {
      const auto& iplanes = m_data.intersected_planes(iedge);
      for (const auto support_plane_idx : iplanes) {

        const auto& sp_iedges = m_data.iedges(support_plane_idx);
        if (sp_iedges.find(iedge) == sp_iedges.end()) {

          if (m_verbose) {
            const std::string msg = "ERROR: iedge " + m_data.str(iedge) +
            " intersects support plane " + std::to_string(support_plane_idx) +
            " but it claims it is not intersected by it!";
            CGAL_assertion_msg(false, msg.c_str());
          }
          return false;
        }
      }
    }
    return true;
  }

  void make_polygons_intersection_free() {

    // First, create all transverse intersection lines.
    using Map_p2vv = std::map<KSR::Idx_set, std::pair<IVertex, IVertex> >;
    Map_p2vv map_p2vv;

    for (const auto ivertex : m_data.ivertices()) {
      const auto key = m_data.intersected_planes(ivertex, false);
      if (key.size() < 2) {
        continue;
      }

      const auto pair = map_p2vv.insert(std::make_pair(
        key, std::make_pair(ivertex, IVertex())));
      const bool is_inserted = pair.second;
      if (!is_inserted) {
        pair.first->second.second = ivertex;
      }
    }

    // Then, intersect these lines to find internal intersection vertices.
    using Pair_pv = std::pair< KSR::Idx_set, KSR::vector<IVertex> >;
    KSR::vector<Pair_pv> todo;
    for (auto it_a = map_p2vv.begin(); it_a != map_p2vv.end(); ++it_a) {
      const auto& set_a = it_a->first;

      todo.push_back(std::make_pair(set_a, KSR::vector<IVertex>()));
      auto& crossed_vertices = todo.back().second;
      crossed_vertices.push_back(it_a->second.first);

      std::set<KSR::Idx_set> done;
      for (auto it_b = map_p2vv.begin(); it_b != map_p2vv.end(); ++it_b) {
        const auto& set_b = it_b->first;

        KSR::size_t common_plane_idx = KSR::no_element();
        std::set_intersection(
          set_a.begin(), set_a.end(), set_b.begin(), set_b.end(),
          boost::make_function_output_iterator(
            [&](const KSR::size_t idx) -> void {
              common_plane_idx = idx;
            }
          )
        );

        if (common_plane_idx != KSR::no_element()) {
          auto union_set = set_a;
          union_set.insert(set_b.begin(), set_b.end());
          if (!done.insert(union_set).second) {
            continue;
          }

          Point_2 inter;
          if (!KSR::intersection(
            m_data.to_2d(common_plane_idx,
              Segment_3(m_data.point_3(it_a->second.first), m_data.point_3(it_a->second.second))),
            m_data.to_2d(common_plane_idx,
              Segment_3(m_data.point_3(it_b->second.first), m_data.point_3(it_b->second.second))),
            inter)) {

            continue;
          }

          crossed_vertices.push_back(
            m_data.add_ivertex(m_data.to_3d(common_plane_idx, inter), union_set));
        }
      }
      crossed_vertices.push_back(it_a->second.second);
    }

    for (auto& t : todo) {
      m_data.add_iedge(t.first, t.second);
    }

    // Refine polygons.
    for (KSR::size_t i = 0; i < m_data.number_of_support_planes(); ++i) {
      Polygon_splitter splitter(m_data);
      splitter.split_support_plane(i);
    }
  }

  void set_k_intersections(const unsigned int k) {

    for (KSR::size_t i = 0; i < m_data.number_of_support_planes(); ++i) {
      for (const auto pface : m_data.pfaces(i)) {
        m_data.k(pface) = k;
      }
    }
  }

  bool initialize_queue()
  {
    std::cout << "Initializing queue for events in [" << m_min_time << ";" << m_max_time << "]" << std::endl;

    m_data.update_positions(m_max_time);

    bool still_running = false;

    for (KSR::size_t i = 0; i < m_data.number_of_support_planes(); ++ i)
    {
      KSR::vector<IEdge> iedges;
      KSR::vector<Segment_2> segments_2;
      KSR::vector<CGAL::Bbox_2> segment_bboxes;
      init_search_structures (i, iedges, segments_2, segment_bboxes);

      for (const PVertex pvertex : m_data.pvertices(i))
        if (compute_events_of_vertex (pvertex, iedges, segments_2, segment_bboxes))
          still_running = true;
    }

    m_data.update_positions(m_min_time);

    return still_running;
  }

  void init_search_structures (KSR::size_t i,
                               KSR::vector<IEdge>& iedges,
                               KSR::vector<Segment_2>& segments_2,
                               KSR::vector<CGAL::Bbox_2>& segment_bboxes)
  {
    // To get random access, copy in vector (suboptimal to do this
    // all the time, maybe this should be done once and for all and
    // replace the set)
    iedges.reserve (m_data.iedges(i).size());
    std::copy (m_data.iedges(i).begin(),
               m_data.iedges(i).end(),
               std::back_inserter(iedges));

    // Precompute segments and bboxes
    segments_2.reserve (iedges.size());
    segment_bboxes.reserve (iedges.size());
    for (const IEdge& iedge : iedges)
    {
      segments_2.push_back (m_data.segment_2 (i, iedge));
      segment_bboxes.push_back (segments_2.back().bbox());
    }
  }

  bool compute_events_of_vertex (const PVertex& pvertex,
                                 const KSR::vector<IEdge>& iedges,
                                 const KSR::vector<Segment_2>& segments_2,
                                 const KSR::vector<CGAL::Bbox_2>& segment_bboxes)
  {
    std::cout.precision(20);
    if (m_data.is_frozen(pvertex))
      return false;

    Segment_2 sv (m_data.point_2 (pvertex, m_min_time),
                  m_data.point_2 (pvertex, m_max_time));
    CGAL::Bbox_2 sv_bbox = sv.bbox();

    if (m_data.has_iedge(pvertex)) // Constrained vertex
    {
      // const auto cutime = m_data.current_time();
      // m_data.update_positions(m_min_time);
      // std::cout << "Computing events for the constrained pvertex: " << m_data.str(pvertex) << ": " << m_data.point_3(pvertex) << std::endl;
      // m_data.update_positions(cutime);

      // Test left and right vertices on mesh face
      PVertex prev;
      PVertex next;
      std::tie (prev, next) = m_data.prev_and_next (pvertex);

      for (const PVertex& pother : { prev, next })
      {
        if (pother == Data::null_pvertex()
            || !m_data.is_active(pother)
            || m_data.has_iedge (pother))
          continue;

        Segment_2 so (m_data.point_2 (pother, m_min_time),
                      m_data.point_2 (pother, m_max_time));
        CGAL::Bbox_2 so_bbox = so.bbox();

        if (!do_overlap (sv_bbox, so_bbox))
          continue;

        Point_2 point;
        if (!KSR::intersection(sv, so, point))
          continue;

        FT dist = CGAL::approximate_sqrt (CGAL::squared_distance (sv.source(), point));
        FT time = dist / m_data.speed(pvertex);

        m_queue.push (Event (true, pvertex, pother, m_min_time + time));

        // std::cout << "pvertex: " << m_data.point_3(pvertex) << std::endl;
        // std::cout << "pother: " << m_data.point_3(pother) << std::endl;
      }

      // Test end-vertices of intersection edge
      IEdge iedge = m_data.iedge(pvertex);
      for (const IVertex& ivertex : { m_data.source(iedge), m_data.target(iedge) })
      {
        if (!m_data.is_active(ivertex))
          continue;
        Point_2 pi = m_data.to_2d (pvertex.first, ivertex);
        if (sv.to_vector() * Vector_2 (sv.source(), pi) < 0)
          continue;

        FT dist = CGAL::approximate_sqrt(CGAL::squared_distance (sv.source(), pi));
        FT time = dist / m_data.speed(pvertex);

        if (time < m_max_time - m_min_time) {
          m_queue.push (Event (true, pvertex, ivertex, m_min_time + time));

          // std::cout << "pvertex: " << m_data.point_3(pvertex) << std::endl;
          // std::cout << "ivertex: " << m_data.point_3(ivertex) << std::endl;
        }
      }
    }
    else // Unconstrained vertex
    {
      PVertex prev = m_data.prev(pvertex);
      PVertex next = m_data.next(pvertex);

      // Test all intersection edges
      for (std::size_t j = 0; j < iedges.size(); ++ j)
      {
        const IEdge& iedge = iedges[j];

        if (m_data.iedge(prev) == iedge ||
            m_data.iedge(next) == iedge)
          continue;
        if (!m_data.is_active(iedge))
          continue;

        if (!CGAL::do_overlap (sv_bbox, segment_bboxes[j]))
          continue;

        Point_2 point;
        if (!KSR::intersection (sv, segments_2[j], point))
          continue;

        FT dist = CGAL::approximate_sqrt (CGAL::squared_distance (m_data.point_2 (pvertex, m_min_time), point));
        FT time = dist / m_data.speed(pvertex);

        m_queue.push (Event (false, pvertex, iedge, m_min_time + time));

        // std::cout << "pvertex: " << m_data.point_3(pvertex) << std::endl;
        // std::cout << "iedge: " << m_data.segment_3(iedge) << std::endl;
      }
    }
    return true;
  }

  const bool are_parallel(
    const Segment_2& seg1, const Segment_2& seg2) {

    const FT tol = FT(1) / FT(100000);
    FT m1 = FT(100000), m2 = FT(100000);

    const FT d1 = (seg1.target().x() - seg1.source().x());
    const FT d2 = (seg2.target().x() - seg2.source().x());

    if (CGAL::abs(d1) > tol)
      m1 = (seg1.target().y() - seg1.source().y()) / d1;
    if (CGAL::abs(d2) > tol)
      m2 = (seg2.target().y() - seg2.source().y()) / d2;

    // return CGAL::parallel(seg1, seg2); // exact version

    if (CGAL::abs(m1 - m2) < tol)
      return true;
    return false;
  }

  void run(const unsigned int k)
  {
    std::cout << "Unstacking queue size: " << m_queue.size() << std::endl;

    KSR::size_t iterations = 0;

    static int iter = 0;

    while (!m_queue.empty())
    {
      Event ev = m_queue.pop();

      FT current_time = ev.time();

      if (iter < 10)
      {
        dump (m_data, "iter_0" + std::to_string(iter));
        dump_event (m_data, ev, "iter_0" + std::to_string(iter));
      }
      else
      {
        dump (m_data, "iter_" + std::to_string(iter));
        dump_event (m_data, ev, "iter_" + std::to_string(iter));
      }

      m_data.update_positions (current_time);

      std::cout << "* APPLYING " << iter << ": " << ev << std::endl << std::endl;

      ++ iter;

      // if (iter == 50) {
      //   exit(0);
      // }

      apply(k, ev);
      check_integrity();

      // m_data.update_positions (0.5 * (current_time + m_queue.next().time()));
      // dump (m_data, "after_" + std::to_string(iter - 1));
      // m_data.update_positions (current_time);
      ++ iterations;
    }
  }

  void apply (
    const unsigned int k,
    const Event& ev)
  {
    PVertex pvertex = ev.pvertex();

    if (ev.is_pvertex_to_pvertex())
    {
      PVertex pother = ev.pother();

      remove_events (pvertex);
      remove_events (pother);

      CGAL_assertion (m_data.has_iedge(pvertex));

      if (m_data.has_iedge(pother)) // Two constrained vertices meet
      {
        CGAL_assertion_msg(false, "TODO: ADD CASE TWO CONSTRAINED PVERTICES MEET!");
      }
      else // One constrained vertex meets a free vertex
      {
        if (m_data.transfer_vertex(pvertex, pother)) {

          if (m_data.has_iedge(pvertex))
            remove_events(m_data.iedge(pvertex), pvertex.first); // Should we remove it here?
          if (m_data.has_iedge(pother))
            remove_events(m_data.iedge(pother), pother.first); // Should we remove it here?
          compute_events_of_vertices(ev.time(), std::array<PVertex,2>{pvertex, pother});

          PVertex prev, next;
          std::tie(prev, next) = m_data.border_prev_and_next(pvertex);

          PVertex pthird = prev;
          if (pthird == pother)
            pthird = next;
          else
            CGAL_assertion(next == pother);

          // remove_events(pthird);
          if (m_data.has_iedge(pthird))
            remove_events(m_data.iedge(pthird), pthird.first); // Should we remove it here?
          compute_events_of_vertices(ev.time(), std::array<PVertex,1>{pthird});

        } else {

          if (m_data.has_iedge(pvertex))
            remove_events(m_data.iedge(pvertex), pvertex.first); // Should we remove it here?
          compute_events_of_vertices(ev.time(), std::array<PVertex,1>{pvertex});
        }
      }
    }
    else if (ev.is_pvertex_to_iedge())
    {
      PVertex prev = m_data.prev(pvertex);
      PVertex next = m_data.next(pvertex);
      IEdge iedge = ev.iedge();
      PFace pface = m_data.pface_of_pvertex (pvertex);

      Segment_2 seg_edge = m_data.segment_2 (pvertex.first, iedge);

      bool done = false;
      for (const PVertex& pother : { prev, next })
      {
        Segment_2 seg (m_data.point_2(pother, ev.time()),
                       m_data.point_2(pvertex, ev.time()));
        CGAL_assertion (seg.squared_length() != 0);

        bool both_are_free = true;
        if (
          m_data.iedge(pvertex) != m_data.null_iedge() ||
          m_data.iedge(pother)  != m_data.null_iedge()) {
          both_are_free = false;
        }

        if (both_are_free && are_parallel(seg, seg_edge)) {

          remove_events(pvertex);
          remove_events(pother);

          bool collision, bbox_reached;
          std::tie(collision, bbox_reached) = m_data.collision_occured(pvertex, iedge);
          // std::tie(collision, bbox_reached) = m_data.is_occupied(pvertex, iedge);
          std::cout << "collision/bbox: " << collision << "/" << bbox_reached << std::endl;

          bool collision_other, bbox_reached_other;
          std::tie(collision_other, bbox_reached_other) = m_data.collision_occured(pother, iedge);
          // std::tie(collision_other, bbox_reached_other) = m_data.is_occupied(pother, iedge);
          std::cout << "other/bbox: " << collision_other << "/" << bbox_reached_other << std::endl;

          std::cout << "k intersections: " << m_data.k(pface) << std::endl;
          bool stop = false;
          if (bbox_reached) {

            CGAL_assertion(bbox_reached_other); // Can we have a case with only one box side reached?
            std::cout << "pv po k bbox" << std::endl;
            stop = true;

          } else if (bbox_reached_other) {

            CGAL_assertion(bbox_reached); // Can we have a case with only one box side reached?
            std::cout << "pv po k bbox" << std::endl;
            stop = true;

          } else if ((collision || collision_other) && m_data.k(pface) == 1) {

            std::cout << "pv po k stop" << std::endl;
            stop = true;

          } else if ((collision || collision_other) && m_data.k(pface) > 1) {

            std::cout << "pv po k continue" << std::endl;
            m_data.k(pface)--;

          } else {

            std::cout << "pv po continue" << std::endl;
            CGAL_assertion(m_data.iedge(pvertex) == m_data.iedge(pother));
            if (m_data.is_occupied(pvertex, iedge).first) {
              CGAL_assertion_msg(false, "TODO: TWO PVERTICES SNEAK ON THE OTHER SIDE EVEN WHEN WE HAVE A POLYGON!");
            }
          }
          CGAL_assertion(m_data.k(pface) >= 1);

          if (stop) // polygon stops
          {
            m_data.crop_polygon(pvertex, pother, iedge);
            remove_events(iedge, pvertex.first);
            compute_events_of_vertices(ev.time(), std::array<PVertex,2>{pvertex, pother});
          }
          else // polygon continues beyond the edge
          {
            PVertex pv0, pv1;
            std::tie(pv0, pv1) = m_data.propagate_polygon(m_data.k(pface), pvertex, pother, iedge);
            remove_events(iedge, pvertex.first);
            compute_events_of_vertices(ev.time(), std::array<PVertex, 4>{pvertex, pother, pv0, pv1});
          }

          done = true;
          break;
        }
      }

      if (!done) {

        remove_events(pvertex);

        bool collision, bbox_reached;
        std::tie(collision, bbox_reached) = m_data.collision_occured(pvertex, iedge);
        // std::tie(collision, bbox_reached) = m_data.is_occupied(pvertex, iedge);
        std::cout << "collision/bbox: " << collision << "/" << bbox_reached << std::endl;

        std::cout << "k intersections: " << m_data.k(pface) << std::endl;
        bool stop = false;
        if (bbox_reached) {

          std::cout << "pv k bbox" << std::endl;
          stop = true;

        } else if (collision && m_data.k(pface) == 1) {

          std::cout << "pv k stop" << std::endl;
          stop = true;

        } else if (collision && m_data.k(pface) > 1) {

          std::cout << "pv k continue" << std::endl;
          m_data.k(pface)--;

        } else {
          std::cout << "pv continue" << std::endl;
        }
        CGAL_assertion(m_data.k(pface) >= 1);

        if (stop) // polygon stops
        {
          const PVertex pvnew = m_data.crop_polygon(pvertex, iedge);
          remove_events(iedge, pvertex.first);
          compute_events_of_vertices(ev.time(), std::array<PVertex,2>{pvertex, pvnew});
        }
        else // polygon continues beyond the edge
        {
          const std::array<PVertex, 3> pvnew = m_data.propagate_polygon(m_data.k(pface), pvertex, iedge);
          remove_events(iedge, pvertex.first);
          compute_events_of_vertices(ev.time(), pvnew);
        }
      }
    }
    else if (ev.is_pvertex_to_ivertex())
    {
      // first, let's gather all vertices that will get merged
      std::vector<PVertex> pvertices
        = m_data.pvertices_around_ivertex (ev.pvertex(), ev.ivertex());

      for (auto& pv: pvertices)
        std::cerr << m_data.point_3(pv) << std::endl;
      std::cerr << std::endl;

      std::cerr << "Found " << pvertices.size() << " pvertices ready to be merged" << std::endl;

      // Remove associated events.
      // for (const PVertex& pvertex : pvertices)
      //   remove_events (pvertex);
      for (std::size_t i = 1; i < pvertices.size() - 1; ++i)
        remove_events (pvertices[i]);

      // Merge them and get the newly created vertices.
      std::vector<IEdge> crossed;
      std::vector<PVertex> new_pvertices
        = m_data.merge_pvertices_on_ivertex(m_min_time, m_max_time, pvertices, ev.ivertex(), crossed);

      // Remove all events of the crossed iedges.
      for (const auto& iedge : crossed)
        remove_events(iedge, pvertex.first);

      // And compute new events.
      CGAL_assertion(new_pvertices.size() > 0);
      compute_events_of_vertices (ev.time(), new_pvertices);
    }
    else
    {
      CGAL_assertion_msg (false, "ERROR: INVALID EVENT!");
    }
  }

  void remove_events (
    const IEdge& iedge,
    const KSR::size_t support_plane_idx) {

    m_queue.erase_vertex_events(iedge, support_plane_idx);
    // std::cout << "erasing events for iedge " << m_data.str(iedge) << std::endl;
    // std::cout << m_data.segment_3(iedge) << std::endl;
  }

  void remove_events (const PVertex& pvertex) {
    m_queue.erase_vertex_events (pvertex);
    // std::cout << "erasing events for pvertex " << m_data.str(pvertex) << " : " << m_data.point_3(pvertex) << std::endl;
  }

  template <typename PVertexRange>
  void compute_events_of_vertices (
    const FT last_event_time, const PVertexRange& pvertices) {

    m_min_time = m_data.current_time();
    m_data.update_positions(m_max_time);

    KSR::vector<IEdge> iedges;
    KSR::vector<Segment_2> segments_2;
    KSR::vector<CGAL::Bbox_2> segment_bboxes;
    init_search_structures(pvertices.front().first, iedges, segments_2, segment_bboxes);

    for (const PVertex& pvertex : pvertices)
      m_data.deactivate(pvertex);

    for (const PVertex& pvertex : pvertices) {
      m_data.set_last_event_time(pvertex, last_event_time);
      compute_events_of_vertex(pvertex, iedges, segments_2, segment_bboxes);
    }

    for (const PVertex& pvertex : pvertices)
      m_data.activate(pvertex);

    m_data.update_positions(m_min_time);
  }

};

} // namespace CGAL

#endif // CGAL_KINETIC_SHAPE_RECONSTRUCTION_3_H
