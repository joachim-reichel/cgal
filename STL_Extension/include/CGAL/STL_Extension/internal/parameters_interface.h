// Copyright (c) 2017  GeometryFactory (France).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL$
// $Id$
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Maxime Gimeno

// List of named parameters that we use in CGAL
CGAL_add_named_parameter(vertex_point_t, vertex_point, vertex_point_map)
CGAL_add_named_parameter(halfedge_index_t, halfedge_index, halfedge_index_map)
CGAL_add_named_parameter(edge_index_t, edge_index, edge_index_map)
CGAL_add_named_parameter(face_index_t, face_index, face_index_map)
CGAL_add_named_parameter(vertex_index_t, vertex_index, vertex_index_map)
CGAL_add_named_parameter(visitor_t, visitor, visitor)

CGAL_add_named_parameter(point_t, point_map, point_map)

CGAL_add_named_parameter(edge_is_constrained_t, edge_is_constrained, edge_is_constrained_map)
CGAL_add_named_parameter(first_index_t, first_index, first_index)
CGAL_add_named_parameter(number_of_iterations_t, number_of_iterations, number_of_iterations)
CGAL_add_named_parameter(verbosity_level_t, verbosity_level, verbosity_level)
CGAL_add_named_parameter(use_binary_mode_t, use_binary_mode, use_binary_mode)

CGAL_add_named_parameter(metis_options_t, METIS_options, METIS_options)
CGAL_add_named_parameter(vertex_partition_id_t, vertex_partition_id, vertex_partition_id_map)
CGAL_add_named_parameter(face_partition_id_t, face_partition_id, face_partition_id_map)

CGAL_add_named_parameter(vertex_output_iterator_t, vertex_output_iterator, vertex_output_iterator)
CGAL_add_named_parameter(face_output_iterator_t, face_output_iterator, face_output_iterator)

CGAL_add_named_parameter(vertex_to_vertex_output_iterator_t, vertex_to_vertex_output_iterator, vertex_to_vertex_output_iterator)
CGAL_add_named_parameter(halfedge_to_halfedge_output_iterator_t, halfedge_to_halfedge_output_iterator, halfedge_to_halfedge_output_iterator)
CGAL_add_named_parameter(face_to_face_output_iterator_t, face_to_face_output_iterator, face_to_face_output_iterator)
CGAL_add_named_parameter(point_to_vertex_output_iterator_t, point_to_vertex_output_iterator, point_to_vertex_output_iterator)
CGAL_add_named_parameter(polygon_to_face_output_iterator_t, polygon_to_face_output_iterator, polygon_to_face_output_iterator)
CGAL_add_named_parameter(point_to_vertex_map_t, point_to_vertex_map, point_to_vertex_map)
CGAL_add_named_parameter(polygon_to_face_map_t, polygon_to_face_map, polygon_to_face_map)

CGAL_add_named_parameter(vertex_to_vertex_map_t, vertex_to_vertex_map, vertex_to_vertex_map)
CGAL_add_named_parameter(halfedge_to_halfedge_map_t, halfedge_to_halfedge_map, halfedge_to_halfedge_map)
CGAL_add_named_parameter(face_to_face_map_t, face_to_face_map, face_to_face_map)
CGAL_add_named_parameter(implementation_tag_t, implementation_tag, implementation_tag)
CGAL_add_named_parameter(prevent_unselection_t, prevent_unselection, prevent_unselection)

CGAL_add_named_parameter(verbose_t, verbose, verbose)

// List of named parameters used for IO
CGAL_add_named_parameter(vertex_normal_output_iterator_t, vertex_normal_output_iterator, vertex_normal_output_iterator)
CGAL_add_named_parameter(vertex_color_output_iterator_t, vertex_color_output_iterator, vertex_color_output_iterator)
CGAL_add_named_parameter(vertex_texture_output_iterator_t, vertex_texture_output_iterator, vertex_texture_output_iterator)
CGAL_add_named_parameter(face_color_output_iterator_t, face_color_output_iterator, face_color_output_iterator)
CGAL_add_named_parameter(vertex_normal_map_t, vertex_normal_map, vertex_normal_map)
CGAL_add_named_parameter(vertex_color_map_t, vertex_color_map, vertex_color_map)
CGAL_add_named_parameter(vertex_texture_map_t, vertex_texture_map, vertex_texture_map)
CGAL_add_named_parameter(face_color_map_t, face_color_map, face_color_map)
CGAL_add_named_parameter(repair_polygon_soup_t, repair_polygon_soup, repair_polygon_soup)
CGAL_add_named_parameter(output_color_t, output_color, output_color)
CGAL_add_named_parameter(stream_precision_t, stream_precision, stream_precision)

