/**
 * \file pointset.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief The fundamental data structure that holds a set of 3D Points
 */

#ifndef POINTSET_H
#define POINTSET_H

#include "./vector3.h"

/**
 * \brief The pointset data structure (current: double linked-list)
 */
struct pointset
{
  struct vector3 *point;
  struct pointset *next;
  struct pointset *prev;
};

/**
 * \brief Creates a new pointset
 * \return Just a NULL pointer indicating an empty ponintset
 */
struct pointset *pointset_new();

/**
 * \brief Frees the memory allocated to a pointset
 * \param set The address to the first node
 */
void pointset_free(struct pointset **set);

// struct vector3 *pointset_insert(struct pointset **set,
//                                      real x,
//                                      real y,
//                                      real z);

/**
 * \brief Inserts a new point into a pointset (head)
 * \param set The pointset head address
 * \param x The X coordinate
 * \param y The Y coordinate
 * \param z The Z coordinate
 * \return The address to the newly added point
 */
struct vector3 *pointset_insert_head(struct pointset **set,
                                     real x,
                                     real y,
                                     real z);

/**
 * \brief Inserts a new point into a pointset (tail)
 * \param set The pointset head address
 * \param x The X coordinate
 * \param y The Y coordinate
 * \param z The Z coordinate
 * \return The address to the newly added point
 */
struct vector3 *pointset_insert_tail(struct pointset **set,
                                     real x,
                                     real y,
                                     real z);

/**
 * \brief Inserts a new point into a pointset after a defined position
 * \param set The pointset head address
 * \param i The point index (head is 0)
 * \param x The X coordinate
 * \param y The Y coordinate
 * \param z The Z coordinate
 * \return The address to the newly added point
 */
struct vector3 *pointset_insert_next(struct pointset **set,
                                     uint i,
                                     real x,
                                     real y,
                                     real z);

/**
 * \brief Inserts a new point into a pointset before a defined position
 * \param set The pointset head address
 * \param i The point index (head is 0)
 * \param x The X coordinate
 * \param y The Y coordinate
 * \param z The Z coordinate
 * \return The address to the newly added point
 */
struct vector3 *pointset_insert_prev(struct pointset **set,
                                     uint i,
                                     real x,
                                     real y,
                                     real z);

/**
 * \brief Makes a copy of a pointset
 * \param set The pointset to be copied
 * \return A new pointset, with equal elements of set
 */
struct pointset *pointset_copy(struct pointset *set);


/**
 * \brief Creates a pointset from a slice of another
 * \param begin The first element (head of a pointset)
 * \param end The last element
 * \param size A pointer to hold the number of elements the segment will have
 * \return The address to a new point set with elements from begin to end
 */
struct pointset *pointset_segment(struct pointset *begin,
                                  struct pointset *end,
                                  uint *size);

/**
 * \brief Creates a a pointset from a slice of another (reverse order)
 * \param begin The first element (head of a pointset)
 * \param end The last element
 * \param size A pointer to hold the number of elements the segment will have
 * \return The address to a new point set with elements from begin to end
 */
struct pointset *pointset_segment_reverse(struct pointset *begin,
                                          struct pointset *end,
                                          uint *size);

/**
 * \brief Gets the size of a pointset
 * \param set The target pointset
 * \return Number of elements in set
 */
uint pointset_size(struct pointset *set);

/**
 * \brief Auxiliary function to swap two points (used by the sort function)
 * \param a The first vector
 * \param b The secont vector
 */
void pointset_swap(struct vector3 **a, struct vector3 **b);

/**
 * \brief Auxiliary function to handle the recursive call of sort
 * \param head The begin of a set
 * \param tail The element to stop the partition
 */
void pointset_recursive_sort(struct pointset *head,
                             struct pointset *tail,
                             int axis);

/**
 * \brief Sorts a pointset
 * \param set The pointset to be sorted
 * \param axis The axis of sorting (0: X, 1: Y, 2: Z)
 */
void pointset_sort(struct pointset *set, int axis);

/**
 * \brief Get the median point from a pointset in respect to a axis
 * \param set The target pointset
 * \param The axis of reference (0: X, 1: Y, 2: Z)
 * \param size The number of elements to consider
 */
struct pointset *pointset_median(struct pointset *set, int axis, uint size);

/**
 * \brief Debugs a pointset
 * \param set The target pointset
 * \param output The file to dump the debug
 */
void pointset_debug(struct pointset *set, FILE *output);

/**
 * \brief Debugs a pointset (reverse order)
 * \param set The target pointset
 * \param output The file to dump the debug
 */
void pointset_debug_reverse(struct pointset *set, FILE *output);

#endif // POINTSET_H
