#include "../../include/module/registration_sync_wrappers.h"

napi_value RegistrationIcpSync(napi_env env, napi_callback_info info)
{
  napi_status status;
  size_t argc = 6;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_object);
  throw_type_error(env, args[1], napi_object);
  throw_type_error(env, args[2], napi_number);
  throw_type_error(env, args[3], napi_number);
  throw_type_error(env, args[4], napi_number);
  throw_type_error(env, args[5], napi_string);

  struct cloud *source = napi_object_to_pontu_cloud(env, args[0]);
  struct cloud *target = napi_object_to_pontu_cloud(env, args[1]);

  real th;
  status = napi_get_value_double(env, args[2], &th);
  check_status(env, status, "Invalid number was passed as third argument!");

  uint k;
  status = napi_get_value_uint32(env, args[3], &k);
  check_status(env, status, "Invalid number was passed as fourth argument!");

  real max_dist;
  status = napi_get_value_double(env, args[4], &max_dist);
  check_status(env, status, "Invalid number was passed as fiveth argument!");

  size_t str_size;
  status = napi_get_value_string_utf8(env, args[5], NULL, 0, &str_size);
  check_status(env, status, "Failed to get fiveth argument size!");
  str_size = str_size + 1;
  char *closest = (char *)malloc(sizeof(char) * str_size);
  status = napi_get_value_string_utf8(env, args[5], closest, str_size, NULL);
  check_status(env, status, "Invalid string was passed as sixth argument!");

  struct matrix *tm;
  struct cloud *aligned;
  // if (strcmp(addon_data->closest, "tree") == 0) {
  //   tm = registration_icp(
  //     addon_data->source,
  //     addon_data->target,
  //     &(addon_data->aligned),
  //     addon_data->th,
  //     addon_data->k,
  //     addon_data->max_dist,
  //     cloud_closest_points_tree);
  // }
  // else {
  tm = registration_icp(
      source,
      target,
      &aligned,
      th,
      k,
      max_dist,
      cloud_closest_points_bf);
  // }

  napi_value rtn;
  if (tm == NULL)
  {
    status = napi_get_null(env, &rtn);
    check_status(env, status, "Failed get null value!");
  }
  else
  {
    napi_value rtn_cloud = pontu_cloud_to_napi_object(env, aligned);
    napi_value rtn_tm = pontu_matrix_to_napi_array(env, tm);

    status = napi_create_object(env, &rtn);
    check_status(env, status, "Failed to create result object!");

    status = napi_set_named_property(env, rtn, "algnCloud", rtn_cloud);
    check_status(env, status, "Failed to set aligned_cloud property!");

    status = napi_set_named_property(env, rtn, "tm", rtn_tm);
    check_status(env, status, "Failed to set tm property!");
  }

  cloud_free(&source);
  cloud_free(&target);
  cloud_free(&aligned);
  matrix_free(&tm);
  free(closest);

  return rtn;
}