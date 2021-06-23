#include "../include/registration.h"

struct matrix *registration_align(struct cloud *source, struct cloud *target)
{
    cloud_calc_centroid(source);
    cloud_calc_centroid(target);

    struct matrix *source_aux = matrix_new(3, 1);
    struct matrix *target_aux = matrix_new(1, 3);

    matrix_set(source_aux, 0, 0, source->centroid->x);
    matrix_set(source_aux, 1, 0, source->centroid->y);
    matrix_set(source_aux, 2, 0, source->centroid->z);

    matrix_set(target_aux, 0, 0, target->centroid->x);
    matrix_set(target_aux, 0, 1, target->centroid->y);
    matrix_set(target_aux, 0, 2, target->centroid->z);

    struct matrix *centroid_prod = algebra_mat_prod(source_aux, target_aux);
    struct matrix *s = matrix_new(3, 3);

    struct pointset *src = source->points;
    struct pointset *tgt = target->points;

    do
    {
        matrix_set(s, 0, 0,
                   matrix_get(s, 0, 0) +
                       src->point->x * tgt->point->x -
                       matrix_get(centroid_prod, 0, 0));

        matrix_set(s, 0, 1,
                   matrix_get(s, 0, 1) +
                       src->point->x * tgt->point->y -
                       matrix_get(centroid_prod, 0, 1));

        matrix_set(s, 0, 2,
                   matrix_get(s, 0, 2) +
                       src->point->x * tgt->point->z -
                       matrix_get(centroid_prod, 0, 2));

        matrix_set(s, 1, 0,
                   matrix_get(s, 1, 0) +
                       src->point->y * tgt->point->x -
                       matrix_get(centroid_prod, 1, 0));

        matrix_set(s, 1, 1,
                   matrix_get(s, 1, 1) +
                       src->point->y * tgt->point->y -
                       matrix_get(centroid_prod, 1, 1));

        matrix_set(s, 1, 2,
                   matrix_get(s, 1, 2) +
                       src->point->y * tgt->point->z -
                       matrix_get(centroid_prod, 1, 2));

        matrix_set(s, 2, 0,
                   matrix_get(s, 2, 0) +
                       src->point->z * tgt->point->x -
                       matrix_get(centroid_prod, 2, 0));

        matrix_set(s, 2, 1,
                   matrix_get(s, 2, 1) +
                       src->point->z * tgt->point->y -
                       matrix_get(centroid_prod, 2, 1));

        matrix_set(s, 2, 2,
                   matrix_get(s, 2, 2) +
                       src->point->z * tgt->point->z -
                       matrix_get(centroid_prod, 2, 2));

        src = src->next;
        tgt = tgt->next;
    } while (src != source->points && tgt != target->points);

    s = algebra_mat_vs_scalar(s, 1.0 / ((double)source->numpts));

    struct matrix *a = algebra_mat_sub(s, algebra_mat_transpose(s));
    struct matrix *q = matrix_new(4, 4);

    real tr_s = algebra_mat_trace(s);

    matrix_set(q, 0, 0, tr_s);
    matrix_set(q, 0, 1, matrix_get(a, 1, 2));
    matrix_set(q, 0, 2, matrix_get(a, 2, 0));
    matrix_set(q, 0, 3, matrix_get(a, 0, 1));
    matrix_set(q, 1, 0, matrix_get(a, 1, 2));
    matrix_set(q, 2, 0, matrix_get(a, 2, 0));
    matrix_set(q, 3, 0, matrix_get(a, 0, 1));

    matrix_set(q, 1, 1, matrix_get(s, 0, 0) * 2.0 - tr_s);
    matrix_set(q, 2, 2, matrix_get(s, 1, 1) * 2.0 - tr_s);
    matrix_set(q, 3, 3, matrix_get(s, 2, 2) * 2.0 - tr_s);

