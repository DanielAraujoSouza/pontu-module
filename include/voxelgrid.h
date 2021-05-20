/**
 * \file voxelgrid.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Voxelgrid subsampling implementation and some author variations.
 */

#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "./vector3.h"
#include "./cloud.h"

/**
 * \brief Executes voxelgrid subsampling
 * \param src The target cloud
 * \param leafsize The size of the sliding cube
 * \return The src cloud with reduced density
 */
struct cloud* voxelgrid_sampling(struct cloud *src, real leafsize);

/**
 * \brief Executes voxelgrid subsampling and uses the voxels centroids
 * \param src The target cloud
 * \param leafsize The size of the sliding cube
 * \return The src cloud with reduced density
 */
struct cloud* voxelgrid_sampling_centroid(struct cloud *src, real leafsize);

#endif // VOXELGRID_H

