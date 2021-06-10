#include "../../include/module/converters.h"

struct vector3 *napi_object_to_pontu_vector3(napi_env env, napi_value point)
{
  napi_status status;
  napi_value x, y, z;
  real x_real, y_real, z_real;

  status = napi_get_named_property(env, point, "x", &x);
  check_status(env, status, "Failed to load x point!");

  status = napi_get_value_double(env, x, &x_real);
  check_status(env, status, "Failed to convert x to real!");

  status = napi_get_named_property(env, point, "y", &y);
  check_status(env, status, "Failed to load y point!");

  status = napi_get_value_double(env, y, &y_real);
  check_status(env, status, "Failed to convert y to real!");

  status = napi_get_named_property(env, point, "z", &z);
  check_status(env, status, "Failed to load z point!");

  status = napi_get_value_double(env, z, &z_real);
  check_status(env, status, "Failed to convert z to real!");

  return vector3_new(x_real, y_real, z_real);
}

napi_value pontu_vector3_to_napi_object(napi_env env, struct vector3 *point)
{
  napi_status status;
  napi_value point_obj, x, y, z;

  status = napi_create_object(env, &point_obj);
  check_status(env, status, "Failed to create vector3 object!");

  status = napi_create_double(env, point->x, &x);
  check_status(env, status, "Failed to convert x to double!");

  status = napi_set_named_property(env, point_obj, "x", x);
  check_status(env, status, "Failed to set property x!");

  status = napi_create_double(env, point->y, &y);
  check_status(env, status, "Failed to convert y to double!");

  status = napi_set_named_property(env, point_obj, "y", y);
  check_status(env, status, "Failed to set property y!");

  status = napi_create_double(env, point->z, &z);
  check_status(env, status, "Failed to convert z to double!");

  status = napi_set_named_property(env, point_obj, "z", z);
  check_status(env, status, "Failed to set property z!");

  return point_obj;
}

struct cloud *napi_object_to_pontu_cloud(napi_env env, napi_value cloud)
{
  struct cloud *new_cloud = cloud_new();

  napi_status status;
  napi_value points;
  uint32_t points_len;

  status = napi_get_named_property(env, cloud, "points", &points);
  check_status(env, status, "Failed to load points!");

  status = napi_get_array_length(env, points, &points_len);
  check_status(env, status, "Failed to load points array length!");

  for (uint32_t i = 0; i < points_len; i++)
  {
    napi_value point;

    status = napi_get_element(env, points, i, &point);
    check_status(env, status, "Failed to load a point from the cloud!");

    cloud_insert_vector3(new_cloud, napi_object_to_pontu_vector3(env, point));
  }

  return new_cloud;
}

napi_value pontu_cloud_to_napi_object(napi_env env, struct cloud *cloud)
{
  napi_status status;
  napi_value cloud_obj, num_pts;

  status = napi_create_object(env, &cloud_obj);
  check_status(env, status, "Failed to create cloud object!");

  status = napi_create_int32(env, cloud->numpts, &num_pts);
  check_status(env, status, "Failed to create numpts property!");

  status = napi_set_named_property(env, cloud_obj, "numpts", num_pts);
  check_status(env, status, "Failed to set numpts property!");

  status = napi_set_named_property(env, cloud_obj, "points", pontu_pointset_to_napi_array(env, cloud->points));
  check_status(env, status, "Failed to set points property!");

  return cloud_obj;
}

napi_value pontu_pointset_to_napi_array(napi_env env, struct pointset *point_set)
{
  napi_status status;
  napi_value points;
  uint numpts = pointset_size(point_set);
  uint32_t i = 0;

  status = napi_create_array_with_length(env, numpts, &points);
  check_status(env, status, "Failed to create points array!");

  struct pointset *s = point_set;
  if (s)
  {
    do
    {
      status = napi_set_element(env, points, i++, pontu_vector3_to_napi_object(env, s->point));
      check_status(env, status, "Failed to set point object!");
      s = s->next;
    } while (s != NULL && s != point_set);
  }
  return points;
}

struct matrix *napi_array_to_pontu_matrix(napi_env env, napi_value rt)
{
  napi_status status;
  uint32_t i_len;
  struct matrix *rt_mat = NULL;

  status = napi_get_array_length(env, rt, &i_len);
  check_status(env, status, "Failed to get matrix row length!");

  for (uint32_t i = 0; i < i_len; i++)
  {
    napi_value row;
    uint32_t j_len;

    status = napi_get_element(env, rt, i, &row);
    check_status(env, status, "Failed to load a row from the matrix!");

    status = napi_get_array_length(env, row, &j_len);
    check_status(env, status, "Failed to get matrix column length!");

    if (rt_mat == NULL)
    {
      rt_mat = matrix_new(1, j_len);
    }
    else if (matrix_add_row(rt_mat) == 0)
    {
      napi_throw_error(env, NULL, "Failed to add row to matrix");
    }

    for (uint32_t j = 0; j < j_len; j++)
    {
      napi_value num;

      status = napi_get_element(env, row, j, &num);
      check_status(env, status, "Failed to get the element from the array!");

      matrix_set(rt_mat, i, j, napi_object_to_pontu_cnum(env, num));
    }
  }

  return rt_mat;
}

napi_value pontu_matrix_to_napi_array(napi_env env, struct matrix *mat)
{
  napi_status status;
  napi_value m_arr;

  status = napi_create_array_with_length(env, mat->rows, &m_arr);
  check_status(env, status, "Failed to create matrix m array!");

  for (uint32_t i = 0; i < mat->rows; i++)
  {
    napi_value n_arr;
    status = napi_create_array_with_length(env, mat->rows, &n_arr);
    check_status(env, status, "Failed to create matrix n array!");

    for (uint32_t j = 0; j < mat->cols; j++)
    {
      status = napi_set_element(env, n_arr, j, pontu_cnum_to_napi_object(env, matrix_get(mat, i, j)));
      check_status(env, status, "Failed to set cnum object!");
    }

    status = napi_set_element(env, m_arr, i, n_arr);
    check_status(env, status, "Failed to set row array!");
  }

  return m_arr;
}

cnum napi_object_to_pontu_cnum(napi_env env, napi_value num)
{
  napi_status status;
  napi_value re_nv, im_nv;
  double re, im;

  status = napi_get_named_property(env, num, "re", &re_nv);
  check_status(env, status, "Failed to load real value!");

  status = napi_get_value_double(env, re_nv, &re);
  check_status(env, status, "Failed to convert re to double!");

  status = napi_get_named_property(env, num, "im", &im_nv);
  check_status(env, status, "Failed to load imaginary value!");

  status = napi_get_value_double(env, im_nv, &im);
  check_status(env, status, "Failed to convert im to double!");

  return re + im * I;
}

napi_value pontu_cnum_to_napi_object(napi_env env, cnum num)
{
  napi_status status;
  napi_value num_obj, re, im;

  status = napi_create_object(env, &num_obj);
  check_status(env, status, "Failed to create cnum object!");

  status = napi_create_double(env, creal(num), &re);
  check_status(env, status, "Failed to convert the real part of the complex number!");

  status = napi_create_double(env, cimag(num), &im);
  check_status(env, status, "Failed to convert the imaginary part of the complex number!");

  status = napi_set_named_property(env, num_obj, "re", re);
  check_status(env, status, "Failed to set re property!");

  status = napi_set_named_property(env, num_obj, "im", im);
  check_status(env, status, "Failed to set im property!");

  return num_obj;
}