// List of named parameters that we use in the package 'Mesh_3'
CGAL_add_named_parameter(vertex_feature_degree_t, vertex_feature_degree, vertex_feature_degree_map)

// List of named parameters used in the package 'Polygon Mesh Processing'
CGAL_add_named_parameter(geom_traits_t, geom_traits, geom_traits)
CGAL_add_named_parameter(vertex_incident_patches_t, vertex_incident_patches, vertex_incident_patches_map)
CGAL_add_named_parameter(density_control_factor_t, density_control_factor, density_control_factor)
CGAL_add_named_parameter(use_delaunay_triangulation_t, use_delaunay_triangulation, use_delaunay_triangulation)
CGAL_add_named_parameter(do_not_use_cubic_algorithm_t, do_not_use_cubic_algorithm, do_not_use_cubic_algorithm)
CGAL_add_named_parameter(do_not_triangulate_faces_t, do_not_triangulate_faces, do_not_triangulate_faces)
CGAL_add_named_parameter(use_2d_constrained_delaunay_triangulation_t, use_2d_constrained_delaunay_triangulation, use_2d_constrained_delaunay_triangulation)
CGAL_add_named_parameter(fairing_continuity_t, fairing_continuity, fairing_continuity)
CGAL_add_named_parameter(sparse_linear_solver_t, sparse_linear_solver, sparse_linear_solver)
CGAL_add_named_parameter(number_of_relaxation_steps_t, number_of_relaxation_steps, number_of_relaxation_steps)
CGAL_add_named_parameter(protect_constraints_t, protect_constraints, protect_constraints)
CGAL_add_named_parameter(relax_constraints_t, relax_constraints, relax_constraints)
CGAL_add_named_parameter(collapse_constraints_t, collapse_constraints, collapse_constraints)
CGAL_add_named_parameter(vertex_is_constrained_t, vertex_is_constrained, vertex_is_constrained_map)
CGAL_add_named_parameter(face_patch_t, face_patch, face_patch_map)
CGAL_add_named_parameter(random_uniform_sampling_t, random_uniform_sampling, use_random_uniform_sampling)
CGAL_add_named_parameter(grid_sampling_t, grid_sampling, use_grid_sampling)
CGAL_add_named_parameter(monte_carlo_sampling_t, monte_carlo_sampling, use_monte_carlo_sampling)
CGAL_add_named_parameter(do_sample_edges_t, do_sample_edges, do_sample_edges)
CGAL_add_named_parameter(do_sample_vertices_t, do_sample_vertices, do_sample_vertices)
CGAL_add_named_parameter(do_sample_faces_t, do_sample_faces, do_sample_faces)
CGAL_add_named_parameter(number_of_points_on_faces_t, number_of_points_on_faces, number_of_points_on_faces)
CGAL_add_named_parameter(number_of_points_per_face_t, number_of_points_per_face, number_of_points_per_face)
CGAL_add_named_parameter(grid_spacing_t, grid_spacing, grid_spacing)
CGAL_add_named_parameter(number_of_points_per_edge_t, number_of_points_per_edge, number_of_points_per_edge)
CGAL_add_named_parameter(number_of_points_on_edges_t, number_of_points_on_edges, number_of_points_on_edges)
CGAL_add_named_parameter(nb_points_per_area_unit_t, nb_points_per_area_unit, number_of_points_per_area_unit)
CGAL_add_named_parameter(nb_points_per_distance_unit_t, nb_points_per_distance_unit, number_of_points_per_distance_unit)
CGAL_add_named_parameter(vertex_mean_curvature_map_t, vertex_mean_curvature_map, vertex_mean_curvature_map)
CGAL_add_named_parameter(vertex_Gaussian_curvature_map_t, vertex_Gaussian_curvature_map, vertex_Gaussian_curvature_map)
CGAL_add_named_parameter(vertex_principal_curvatures_and_directions_map_t, vertex_principal_curvatures_and_directions_map, vertex_principal_curvatures_and_directions_map)
CGAL_add_named_parameter(vertex_mean_curvature_t, vertex_mean_curvature, vertex_mean_curvature)
CGAL_add_named_parameter(vertex_Gaussian_curvature_t, vertex_Gaussian_curvature, vertex_Gaussian_curvature)
CGAL_add_named_parameter(vertex_principal_curvatures_and_directions_t, vertex_principal_curvatures_and_directions, vertex_principal_curvatures_and_directions)
CGAL_add_named_parameter(ball_radius_t, ball_radius, ball_radius)
CGAL_add_named_parameter(outward_orientation_t, outward_orientation, outward_orientation)
CGAL_add_named_parameter(overlap_test_t, overlap_test, do_overlap_test_of_bounded_sides)
CGAL_add_named_parameter(preserve_genus_t, preserve_genus, preserve_genus)
CGAL_add_named_parameter(apply_per_connected_component_t, apply_per_connected_component, apply_per_connected_component)
CGAL_add_named_parameter(projection_functor_t, projection_functor, projection_functor)
CGAL_add_named_parameter(allow_move_functor_t, allow_move_functor, allow_move_functor)
CGAL_add_named_parameter(throw_on_self_intersection_t, throw_on_self_intersection, throw_on_self_intersection)
CGAL_add_named_parameter(clip_volume_t, clip_volume, clip_volume)
CGAL_add_named_parameter(use_compact_clipper_t, use_compact_clipper, use_compact_clipper)
CGAL_add_named_parameter(output_iterator_t, output_iterator, output_iterator)
CGAL_add_named_parameter(erase_all_duplicates_t, erase_all_duplicates, erase_all_duplicates)
CGAL_add_named_parameter(require_same_orientation_t, require_same_orientation, require_same_orientation)
CGAL_add_named_parameter(face_size_map_t, face_size_map, face_size_map)
CGAL_add_named_parameter(snapping_tolerance_t, snapping_tolerance, snapping_tolerance)
CGAL_add_named_parameter(use_safety_constraints_t, use_safety_constraints, use_safety_constraints)
CGAL_add_named_parameter(use_angle_smoothing_t, use_angle_smoothing, use_angle_smoothing)
CGAL_add_named_parameter(use_area_smoothing_t, use_area_smoothing, use_area_smoothing)
CGAL_add_named_parameter(use_Delaunay_flips_t, use_Delaunay_flips, use_Delaunay_flips)
CGAL_add_named_parameter(do_project_t, do_project, do_project)
CGAL_add_named_parameter(do_split_t, do_split, do_split)
CGAL_add_named_parameter(do_collapse_t, do_collapse, do_collapse)
CGAL_add_named_parameter(do_flip_t, do_flip, do_flip)
CGAL_add_named_parameter(do_orientation_tests_t, do_orientation_tests, do_orientation_tests)
CGAL_add_named_parameter(do_self_intersection_tests_t, do_self_intersection_tests, do_self_intersection_tests)
CGAL_add_named_parameter(error_codes_t, error_codes, error_codes)
CGAL_add_named_parameter(volume_inclusions_t, volume_inclusions, volume_inclusions)
CGAL_add_named_parameter(face_cc_map_t, face_connected_component_map, face_connected_component_map)
CGAL_add_named_parameter(ccid_to_vid_vector_t, connected_component_id_to_volume_id, connected_component_id_to_volume_id)
CGAL_add_named_parameter(is_cc_outward_oriented_bs_t, is_cc_outward_oriented, is_cc_outward_oriented)
CGAL_add_named_parameter(intersecting_volume_pairs_t, intersecting_volume_pairs_output_iterator, intersecting_volume_pairs_output_iterator)
CGAL_add_named_parameter(i_used_as_a_predicate_t, i_used_as_a_predicate, i_used_as_a_predicate)
CGAL_add_named_parameter(nesting_levels_t, nesting_levels, nesting_levels)
CGAL_add_named_parameter(i_used_for_volume_orientation_t, i_used_for_volume_orientation, i_used_for_volume_orientation)
CGAL_add_named_parameter(area_threshold_t, area_threshold, area_threshold)
CGAL_add_named_parameter(halfedges_keeper_t, halfedges_keeper, halfedges_keeper)
CGAL_add_named_parameter(volume_threshold_t, volume_threshold, volume_threshold)
CGAL_add_named_parameter(dry_run_t, dry_run, dry_run)
CGAL_add_named_parameter(do_not_modify_t, do_not_modify, do_not_modify)
CGAL_add_named_parameter(allow_self_intersections_t, allow_self_intersections, allow_self_intersections)
CGAL_add_named_parameter(non_manifold_feature_map_t, non_manifold_feature_map, non_manifold_feature_map)
CGAL_add_named_parameter(polyhedral_envelope_epsilon_t, polyhedral_envelope_epsilon, polyhedral_envelope_epsilon)
CGAL_add_named_parameter(match_faces_t, match_faces, match_faces)
CGAL_add_named_parameter(face_epsilon_map_t, face_epsilon_map, face_epsilon_map)
CGAL_add_named_parameter(maximum_number_t, maximum_number, maximum_number)
CGAL_add_named_parameter(use_one_sided_hausdorff_t, use_one_sided_hausdorff, use_one_sided_hausdorff)
CGAL_add_named_parameter(cap_threshold_t, cap_threshold, cap_threshold)
CGAL_add_named_parameter(needle_threshold_t, needle_threshold, needle_threshold)
CGAL_add_named_parameter(flip_triangle_height_threshold_t, flip_triangle_height_threshold, flip_triangle_height_threshold)
CGAL_add_named_parameter(collapse_length_threshold_t, collapse_length_threshold, collapse_length_threshold)
CGAL_add_named_parameter(features_angle_bound_t, features_angle_bound, features_angle_bound)
CGAL_add_named_parameter(mesh_edge_size_t, mesh_edge_size, mesh_edge_size)
CGAL_add_named_parameter(mesh_facet_size_t, mesh_facet_size, mesh_facet_size)
CGAL_add_named_parameter(mesh_facet_angle_t, mesh_facet_angle, mesh_facet_angle)
CGAL_add_named_parameter(mesh_facet_distance_t, mesh_facet_distance, mesh_facet_distance)
CGAL_add_named_parameter(mesh_facet_topology_t, mesh_facet_topology, mesh_facet_topology)
CGAL_add_named_parameter(polyline_constraints_t, polyline_constraints, polyline_constraints)
CGAL_add_named_parameter(do_scale_t, do_scale, do_scale)
CGAL_add_named_parameter(vertex_corner_map_t, vertex_corner_map, vertex_corner_map)
CGAL_add_named_parameter(patch_normal_map_t, patch_normal_map, patch_normal_map)
CGAL_add_named_parameter(region_primitive_map_t, region_primitive_map, region_primitive_map)
CGAL_add_named_parameter(postprocess_regions_t, postprocess_regions, postprocess_regions)
CGAL_add_named_parameter(sizing_function_t, sizing_function, sizing_function)

