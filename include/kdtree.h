/**
 * \file kdtree.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief 3D space partitioning using the kdtree data structure.
 */

#ifndef KDTREE_H
#define KDTREE_H

#include "./vector3.h"
#include "./pointset.h"

/**
 * \brief The kdtree data structure
 */
struct kdtree {
	struct pointset *points;
	struct pointset *median;
	int axis;
	uint numpts;
	struct kdtree *left;
	struct kdtree *right;
};

/**
 * \brief Creates a new kdtree node
 * \param points Points to be holded by the node
 * \param numpts The number of elements in points
 * \param axis The axis of partitioning
 * \return The address of a new kdtree node
 */
struct kdtree *kdtree_new(struct pointset *points, uint numpts, int axis);

/**
 * \brief Frees the memory allocated by a kdtree
 * \param kdt The address to the root node
 */
void kdtree_free(struct kdtree **kdt);

/**
 * \brief Executes the partitioning algorithm
 */
void kdtree_partitionate(struct kdtree *kdt);

/**
 * \brief Calculates if a node is inside the hyperplane boundary spaned by root
 * \param k1 The root node
 * \param k2 The target node
 * \return The distance from k2 to the kyperplane of k1
 */
real kdtree_dist_hyperplane(struct kdtree *k1, struct kdtree *k2);

/**
 * \brief Returns the closest node of a kdtree in respect to a point
 * \param kdt The root node
 * \param p The target point
 * \param r A pointer to hold the current best distance
 * \return The node of kdt closest to p
 */
struct kdtree *kdtree_closest_node(struct kdtree *kdt,
                                   struct vector3 *p,
                                   real *r);

/**
 * \brief Returns the closest point of a kdtree in respect to a point
 * \param node The root node
 * \param current The current best node
 * \param point The target point
 * \param best The current best point
 * \param radius A pointer to hold the distance to the hyperplane
 * \param dist A pointer to hold the best distance
 */
void kdtree_closest_point(struct kdtree *node,
                          struct kdtree* current,
                          struct vector3 *point,
                          struct vector3 **best,
                          real *radius,
                          real *dist);

/**
 * \brief Finds the nearest neighbor to a point in a kdtree
 * \param kdt The root node
 * \param point The target points
 * \return Address to the closest point
 */
struct vector3 *kdtree_nearest_neighbor(struct kdtree *kdt,
                                        struct vector3 *point);

/**
 * \brief Debugs a kdtree
 * \param kdt The root node
 * \param output The file to dump the debug at
 */
void kdtree_debug(struct kdtree *kdt, FILE *output);

#endif // KDTREE_H

