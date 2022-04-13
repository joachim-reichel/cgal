// Copyright (c) 2010-2011 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL$
// $Id$
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//
#ifndef COMBINATORIAL_MAP_FWD_H
#define COMBINATORIAL_MAP_FWD_H 1

#include <CGAL/memory.h>
#include <CGAL/tags.h>
#include <boost/mpl/has_xxx.hpp>

namespace CGAL {

struct Generic_map_min_items;

template<unsigned int d_, class Items_, class Alloc_>
class Combinatorial_map_storage_1;

template<unsigned int d_, class Items_, class Alloc_>
class Combinatorial_map_storage_2;

namespace internal
{
BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(Has_use_index_tag,Use_index,false)
template<typename T, bool typedefined=Has_use_index_tag<T>::value>
struct Default_storage_for_cmap
{
  template<unsigned int d_, class Items_, class Alloc_>
  using type=Combinatorial_map_storage_1<d_, Items_, Alloc_>;
};
template<typename T>
struct Default_storage_for_cmap<T, true>
{
  template<unsigned int d_, class Items_, class Alloc_>
  using type=Combinatorial_map_storage_2<d_, Items_, Alloc_>;
};
} // namespace internal

template<unsigned int d_, class Refs_,
         class Items_=Generic_map_min_items,
         class Alloc_=CGAL_ALLOCATOR(int),
         class Storage_=typename internal::Default_storage_for_cmap<Items_>::
         type<d_, Items_, Alloc_>>
class Combinatorial_map_base;

template<unsigned int d_,
         class Items_=Generic_map_min_items,
         class Alloc_=CGAL_ALLOCATOR(int),
         class Storage_=typename internal::Default_storage_for_cmap<Items_>::
         type<d_, Items_, Alloc_>>
class Combinatorial_map;

} // CGAL

#endif // COMBINATORIAL_MAP_FWD_H