// List of named parameters that we use in the package 'Surface Mesh Simplification'
CGAL_add_named_parameter(get_cost_policy_t, get_cost_policy, get_cost)
CGAL_add_named_parameter(get_placement_policy_t, get_placement_policy, get_placement)
CGAL_add_named_parameter(filter_t, filter, filter)
CGAL_add_named_parameter(use_relaxed_order_t, use_relaxed_order, use_relaxed_order)

//to be documented
CGAL_add_named_parameter(face_normal_t, face_normal, face_normal_map)
CGAL_add_named_parameter(random_seed_t, random_seed, random_seed)
CGAL_add_named_parameter(do_lock_mesh_t, do_lock_mesh, do_lock_mesh)
CGAL_add_named_parameter(do_simplify_border_t, do_simplify_border, do_simplify_border)
CGAL_add_named_parameter(algorithm_t, algorithm, algorithm)
CGAL_add_named_parameter(use_smoothing_t, use_smoothing, use_smoothing)

//internal
CGAL_add_named_parameter(weight_calculator_t, weight_calculator, weight_calculator)
CGAL_add_named_parameter(use_bool_op_to_clip_surface_t, use_bool_op_to_clip_surface, use_bool_op_to_clip_surface)

// List of named parameters used in the Point Set Processing package
CGAL_add_named_parameter(query_point_t, query_point_map, query_point_map)
CGAL_add_named_parameter(normal_t, normal_map, normal_map)
CGAL_add_named_parameter(diagonalize_traits_t, diagonalize_traits, diagonalize_traits)
CGAL_add_named_parameter(svd_traits_t, svd_traits, svd_traits)
CGAL_add_named_parameter(callback_t, callback, callback)
CGAL_add_named_parameter(sharpness_angle_t, sharpness_angle, sharpness_angle)
CGAL_add_named_parameter(edge_sensitivity_t, edge_sensitivity, edge_sensitivity)
CGAL_add_named_parameter(neighbor_radius_t, neighbor_radius, neighbor_radius)
CGAL_add_named_parameter(number_of_output_points_t, number_of_output_points, number_of_output_points)
CGAL_add_named_parameter(size_t, size, size)
CGAL_add_named_parameter(maximum_variation_t, maximum_variation, maximum_variation)
CGAL_add_named_parameter(degree_fitting_t, degree_fitting, degree_fitting)
CGAL_add_named_parameter(degree_monge_t, degree_monge, degree_monge)
CGAL_add_named_parameter(threshold_percent_t, threshold_percent, threshold_percent)
CGAL_add_named_parameter(threshold_distance_t, threshold_distance, threshold_distance)
CGAL_add_named_parameter(attraction_factor_t, attraction_factor, attraction_factor)
CGAL_add_named_parameter(plane_t, plane_map, plane_map)
CGAL_add_named_parameter(plane_index_t, plane_index_map, plane_index_map)
CGAL_add_named_parameter(select_percentage_t, select_percentage, select_percentage)
CGAL_add_named_parameter(require_uniform_sampling_t, require_uniform_sampling, require_uniform_sampling)
CGAL_add_named_parameter(point_is_constrained_t, point_is_constrained, point_is_constrained_map)
CGAL_add_named_parameter(maximum_number_of_faces_t, maximum_number_of_faces, maximum_number_of_faces)
CGAL_add_named_parameter(transformation_t, transformation, transformation)
CGAL_add_named_parameter(point_set_filters_t, point_set_filters, point_set_filters)
CGAL_add_named_parameter(matcher_t, matcher, matcher)
CGAL_add_named_parameter(outlier_filters_t, outlier_filters, outlier_filters)
CGAL_add_named_parameter(error_minimizer_t, error_minimizer, error_minimizer)
CGAL_add_named_parameter(transformation_checkers_t, transformation_checkers, transformation_checkers)
CGAL_add_named_parameter(inspector_t, inspector, inspector)
CGAL_add_named_parameter(logger_t, logger, logger)
CGAL_add_named_parameter(pointmatcher_config_t, pointmatcher_config, pointmatcher_config)
CGAL_add_named_parameter(adjacencies_t, adjacencies, adjacencies)
CGAL_add_named_parameter(scan_angle_t, scan_angle_map, scan_angle_map)
CGAL_add_named_parameter(scanline_id_t, scanline_id_map, scanline_id_map)
CGAL_add_named_parameter(min_points_per_cell_t, min_points_per_cell, min_points_per_cell)
CGAL_add_named_parameter(scalar_t, scalar_map, scalar_map)