    matrix_set(q, 1, 2, matrix_get(s, 0, 1) + matrix_get(s, 1, 0));
    matrix_set(q, 1, 3, matrix_get(s, 0, 2) + matrix_get(s, 2, 0));
    matrix_set(q, 2, 3, matrix_get(s, 1, 2) + matrix_get(s, 2, 1));

    matrix_set(q, 2, 1, matrix_get(q, 1, 2));
    matrix_set(q, 3, 1, matrix_get(q, 1, 3));
    matrix_set(q, 3, 2, matrix_get(q, 2, 3));

    struct matrix *eig_val = algebra_mat_eigen_val(q);
    struct matrix *eig_vec = algebra_mat_eigen_vec(q, eig_val);

    uint max_eig_idx = 0;
    real eig_aux = creal(matrix_get(eig_val, 0, 0));
    for (uint i = 0; i < eig_val->rows; i++)
    {
        if (eig_aux < creal(matrix_get(eig_val, i, 0)))
        {
            eig_aux = creal(matrix_get(eig_val, i, 0));
            max_eig_idx = i;
        }
    }

    struct matrix *r = matrix_new(3, 3);

    real q0 = creal(matrix_get(eig_vec, 0, max_eig_idx));
    real q1 = creal(matrix_get(eig_vec, 1, max_eig_idx));
    real q2 = creal(matrix_get(eig_vec, 2, max_eig_idx));
    real q3 = creal(matrix_get(eig_vec, 3, max_eig_idx));

    real mag = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);

    q0 /= mag;
    q1 /= mag;
    q2 /= mag;
    q3 /= mag;

    matrix_set(r, 0, 0, q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);
    matrix_set(r, 0, 1, 2.0 * (q1 * q2 - q0 * q3));
    matrix_set(r, 0, 2, 2.0 * (q1 * q3 + q0 * q2));
    matrix_set(r, 1, 0, 2.0 * (q1 * q2 + q0 * q3));
    matrix_set(r, 1, 1, q0 * q0 + q2 * q2 - q1 * q1 - q3 * q3);
    matrix_set(r, 1, 2, 2.0 * (q2 * q3 - q0 * q1));
    matrix_set(r, 2, 0, 2.0 * (q1 * q3 - q0 * q2));
    matrix_set(r, 2, 1, 2.0 * (q2 * q3 + q0 * q1));
    matrix_set(r, 2, 2, q0 * q0 + q3 * q3 - q1 * q1 - q2 * q2);

    matrix_free(&target_aux);
    target_aux = matrix_new(3, 1);
    matrix_set(target_aux, 0, 0, target->centroid->x);
    matrix_set(target_aux, 1, 0, target->centroid->y);
    matrix_set(target_aux, 2, 0, target->centroid->z);

    struct matrix *t = algebra_mat_sub(target_aux,
                                       algebra_mat_prod(r, source_aux));
    struct matrix *rt = matrix_new(4, 4);

    matrix_set(rt, 0, 0, creal(matrix_get(r, 0, 0)));
    matrix_set(rt, 0, 1, creal(matrix_get(r, 0, 1)));
    matrix_set(rt, 0, 2, creal(matrix_get(r, 0, 2)));
    matrix_set(rt, 1, 0, creal(matrix_get(r, 1, 0)));
    matrix_set(rt, 1, 1, creal(matrix_get(r, 1, 1)));
    matrix_set(rt, 1, 2, creal(matrix_get(r, 1, 2)));
    matrix_set(rt, 2, 0, creal(matrix_get(r, 2, 0)));
    matrix_set(rt, 2, 1, creal(matrix_get(r, 2, 1)));
    matrix_set(rt, 2, 2, creal(matrix_get(r, 2, 2)));

    matrix_set(rt, 0, 3, creal(matrix_get(t, 0, 0)));
    matrix_set(rt, 1, 3, creal(matrix_get(t, 1, 0)));
    matrix_set(rt, 2, 3, creal(matrix_get(t, 2, 0)));

    matrix_set(rt, 3, 0, 0.0);
    matrix_set(rt, 3, 1, 0.0);
    matrix_set(rt, 3, 2, 0.0);
    matrix_set(rt, 3, 3, 1.0);

    matrix_free(&t);
    matrix_free(&target_aux);
    matrix_free(&source_aux);
    matrix_free(&r);
    matrix_free(&eig_val);
    matrix_free(&eig_vec);
    matrix_free(&q);
    matrix_free(&a);
    matrix_free(&s);
    matrix_free(&centroid_prod);

    if (!matrix_is_valid(rt))
    {
        return NULL;
    }

    return rt;
}

