/**
 * \file registration.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Functions for clouds registrations.
 */

#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "./cloud.h"

/**
 * \brief Finds the transformation matrix 4x4 (rotation + translation)
 * \param source The source cloud
 * \param target The target cloud
 * \return The transformation matrix 4x4
 */
struct matrix *registration_align(struct cloud *source, struct cloud *target);

/**
 * \brief Applies the ICP algorithm to register source with target
 * \param source The source cloud
 * \param target The target cloud
 * \param aligned Pointer to the output cloud registered
 * \param th Error diference stop criteria
 * \param k Maximun number of iterations
 * \param max_dist Maximun distance between matches
 * \param cp Closest points call back
 * \return The transformation matrix 4x4
 */
struct matrix *registration_icp(struct cloud *source,
                                struct cloud *target,
                                struct cloud **aligned,
                                real th,
                                uint k,
                                real max_dist,
                                closest_points_func cp);

#endif // REGISTRATION_H