// List of named parameters used in Surface_mesh_approximation package
CGAL_add_named_parameter(verbose_level_t, verbose_level, verbose_level)
CGAL_add_named_parameter(seeding_method_t, seeding_method, seeding_method)
CGAL_add_named_parameter(max_number_of_proxies_t, max_number_of_proxies, max_number_of_proxies)
CGAL_add_named_parameter(min_error_drop_t, min_error_drop, min_error_drop)
CGAL_add_named_parameter(number_of_relaxations_t, number_of_relaxations, number_of_relaxations)

// List of named parameters used in Optimal_bounding_box package
CGAL_add_named_parameter(use_convex_hull_t, use_convex_hull, use_convex_hull)

// meshing parameters
CGAL_add_named_parameter(boundary_subdivision_ratio_t, boundary_subdivision_ratio, boundary_subdivision_ratio)
CGAL_add_named_parameter(subdivision_ratio_t, subdivision_ratio, subdivision_ratio)
CGAL_add_named_parameter(relative_to_chord_t, relative_to_chord, relative_to_chord)
CGAL_add_named_parameter(with_dihedral_angle_t, with_dihedral_angle, with_dihedral_angle)
CGAL_add_named_parameter(optimize_anchor_location_t, optimize_anchor_location, optimize_anchor_location)
CGAL_add_named_parameter(optimize_boundary_anchor_location_t, optimize_boundary_anchor_location, optimize_boundary_anchor_location)
CGAL_add_named_parameter(pca_plane_t, pca_plane, pca_plane)

