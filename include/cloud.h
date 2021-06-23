/**
 * \file cloud.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief The point cloud structure and its functions.
 */

#ifndef CLOUD_H
#define CLOUD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./vector3.h"
#include "./pointset.h"
#include "./plane.h"
#include "./algebra.h"
#include "./octree.h"

#define CLOUD_MAXBUFFER 1024

/**
 * \brief Struct to store a cloud
 */
struct cloud
{
	struct pointset *points;
	uint numpts;
	struct vector3 *centroid;
	struct octree *tree;
};

typedef struct cloud *(*closest_points_func)(struct cloud *, struct cloud *);

/**
 * \brief Initializes a cloud
 * \return Pointer to the new cloud (empty)
 */
struct cloud *cloud_new();

/**
 * \brief Frees a cloud
 * \param cloud Cloud to be freed
 */
void cloud_free(struct cloud **cloud);

/**
 * \brief Adds a new point in the cloud (3 real numbers);
 * \param cloud Target cloud
 * \param x Coordinate x
 * \param y Coordinate y
 * \param z Coordinate z
 * \return Pointer to the new point on the cloud or NULL if it fails to be 
 * allocated
 */
struct vector3 *cloud_insert_real(struct cloud *cloud, real x, real y, real z);
/**
 * \brief Adds a new point in the cloud (vector3);
 * \param cloud Target cloud
 * \param p Point to be added
 * \return Pointer to the new point on the cloud or NULL if it fails to be 
 * allocated
 */
struct vector3 *cloud_insert_vector3(struct cloud *cloud, struct vector3 *p);

/**
 * \brief Counts points in a cloud
 * \param cloud Target cloud
 * \return Number of points in the cloud
 */
uint cloud_size(struct cloud *cloud);

/**
 * \brief Loads cloud from a XYZ file
 * \param filename File name
 * \return Cloud loaded from the file or NULL if it fails to allocate memory
 */
struct cloud *cloud_load_xyz(const char *filename);

/**
 * \brief Loads cloud from a CSV file
 * \param filename File name
 * \return Cloud loaded from the file or NULL if it fails to allocate memory
 */
struct cloud *cloud_load_csv(const char *filename);

/**
 * \brief Loads cloud from a PLY file (x y z always first properties!)
 * \param filename File name
 * \return Cloud loaded from the file or NULL if it fails to allocate memory
 */
struct cloud *cloud_load_ply(const char *filename);

/**
 * \brief Loads cloud from a PCD file (DATA ASCII only!)
 * \param filename File name
 * \return Cloud loaded from the file or NULL if it fails to allocate memory
 */
struct cloud *cloud_load_pcd(const char *filename);

// Temporary
struct cloud *cloud_load_json(const char *filename);

/**
 * \brief Loads cloud from an OBJ file
 * \param filename File name
 * \return Cloud loaded from the file or NULL if it fails to allocate memory
 */
struct cloud *cloud_load_obj(const char *filename);

/**
 * \brief Saves a cloud in a XYZ file
 * \param cloud Cloud to be saved
 * \param filename Destination
 * \return 0 if it fails, or 1 if not
 */
int cloud_save_xyz(struct cloud *cloud, const char *filename);

/**
 * \brief Saves a cloud in a CSV file
 * \param cloud Cloud to be saved
 * \param filename Destination
 * \return 0 if it fails, or 1 if not
 */
int cloud_save_csv(struct cloud *cloud, const char *filename);

/**
 * \brief Saves a cloud in a PLY file
 * \param cloud Cloud to be saved
 * \param filename Destination
 * \return 0 if it fails, or 1 if not
 */
int cloud_save_ply(struct cloud *cloud, const char *filename);

/**
 * \brief Saves a cloud in a PCD file
 * \param cloud Cloud to be saved
 * \param filename Destination
 * \return 0 if it fails, or 1 if not
 */
int cloud_save_pcd(struct cloud *cloud, const char *filename);