struct matrix *registration_icp(struct cloud *source,
                                struct cloud *target,
                                struct cloud **aligned,
                                real th,
                                uint k,
                                real max_dist,
                                closest_points_func closest)
{
    real cur_rmse;
    real last_rmse = INFINITY;
    real rmse_diff = INFINITY;
    char conv = 0;
    uint i = 0;
    struct cloud *src;
    struct cloud *tgt;
    struct matrix *rt_final = matrix_new(4, 4);
    matrix_set(rt_final, 0, 0, 1.0);
    matrix_set(rt_final, 1, 1, 1.0);
    matrix_set(rt_final, 2, 2, 1.0);
    matrix_set(rt_final, 3, 3, 1.0);

    *aligned = cloud_copy(source);

    do
    {
        if (cloud_size(*aligned) < cloud_size(target))
        {
            src = cloud_copy(*aligned);
            tgt = closest(src, target);
        }
        else
        {
            tgt = cloud_copy(target);
            src = closest(tgt, *aligned);
        }

        struct pointset *src_ps = src->points;
        struct pointset *tgt_ps = tgt->points;

        struct cloud *nsrc = cloud_new();
        struct cloud *ntgt = cloud_new();

        for (uint j = 0; j < cloud_size(src); j++)
        {
            real aux_dist = vector3_distance(src_ps->point, tgt_ps->point);
            if (aux_dist <= max_dist)
            {
                cloud_insert_vector3(nsrc, src_ps->point);
                cloud_insert_vector3(ntgt, tgt_ps->point);
            }

            src_ps = src_ps->next;
            tgt_ps = tgt_ps->next;
        }

        cloud_free(&src);
        cloud_free(&tgt);

        if ((4 > cloud_size(nsrc)) ||
            (4 > cloud_size(ntgt)))
        {
            cloud_free(&nsrc);
            cloud_free(&ntgt);
            cloud_free(aligned);
            printf("!!! no enough correspondence pairs !!!\n");
            printf("!!! try to use looser max_dist parameter !!!\n");
            return NULL;
        }

        src = nsrc;
        tgt = ntgt;

        cur_rmse = cloud_rmse_sorted(src, tgt);
        printf("LAST RMSE: %lf, RMSE: %lf\n", last_rmse, cur_rmse);
        if (i == 0)
        {
            if (i == k)
            {
                break;
            }
            else
            {
                i++;
            }
        }
        else
        {
            rmse_diff = last_rmse - cur_rmse;
            printf("DELTA: %lf\n", rmse_diff);
            if (rmse_diff <= th)
            {
                conv = 1;
            }

            if (conv || i == k || rmse_diff <= 0.0)
            {
                break;
            }
            else
            {
                i++;
            }
        }

        last_rmse = cur_rmse;

        struct matrix *rt = registration_align(src, tgt);
        if (rt == NULL)
        {
            break;
        }

        struct matrix *aux = algebra_mat_prod(rt, rt_final);
        matrix_free(&rt_final);
        rt_final = aux;

        cloud_transform(*aligned, rt);

        matrix_free(&rt);
    } while (1);

    if (src != NULL)
    {
        cloud_free(&src);
    }

    if (tgt != NULL)
    {
        cloud_free(&tgt);
    }

    printf("ICP done:\n\tk=%d\n\trmse=%lf\n\tdelta=%lf\n\tconverged=%s\n",
           i,
           cur_rmse,
           rmse_diff,
           conv ? "true" : "false");

    return rt_final;
}