// tetrahedral remeshing parameters
CGAL_add_named_parameter(remesh_boundaries_t, remesh_boundaries, remesh_boundaries)
CGAL_add_named_parameter(cell_selector_t, cell_selector, cell_is_selected_map)
CGAL_add_named_parameter(facet_is_constrained_t, facet_is_constrained, facet_is_constrained_map)
CGAL_add_named_parameter(smooth_constrained_edges_t, smooth_constrained_edges, smooth_constrained_edges)

// List of named parameters used in Alpha_wrap_3
CGAL_add_named_parameter(do_enforce_manifoldness_t, do_enforce_manifoldness, do_enforce_manifoldness)
CGAL_add_named_parameter(seed_points_t, seed_points, seed_points)
CGAL_add_named_parameter(refine_triangulation_t, refine_triangulation, refine_triangulation)
CGAL_add_named_parameter(keep_inner_connected_components_t, keep_inner_connected_components, keep_inner_connected_components)

// SMDS_3 parameters
CGAL_add_named_parameter(surface_facets_t, surface_facets, surface_facets)
CGAL_add_named_parameter(subdomain_indices_t, subdomain_indices, subdomain_indices)
CGAL_add_named_parameter(all_vertices_t, all_vertices, all_vertices)
CGAL_add_named_parameter(all_cells_t, all_cells, all_cells)
CGAL_add_named_parameter(rebind_labels_t, rebind_labels, rebind_labels)
CGAL_add_named_parameter(show_patches_t, show_patches, show_patches)
CGAL_add_named_parameter(allow_non_manifold_t, allow_non_manifold, allow_non_manifold)