/**
 * \brief Makes a copy of a cloud
 * \param cloud The cloud to be copied
 * \return A copy of the input cloud
 */
struct cloud *cloud_copy(struct cloud *cloud);

/**
 * \brief Generates a tree data structure that partitionates the cloud
 * \param cloud The target cloud
 */
void cloud_partitionate(struct cloud *cloud);

/**
 * \brief Calculates the geometric centroid of a cloud
 * \param cloud Target cloud
 * \return Point with the coordinates of the geometric centroid of the cloud
 */
struct vector3 *cloud_calc_centroid(struct cloud *cloud);

/**
 * \brief Gets the geometric centroid of a cloud
 * \param cloud Target cloud
 * \return Point with the coordinates of the geometric centroid of the cloud
 */
struct vector3 *cloud_get_centroid(struct cloud *cloud);

/**
 * \brief Scales a cloud from a factor
 * \param cloud Target cloud
 * \param f Factor
 */
void cloud_scale(struct cloud *cloud, real f);

/**
 * \brief Translate a cloud from an origin to a destination
 * \param cloud Target cloud
 * \param source Origin vector
 * \param target Destination vector
 */
void cloud_translate_vector_dir(struct cloud *cloud,
																struct vector3 *source,
																struct vector3 *target);

/**
 * \brief Translate a cloud from a target vector
 * \param cloud Target cloud
 * \param dest Destination vector
 * \param t Transformation vector
 */
void cloud_translate_vector(struct cloud *cloud, struct vector3 *dest);

/**
 * \brief Translate a vector from coordinates
 * \param cloud Target cloud
 * \param x Coordinate x
 * \param y Coordinate y
 * \param z Coordinate z
 */
void cloud_translate_real(struct cloud *cloud, real x, real y, real z);

/**
 * \brief Transform a cloud with a matrix 4x4 (rotation and translation)
 * \param cloud Target cloud
 * \param rt Transformation matrix 4x4
 */
void cloud_transform(struct cloud *cloud, struct matrix *rt);

/**
 * \brief Sort a cloud by an axis using quick sort
 * \param cloud Target cloud
 * \param axis The axis to sort with (0: x, 1: y, 2: z)
 */
void cloud_sort(struct cloud *cloud, int axis);

/**
 * \brief Concatenates two clouds
 * \param c1 First cloud
 * \param c2 Second cloud
 * \return New cloud with c1 and c2 points
 */
struct cloud *cloud_concat(struct cloud *c1, struct cloud *c2);

/**
 * \brief Calculates the size of the 3 axis on a cloud
 * \param cloud Target cloud
 * \return Point where each coordinate is the size of its own the dimension
 */
struct vector3 *cloud_axis_size(struct cloud *cloud);

/**
 * \brief Calculates the area of the bounding box where a cloud is
 * \param cloud Target cloud
 * \return Area of the cloud bbox
 */
real cloud_boundingbox_area(struct cloud *cloud);

/**
 * \brief Calculates the volue of the boundig box where a cloud is
 * \param cloud Target cloud
 * \return Volume of the cloud bbox
 */
real cloud_boundingbox_volume(struct cloud *cloud);

/**
 * \brief Calculates the volume of a cloud function
 * \param cloud Target cloud
 * \return Volume of the cloud function (sum of central distances)
 */
real cloud_function_volume(struct cloud *cloud);

/**
 * \brief Crops a cloud with a circular cut from an reference point
 * \param cloud Target cloud
 * \param p Reference
 * \param r Radius for the crop (mm)
 * \return Cropped cloud
 */
struct cloud *cloud_cut_radius(struct cloud *cloud, struct vector3 *p, real r);

/**
 * \brief Crops a cloud with a cut from the centroid
 * \param cloud Target cloud
 * \param r Value to cut (mm)
 * \return Cropped cloud
 */
struct cloud *cloud_cut_centroid(struct cloud *cloud, real r);

