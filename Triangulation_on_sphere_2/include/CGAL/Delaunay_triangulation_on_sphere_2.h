// Copyright (c) 1997, 2012, 2019 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Mariette Yvinec,
//                 Claudia Werner
//                 Mael Rouxel-Labbé

#ifndef CGAL_DELAUNAY_TRIANGULATION_ON_SPHERE_2_H
#define CGAL_DELAUNAY_TRIANGULATION_ON_SPHERE_2_H

#include <CGAL/license/Triangulation_on_sphere_2.h>

#include <CGAL/Triangulation_on_sphere_2.h>
#include <CGAL/Triangulation_on_sphere_face_base_2.h>
#include <CGAL/Triangulation_on_sphere_vertex_base_2.h>
#include <CGAL/Triangulation_on_sphere_2/IO/OFF.h>

#include <CGAL/enum.h>
#include <CGAL/Origin.h>
#include <CGAL/utility.h>
#include <CGAL/spatial_sort_on_sphere.h>
#include <CGAL/Spatial_sort_traits_adapter_3.h>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/property_map/function_property_map.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <utility>
#include <vector>

namespace CGAL {

template <class Gt,
          class Tds = Triangulation_data_structure_2<
                        Triangulation_on_sphere_vertex_base_2<Gt>,
                        Triangulation_on_sphere_face_base_2<Gt> > >
class Delaunay_triangulation_on_sphere_2
  : public Triangulation_on_sphere_2<Gt, Tds>
{
  typedef Delaunay_triangulation_on_sphere_2<Gt, Tds> Self;
  typedef Triangulation_on_sphere_2<Gt, Tds>          Base;

public:
  typedef Gt                                      Geom_traits;
  typedef Tds                                     Triangulation_data_structure;

  typedef typename Geom_traits::FT                FT;
  typedef typename Geom_traits::Point_3           Point_3;
  typedef typename Geom_traits::Point_on_sphere_2 Point;
  typedef typename Geom_traits::Segment_3         Segment_3;
  typedef typename Geom_traits::Arc_on_sphere_2   Arc_on_sphere_2;

  typedef typename Base::Vertex                   Vertex;
  typedef typename Base::Vertex_handle            Vertex_handle;
  typedef typename Base::Edge                     Edge;
  typedef typename Base::Face_handle              Face_handle;

  typedef typename Base::Locate_type              Locate_type;

  typedef typename Base::Edge_circulator          Edge_circulator;
  typedef typename Base::Face_circulator          Face_circulator;
  typedef typename Base::All_vertices_iterator    All_vertices_iterator;
  typedef typename Base::All_edges_iterator       All_edges_iterator;
  typedef typename Base::All_faces_iterator       All_faces_iterator;

#ifndef CGAL_CFG_USING_BASE_MEMBER_BUG_2
  using Base::cw;
  using Base::ccw;
  using Base::tds;
  using Base::dimension;
  using Base::geom_traits;
  using Base::all_faces_begin;
  using Base::number_of_faces;
  using Base::number_of_vertices;
  using Base::point;
  using Base::all_faces_end;
  using Base::all_edges_begin;
  using Base::all_edges_end;
  using Base::vertices_begin;
  using Base::vertices_end;
  using Base::collinear_between;
  using Base::orientation_on_sphere;
  using Base::show_face;
  using Base::show_vertex;
  using Base::delete_faces;
  using Base::compare;
  using Base::VERTEX;
  using Base::EDGE;
  using Base::FACE;
  using Base::CONTOUR;
  using Base::OUTSIDE_CONVEX_HULL;
  using Base::OUTSIDE_AFFINE_HULL;
  using Base::NOT_ON_SPHERE;
  using Base::TOO_CLOSE;
#endif

  // class to sort points lexicographically.
  // This sorting is used for the symbolic perturbation in side_of_oriented_circle
  class Perturbation_order
  {
    const Self *t;
  public:
    Perturbation_order(const Self *tr) : t(tr) { }

    bool operator()(const Point* p, const Point* q) const
    {
      return t->compare(*p, *q) == SMALLER;
    }
  };

public:
  // CONSTRUCTORS
  Delaunay_triangulation_on_sphere_2(const Geom_traits& gt = Geom_traits()) : Base(gt) { }
  Delaunay_triangulation_on_sphere_2(const Point_3& center, const FT radius)
    : Base(center, radius)
  { }

  template <typename InputIterator>
  Delaunay_triangulation_on_sphere_2(InputIterator first, InputIterator beyond,
                                     const Geom_traits& gt = Geom_traits())
    : Delaunay_triangulation_on_sphere_2(gt)
  {
    insert(first, beyond);
  }

  // @todo complete copy operators

  // Predicates & Constructions
  Oriented_side side_of_oriented_circle(const Point& p, const Point& q, const Point& r, const Point& s, bool perturb = false) const;
  Oriented_side side_of_oriented_circle(const Face_handle f, const Point& p, bool perturb = false) const;
  bool test_conflict(const Point& p, Face_handle fh) const;

  //INSERTION
private:
  template <class OutputItFaces, class OutputItBoundaryEdges>
  std::pair<OutputItFaces, OutputItBoundaryEdges>
  non_recursive_propagate_conflicts(const Point& p,
                                    const Face_handle fh,
                                    const int i,
                                    std::pair<OutputItFaces,OutputItBoundaryEdges> pit) const
  {
    std::stack<std::pair<Face_handle, int> > stack;
    stack.push(std::make_pair(fh, i));

    while(!stack.empty())
    {
      const Face_handle fh = stack.top().first;
      const int i = stack.top().second;
      stack.pop();
      Face_handle fn = fh->neighbor(i);
      if(!test_conflict(p,fn))
      {
        *(pit.second)++ = Edge(fn, fn->index(fh));
      }
      else
      {
        *(pit.first)++ = fn;
        int j = fn->index(fh);

        // In the non-recursive version, we walk via 'ccw(j)' first. Here, we are filling the stack
        // and the order is thus the opposite (we want the top element of the stack to be 'ccw(j)')
        stack.push(std::make_pair(fn, cw(j)));
        stack.push(std::make_pair(fn, ccw(j)));
      }
    }

    return pit;
  }

  template <typename OutputItFaces, typename OutputItBoundaryEdges>
  std::pair<OutputItFaces, OutputItBoundaryEdges>
  propagate_conflicts(const Point& p,
                      Face_handle fh,
                      int i,
                      std::pair<OutputItFaces, OutputItBoundaryEdges> pit,
                      int depth = 0) const
  {
    if(depth == 100)
      return non_recursive_propagate_conflicts(p, fh, i, pit);

    Face_handle fn = fh->neighbor(i);
    if(fn->tds_data().is_in_conflict())
      return pit;

    if(!test_conflict(p, fn))
    {
      *(pit.second)++ = Edge(fn, fn->index(fh));
    }
    else
    {
      *(pit.first)++ = fn;
      fn->tds_data().mark_in_conflict();
      int j = fn->index(fh);
      pit = propagate_conflicts(p, fn, ccw(j), pit, depth + 1);
      pit = propagate_conflicts(p, fn, cw(j), pit, depth + 1);
    }

    return pit;
  }

public:
  template <typename OutputItFaces, typename OutputItBoundaryEdges>
  std::pair<OutputItFaces,OutputItBoundaryEdges>
  get_conflicts_and_boundary(const Point& p,
                             OutputItFaces fit,
                             OutputItBoundaryEdges eit,
                             Face_handle fh) const
  {
    CGAL_precondition(dimension() == 2);
    CGAL_precondition(test_conflict(p, fh));

    *fit++ = fh; //put fh in OutputItFaces
    fh->tds_data().mark_in_conflict();

    std::pair<OutputItFaces, OutputItBoundaryEdges> pit = std::make_pair(fit, eit);
    pit = propagate_conflicts(p, fh, 0, pit);
    pit = propagate_conflicts(p, fh, 1, pit);
    pit = propagate_conflicts(p, fh, 2, pit);

    return std::make_pair(fit, eit);
  }

public:
  Vertex_handle insert(const Point& p, Face_handle f = Face_handle());
  Vertex_handle push_back(const Point& p, Face_handle f = Face_handle()) { return insert(p, f); }
  Vertex_handle insert(const Point& p, Locate_type lt, Face_handle loc, int li);
  Vertex_handle insert_first(const Point& p);
  Vertex_handle insert_second(const Point& p);
  Vertex_handle insert_third(const Point& p);
  Vertex_handle insert_outside_affine_hull_regular(const Point& p);
  Vertex_handle insert_cocircular(const Point& p, Locate_type lt, Face_handle loc);

  template <typename InputIterator>
  int insert(InputIterator first, InputIterator beyond,
             typename std::enable_if<
                        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                                     Point>::value>::type* = nullptr);

  // For convenience when P3 != PoS2
  template <typename P>
  Vertex_handle insert(const P& p,
                       Face_handle f = Face_handle(),
                       typename std::enable_if<!std::is_same<P, Point>::value>::type* = nullptr)
  {
    return insert(geom_traits().construct_point_on_sphere_2_object()(p), f);
  }

  template <typename InputIterator>
  int insert(InputIterator first, InputIterator beyond,
             typename std::enable_if<
                        !std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                                      Point>::value>::type* = nullptr)
  {
    typename Geom_traits::Construct_point_on_sphere_2 cst = geom_traits().construct_point_on_sphere_2_object();

    return insert(boost::make_transform_iterator(first, cst),
                  boost::make_transform_iterator(beyond, cst));
  }

  bool update_ghost_faces(Vertex_handle v = Vertex_handle(), bool first = false);

  //REMOVAL
  void remove_degree_3(Vertex_handle v, Face_handle f = Face_handle());
  void remove_1D(Vertex_handle v);
  void remove_2D(Vertex_handle v);
  void remove(Vertex_handle v);

  bool test_dim_down(Vertex_handle v);
  bool test_dim_up(const Point& p) const;
  void fill_hole_regular(std::list<Edge>& hole);

  // Dual
  Point_3 circumcenter(const Point_3& p0, const Point_3& p1, const Point_3& p2) const;
  Point_3 circumcenter(const Face_handle f) const;
  Point_3 dual(const Face_handle f) const;
  Point circumcenter_on_sphere(const Point& p0, const Point& p1, const Point& p2) const;
  Point circumcenter_on_sphere(const Face_handle f) const;
  Point dual_on_sphere(const Face_handle f) const;
  Segment_3 dual(const Edge& e) const;
  Segment_3 dual(const Edge_circulator ec) const { return dual(*ec); }
  Segment_3 dual(const All_edges_iterator ei) const { return dual(*ei); }
  Arc_on_sphere_2 arc_dual(const Edge& e) const;
  Arc_on_sphere_2 arc_dual(const Edge_circulator ec) const { return arc_dual(*ec); }
  Arc_on_sphere_2 arc_dual(const All_edges_iterator ei) const { return arc_dual(*ei); }

  // Validity
  void check_neighboring_faces() const;
  bool is_plane() const;
  bool is_valid(bool verbose = false, int level = 0) const;
  bool is_valid_face(Face_handle fh, bool verbose = false, int level = 0) const;
  bool is_valid_vertex(Vertex_handle fh, bool verbose = false, int level = 0) const;
};

// ------------------------ PREDICATES / CONSTRUCTIONS --------------------------------//

// computes the power test of 4 points. 'perturb' defines whether a symbolic perturbation
// is used (by default, perturb == false)
// in the perturbation the smallest vertex is in conflict with the others
template <typename Gt, typename Tds>
inline
Oriented_side
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
side_of_oriented_circle(const Point& p0, const Point& p1, const Point& p2, const Point& p,
                        bool perturb) const
{
  // Specificity of the ToS_2: the in-circle is a call to orientation_3
  Oriented_side os = orientation(p0, p1, p2, p);
  if(os != ON_ORIENTED_BOUNDARY || !perturb)
    return os;

  // We are now in a degenerate case => we do a symbolic perturbation.

  // We sort the points lexicographically.
  const Point* points[4] = { &p0, &p1, &p2, &p };
  std::sort(points, points + 4, Perturbation_order(this));

  // @fixme revert to the former approach?
  for(int i=3; i>0; --i)
  {
    if(points[i] == &p)
      return ON_NEGATIVE_SIDE; // since p0 p1 p2 are non collinear and positively oriented

    Orientation o;
    if ((points[i] == &p2) && ((o = orientation_on_sphere(p0, p1, p)) != COLLINEAR))
      return Oriented_side(o);
    if ((points[i] == &p1) && ((o = orientation_on_sphere(p0, p, p2)) != COLLINEAR))
      return Oriented_side(o);
    if ((points[i] == &p0) && ((o = orientation_on_sphere(p, p1, p2)) != COLLINEAR))
      return Oriented_side(o);
  }

  CGAL_assertion(false);
  return ON_NEGATIVE_SIDE;
}

template <typename Gt, typename Tds>
Oriented_side
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
side_of_oriented_circle(const Face_handle f, const Point& p, bool perturb) const
{
  return side_of_oriented_circle(point(f, 0), point(f, 1), point(f, 2), p, perturb);
}

// tests whether there is a conflict between p and the face fh
template <typename Gt, typename Tds>
inline bool
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
test_conflict(const Point& p, Face_handle fh) const
{
  return(side_of_oriented_circle(fh, p, true) != ON_NEGATIVE_SIDE);
}

// ------------------------ INSERTION --------------------------------//
template <typename Gt, typename Tds>
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Vertex_handle
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
insert(const Point& p, Face_handle start)
{
  Locate_type lt;
  int li;
  Face_handle loc = Base::locate(p, lt, li, start);

  switch(lt)
  {
    case NOT_ON_SPHERE:
      return Vertex_handle();
    case TOO_CLOSE:
    {
      if(dimension() == 2)
        return loc->vertex(li);

      return Vertex_handle(); // @fixme why is nothing returned if the dimension is e.g. 1 ?
    }
    case VERTEX:
    {
      if(number_of_vertices() == 1)
        return vertices_begin();

      return (loc->vertex(li));
    }
    default: // the point can be inserted
      return insert(p, lt, loc, li);
  }
}

// inserts a new point to a 1d triangulation, the new point is also coplanar with the existing points.
template <typename Gt, typename Tds>
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Vertex_handle
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
insert_cocircular(const Point& p,
                  Locate_type /*lt*/,
                  Face_handle loc)
{
  CGAL_precondition(!test_dim_up(p));
  CGAL_precondition(dimension() == 1);

  Vertex_handle v0 = loc->vertex(0);
  Vertex_handle v1 = loc->vertex(1);
  Vertex_handle v = tds().create_vertex();
  v->set_point(p);

  Face_handle f1 = tds().create_face(v0, v, Vertex_handle());
  Face_handle f2 = tds().create_face(v, v1, Vertex_handle());

  v->set_face(f1);
  v0->set_face(f1);
  v1->set_face(f2);

  tds().set_adjacency(f1,0, f2,1);
  tds().set_adjacency(f1,1, loc->neighbor(1),0);
  tds().set_adjacency(f2,0, loc->neighbor(0),1);

  tds().delete_face(loc);

  update_ghost_faces(v);
  return v;
}

template <typename Gt, typename Tds>
typename Triangulation_on_sphere_2<Gt, Tds>::Vertex_handle
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
insert_first(const Point& p)
{
  CGAL_precondition(number_of_vertices() == 0);
  Vertex_handle v = tds().insert_first();
  v->set_point(p);
  return v;
}

template <typename Gt, typename Tds>
typename Triangulation_on_sphere_2<Gt, Tds>::Vertex_handle
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
insert_second(const Point& p)
{
  CGAL_precondition(number_of_vertices() == 1);
  Vertex_handle v = tds().insert_second();
  v->set_point(p);
  return v;
}

template <typename Gt, typename Tds>
typename Triangulation_on_sphere_2<Gt, Tds>::Vertex_handle
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
insert_third(const Point& p)
{
  std::cout << "insert_third" << std::endl;
  CGAL_assertion(number_of_vertices() == 2);

  Vertex_handle v = vertices_begin();
  Vertex_handle u = v->face()->neighbor(0)->vertex(0);
  Vertex_handle nv;

  // orientation is given by the first 2 points
  if(collinear_between(point(v), point(u), p) ||
     orientation_on_sphere(point(u), point(v), p) == LEFT_TURN)
  {
    nv = Base::tds().insert_dim_up(v, false);
  }
  else
  {
    nv = Base::tds().insert_dim_up(v, true);
  }

  nv->set_point(p);

  std::cout << "actual #faces " << tds().faces().size() << std::endl;

  Face_handle f = all_edges_begin()->first;
  CGAL_assertion(orientation_on_sphere(point(f, 0),
                                       point(f, 1),
                                       point(f->neighbor(0), 1)) != RIGHT_TURN);

  update_ghost_faces(nv);
  return nv;
}

//inserts a new point which lies outside the affine hull of the other points
template <typename Gt, typename Tds>
typename Triangulation_on_sphere_2<Gt, Tds>::Vertex_handle
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
insert_outside_affine_hull_regular(const Point& p)
{
  std::cout << "insert_outside_affine_hull_regular" << std::endl;
  CGAL_precondition(dimension() == 1 && number_of_vertices() >= 3);

  bool conform = false;
  Face_handle f = (all_edges_begin())->first;
  Face_handle fn = f->neighbor(0);
  const Point& p0 = point(f, 0);
  const Point& p1 = point(f, 1);
  const Point& p2 = point(fn, 1);

  CGAL_assertion(orientation_on_sphere(p0, p1, p2) != NEGATIVE);
  Orientation orient2 = side_of_oriented_circle(p0, p1, p2, p);

  if(orient2 == POSITIVE)
    conform = true;

  // find smallest vertex this step garanties a unique triangulation
  Vertex_handle w = vertices_begin();
  All_vertices_iterator vi;
  for(vi=vertices_begin(); vi!=vertices_end(); ++vi)
  {
    if(compare(point(vi), point(w)) == SMALLER)
      w = vi;
  }

  Vertex_handle v = tds().insert_dim_up(w, conform);
  v->set_point(p);

  update_ghost_faces(v, true /*dimension is increasing to 2*/);

  return v;
}

// inserts a point which location is known. Calls the corresponding insert-function
// e.g. insert_first
template <typename Gt, typename Tds>
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Vertex_handle
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
insert(const Point& p, Locate_type lt, Face_handle loc, int /*li*/)
{
  Vertex_handle v;
  switch(dimension())
  {
    case -2: // empty
      return insert_first(p);
    case -1: // 1 vertex
      return insert_second(p);
    case 0: // 2 vertices
      return insert_third(p);
    case 1:
    {
      if(test_dim_up(p))
        return insert_outside_affine_hull_regular(p);
      else
        return insert_cocircular(p, lt, loc);
    }
    case 2:
    {
      std::vector<Face_handle> faces;
      std::vector<Edge> edges;
      faces.reserve(32);
      edges.reserve(32);

      get_conflicts_and_boundary(p, std::back_inserter(faces), std::back_inserter(edges), loc);

      for(Face_handle fh : faces)
        fh->tds_data().clear();

      v = tds().star_hole(edges.begin(), edges.end());
      v->set_point(p);
      delete_faces(faces.begin(), faces.end());

      if(lt != FACE)
        update_ghost_faces(v);

      return v;
    }
  }

  CGAL_assertion(false);
  return v;
}

/*
 * method to test and mark faces incident to v as ghost-faces or solid-faces.
 * the Boolean 'first' defines whether the dimension of the triangulation increased
 * from 1 to 2 by inserting v. If this is the case, all faces are tested.
 */
template <typename Gt, typename Tds>
bool
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
update_ghost_faces(Vertex_handle v, bool first)
{
  if(number_of_vertices() < 3)
    return false;

  CGAL_assertion(dimension() >= 1);

  bool ghost_found = false;
  if(dimension() == 1)
  {
    All_edges_iterator eit = all_edges_begin();
    for(; eit!=all_edges_end(); ++eit)
    {
      Face_handle f = eit->first;
      Face_handle fn = f->neighbor(0);
      const Point& q = point(fn, 1);
      if(this->collinear_between(point(f, 0), point(f, 1), q))
      {
        f->set_ghost(true);
        ghost_found = true;
      }
      else
      {
        f->set_ghost(false);
      }
    }
  }
  else // dimension==2
  {
    if(first) // first time dimension 2
    {
      All_faces_iterator fi = all_faces_begin();
      for(; fi!=all_faces_end(); fi++)
      {
        if(orientation_on_sphere(fi) != POSITIVE)
        {
          fi->set_ghost(true);
          ghost_found = true;
        }
        else
        {
          fi->set_ghost(false);
        }
      }
    }
    else // not first
    {
      CGAL_assertion(v != Vertex_handle());
      Face_circulator fc = this->incident_faces(v, v->face());
      Face_circulator done(fc);
      do
      {
        if(orientation_on_sphere(fc) != POSITIVE)
        {
          fc->set_ghost(true);
          ghost_found = true;
        }
        else
        {
          fc->set_ghost(false);
        }
      }
      while(++fc != done);
    }
  }

  return ghost_found;
}

template <typename Gt, typename Tds>
template <typename InputIterator>
int
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
insert(InputIterator first, InputIterator beyond,
       typename std::enable_if<
                  std::is_same<typename std::iterator_traits<InputIterator>::value_type, Point>::value>::type*)
{
  int n = number_of_vertices();

  std::vector<Point> points(first, beyond);
  CGAL::cpp98::random_shuffle(points.begin(), points.end());

  typename Geom_traits::Construct_point_3 cp3 = geom_traits().construct_point_3_object();

  typedef typename Geom_traits::Construct_point_3 Construct_point_3;
  typedef typename boost::result_of<const Construct_point_3(const Point&)>::type Ret;
  typedef boost::function_property_map<Construct_point_3,Point, Ret> fpmap;
  typedef CGAL::Spatial_sort_traits_adapter_3<Geom_traits, fpmap> Search_traits_3;

  // @todo:
  // - bench this
  // - should points not on the sphere already be filtered because it might mess up the sort?
  spatial_sort_on_sphere(points.begin(), points.end(),
                         Search_traits_3(
                           boost::make_function_property_map<Point, Ret, Construct_point_3>(
                             geom_traits().construct_point_3_object()), geom_traits()),
                         square(geom_traits().radius()), geom_traits().center());

  Face_handle hint;
  Vertex_handle v;
  for(typename std::vector<Point>::const_iterator p=points.begin(), end=points.end(); p!=end; ++p)
  {
    v = insert(*p, hint);
    if(v != Vertex_handle()) // could happen if the point is not on the sphere
      hint = v->face();
  }

  return number_of_vertices() - n;
}

//-------------------------------------REMOVAL----------------------------------------------------//
template <typename Gt, typename Tds>
void
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
remove_degree_3(Vertex_handle v, Face_handle f)
{
  CGAL_precondition(v != Vertex_handle());

  if(f == Face_handle())
    f = v->face();

  tds().remove_degree_3(v, f);
}

template <typename Gt, typename Tds>
void
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
remove(Vertex_handle v)
{
  CGAL_precondition(v != Vertex_handle());

  if(number_of_vertices() <= 3)
    tds().remove_dim_down(v);
  else if(dimension() == 2)
    remove_2D(v);
  else
    remove_1D(v);
}

template <typename Gt, typename Tds>
void
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
remove_1D(Vertex_handle v)
{
  CGAL_precondition(v != Vertex_handle());

  tds().remove_1D(v);
  update_ghost_faces();
}

template <typename Gt, typename Tds>
void
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
remove_2D(Vertex_handle v)
{
  CGAL_precondition(dimension() == 2);

  if(test_dim_down(v)) // resulting triangulation has dim 1
  {
    tds().remove_dim_down(v);
    update_ghost_faces(); //1d triangulation, no vertex needed to update ghost-faces
  }
  else
  {
    std::list<Edge> hole;
    tds().make_hole(v, hole);
    fill_hole_regular(hole);
  }
}

// tests whether the dimension will decrease when removing v from the triangulation.
template <typename Gt, typename Tds>
bool
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
test_dim_down(Vertex_handle v)
{
  CGAL_precondition(dimension() == 2 && number_of_vertices() >= 4);

  bool will_decrease = true;
  if(number_of_vertices() == 4)
    return will_decrease;

  All_vertices_iterator it = (vertices_begin() != v) ? vertices_begin() : std::next(vertices_begin());
  All_vertices_iterator it2 = std::next(it, (std::next(it) != v) ? 1 : 2);
  All_vertices_iterator it3 = std::next(it2, (std::next(it2) != v) ? 1 : 2);
  All_vertices_iterator it4 = std::next(it3, (std::next(it3) != v) ? 1 : 2);

  for(;;)
  {
    if(it4 == vertices_end())
      break;

    CGAL_assertion(it != v && it2 != v && it3 != v && it4 != v);

    if(side_of_oriented_circle(it->point(), it2->point(), it3->point(), it4->point()) != ON_ORIENTED_BOUNDARY)
      return false;

    std::advance(it, (std::next(it) != v) ? 1 : 2);
    std::advance(it2, (std::next(it2) != v) ? 1 : 2);
    std::advance(it3, (std::next(it3) != v) ? 1 : 2);
    std::advance(it4, (std::next(it4) != v) ? 1 : 2);
  }

  return true;
}

// tests whether the dimension will increase when adding p to the triangulation.
template <typename Gt, typename Tds>
bool
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
test_dim_up(const Point& p) const
{
  CGAL_precondition(dimension() == 1);

  Face_handle f = all_edges_begin()->first;
  Vertex_handle v1 = f->vertex(0);
  Vertex_handle v2 = f->vertex(1);
  Vertex_handle v3 = f->neighbor(0)->vertex(1);

  return (side_of_oriented_circle(point(v1), point(v2), point(v3), p) != ON_ORIENTED_BOUNDARY);
}

//fill the hole in a triangulation after vertex removal.
template <typename Gt, typename Tds>
void
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
fill_hole_regular(std::list<Edge>& first_hole)
{
  typedef std::list<Edge> Hole;
  typedef std::list<Hole> Hole_list;

  Hole hole;
  Hole_list hole_list;
  Face_handle ff, fn;
  int i, ii, in;

  hole_list.push_front(first_hole);

  while(!hole_list.empty())
  {
    hole = hole_list.front();
    hole_list.pop_front();
    typename Hole::iterator hit = hole.begin();

    // if the hole has only three edges, create the triangle
    if(hole.size() == 3)
    {
      Face_handle newf = tds().create_face();
      hit = hole.begin();
      for(int j=0; j<3; ++j)
      {
        ff = (*hit).first;
        ii = (*hit).second;
        hit++;
        ff->set_neighbor(ii, newf);
        newf->set_neighbor(j, ff);
        newf->set_vertex(newf->ccw(j), ff->vertex(ff->cw(ii)));
      }

      if(orientation_on_sphere(newf) != POSITIVE)
        newf->set_ghost(true);

      continue;
    }

    // else find an edge with two vertices on the hole boundary and the new triangle adjacent to that edge
    // cut the hole and push it back

    // take the first neighboring face and pop it;
    ff = hole.front().first;
    ii = hole.front().second;
    hole.pop_front();

    Vertex_handle v0 = ff->vertex(ff->cw(ii));
    const Point& p0 = point(v0);
    Vertex_handle v1 = ff->vertex(ff->ccw(ii));
    const Point& p1 = point(v1);
    Vertex_handle v2 = Vertex_handle();
    Point p2;
    Vertex_handle vv;
    Point p;

    typename Hole::iterator hdone = hole.end();
    hit = hole.begin();
    typename Hole::iterator cut_after(hit);

    // if tested vertex is c with respect to the vertex opposite
    // to NULL neighbor,
    // stop at the before last face;
    --hdone;

    while(hit != hdone)
    {
      fn = (*hit).first;
      in = (*hit).second;
      vv = fn->vertex(ccw(in));
      p = point(vv);

      if(/*orientation_on_sphere(p0, p1, p) == COUNTERCLOCKWISE*/ 1) // @fixme '1' ???
      {
        if(v2 == Vertex_handle())
        {
          v2 = vv;
          p2 = p;
          cut_after=hit;
        }
        else if(side_of_oriented_circle(p0, p1, p2, p) == ON_POSITIVE_SIDE)
        {
          v2 = vv;
          p2 = p;
          cut_after = hit;
        }
      }

      ++hit;
    }

    // create new triangle and update adjacency relations
    Face_handle newf = tds().create_face(v0, v1, v2);
    newf->set_neighbor(2, ff);
    ff->set_neighbor(ii, newf);
    if(orientation_on_sphere(newf) != POSITIVE)
      newf->set_ghost(true);

    //update the hole and push back in the Hole_List stack
    // if v2 belongs to the neighbor following or preceding *f
    // the hole remain a single hole
    // otherwise it is split in two holes

    fn = hole.front().first;
    in = hole.front().second;
    if(fn->has_vertex(v2, i) && i == fn->ccw(in))
    {
      newf->set_neighbor(0, fn);
      fn->set_neighbor(in, newf);
      hole.pop_front();
      hole.push_front(Edge(newf, 1));
      hole_list.push_front(hole);
    }
    else
    {
      fn = hole.back().first;
      in = hole.back().second;
      if(fn->has_vertex(v2, i) && i == fn->cw(in))
      {
        newf->set_neighbor(1, fn);
        fn->set_neighbor(in, newf);
        hole.pop_back();
        hole.push_back(Edge(newf, 0));
        hole_list.push_front(hole);
      }
      else
      { // split the hole in two holes
        Hole new_hole;
        ++cut_after;
        while(hole.begin() != cut_after)
        {
          new_hole.push_back(hole.front());
          hole.pop_front();
        }

        hole.push_front(Edge(newf, 1));
        new_hole.push_front(Edge(newf, 0));
        hole_list.push_front(hole);
        hole_list.push_front(new_hole);
      }
    }
  }
}

//-----------------dual------------------------
template<class Gt, class Tds>
inline
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Point_3
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
circumcenter(const Point_3& p0, const Point_3& p1, const Point_3& p2) const
{
  return geom_traits().construct_circumcenter_3_object()(p0, p1, p2);
}

template <typename Gt, typename Tds>
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Point_3
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
circumcenter(const Face_handle f) const
{
  typename Geom_traits::Construct_point_3 cp3 = geom_traits().construct_point_3_object();
  return circumcenter(cp3(point(f, 0)), cp3(point(f, 1)), cp3(point(f, 2)));
}

template<class Gt, class Tds>
inline
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Point
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
circumcenter_on_sphere(const Point& p0, const Point& p1, const Point& p2) const
{
  return geom_traits().construct_circumcenter_on_sphere_2_object()(p0, p1, p2);
}

template <typename Gt, typename Tds>
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Point
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
circumcenter_on_sphere(const Face_handle f) const
{
  return circumcenter(point(f, 0), point(f, 1), point(f, 2));
}

template <typename Gt, typename Tds>
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Point
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
dual_on_sphere(const Face_handle f) const
{
  return circumcenter_on_sphere(f);
}

// Following methods are used to compute the Voronoi-Diagram
template <typename Gt, typename Tds>
inline
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Point_3
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
dual(const Face_handle f) const
{
  CGAL_precondition(tds().is_face(f));
  CGAL_precondition(this->dimension() == 2);
  return circumcenter(f);
}

template <typename Gt, typename Tds>
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Segment_3
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
dual(const Edge& e) const
{
  CGAL_precondition(tds().is_edge(e.first, e.second));
  CGAL_precondition(dimension() == 2);

  return geom_traits().construct_segment_3_object()(dual(e.first),
                                                    dual(e.first->neighbor(e.second)));
}

template <typename Gt, typename Tds>
typename Delaunay_triangulation_on_sphere_2<Gt, Tds>::Arc_on_sphere_2
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
arc_dual(const Edge& e) const
{
  CGAL_precondition(tds().is_edge(e.first, e.second));
  CGAL_precondition(dimension() == 2);

  // @fixme ensure in arc_dual and arc_segment that 'cp' and 'cq' are in the correct order
  return geom_traits().construct_arc_on_sphere_2_object()(dual(e.first),
                                                          dual(e.first->neighbor(e.second)));
}

//-------------------------------------------CHECK------------------------------------------------//
// checks whether neighboring faces are linked correctly to each other.
template <typename Gt, typename Tds>
void
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
check_neighboring_faces() const
{
  if(dimension() == 1)
  {
    for(All_faces_iterator eit=all_faces_begin(); eit!=all_faces_end(); ++eit)
    {
      Face_handle f1 = eit->neighbor(0);
      Face_handle f2 = eit->neighbor(1);
      CGAL_assertion(f1->has_neighbor(eit));
      CGAL_assertion(f2->has_neighbor(eit));
    }
  }

  for(All_faces_iterator eit=all_faces_begin(); eit!=all_faces_end(); ++eit)
  {
    Face_handle f1 = eit->neighbor(0);
    Face_handle f2 = eit->neighbor(1);
    Face_handle f3 = eit->neighbor(2);
    CGAL_assertion(f1->has_neighbor(eit));
    CGAL_assertion(f2->has_neighbor(eit));
    CGAL_assertion(f3->has_neighbor(eit));
  }
}

//checks whether a given triangulation is plane (all points are coplanar)
template <typename Gt, typename Tds>
bool
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
is_plane() const
{
  if(number_of_vertices() <= 3)
    return true;

  bool plane = true;

  All_vertices_iterator it1 = vertices_begin(), it2(it1), it3(it1), it4(it1);
  std::advance(it2, 1);
  std::advance(it3, 2);
  std::advance(it4, 3);

  while(it4 != vertices_end())
  {
    Orientation s = side_of_oriented_circle(point(it1), point(it2), point(it3), point(it4));
    plane = plane && s == ON_ORIENTED_BOUNDARY;

    if(!plane)
      return false;

    ++it1;
    ++it2;
    ++it3;
    ++it4;
  }

  return true;
}

template <typename Gt, typename Tds>
bool
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
is_valid(bool verbose, int level) const
{
  bool result = true;

  if(!tds().is_valid(verbose, level))
  {
    if(verbose)
      std::cerr << "invalid data structure" << std::endl;

    CGAL_assertion(false);
    return false;
  }

  for(All_faces_iterator fit=all_faces_begin(); fit!=all_faces_end(); ++fit)
    is_valid_face(fit, verbose, level);

  for(All_vertices_iterator vit=vertices_begin(); vit!=vertices_end(); ++vit)
    is_valid_vertex(vit, verbose, level);

  switch(dimension())
  {
    case 0:
      break;
    case 1:
      CGAL_assertion(this->is_plane());
      break;
    case 2:
      for(All_faces_iterator it=all_faces_begin(); it!=all_faces_end(); ++it)
      {
        Orientation s = orientation_on_sphere(point(it, 0), point(it, 1), point(it, 2));
        result = result && (s != NEGATIVE || it->is_ghost());
        CGAL_assertion(result);
      }

      result = result && (number_of_faces() == 2*(number_of_vertices()) - 4);
      CGAL_assertion(result);
      break;
  }

  // in any dimension
  if(verbose)
    std::cerr << " number of vertices " << number_of_vertices() << "\t" << std::endl;

  CGAL_assertion(result);
  return result;
}

template <typename Gt, typename Tds>
bool
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
is_valid_vertex(Vertex_handle vh, bool verbose, int /*level*/) const
{
  bool result = vh->face()->has_vertex(vh);
  if(!result)
  {
    if(verbose)
    {
      std::cerr << " from is_valid_vertex " << std::endl;
      std::cerr << "normal vertex " << &(*vh) << std::endl;
      show_vertex(vh);
      std::cerr << "\nvh_>face " << &*(vh->face()) << " " << std::endl;

      show_face(vh->face());
    }

    CGAL_assertion(false);
    return false;
  }

  return true;
}

template <typename Gt, typename Tds>
bool
Delaunay_triangulation_on_sphere_2<Gt, Tds>::
is_valid_face(Face_handle fh, bool verbose, int /*level*/) const
{
  bool result = true;
  for(int i=0; i<+2; ++i)
  {
    Orientation test = side_of_oriented_circle(fh, point(fh->vertex(i)));
    result = result && test == ON_ORIENTED_BOUNDARY;
    CGAL_assertion(result);
  }

  if(!result)
  {
    if(verbose)
    {
      std::cerr << " from is_valid_face " << std::endl;
      std::cerr << " face " << std::endl;
      show_face(fh);
    }
  }

  CGAL_assertion(result);
  return result;
}

} // namespace CGAL

#endif // CGAL_DELAUNAY_TRIANGULATION_ON_SPHERE_2_H
