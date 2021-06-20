#include "../../include/module/cloud_sync_wrappers.h"

napi_value CloudSaveSync(napi_env env, napi_callback_info info)
{
  napi_status status;
  size_t argc = 2;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_object);
  throw_type_error(env, args[1], napi_string);

  struct cloud *cloud = napi_object_to_pontu_cloud(env, args[0]);

  size_t str_size;
  status = napi_get_value_string_utf8(env, args[1], NULL, 0, &str_size);
  check_status(env, status, "Failed to get filename size!");
  str_size = str_size + 1;
  char *filename = (char *)malloc(sizeof(char) * str_size);
  status = napi_get_value_string_utf8(env, args[1], filename, str_size, NULL);
  check_status(env, status, "Failed to get filename!");

  const char *ext = file_ext(filename);

  int result = 0;
  if (strcmp(ext, "csv") == 0)
  {
    result = cloud_save_csv(cloud, filename);
  }
  else if (strcmp(ext, "pcd") == 0)
  {
    result = cloud_save_pcd(cloud, filename);
  }
  else if (strcmp(ext, "ply") == 0)
  {
    result = cloud_save_ply(cloud, filename);
  }
  else if (strcmp(ext, "xyz") == 0)
  {
    result = cloud_save_xyz(cloud, filename);
  }

  napi_value rtn;
  if (result == 0)
  {
    status = napi_get_null(env, &rtn);
    check_status(env, status, "Failed get null value!");
  }
  else
  {
    rtn = pontu_cloud_to_napi_object(env, cloud);
  }

  return rtn;
}

napi_value CloudLoadSync(napi_env env, napi_callback_info info)
{
  napi_status status;

  size_t argc = 1;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_string);
  size_t str_size;
  status = napi_get_value_string_utf8(env, args[0], NULL, 0, &str_size);
  check_status(env, status, "Failed to get filename size!");
  str_size = str_size + 1;
  char *filename = (char *)malloc(sizeof(char) * str_size);
  status = napi_get_value_string_utf8(env, args[0], filename, str_size, NULL);
  check_status(env, status, "Failed to get filename!");

  const char *ext = file_ext(filename);
  struct cloud *cloud = NULL;

  if (strcmp(ext, "csv") == 0 || strcmp(ext, "CSV") == 0)
  {
    cloud = cloud_load_csv(filename);
  }
  else if (strcmp(ext, "json") == 0 || strcmp(ext, "JSON") == 0)
  {
    cloud = cloud_load_json(filename);
  }
  else if (strcmp(ext, "obj") == 0 || strcmp(ext, "OBJ") == 0)
  {
    cloud = cloud_load_obj(filename);
  }
  else if (strcmp(ext, "pcd") == 0 || strcmp(ext, "PCD") == 0)
  {
    cloud = cloud_load_pcd(filename);
  }
  else if (strcmp(ext, "ply") == 0 || strcmp(ext, "PLY") == 0)
  {
    cloud = cloud_load_ply(filename);
  }
  else if (strcmp(ext, "xyz") == 0 || strcmp(ext, "XYZ") == 0)
  {
    cloud = cloud_load_xyz(filename);
  }

  napi_value rtn;
  if (cloud == NULL)
  {
    status = napi_get_null(env, &rtn);
    check_status(env, status, "Failed get null value!");
  }
  else
  {
    rtn = pontu_cloud_to_napi_object(env, cloud);
  }

  return rtn;
}

napi_value CloudRmseSync(napi_env env, napi_callback_info info)
{
  napi_status status;

  size_t argc = 4;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_object);
  throw_type_error(env, args[1], napi_object);
  throw_type_error(env, args[2], napi_number);
  throw_type_error(env, args[3], napi_string);

  struct cloud *source = napi_object_to_pontu_cloud(env, args[0]);
  struct cloud *target = napi_object_to_pontu_cloud(env, args[1]);

  real max_dist;
  status = napi_get_value_double(env, args[2], &max_dist);
  check_status(env, status, "Invalid number was passed as thrid argument!");

  size_t str_size;
  status = napi_get_value_string_utf8(env, args[3], NULL, 0, &str_size);
  check_status(env, status, "Failed to get fiveth argument size!");
  str_size = str_size + 1;
  char *closest = (char *)malloc(sizeof(char) * str_size);
  status = napi_get_value_string_utf8(env, args[3], closest, str_size, NULL);
  check_status(env, status, "Invalid string was passed as fourth argument!");

  real rmse;
  // if (strcmp(closest, "tree") == 0) {
  //   rmse = cloud_rmse(
  //     addon_data->source,
  //     addon_data->target,
  //     addon_data->max_dist,
  //     cloud_closest_points_tree);
  // }
  // else {
  rmse = cloud_rmse(source, target, max_dist, cloud_closest_points_bf);
  // }

  napi_value rtn;
  status = napi_create_double(env, rmse, &rtn);
  check_status(env, status, "Failed to convert rmse value!");

  return rtn;
}

napi_value CloudTransformSync(napi_env env, napi_callback_info info)
{
  napi_status status;
  size_t argc = 2;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_object);
  throw_type_error(env, args[1], napi_object);

  struct cloud *cloud = napi_object_to_pontu_cloud(env, args[0]);
  struct matrix *rt = napi_array_to_pontu_matrix(env, args[1]);

  cloud_transform(cloud, rt);

  napi_value rtn;
  if (cloud == NULL)
  {
    status = napi_get_null(env, &rtn);
    check_status(env, status, "Failed get null value!");
  }
  else
  {
    rtn = pontu_cloud_to_napi_object(env, cloud);
  }

  return rtn;
}