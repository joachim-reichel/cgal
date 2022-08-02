// Copyright (c) 2022 GeometryFactory (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Hossam Saeed
//

#ifndef CGAL_POLYGON_MESH_PROCESSING_INTERPOLATED_CORRECTED_CURVATURE_MEASURES_H
#define CGAL_POLYGON_MESH_PROCESSING_INTERPOLATED_CORRECTED_CURVATURE_MEASURES_H
#endif

#include <CGAL/license/Polygon_mesh_processing/interpolated_corrected_curvature_measures.h>

#include <CGAL/assertions.h>
#include <CGAL/Polygon_mesh_processing/internal/named_params_helper.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Named_function_parameters.h>
#include <CGAL/property_map.h>

#include <numeric>
#include <queue>
#include <unordered_set>
#include <functional>

namespace CGAL {

namespace Polygon_mesh_processing {

/*!
 * \ingroup PMP_corrected_curvatures_grp
 * Enumeration type used to specify which measure of a given face
 * is computed for the interpolated corrected curvature functions
 */
// enum
enum Curvature_measure_index {
    MU0_AREA_MEASURE, ///< corrected area density
    MU1_MEAN_CURVATURE_MEASURE, ///< corrected mean curvature density
    MU2_GAUSSIAN_CURVATURE_MEASURE ///< corrected gaussian curvature density
};

/**
* \ingroup PMP_corrected_curvatures_grp
*
* computes the interpolated corrected area measure (mu0) of a specific face.
*
* @tparam GT is the geometric traits class.
*
* @param x is a vector of the vertex positions of the face.
* @param u is a vector of the vertex nomrals of the face.
*
* @return a scalar of type `GT::FT`.
* This is the value of the interpolated corrected area measure of the given face.
*
* @see `interpolated_corrected_mean_curvature_measure_face()`
* @see `interpolated_corrected_gaussian_curvature_measure_face()`
* @see `interpolated_corrected_measure_mesh()`
*/
template<typename GT>
typename GT::FT interpolated_corrected_area_measure_face(const std::vector<typename GT::Vector_3>& u,
                                                         const std::vector<typename GT::Vector_3>& x = {})
{
    std::size_t n = x.size();
    CGAL_precondition(u.size() == n);
    CGAL_precondition(n >= 3);

    typename GT::Construct_cross_product_vector_3 cross_product;

    // Triangle: use triangle formula
    if (n == 3)
    {
        const typename GT::Vector_3 um = (u[0] + u[1] + u[2]) / 3.0;
        return 0.5 * um * cross_product(x[1] - x[0], x[2] - x[0]);
    }
    // Quad: use bilinear interpolation formula
    else if (n == 4)
    {
        // for the formulas below, values of verices 2 & 3 are swapped (compared to paper) to correct order.
        // the indices in paper vs in here are: 00 = 0, 10 = 1, 11 = 2, 01 = 3
        return (1 / 36.0) * (
              (4 * u[0] + 2 * u[1] + 2 * u[3] + u[2]) * cross_product(u[1] - u[0], u[3] - u[0])
            + (2 * u[0] + 4 * u[1] + u[3] + 2 * u[2]) * cross_product(u[1] - u[0], u[2] - u[1])
            + (2 * u[0] + u[1] + 4 * u[3] + 2 * u[2]) * cross_product(u[2] - u[3], u[3] - u[0])
            + (u[0] + 2 * u[1] + 2 * u[3] + 4 * u[2]) * cross_product(u[2] - u[3], u[2] - u[1])
            );
    }
    // N-gon: split into n triangles by polygon center and use triangle formula for each
    else
    {
        typename GT::FT mu0 = 0;

        // getting center of points
        typename GT::Vector_3 xc =
            std::accumulate(x.begin(), x.end(), typename GT::Vector_3(0, 0, 0));
        xc /= n;

        // getting unit average normal of points
        typename GT::Vector_3 uc =
            std::accumulate(u.begin(), u.end(), typename GT::Vector_3(0, 0, 0));
        uc /= sqrt(uc * uc);

        // summing each triangle's measure after triangulation by barycenter split.
        for (std::size_t i = 0; i < n; i++)
        {
            mu0 += interpolated_corrected_area_measure_face<GT>(
                std::vector<typename GT::Vector_3> {u[i], u[i + 1 % n], uc},
                std::vector<typename GT::Vector_3> {x[i], x[i + 1 % n], xc}
            );
        }
        return mu0;
    }
}

/**
* \ingroup PMP_corrected_curvatures_grp
*
* computes the interpolated corrected mean curvature measure (mu1) of a specific face.
*
* @tparam GT is the geometric traits class.
*
* @param x is a vector of the vertex positions of the face.
* @param u is a vector of the vertex nomrals of the face.
*
* @return a scalar of type `GT::FT`.
* This is the value of the interpolated corrected mean curvature measure of the given face.
*
* @see `interpolated_corrected_gaussian_curvature_measure_face()`
* @see `interpolated_corrected_area_measure_face()`
* @see `interpolated_corrected_measure_mesh()`
*/
template<typename GT>
typename GT::FT interpolated_corrected_mean_curvature_measure_face(const std::vector<typename GT::Vector_3>& u,
                                                                   const std::vector<typename GT::Vector_3>& x = {})
{
    std::size_t n = x.size();
    CGAL_precondition(u.size() == n);
    CGAL_precondition(n >= 3);

    typename GT::Construct_cross_product_vector_3 cross_product;

    // Triangle: use triangle formula
    if (n == 3)
    {
        const typename GT::Vector_3 um = (u[0] + u[1] + u[2]) / 3.0;

        return 0.5 * um * (cross_product(u[2] - u[1], x[0])
            + cross_product(u[0] - u[2], x[1])
            + cross_product(u[1] - u[0], x[2]));
    }
    // Quad: use bilinear interpolation formula
    else if (n == 4)
    {
        // for the formulas below, values of verices 2 & 3 are swapped (compared to paper) to correct order.
        // the indices in paper vs in here are: 00 = 0, 10 = 1, 11 = 2, 01 = 3

        const typename GT::Vector_3 u02 = u[2] - u[0];
        const typename GT::Vector_3 u13 = u[3] - u[1];
        const typename GT::Vector_3 x0_cross = cross_product(u13, x[0]);
        const typename GT::Vector_3 x1_cross = -cross_product(u02, x[1]);
        const typename GT::Vector_3 x3_cross = cross_product(u02, x[3]);
        const typename GT::Vector_3 x2_cross = -cross_product(u13, x[2]);

        return (1 / 12.0) * (
              u[0] * (2 * x0_cross - cross_product((u[3] + u[2]), x[1]) + cross_product((u[1] + u[2]), x[3]) + x2_cross)
            + u[1] * (cross_product((u[3] + u[2]), x[0]) + 2 * x1_cross + x3_cross - cross_product((u[0] + u[3]), x[2]))
            + u[3] * (-cross_product((u[1] + u[2]), x[0]) + x1_cross + 2 * x3_cross + cross_product((u[0] + u[1]), x[2]))
            + u[2] * (x0_cross + cross_product((u[0] + u[3]), x[1]) - cross_product((u[0] + u[1]), x[3]) + 2 * x2_cross)
            );
    }
    // N-gon: split into n triangles by polygon center and use triangle formula for each
    else
    {
        typename GT::FT mu1 = 0;

        // getting center of points
        typename GT::Vector_3 xc =
            std::accumulate(x.begin(), x.end(), typename GT::Vector_3(0, 0, 0));
        xc /= n;

        // getting unit average normal of points
        typename GT::Vector_3 uc =
            std::accumulate(u.begin(), u.end(), typename GT::Vector_3(0, 0, 0));
        uc /= sqrt(uc * uc);

        // summing each triangle's measure after triangulation by barycenter split.
        for (std::size_t i = 0; i < n; i++)
        {
            mu1 += interpolated_corrected_mean_curvature_measure_face<GT>(
                std::vector<typename GT::Vector_3> {u[i], u[i + 1 % n], uc},
                std::vector<typename GT::Vector_3> {x[i], x[i + 1 % n], xc}
            );
        }
        return mu1;
    }
}

/**
* \ingroup PMP_corrected_curvatures_grp
*
* computes the interpolated corrected gaussian curvature measure (mu2) of a specific face.
*
* @tparam GT is the geometric traits class.
*
* @param x is a vector of the vertex positions of the face.
* @param u is a vector of the vertex nomrals of the face.
*
* @return a scalar of type `GT::FT`.
* This is the value of the interpolated corrected gaussian curvature measure of the given face.
*
* @see `interpolated_corrected_mean_curvature_measure_face()`
* @see `interpolated_corrected_area_measure_face()`
* @see `interpolated_corrected_measure_mesh()`
*/
template<typename GT>
typename GT::FT interpolated_corrected_gaussian_curvature_measure_face(const std::vector<typename GT::Vector_3>& u,
                                                                       const std::vector<typename GT::Vector_3>& x = {})
{
    std::size_t n = u.size();
    CGAL_precondition(n >= 3);

    typename GT::Construct_cross_product_vector_3 cross_product;

    // Triangle: use triangle formula
    if (n == 3)
    {
        return 0.5 * u[0] * cross_product(u[1], u[2]);
    }
    // Quad: use bilinear interpolation formula
    else if (n == 4)
    {
        // for the formulas below, values of verices 2 & 3 are swapped (compared to paper) to correct order.
        // the indices in paper vs in here are: 00 = 0, 10 = 1, 11 = 2, 01 = 3
        return (1 / 36.0) * (
              (4 * u[0] + 2 * u[1] + 2 * u[3] + u[2]) * cross_product(x[1] - x[0], x[3] - x[0])
            + (2 * u[0] + 4 * u[1] + u[3] + 2 * u[2]) * cross_product(x[1] - x[0], x[2] - x[1])
            + (2 * u[0] + u[1] + 4 * u[3] + 2 * u[2]) * cross_product(x[2] - x[3], x[3] - x[0])
            + (u[0] + 2 * u[1] + 2 * u[3] + 4 * u[2]) * cross_product(x[2] - x[3], x[2] - x[1])
            );
    }
    // N-gon: split into n triangles by polygon center and use triangle formula for each
    else
    {
        typename GT::FT mu2 = 0;

        // getting unit average normal of points
        typename GT::Vector_3 uc =
            std::accumulate(u.begin(), u.end(), typename GT::Vector_3(0, 0, 0));
        uc /= sqrt(uc * uc);

        // summing each triangle's measure after triangulation by barycenter split.
        for (std::size_t i = 0; i < n; i++)
        {
            mu2 += interpolated_corrected_gaussian_curvature_measure_face<GT>(
                std::vector<typename GT::Vector_3> {u[i], u[i + 1 % n], uc}
            );
        }
        return mu2;
    }
}

/**
* \ingroup PMP_corrected_curvatures_grp
*
* computes the interpolated corrected anisotropic measure (muXY) of a specific face.
*
* @tparam GT is the geometric traits class.
*
* @param u is a vector of the vertex nomrals of the face.
* @param x is a vector of the vertex positions of the face.
*
* @return an array of scalar values for each combination of the standard basis (3x3) of type `std::array<typename GT::FT, 3 * 3>`.
* These are the values of the interpolated corrected anisotropic measure of the given face.
*
* @see `interpolated_corrected_anisotropic_measure_mesh()`
*/
template<typename GT>
std::array<typename GT::FT, 3 * 3> interpolated_corrected_anisotropic_measure_face(const std::vector<typename GT::Vector_3>& u,
                                                                                   const std::vector<typename GT::Vector_3>& x)
{
    std::size_t n = x.size();
    CGAL_precondition(u.size() == n);
    CGAL_precondition(n >= 3);

    typename GT::Construct_cross_product_vector_3 cross_product;
    std::array<typename GT::FT, 3 * 3> muXY;

    // Triangle: use triangle formula
    if (n == 3)
    {
        const typename GT::Vector_3 u01 = u[1] - u[0];
        const typename GT::Vector_3 u02 = u[2] - u[0];
        const typename GT::Vector_3 x01 = x[1] - x[0];
        const typename GT::Vector_3 x02 = x[2] - x[0];
        const typename GT::Vector_3 um = (u[0] + u[1] + u[2]) / 3.0;

        for (std::size_t ix = 0; ix < 3; ix++)
        {
            const typename GT::Vector_3 X(0, 0, 0);
            X[ix] = 1;
            for (std::size_t iy = 0; iy < 3; iy++)
                muXY[ix * 3 + iy] = 0.5 * um * (cross_product(u02[iy] * X, x01) - cross_product(u01[iy] * X, x02));
        }
    }
    // Quad: use bilinear interpolation formula
    else if (n == 4)
    {
        // for the formulas below, values of verices 2 & 3 are swapped (compared to paper) to correct order.
        // the indices in paper vs in here are: 00 = 0, 10 = 1, 11 = 2, 01 = 3
        for (std::size_t ix = 0; ix < 3; ix++)
        {
            const typename GT::Vector_3 X(0, 0, 0);
            X[ix] = 1;
            const typename GT::Vector_3 u0xX = cross_product(u[0], X);
            const typename GT::Vector_3 u1xX = cross_product(u[1], X);
            const typename GT::Vector_3 u2xX = cross_product(u[2], X);
            const typename GT::Vector_3 u3xX = cross_product(u[3], X);

            for (std::size_t iy = 0; iy < 3; iy++)
                muXY[ix * 3 + iy] = (1 / 72.0) * (

                    u[0][iy] * (    u0xX * (    - x[0] - 11 * x[1] + 13 * x[3] -      x[2])
                                  + u1xX * ( -5 * x[0] -  7 * x[1] + 11 * x[3] +      x[2])
                                  + u3xX * (      x[0] -  7 * x[1] + 11 * x[3] -  5 * x[2])
                                  + u2xX * (    - x[0] -  5 * x[1] +  7 * x[3] -      x[2])
                      )
                  + u[1][iy] * (    u0xX * ( 13 * x[0] -      x[1] -  7 * x[3] -  5 * x[2])
                                  + u1xX * ( 17 * x[0] -  5 * x[1] -  5 * x[3] -  7 * x[2])
                                  + u3xX * (  5 * x[0] +      x[1] +      x[3] -  7 * x[2])
                                  + u2xX * (  7 * x[0] -      x[1] +  5 * x[3] - 11 * x[2])
                      )
                  + u[2][iy] * (    u0xX * (-11 * x[0] +  5 * x[1] -      x[3] +  7 * x[2])
                                  + u1xX * (- 7 * x[0] +      x[1] +      x[3] +  5 * x[2])
                                  + u3xX * (- 7 * x[0] -  5 * x[1] -  5 * x[3] + 17 * x[2])
                                  + u2xX * (- 5 * x[0] -  7 * x[1] -      x[3] + 13 * x[2])
                      )
                  + u[3][iy] * (    u0xX * (-     x[0] +  7 * x[1] -  5 * x[3] -      x[2])
                                  + u1xX * (- 5 * x[0] + 11 * x[1] -  7 * x[3] +      x[2])
                                  + u3xX * (      x[0] + 11 * x[1] -  7 * x[3] -  5 * x[2])
                                  + u2xX * (-     x[0] + 13 * x[1] - 11 * x[3] -      x[2])
                      )

                  );
        }
    }
    // N-gon: split into n triangles by polygon center and use triangle formula for each
    else
    {
        typename GT::FT muXY = 0;

        // getting center of points
        typename GT::Vector_3 xc =
            std::accumulate(x.begin(), x.end(), typename GT::Vector_3(0, 0, 0));
        xc /= n;

        // getting unit average normal of points
        typename GT::Vector_3 uc =
            std::accumulate(u.begin(), u.end(), typename GT::Vector_3(0, 0, 0));
        uc /= sqrt(uc * uc);

        // summing each triangle's measure after triangulation by barycenter split.
        for (std::size_t i = 0; i < n; i++)
        {
            std::array<typename GT::FT, 3 * 3> muXY_curr_triangle =
                interpolated_corrected_anisotropic_measure_face<GT>(
                    std::vector<typename GT::Vector_3> {u[i], u[i + 1 % n], uc},
                    std::vector<typename GT::Vector_3> {x[i], x[i + 1 % n], xc}
                );

            for (std::size_t ix = 0; ix < 3; ix++)
                for (std::size_t iy = 0; iy < 3; iy++)
                    muXY[ix * 3 + iy] += muXY_curr_triangle[ix * 3 + iy];
        }
    }
    return muXY;
}


/**
* \ingroup PMP_corrected_curvatures_grp
*
* computes the interpolated corrected curvature measure on each face of the mesh
*
* @tparam PolygonMesh a model of `FaceGraph`
* @tparam FaceMeasureMap a a model of `WritablePropertyMap` with
* `boost::graph_traits<PolygonMesh>::%face_descriptor` as key type and `GT::FT` as value type.
* @tparam NamedParameters a sequence of \ref bgl_namedparameters "Named Parameters"
*
* @param pmesh the polygon mesh
* @param fmm (face measure map) the property map used for storing the computed interpolated corrected measure
* @param mu_i an enum for choosing between computing
*             the area measure, the mean curvature measure or the gaussian curvature measure
* @param np an optional sequence of \ref bgl_namedparameters "Named Parameters" among the ones listed below
*
* \cgalNamedParamsBegin
*   \cgalParamNBegin{vertex_point_map}
*     \cgalParamDescription{a property map associating points to the vertices of `pmesh`}
*     \cgalParamType{a class model of `ReadablePropertyMap` with
*                    `boost::graph_traits<PolygonMesh>::%vertex_descriptor`
*                    as key type and `%Point_3` as value type}
*     \cgalParamDefault{`boost::get(CGAL::vertex_point, pmesh)`}
*     \cgalParamExtra{If this parameter is omitted, an internal property map for
*                     `CGAL::vertex_point_t` must be available in `PolygonMesh`.}
*   \cgalParamNEnd
*
*   \cgalParamNBegin{vertex_normal_map}
*     \cgalParamDescription{a property map associating normal vectors to the vertices of `pmesh`}
*     \cgalParamType{a class model of `ReadablePropertyMap` with
*                    `boost::graph_traits<PolygonMesh>::%vertex_descriptor`
*                    as key type and `%Vector_3` as value type}
*     \cgalParamDefault{`get(dynamic_vertex_property_t<GT::Vector_3>(), pmesh)`}
*     \cgalParamExtra{If this parameter is omitted, vertex normals should be
*                     computed inside the function body.}
*   \cgalParamNEnd
*
* \cgalNamedParamsEnd
*
* @see `interpolated_corrected_area_measure_face()`
* @see `interpolated_corrected_mean_curvature_measure_face()`
* @see `interpolated_corrected_gaussian_curvature_measure_face()`
*/
template<typename PolygonMesh, typename FaceMeasureMap,
    typename NamedParameters = parameters::Default_named_parameters>
    void
    interpolated_corrected_measure_mesh(const PolygonMesh& pmesh,
                                        FaceMeasureMap fmm,
                                        const Curvature_measure_index mu_i,
                                        const NamedParameters& np = parameters::default_values())
{

    typedef typename GetGeomTraits<PolygonMesh, NamedParameters>::type GT;
    typedef dynamic_vertex_property_t<typename GT::Vector_3> Vector_map_tag;
    typedef typename boost::property_map<PolygonMesh, Vector_map_tag>::const_type Default_vector_map;
    typedef typename internal_np::Lookup_named_param_def<internal_np::vertex_normal_map_t,
        NamedParameters,
        Default_vector_map>::type       VNM;

    using parameters::choose_parameter;
    using parameters::get_parameter;
    using parameters::is_default_parameter;

    typedef typename boost::graph_traits<PolygonMesh>::face_descriptor face_descriptor;
    typedef typename boost::graph_traits<PolygonMesh>::vertex_descriptor vertex_descriptor;
    typedef typename boost::graph_traits<PolygonMesh>::halfedge_descriptor halfedge_descriptor;

    typename GetVertexPointMap<PolygonMesh, NamedParameters>::const_type
        vpm = choose_parameter(get_parameter(np, CGAL::vertex_point),
            get_const_property_map(CGAL::vertex_point, pmesh));

    VNM vnm = choose_parameter(get_parameter(np, internal_np::vertex_normal_map),
                               get(Vector_map_tag(), pmesh));

    if (is_default_parameter<NamedParameters, internal_np::vertex_normal_map_t>::value)
        compute_vertex_normals(pmesh, vnm, np);

    typedef typename property_map_value<PolygonMesh, FaceMeasureMap>::type measure;

    std::function
        <typename GT::FT(const std::vector<typename GT::Vector_3>&, const std::vector<typename GT::Vector_3>&)>
        iccm_function;
    switch (mu_i)
    {
    case MU0_AREA_MEASURE:
        iccm_function = &interpolated_corrected_area_measure_face<GT>;
        break;
    case MU1_MEAN_CURVATURE_MEASURE:
        iccm_function = &interpolated_corrected_mean_curvature_measure_face<GT>;
        break;
    case MU2_GAUSSIAN_CURVATURE_MEASURE:
        iccm_function = &interpolated_corrected_gaussian_curvature_measure_face<GT>;
        break;
    }

    for (face_descriptor f : faces(pmesh))
    {
        std::vector<typename GT::Vector_3> x;
        std::vector<typename GT::Vector_3> u;

        for (vertex_descriptor v : vertices_around_face(halfedge(f, pmesh), pmesh))
        {
            typename GT::Point_3 p = get(vpm, v);
            x.push_back(typename GT::Vector_3(p.x(), p.y(), p.z()));
            u.push_back(get(vnm, v));
        }

        put(fmm, f, iccm_function(u, x));
    }
}

//
//template<typename GT>
//typename GT::FT triangle_in_ball_ratio_1(const typename GT::Vector_3 x1,
//                                         const typename GT::Vector_3 x2,
//                                         const typename GT::Vector_3 x3,
//                                         const typename GT::FT r,
//                                         const typename GT::Vector_3 c,
//                                         const std::size_t res = 3)
//{
//    const typename GT::FT R = r * r;
//    const typename GT::FT acc = 1.0 / res;
//    std::size_t samples_in = 0;
//    for (GT::FT alpha = acc / 3; alpha < 1; alpha += acc)
//        for (GT::FT beta = acc / 3; beta < 1 - alpha; beta += acc)
//        {
//            if ((alpha * x1 + beta * x2 + (1 - alpha - beta) * x3 - c).squared_length() < R)
//                samples_in++;
//        }
//    return samples_in / (typename GT::FT)(res * (res + 1) / 2);
//}


template<typename GT>
typename GT::FT face_in_ball_ratio_2(const std::vector<typename GT::Vector_3>& x,
                                     const typename GT::FT r,
                                     const typename GT::Vector_3 c)
{
    std::size_t n = x.size();

    // getting center of points
    typename GT::Vector_3 xm =
        std::accumulate(x.begin(), x.end(), typename GT::Vector_3(0, 0, 0));
    xm /= n;

    typename GT::FT d_min = (xm - c).squared_length();
    typename GT::FT d_max = d_min;

    for (const typename GT::Vector_3 xi : x)
    {
        const typename GT::FT d_sq = (xi - c).squared_length();
        d_max = std::max(d_sq, d_max);
        d_min = std::min(d_sq, d_min);
    }

    if (d_max <= r * r) return 1.0;
    else if (r * r <= d_min) return 0.0;

    d_max = sqrt(d_max);
    d_min = sqrt(d_min);

    return (r - d_min) / (d_max - d_min);
}

template<typename PolygonMesh, typename FaceMeasureMap, typename VertexCurvatureMap,
    typename NamedParameters = parameters::Default_named_parameters>
    void expand_interpolated_corrected_measure_vertex(const PolygonMesh& pmesh,
        FaceMeasureMap fmm,
        VertexCurvatureMap vcm,
        const typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
        const NamedParameters& np = parameters::default_values())
{
    using parameters::choose_parameter;
    using parameters::get_parameter;

    typedef typename GetGeomTraits<PolygonMesh, NamedParameters>::type GT;
    typedef typename boost::graph_traits<PolygonMesh>::face_descriptor face_descriptor;
    typedef typename boost::graph_traits<PolygonMesh>::vertex_descriptor vertex_descriptor;

    const typename GetGeomTraits<PolygonMesh, NamedParameters>::type::FT
        r = choose_parameter(get_parameter(np, internal_np::ball_radius), 0.01);

    typename GetVertexPointMap<PolygonMesh, NamedParameters>::const_type
        vpm = choose_parameter(get_parameter(np, CGAL::vertex_point),
            get_const_property_map(CGAL::vertex_point, pmesh));


    std::queue<face_descriptor> bfs_q;
    std::unordered_set<face_descriptor> bfs_v;

    typename GT::Point_3 vp = get(vpm, v);
    typename GT::Vector_3 c = typename GT::Vector_3(vp.x(), vp.y(), vp.z());

    typename GT::FT corrected_mui = 0;

    for (face_descriptor f : faces_around_target(halfedge(v, pmesh), pmesh)) {
        if (f != boost::graph_traits<PolygonMesh>::null_face())
        {
            bfs_q.push(f);
            bfs_v.insert(f);
        }
    }
    while (!bfs_q.empty()) {
        face_descriptor fi = bfs_q.front();
        bfs_q.pop();

        // looping over vertices in face to get point coordinates
        std::vector<typename GT::Vector_3> x;
        for (vertex_descriptor vi : vertices_around_face(halfedge(fi, pmesh), pmesh))
        {
            typename GT::Point_3 pi = get(vpm, vi);
            x.push_back(typename GT::Vector_3(pi.x(), pi.y(), pi.z()));
        }

        const typename GT::FT f_ratio = face_in_ball_ratio_2<GT>(x, r, c);

        if (f_ratio > 0.00000001)
        {
            corrected_mui += f_ratio * get(fmm, fi);
            for (face_descriptor fj : faces_around_face(halfedge(fi, pmesh), pmesh))
            {
                if (bfs_v.find(fj) == bfs_v.end() && fj != boost::graph_traits<PolygonMesh>::null_face())
                {
                    bfs_q.push(fj);
                    bfs_v.insert(fj);
                }
            }
        }
    }

    put(vcm, v, corrected_mui);
}

template<typename PolygonMesh, typename VertexCurvatureMap,
    typename NamedParameters = parameters::Default_named_parameters>
    void interpolated_corrected_mean_curvature(const PolygonMesh& pmesh,
        VertexCurvatureMap vcm,
        const NamedParameters& np = parameters::default_values())
{
    typedef typename GetGeomTraits<PolygonMesh, NamedParameters>::type GT;
    typedef typename boost::graph_traits<PolygonMesh>::face_descriptor face_descriptor;
    typedef std::unordered_map<face_descriptor, typename GT::FT> FaceMeasureMap_tag;
    typedef typename boost::graph_traits<PolygonMesh>::vertex_descriptor vertex_descriptor;
    typedef std::unordered_map<vertex_descriptor, typename GT::FT> VertexMeasureMap_tag;

    FaceMeasureMap_tag mu0_init, mu1_init;
    boost::associative_property_map<FaceMeasureMap_tag>
        mu0_map(mu0_init), mu1_map(mu1_init);

    VertexMeasureMap_tag mu0_expand_init, mu1_expand_init;
    boost::associative_property_map<VertexMeasureMap_tag>
        mu0_expand_map(mu0_expand_init), mu1_expand_map(mu1_expand_init);

    interpolated_corrected_measure_mesh(pmesh, mu0_map, MU0_AREA_MEASURE);
    interpolated_corrected_measure_mesh(pmesh, mu1_map, MU1_MEAN_CURVATURE_MEASURE);

    for (vertex_descriptor v : vertices(pmesh))
    {
        expand_interpolated_corrected_measure_vertex(pmesh, mu0_map, mu0_expand_map, v, np);
        expand_interpolated_corrected_measure_vertex(pmesh, mu1_map, mu1_expand_map, v, np);

        typename GT::FT v_mu0 = get(mu0_expand_map, v);
        if (v_mu0 > 0.00000001)
            put(vcm, v, 0.5 * get(mu1_expand_map, v) / v_mu0);
        else
            put(vcm, v, 0);
    }
}

template<typename PolygonMesh, typename VertexCurvatureMap,
    typename NamedParameters = parameters::Default_named_parameters>
    void interpolated_corrected_gaussian_curvature(const PolygonMesh& pmesh,
        VertexCurvatureMap vcm,
        const NamedParameters& np = parameters::default_values())
{
    typedef typename GetGeomTraits<PolygonMesh, NamedParameters>::type GT;
    typedef typename boost::graph_traits<PolygonMesh>::face_descriptor face_descriptor;
    typedef std::unordered_map<face_descriptor, typename GT::FT> FaceMeasureMap_tag;
    typedef typename boost::graph_traits<PolygonMesh>::vertex_descriptor vertex_descriptor;
    typedef std::unordered_map<vertex_descriptor, typename GT::FT> VertexMeasureMap_tag;

    FaceMeasureMap_tag mu0_init, mu2_init;
    boost::associative_property_map<FaceMeasureMap_tag>
        mu0_map(mu0_init), mu2_map(mu2_init);

    VertexMeasureMap_tag mu0_expand_init, mu2_expand_init;
    boost::associative_property_map<VertexMeasureMap_tag>
        mu0_expand_map(mu0_expand_init), mu2_expand_map(mu2_expand_init);

    interpolated_corrected_measure_mesh(pmesh, mu0_map, MU0_AREA_MEASURE);
    interpolated_corrected_measure_mesh(pmesh, mu2_map, MU2_GAUSSIAN_CURVATURE_MEASURE);

    for (vertex_descriptor v : vertices(pmesh))
    {
        expand_interpolated_corrected_measure_vertex(pmesh, mu0_map, mu0_expand_map, v, np);
        expand_interpolated_corrected_measure_vertex(pmesh, mu2_map, mu2_expand_map, v, np);

        typename GT::FT v_mu0 = get(mu0_expand_map, v);
        if(v_mu0 > 0.00000001)
            put(vcm, v, get(mu2_expand_map, v) / v_mu0);
        else
            put(vcm, v, 0);
    }
}

}
}
