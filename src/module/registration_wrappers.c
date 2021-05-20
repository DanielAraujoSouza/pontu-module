#include "../../include/module/registration_wrappers.h"

// Registration icp methods
static void RegistrationIcpExecute(napi_env env, void* data) {
  RegistrationIcpData *addon_data = (RegistrationIcpData*)data;
  
  if (strcmp(addon_data->closest, "tree") == 0) {
    addon_data->tm = registration_icp(
      addon_data->source,
      addon_data->target,
      &(addon_data->aligned),
      addon_data->th,
      addon_data->k,
      addon_data->max_dist,
      cloud_closest_points_tree);
  }
  else {
    addon_data->tm = registration_icp(
      addon_data->source,
      addon_data->target,
      &(addon_data->aligned),
      addon_data->th,
      addon_data->k,
      addon_data->max_dist,
      cloud_closest_points_bf);
  }
}

static void RegistrationIcpComplete(napi_env env, napi_status status, void* data) {
  if (status != napi_ok) {
    return;
  }

  RegistrationIcpData* addon_data = (RegistrationIcpData*)data;

  if (addon_data->tm == NULL) {
    napi_value undefined;
    status = napi_get_undefined(env, &undefined);
    check_status(env, status, "Failed get undefined value!");

    status = napi_reject_deferred(env, addon_data->deferred, undefined);
    check_status(env, status, "Failed to reject promise!");
  }
  else {
    napi_value cloud = pontu_cloud_to_napi_object(env, addon_data->aligned);
    napi_value tm = pontu_matrix_to_napi_array(env, addon_data->tm);
    
    napi_value icp_result;
    status = napi_create_object(env, &icp_result);
    check_status(env, status, "Failed to create result object!");

    status = napi_set_named_property(env, icp_result, "algnCloud", cloud);
    check_status(env, status, "Failed to set aligned_cloud property!");

    status = napi_set_named_property(env, icp_result, "tm", tm);
    check_status(env, status, "Failed to set tm property!");

    status = napi_resolve_deferred(env, addon_data->deferred, icp_result);
    check_status(env, status, "Failed to resolve promise!");

  }

  napi_delete_async_work(env, addon_data->work);
  check_status(env, status, "Failed to delete async work!");

  addon_data->work = NULL;
  addon_data->deferred = NULL;
  cloud_free(&(addon_data->source));
  cloud_free(&(addon_data->target));
  cloud_free(&(addon_data->aligned));
  matrix_free(&(addon_data->tm));
  free(addon_data->closest);
  free(addon_data);
}

napi_value RegistrationIcpPromise(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value work_name, promise;

  RegistrationIcpData *addon_data = (RegistrationIcpData*)malloc(sizeof(*addon_data));
  addon_data->work = NULL;

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

  addon_data->source = napi_object_to_pontu_cloud(env, args[0]);
  addon_data->target = napi_object_to_pontu_cloud(env, args[1]);

  status = napi_get_value_double(env, args[2], &(addon_data->th));
  check_status(env, status, "Invalid number was passed as third argument!");

  status = napi_get_value_uint32(env, args[3], &(addon_data->k));
  check_status(env, status, "Invalid number was passed as fourth argument!");

  status = napi_get_value_double(env, args[4], &(addon_data->max_dist));
  check_status(env, status, "Invalid number was passed as fiveth argument!");
  
  size_t str_size;
  status = napi_get_value_string_utf8(env, args[5], NULL, 0, &str_size);
  check_status(env, status, "Failed to get fiveth argument size!");
  str_size = str_size + 1;
  addon_data->closest = (char*)malloc(sizeof(char) * str_size);
  status = napi_get_value_string_utf8(env, args[5], addon_data->closest, str_size, NULL);
  check_status(env, status, "Invalid string was passed as sixth argument!");

  status = napi_create_promise(env, &(addon_data->deferred), &promise);
  check_status(env, status, "Failed to create promise!");

  status = napi_create_string_utf8(env, 
    "Node-API Deferred Promise - registration_icp", 
    NAPI_AUTO_LENGTH, 
    &work_name);
  check_status(env, status, "Failed to create work name!");

  status = napi_create_async_work(env,
    NULL,
    work_name,
    RegistrationIcpExecute,
    RegistrationIcpComplete,
    addon_data,
    &(addon_data->work));
  check_status(env, status, "Failed to create async work!");

  status = napi_queue_async_work(env, addon_data->work);
  check_status(env, status, "Failed to queue async work!");

  return promise;
}