/**
 * \brief Crops a cloud in a direction from a reference
 * \param cloud Target cloud
 * \param plane Plane to cut
 * \return Cropped cloud
 */
struct cloud *cloud_cut_plane(struct cloud *cloud, struct plane *plane);

/**
 * \brief Split a cloud in two with a plane
 * \param src Cloud to be split
 * \param plane The plane to be used to split the cloud
 * \param p1 First slice of the split cloud
 * \param p2 Second slice of the split cloud
 * \return 0 if it fails, or 1 if not
 */
int cloud_plane_partition(struct cloud *src,
													struct plane *plane,
													struct cloud *p1,
													struct cloud *p2);

/**
 * \brief Gets the farthest point in the positive region of a plane
 * \param src Target cloud
 * \param plane Reference plane
 * \return The point in cloud farthest of the plane
 */
struct vector3 *cloud_max_distance_from_plane(struct cloud *cloud,
																							struct plane *plane);

/**
 * \brief Subsample points of the clound in a cylinder
 * \param cloud Target cloud
 * \param ref Reference point
 * \param dir Direction of the cylinder height
 * \param radius Radius of the cylinder
 * \return Subsample inserted in the cylinder
 */
struct cloud *cloud_cut_cylinder(struct cloud *cloud,
																 struct vector3 *ref,
																 struct vector3 *dir,
																 real radius);

/**
 * \brief Extracts segment of points in the cloud
 * \param cloud Target cloud
 * \param ref Reference point
 * \param dir Direction of the segment
 * \param epslon Width of the segment (mm)
 * \return Subsample of the cloud with points of the segment
 */
struct cloud *cloud_segment(struct cloud *cloud,
														struct vector3 *ref,
														struct vector3 *dir,
														real epslon);

/**
 * \brief Gets the closest point of the cloud to an point
 * \param cloud Target cloud
 * \param point Target point
 * \return Address to the closest point
 */
struct vector3 *cloud_closest_point(struct cloud *cloud, struct vector3 *point);

/**
 * \brief Gets the closest point of the cloud to an point (get node)
 * \param cloud Target cloud
 * \param point Target point
 * \return Address of the pointset node containing the closest point
 */
struct pointset *cloud_closest_point_set(struct cloud *cloud,
																				 struct vector3 *point);

/**
 * \brief Gets point closest to a cloud centroid
 * \param cloud Target cloud
 * \return Closest point
 */
struct vector3 *cloud_closest_to_centroid(struct cloud *cloud);

/**
 * \brief Finds the closest point of a cloud to another cloud
 * \param source Source cloud
 * \param target Target cloud (where I want to find the closest point)
 * \param src_pt The point on source that yields the closest distance
 * \param tgt_pt The point on target that yields the closest distance
 * \return The closest distance between source and target
 */
real cloud_nearest_neighbors_bruteforce(struct cloud *source,
																				struct cloud *target,
																				struct vector3 **src_pt,
																				struct vector3 **tgt_pt);

/**
 * \brief Finds the closest point of a cloud to another cloud using partitioning
 * \param source Source cloud
 * \param target Target cloud (where I want to find the closest point)
 * \param src_pt The point on source that yields the closest distance
 * \param tgt_pt The point on target that yields the closest distance
 * \return The closest distance between source and target
 */
real cloud_nearest_neighbors_partition(struct cloud *source,
																			 struct cloud *target,
																			 struct vector3 **src_pt,
																			 struct vector3 **tgt_pt);

/**
 * \brief Gets the point with the lesser coordinate x
 * \param cloud Target cloud
 * \return Point with the lesser x
 */
struct vector3 *cloud_min_x(struct cloud *cloud);

/**
 * \brief Gets the point with the lesser coordinate y
 * \param cloud Target cloud
 * \return Point with the lesser y
 */
struct vector3 *cloud_min_y(struct cloud *cloud);

