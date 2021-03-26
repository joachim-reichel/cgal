// Copyright (c) 2020 GeometryFactory SARL (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Dmitry Anisimov
//

#ifndef CGAL_SHAPE_DETECTION_REGION_GROWING_INTERNAL_TRAITS_H
#define CGAL_SHAPE_DETECTION_REGION_GROWING_INTERNAL_TRAITS_H

#include <CGAL/license/Shape_detection.h>

// STL includes.
#include <vector>
#include <algorithm>
#include <type_traits>

// CGAL includes.
#include <CGAL/assertions.h>
#include <CGAL/number_utils.h>

// Internal includes.
#include <CGAL/Shape_detection/Region_growing/internal/utils.h>

namespace CGAL {
namespace Shape_detection {
namespace internal {

  template<typename GeomTraits>
  struct Polyline_graph_traits_2 {
    using Segment = typename GeomTraits::Segment_2;
    using Construct_segment = typename GeomTraits::Construct_segment_2;
    decltype(auto) construct_segment_object() const {
      return GeomTraits().construct_segment_2_object();
    }
  };

  template<typename GeomTraits>
  struct Polyline_graph_traits_3 {
    using Segment = typename GeomTraits::Segment_3;
    using Construct_segment = typename GeomTraits::Construct_segment_3;
    decltype(auto) construct_segment_object() const {
      return GeomTraits().construct_segment_3_object();
    }
  };

  template<typename GeomTraits>
  struct Region_growing_traits_2 {
    using Point = typename GeomTraits::Point_2;
    using Segment = typename GeomTraits::Segment_2;
    using Vector = typename GeomTraits::Vector_2;
    using Line = typename GeomTraits::Line_2;

    using Compute_squared_length = typename GeomTraits::Compute_squared_length_2;
    using Compute_squared_distance = typename GeomTraits::Compute_squared_distance_2;
    using Compute_scalar_product = typename GeomTraits::Compute_scalar_product_2;

    const GeomTraits& m_traits;
    Region_growing_traits_2(const GeomTraits& traits) :
    m_traits(traits)
    { }

    decltype(auto) compute_squared_length_object() const {
      return m_traits.compute_squared_length_2_object();
    }
    decltype(auto) compute_squared_distance_object() const {
      return m_traits.compute_squared_distance_2_object();
    }
    decltype(auto) compute_scalar_product_object() const {
      return m_traits.compute_scalar_product_2_object();
    }

    template<typename InputRange, typename ItemMap>
    decltype(auto) create_line(
      const InputRange& input_range, const ItemMap item_map,
      const std::vector<std::size_t>& region) const {
      return internal::create_line_2(
        input_range, item_map, region, m_traits);
    }
  };

  template<typename GeomTraits>
  struct Region_growing_traits_3 {
    using Point = typename GeomTraits::Point_3;
    using Segment = typename GeomTraits::Segment_3;
    using Vector = typename GeomTraits::Vector_3;
    using Line = typename GeomTraits::Line_3;

    using Compute_squared_length = typename GeomTraits::Compute_squared_length_3;
    using Compute_squared_distance = typename GeomTraits::Compute_squared_distance_3;
    using Compute_scalar_product = typename GeomTraits::Compute_scalar_product_3;

    const GeomTraits& m_traits;
    Region_growing_traits_3(const GeomTraits& traits) :
    m_traits(traits)
    { }

    decltype(auto) compute_squared_length_object() const {
      return m_traits.compute_squared_length_3_object();
    }
    decltype(auto) compute_squared_distance_object() const {
      return m_traits.compute_squared_distance_3_object();
    }
    decltype(auto) compute_scalar_product_object() const {
      return m_traits.compute_scalar_product_3_object();
    }

    template<typename InputRange, typename ItemMap>
    decltype(auto) create_line(
      const InputRange& input_range, const ItemMap item_map,
      const std::vector<std::size_t>& region) const {
      return internal::create_line_3(
        input_range, item_map, region, m_traits);
    }
  };

} // namespace internal
} // namespace Shape_detection
} // namespace CGAL

#endif // CGAL_SHAPE_DETECTION_REGION_GROWING_INTERNAL_TRAITS_H
