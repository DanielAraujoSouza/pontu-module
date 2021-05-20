#include "../include/voxelgrid.h"

struct cloud *voxelgrid_sampling_centroid(struct cloud *src, real leafsize) {
    if (leafsize <= 0.0) {
        return NULL;
    }

    struct cloud *output = cloud_new();
    real apothem = leafsize / 2.0;

    real max_x = cloud_max_x(src)->x + apothem;
    real min_x = cloud_min_x(src)->x - apothem;
    real max_y = cloud_max_y(src)->y + apothem;
    real min_y = cloud_min_y(src)->y - apothem;
    real max_z = cloud_max_z(src)->z + apothem;
    real min_z = cloud_min_z(src)->z - apothem;

    real c_x;
    real c_y;
    real c_z;

    uint len_x = ceil((max_x - min_x) / leafsize);
    uint len_y = ceil((max_y - min_y) / leafsize);
    uint len_z = ceil((max_z - min_z) / leafsize);
    uint total_len = len_x * len_y * len_z;
    uint id_aux;

    struct vector3 **closest_points = calloc(sizeof (struct vector3 *),  
                                             total_len);
    if (closest_points == NULL) {
        cloud_free(&output);
        return NULL;
    }

    for (struct pointset *s = src->points; s != NULL; s = s->next) {
        uint pos_x = ceil((s->point->x - min_x) / leafsize) - 1;
        uint pos_y = ceil((s->point->y - min_y) / leafsize) - 1;
        uint pos_z = ceil((s->point->z - min_z) / leafsize) - 1;

        id_aux = (pos_z * len_x * len_y) + (pos_y * len_x) + pos_x;
        closest_points[id_aux] = s->point;
    }

    for (uint i = 0; i < len_x; i++) {
        c_x = (i * leafsize) + apothem + min_x;
        for (uint j = 0; j < len_y; j++) {
            c_y = (j * leafsize) + apothem + min_y;
            for (uint k = 0; k < len_z; k++) {
                c_z = (k * leafsize) + apothem + min_z;
                id_aux = (k * len_x * len_y) + (j * len_x) + i;
                if (closest_points[id_aux] != NULL) {
                    cloud_insert_real(output, c_x, c_y, c_z);
                }
            }
        }
    }

    free(closest_points);
    return output;
}

struct cloud *voxelgrid_sampling(struct cloud *src, real leafsize) {
    if (leafsize <= 0.0) {
        return NULL;
    }

    struct cloud *output = cloud_new();
    real apothem = leafsize / 2.0;

    real max_x = cloud_max_x(src)->x + apothem;
    real min_x = cloud_min_x(src)->x - apothem;
    real max_y = cloud_max_y(src)->y + apothem;
    real min_y = cloud_min_y(src)->y - apothem;
    real max_z = cloud_max_z(src)->z + apothem;
    real min_z = cloud_min_z(src)->z - apothem;

    real c_x;
    real c_y;
    real c_z;

    uint len_x = ceil((max_x - min_x) / leafsize);
    uint len_y = ceil((max_y - min_y) / leafsize);
    uint len_z = ceil((max_z - min_z) / leafsize);
    uint total_len = len_x * len_y * len_z;
    uint id_aux;
    struct vector3 *point_aux = vector3_zero();
    real dist;

    struct vector3 **closest_points = calloc(sizeof (struct vector3 *),  
                                             total_len);
    if (closest_points == NULL) {
        cloud_free(&output);
        return NULL;
    }
    
    for (struct pointset *s = src->points; s != NULL; s = s->next) {
        uint pos_x = ceil((s->point->x - min_x) / leafsize) - 1;
        uint pos_y = ceil((s->point->y - min_y) / leafsize) - 1;
        uint pos_z = ceil((s->point->z - min_z) / leafsize) - 1;

        id_aux = (pos_z * len_x * len_y) + (pos_y * len_x) + pos_x;
        if (closest_points[id_aux] != NULL) {
            c_x = (pos_x * leafsize) + apothem + min_x;
            c_y = (pos_y * leafsize) + apothem + min_y;
            c_z = (pos_z * leafsize) + apothem + min_z;
            
            vector3_set(point_aux, c_x, c_y, c_z);
            dist = vector3_distance(point_aux, closest_points[id_aux]);
            
            if (vector3_distance(point_aux, s->point) < dist) {
                closest_points[id_aux] = s->point;
            }
        } else {
            closest_points[id_aux] = s->point;
        }
    }

    for (uint i = 0; i < total_len; i++) {
        if (closest_points[i] != NULL) {
            cloud_insert_vector3(output, closest_points[i]);
        }
    }

    vector3_free(&point_aux);
    free(closest_points);
    return output;
}