/**
 * \brief Gets the point with the lesser coordinate z
 * \param cloud Target cloud
 * \return Point with the lesser z
 */
struct vector3 *cloud_min_z(struct cloud *cloud);

/**
 * \brief Gets the point with the greater coordinate x
 * \param cloud Target cloud
 * \return Point with the greater x
 */
struct vector3 *cloud_max_x(struct cloud *cloud);

/**
 * \brief Gets the point with the greater coordinate y
 * \param cloud Target cloud
 * \return Point with the greater y
 */
struct vector3 *cloud_max_y(struct cloud *cloud);

/**
 * \brief Gets the point with the greater coordinate z
 * \param cloud Target cloud
 * \return Point with the greater z
 */
struct vector3 *cloud_max_z(struct cloud *cloud);

/**
 * \brief Calculates the greater distance between a point and points in a cloud
 * \param cloud Target cloud
 * \param p Target point
 * \return The greater distance possible from a point of the cloud to the point 
 * p
 */
real cloud_max_distance(struct cloud *cloud, struct vector3 *p);

/**
 * \brief Calculates the distance between the centroid of a cloud and the farthest
 * point to it
 * \param cloud Target cloud
 * \return The greater distance possible from the centroid of a cloud to a point 
 * of it
 */
real cloud_max_distance_from_centroid(struct cloud *cloud);

/**
 * \brief Adjuste a plane to a cloud using point function
 * \param cloud Cloud to find the plane
 * \param ref Reference point
 * \return Best plane to describe the cloud
 */
struct plane *cloud_dispersion_plane(struct cloud *cloud, struct vector3 *ref);

/**
 * \brief Gets normal direction of the dispersion of a clound based on a 
 * reference point
 * \param cloud Target cloud
 * \param ref Reference point
 * \return Normal to the plane that is the best to describe the cloud
 */
struct vector3 *cloud_normal(struct cloud *cloud, struct vector3 *ref);

/**
 * \brief Adjusts a plane toa cloud globally
 * \param cloud Target cloud
 * \return Plane that better fits the cloud
 */
struct plane *cloud_plane_fitting(struct cloud *cloud);

/**
 * \brief Gets the farthest point from the cloud to the plane that better fits 
 * the cloud
 * \param cloud Target cloud
 * \return The farthest point from the cloud to the plane that better fits the
 * cloud
 */
struct vector3 *cloud_point_faraway_bestfit(struct cloud *cloud);

/**
 * \brief Global curvature of a cloud
 * \param cloud Target cloud
 * \return Fits the cloud in a sphere and returns 1/R (R = radius of the sphere)
 */
real cloud_curvature(struct cloud *cloud);

/**
 * \brief Calculates the rooted mean square error between two clouds already sorted
 * \param source Source cloud
 * \param target Target cloud
 * \return A real with the rmse value.
 */
real cloud_rmse_sorted(struct cloud *source, struct cloud *target);

/**
 * \brief Calculates the rooted mean square error between two clouds
 * \param source Source cloud
 * \param target Target cloud
 * \return A real with the rmse value.
 */
real cloud_rmse(struct cloud *source,
								struct cloud *target,
								real max_dist,
								closest_points_func cp);

/**
 * \brief Debugs a cloud
 * \param cloud Cloud to be displayed
 * \param output File to save the cloud
 */
void cloud_debug(struct cloud *cloud, FILE *filename);

/**
 * \brief Equivalence cloud with closest points of source in target (bruteforce)
 * \param source The source cloud
 * \param target The target cloud
 * \return The equivalence cloud
 */
struct cloud *cloud_closest_points_bf(struct cloud *source,
																			struct cloud *target);

/**
 * \brief Equivalence cloud with closest points of source in target (using tree)
 * \param source The source cloud
 * \param target The target cloud
 * \return The equivalence cloud
 */
struct cloud *cloud_closest_points_tree(struct cloud *source,
																				struct cloud *target);

#endif // CLOUD_H
