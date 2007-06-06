#define pixman_add_trapezoids s_render_pixman_add_trapezoids
#define pixman_color_to_pixel s_render_pixman_color_to_pixel
#define composeFunctions s_render_pixman_compose_functions
#define fbComposeSetupMMX s_render_pixman_compose_setup_mmx
#define pixman_composite s_render_pixman_composite
#define fbCompositeCopyAreammx s_render_pixman_composite_copy_area_mmx
#define fbCompositeSolidMask_nx8888x0565Cmmx s_render_pixman_composite_solid_mask_nx8888x0565Cmmx
#define fbCompositeSolidMask_nx8888x8888Cmmx s_render_pixman_composite_solid_mask_nx8888x8888Cmmx
#define fbCompositeSolidMask_nx8x0565mmx s_render_pixman_composite_solid_mask_nx8x0565mmx
#define fbCompositeSolidMask_nx8x8888mmx s_render_pixman_composite_solid_mask_nx8x8888mmx
#define fbCompositeSolidMaskSrc_nx8x8888mmx s_render_pixman_composite_solid_mask_src_nx8x8888mmx
#define fbCompositeSolid_nx0565mmx s_render_pixman_composite_solid_nx0565mmx
#define fbCompositeSolid_nx8888mmx s_render_pixman_composite_solid_nx8888mmx
#define fbCompositeSrc_8888RevNPx0565mmx s_render_pixman_composite_src_8888RevNPx0565mmx
#define fbCompositeSrc_8888RevNPx8888mmx s_render_pixman_composite_src_8888RevNPx8888_mmx
#define fbCompositeSrc_8888x0565mmx s_render_pixman_composite_src_8888x0565mmx
#define fbCompositeSrc_8888x8888mmx s_render_pixman_composite_src_8888x8888mmx
#define fbCompositeSrc_8888x8x8888mmx s_render_pixman_composite_src_8888x8x8888mmx
#define fbCompositeSrcAdd_8000x8000mmx s_render_pixman_composite_src_add_8000x8000mmx
#define fbCompositeSrcAdd_8888x8888mmx s_render_pixman_composite_src_add_8888x8888mmx
#define fbCompositeSrc_x888x8x8888mmx s_render_pixman_composite_src_x888x8x8888mmx
#define pixman_composite_trapezoids s_render_pixman_composite_trapezoids
#define pixman_composite_tri_fan s_render_pixman_composite_tri_fan
#define pixman_composite_tri_strip s_render_pixman_composite_tri_strip
#define pixman_composite_triangles s_render_pixman_composite_triangles
#define fbCopyAreammx s_render_pixman_copy_area_mmx
#define pixman_fill_rectangle s_render_pixman_fill_rectangle
#define pixman_fill_rectangles s_render_pixman_fill_rectangles
#define pixman_format_create s_render_pixman_format_create
#define pixman_format_create_masks s_render_pixman_format_create_masks
#define pixman_format_destroy s_render_pixman_format_destroy
#define pixman_format_get_masks s_render_pixman_format_get_masks
#define pixman_format_init s_render_pixman_format_init
#if defined(USE_MMX) && !defined(__amd64__) && !defined(__x86_64__)
#define fbHaveMMX s_render_pixman_have_mmx
#endif
#define pixman_image_create s_render_pixman_image_create
#define pixman_image_create_for_data s_render_pixman_image_create_for_data
#define pixman_image_destroy s_render_pixman_image_destroy
#define pixman_image_get_data s_render_pixman_image_get_data
#define pixman_image_get_depth s_render_pixman_image_get_depth
#define pixman_image_get_format s_render_pixman_image_get_format
#define pixman_image_get_height s_render_pixman_image_get_height
#define pixman_image_get_stride s_render_pixman_image_get_stride
#define pixman_image_get_width s_render_pixman_image_get_width
#define pixman_image_set_clip_region s_render_pixman_image_set_clip_region
#define pixman_image_set_component_alpha s_render_pixman_image_set_component_alpha
#define pixman_image_set_filter s_render_pixman_image_set_filter
#define pixman_image_set_repeat s_render_pixman_image_set_repeat
#define pixman_image_set_transform s_render_pixman_image_set_transform
#define pixman_image_create_linear_gradient s_render_pixman_image_create_linear_gradient
#define pixman_image_create_radial_gradient s_render_pixman_image_create_radial_gradient
#define miIsSolidAlpha s_render_pixman_is_solid_alpha
#define pixman_pixel_to_color s_render_pixman_pixel_to_color
#define pixman_region_append s_render_pixman_region_append
#define pixman_region_contains_point s_render_pixman_region_contains_point
#define pixman_region_contains_rectangle s_render_pixman_region_contains_rectangle
#define pixman_region_copy s_render_pixman_region_copy
#define pixman_region_create s_render_pixman_region_create
#define pixman_region_create_simple s_render_pixman_region_create_simple
#define pixman_region_destroy s_render_pixman_region_destroy
#define pixman_region_empty s_render_pixman_region_empty
#define pixman_region_extents s_render_pixman_region_extents
#define pixman_region_intersect s_render_pixman_region_intersect
#define pixman_region_inverse s_render_pixman_region_inverse
#define pixman_region_not_empty s_render_pixman_region_not_empty
#define pixman_region_num_rects s_render_pixman_region_num_rects
#define pixman_region_rects s_render_pixman_region_rects
#define pixman_region_reset s_render_pixman_region_reset
#define pixman_region_subtract s_render_pixman_region_subtract
#define pixman_region_translate s_render_pixman_region_translate
#define pixman_region_union s_render_pixman_region_union
#define pixman_region_union_rect s_render_pixman_region_union_rect
#define pixman_region_validate s_render_pixman_region_validate
#define RenderEdgeInit s_render_pixman_render_edge_init
#define RenderEdgeStep s_render_pixman_render_edge_step
#define RenderLineFixedEdgeInit s_render_pixman_render_line_fixed_edge_init
#define RenderSampleCeilY s_render_pixman_render_sample_ceil_y
#define RenderSampleFloorY s_render_pixman_render_sample_floor_y
#define fbSolidFillmmx s_render_pixman_solid_fill_mmx