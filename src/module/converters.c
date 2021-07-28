#include "../../include/module/converters.h"

napi_status napi_object_to_pontu_vector3(napi_env env, napi_value point, struct vector3 **new_vector3)
{
  napi_value x, y, z;
  real x_real, y_real, z_real;

  if (napi_get_named_property(env, point, "x", &x) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_value_double(env, x, &x_real) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_named_property(env, point, "y", &y) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_value_double(env, y, &y_real) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_named_property(env, point, "z", &z) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_value_double(env, z, &z_real) != napi_ok)
  {
    return napi_generic_failure;
  }

  *new_vector3 = vector3_new(x_real, y_real, z_real);
  if (*new_vector3 == NULL)
  {
    return napi_generic_failure;
  }

  return napi_ok;
}

napi_status pontu_vector3_to_napi_object(napi_env env, struct vector3 *point, napi_value *point_obj)
{
  if (point == NULL)
  {
    return napi_generic_failure;
  }

  napi_value x, y, z;

  if (napi_create_object(env, point_obj) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_create_double(env, point->x, &x) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_set_named_property(env, *point_obj, "x", x) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_create_double(env, point->y, &y) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_set_named_property(env, *point_obj, "y", y) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_create_double(env, point->z, &z) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_set_named_property(env, *point_obj, "z", z) != napi_ok)
  {
    return napi_generic_failure;
  }

  return napi_ok;
}

napi_status napi_object_to_pontu_cloud(napi_env env, napi_value cloud, struct cloud **new_cloud)
{
  if (cloud == NULL)
  {
    return napi_generic_failure;
  }

  *new_cloud = cloud_new();

  napi_value points;
  uint32_t points_len;

  if (napi_get_named_property(env, cloud, "points", &points) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_array_length(env, points, &points_len) != napi_ok)
  {
    return napi_generic_failure;
  }

  for (uint32_t i = 0; i < points_len; i++)
  {
    napi_value point;

    if (napi_get_element(env, points, i, &point) != napi_ok)
    {
      return napi_generic_failure;
    }

    struct vector3 *new_vector3 = NULL;
    if (napi_object_to_pontu_vector3(env, point, &new_vector3) != napi_ok)
    {
      vector3_free(&new_vector3);
      return napi_generic_failure;
    }

    cloud_insert_vector3(*new_cloud, new_vector3);
    vector3_free(&new_vector3);
  }

  return napi_ok;
}

napi_status pontu_cloud_to_napi_object(napi_env env, struct cloud *cloud, napi_value *cloud_obj)
{
  if (cloud == NULL)
  {
    return napi_generic_failure;
  }

  napi_value num_pts, points;

  if (napi_create_object(env, cloud_obj) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_create_int32(env, (int)cloud->numpts, &num_pts) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_set_named_property(env, *cloud_obj, "numpts", num_pts) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (pontu_pointset_to_napi_array(env, cloud->points, &points) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_set_named_property(env, *cloud_obj, "points", points) != napi_ok)
  {
    return napi_generic_failure;
  }

  return napi_ok;
}

napi_status pontu_pointset_to_napi_array(napi_env env, struct pointset *point_set, napi_value *points)
{
  if (point_set == NULL)
  {
    return napi_generic_failure;
  }

  uint numpts = pointset_size(point_set);
  uint32_t i = 0;

  if (napi_create_array_with_length(env, numpts, points) != napi_ok)
  {
    return napi_generic_failure;
  }

  struct pointset *s = point_set;
  if (s)
  {
    do
    {
      napi_value point_obj;
      if (pontu_vector3_to_napi_object(env, s->point, &point_obj) != napi_ok)
      {
        return napi_generic_failure;
      }

      if (napi_set_element(env, *points, i++, point_obj) != napi_ok)
      {
        return napi_generic_failure;
      }
      s = s->next;
    } while (s != NULL && s != point_set);
  }
  return napi_ok;
}

napi_status napi_array_to_pontu_matrix(napi_env env, napi_value rt, struct matrix **rt_mat)
{
  uint32_t i_len;

  if (napi_get_array_length(env, rt, &i_len) != napi_ok)
  {
    return napi_generic_failure;
  }

  for (uint32_t i = 0; i < i_len; i++)
  {
    napi_value row;
    uint32_t j_len;

    if (napi_get_element(env, rt, i, &row) != napi_ok)
    {
      return napi_generic_failure;
    }

    if (napi_get_array_length(env, row, &j_len) != napi_ok)
    {
      return napi_generic_failure;
    }

    if (*rt_mat == NULL)
    {
      *rt_mat = matrix_new(1, j_len);
      if (*rt_mat == NULL)
      {
        return napi_generic_failure;
      }
    }
    else if (matrix_add_row(*rt_mat) == 0)
    {
      return napi_generic_failure;
    }

    for (uint32_t j = 0; j < j_len; j++)
    {
      napi_value num;
      cnum element = 0;

      if (napi_get_element(env, row, j, &num) != napi_ok)
      {
        return napi_generic_failure;
      }

      if (napi_object_to_pontu_cnum(env, num, &element) != napi_ok)
      {
        return napi_generic_failure;
      }

      matrix_set(*rt_mat, i, j, element);
    }
  }

  return napi_ok;
}

napi_status pontu_matrix_to_napi_array(napi_env env, struct matrix *mat, napi_value *m_arr)
{
  if (mat == NULL)
  {
    return napi_generic_failure;
  }

  if (napi_create_array_with_length(env, mat->rows, m_arr) != napi_ok)
  {
    return napi_generic_failure;
  }

  for (uint32_t i = 0; i < mat->rows; i++)
  {
    napi_value n_arr;
    if (napi_create_array_with_length(env, mat->rows, &n_arr) != napi_ok)
    {
      return napi_generic_failure;
    }

    for (uint32_t j = 0; j < mat->cols; j++)
    {
      napi_value element;
      if (pontu_cnum_to_napi_object(env, matrix_get(mat, i, j), &element) != napi_ok)
      {
        return napi_generic_failure;
      }

      if (napi_set_element(env, n_arr, j, element) != napi_ok)
      {
        return napi_generic_failure;
      }
    }

    if (napi_set_element(env, *m_arr, i, n_arr) != napi_ok)
    {
      return napi_generic_failure;
    }
  }

  return napi_ok;
}

napi_status napi_object_to_pontu_cnum(napi_env env, napi_value num, cnum *ncpx)
{
  napi_value re_nv, im_nv;
  double re, im;

  if (napi_get_named_property(env, num, "re", &re_nv) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_value_double(env, re_nv, &re) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_named_property(env, num, "im", &im_nv) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_get_value_double(env, im_nv, &im) != napi_ok)
  {
    return napi_generic_failure;
  }

  *ncpx = re + im * I;
  return napi_ok;
}

napi_status pontu_cnum_to_napi_object(napi_env env, cnum num, napi_value *num_obj)
{
  napi_value re, im;

  if (napi_create_object(env, num_obj) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_create_double(env, creal(num), &re) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_create_double(env, cimag(num), &im) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_set_named_property(env, *num_obj, "re", re) != napi_ok)
  {
    return napi_generic_failure;
  }

  if (napi_set_named_property(env, *num_obj, "im", im) != napi_ok)
  {
    return napi_generic_failure;
  }

  return napi_ok;
}