// output parameters
CGAL_add_named_parameter(face_proxy_map_t, face_proxy_map, face_proxy_map)
CGAL_add_named_parameter(proxies_t, proxies, proxies)
CGAL_add_named_parameter(anchors_t, anchors, anchors)
CGAL_add_named_parameter(triangles_t, triangles, triangles)

CGAL_add_named_parameter(number_of_samples_t, number_of_samples, number_of_samples)
CGAL_add_named_parameter(accuracy_t, accuracy, accuracy)
CGAL_add_named_parameter(maximum_running_time_t, maximum_running_time, maximum_running_time)
CGAL_add_named_parameter(overlap_t, overlap, overlap)
CGAL_add_named_parameter(maximum_normal_deviation_t, maximum_normal_deviation, maximum_normal_deviation)

// List of named parameters used in Shape_detection package
CGAL_add_named_parameter(maximum_angle_t, maximum_angle, maximum_angle)
CGAL_add_named_parameter(maximum_distance_t, maximum_distance, maximum_distance)
CGAL_add_named_parameter(minimum_region_size_t, minimum_region_size, minimum_region_size)
CGAL_add_named_parameter(sphere_radius_t, sphere_radius, sphere_radius)
CGAL_add_named_parameter(k_neighbors_t, k_neighbors, k_neighbors)
CGAL_add_named_parameter(item_map_t, item_map, item_map)
CGAL_add_named_parameter(cosine_of_maximum_angle_t, cosine_of_maximum_angle, cosine_of_maximum_angle)
CGAL_add_named_parameter(minimum_radius_t, minimum_radius, minimum_radius)
CGAL_add_named_parameter(maximum_radius_t, maximum_radius, maximum_radius)

// List of named parameters used in Shape_regularization package
CGAL_add_named_parameter(minimum_length_t, minimum_length, minimum_length)
CGAL_add_named_parameter(maximum_offset_t, maximum_offset, maximum_offset)
CGAL_add_named_parameter(regularize_parallelism_t, regularize_parallelism, regularize_parallelism)
CGAL_add_named_parameter(regularize_orthogonality_t, regularize_orthogonality, regularize_orthogonality)
CGAL_add_named_parameter(regularize_coplanarity_t, regularize_coplanarity, regularize_coplanarity)
CGAL_add_named_parameter(regularize_axis_symmetry_t, regularize_axis_symmetry, regularize_axis_symmetry)
CGAL_add_named_parameter(symmetry_direction_t, symmetry_direction, symmetry_direction)
CGAL_add_named_parameter(preserve_order_t, preserve_order, preserve_order)
CGAL_add_named_parameter(adjust_directions_t, adjust_directions, adjust_directions)
CGAL_add_named_parameter(segment_t, segment_map, segment_map)

// List of named parameters used in Mesh_2 package
CGAL_add_named_parameter_with_compatibility(seeds_t, seeds, seeds)
CGAL_add_named_parameter_with_compatibility(domain_is_initialized_t, domain_is_initialized, domain_is_initialized)
CGAL_add_named_parameter_with_compatibility(seeds_are_in_domain_t, seeds_are_in_domain, seeds_are_in_domain)
CGAL_add_extra_named_parameter_with_compatibility(seeds_are_in_domain_t, seeds_are_in_domain, mark)
CGAL_add_named_parameter_with_compatibility(criteria_t, criteria, criteria)
CGAL_add_named_parameter_with_compatibility(convergence_ratio_t, convergence_ratio, convergence)
CGAL_add_named_parameter_with_compatibility(vertex_freeze_bound_t, vertex_freeze_bound, freeze_bound)
CGAL_add_named_parameter_with_compatibility(i_seed_begin_iterator_t, i_seed_begin_iterator, seeds_begin)
CGAL_add_named_parameter_with_compatibility(i_seed_end_iterator_t, i_seed_end_iterator, seeds_end)

//List of named parameters used in exude_mesh_3.h
CGAL_add_named_parameter_with_compatibility(lower_sliver_bound_t,lower_sliver_bound,sliver_bound)
CGAL_add_named_parameter_with_compatibility(freeze_t,freeze,do_freeze)
//List of named parameters used in perturb_mesh_3.h
CGAL_add_named_parameter_with_compatibility(sliver_criteria_t, sliver_criteria, sliver_criterion)
CGAL_add_named_parameter_with_compatibility(perturb_vector_t, perturb_vector, perturbation_vector)

//List of named parameters used in refine_mesh_3.h
CGAL_add_extra_named_parameter_with_compatibility(number_of_iterations_t, number_of_iterations, max_iteration_number)
CGAL_add_extra_named_parameter_with_compatibility(maximum_running_time_t, maximum_running_time, time_limit)
CGAL_add_named_parameter_with_compatibility(mesh_topology_number_t, mesh_topology_number, mesh_topology)
CGAL_add_named_parameter_with_compatibility(dump_after_init_prefix_param_t, dump_after_init_prefix_param, dump_after_init_prefix)
CGAL_add_named_parameter_with_compatibility(dump_after_refine_surface_prefix_param_t, dump_after_refine_surface_prefix_param, dump_after_refine_surface_prefix)
CGAL_add_named_parameter_with_compatibility(dump_after_refine_prefix_param_t, dump_after_refine_prefix_param, dump_after_refine_prefix)
CGAL_add_named_parameter_with_compatibility(dump_after_glob_opt_prefix_param_t, dump_after_glob_opt_prefix_param, dump_after_glob_opt_prefix)
CGAL_add_named_parameter_with_compatibility(dump_after_perturb_prefix_param_t, dump_after_perturb_prefix_param, dump_after_perturb_prefix)
CGAL_add_named_parameter_with_compatibility(dump_after_exude_prefix_param_t, dump_after_exude_prefix_param, dump_after_exude_prefix)
CGAL_add_named_parameter_with_compatibility(number_of_initial_points_param_t, number_of_initial_points_param, number_of_initial_points)
CGAL_add_named_parameter_with_compatibility(maximal_number_of_vertices_param_t, maximal_number_of_vertices_param, maximal_number_of_vertices)
CGAL_add_named_parameter_with_compatibility(nonlinear_growth_of_balls_param_t, nonlinear_growth_of_balls_param, nonlinear_growth_of_balls)
CGAL_add_named_parameter_with_compatibility(pointer_to_error_code_param_t, pointer_to_error_code_param, pointer_to_error_code)
CGAL_add_named_parameter_with_compatibility(pointer_to_stop_atomic_boolean_param_t, pointer_to_stop_atomic_boolean_param, pointer_to_stop_atomic_boolean)
CGAL_add_named_parameter_with_compatibility(exude_options_param_t, exude_options_param, exude_options)
CGAL_add_named_parameter_with_compatibility(perturb_options_param_t, perturb_options_param, perturb_options)
CGAL_add_named_parameter_with_compatibility(odt_options_param_t, odt_options_param, odt_options)
CGAL_add_named_parameter_with_compatibility(lloyd_options_param_t, lloyd_options_param, lloyd_options)
CGAL_add_named_parameter_with_compatibility(do_reset_c3t3_t, do_reset_c3t3, do_reset_c3t3)
CGAL_add_named_parameter_with_compatibility(mesh_param_t, mesh_param, mesh_options)
CGAL_add_named_parameter_with_compatibility(manifold_param_t, manifold_param, manifold_option)
CGAL_add_named_parameter_with_compatibility(features_option_param_t,features_options_param,features_options)

CGAL_add_named_parameter_with_compatibility_cref_only(image_3_param_t, image_3_param, image)
CGAL_add_named_parameter_with_compatibility(iso_value_param_t, iso_value_param, iso_value)
CGAL_add_named_parameter_with_compatibility(image_subdomain_index_t, image_subdomain_index, image_values_to_subdomain_indices)
CGAL_add_named_parameter_with_compatibility(voxel_value_t, voxel_value, value_outside)
CGAL_add_named_parameter_with_compatibility(error_bound_t, error_bound, relative_error_bound)
CGAL_add_named_parameter_with_compatibility(rng_t, rng, p_rng)
CGAL_add_named_parameter_with_compatibility(null_subdomain_index_param_t,null_subdomain_index_param, null_subdomain_index)
CGAL_add_named_parameter_with_compatibility(surface_patch_index_t, surface_patch_index, construct_surface_patch_index)
CGAL_add_named_parameter_with_compatibility_ref_only(weights_param_t, weights_param, weights)
CGAL_add_named_parameter_with_compatibility(features_detector_param_t, features_detector_param, features_detector)
CGAL_add_named_parameter_with_compatibility(input_features_param_t, input_features_param, input_features)

CGAL_add_named_parameter_with_compatibility(edge_size_param_t, edge_size_param, edge_size)
CGAL_add_named_parameter_with_compatibility_cref_only(edge_sizing_field_param_t, edge_sizing_field_param, edge_sizing_field)
CGAL_add_named_parameter_with_compatibility(edge_min_size_param_t, edge_min_size_param, edge_min_size)
CGAL_add_named_parameter_with_compatibility(facet_angle_param_t, facet_angle_param, facet_angle)
CGAL_add_named_parameter_with_compatibility(facet_size_param_t, facet_size_param, facet_size)
CGAL_add_named_parameter_with_compatibility_cref_only(facet_sizing_field_param_t, facet_sizing_field_param, facet_sizing_field)
CGAL_add_named_parameter_with_compatibility_cref_only(facet_distance_param_t, facet_distance_param, facet_distance)
CGAL_add_named_parameter_with_compatibility(facet_min_size_param_t, facet_min_size_param, facet_min_size)
CGAL_add_named_parameter_with_compatibility(facet_topology_param_t, facet_topology_param, facet_topology)
CGAL_add_named_parameter_with_compatibility(cell_radius_edge_param_t, cell_radius_edge_param, cell_radius_edge)
CGAL_add_named_parameter_with_compatibility(cell_radius_edge_ratio_param_t, cell_radius_edge_ratio_param, cell_radius_edge_ratio)
CGAL_add_named_parameter_with_compatibility(cell_size_param_t, cell_size_param, cell_size)
CGAL_add_named_parameter_with_compatibility_cref_only(cell_sizing_field_param_t, cell_sizing_field_param, cell_sizing_field)
CGAL_add_named_parameter_with_compatibility_cref_only(sizing_field_param_t, sizing_field_param, sizing_field)
CGAL_add_named_parameter_with_compatibility(cell_min_size_param_t, cell_min_size_param, cell_min_size)

CGAL_add_named_parameter_with_compatibility(function_param_t, function_param, function)
CGAL_add_named_parameter_with_compatibility(bounding_object_param_t, bounding_object_param, bounding_object)

//List of named parameters used in Straight_skeleton_2
CGAL_add_named_parameter_with_compatibility_ref_only(angles_param_t, angles_param, angles)
CGAL_add_named_parameter(maximum_height_t, maximum_height, maximum_height)
