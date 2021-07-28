#include "../../include/module/registration_wrappers.h"

// Registration icp methods

static void RegistrationIcpExecute(napi_env env, void *data)
{
  RegistrationIcpData *addon_data = (RegistrationIcpData *)data;

  if (addon_data->err_info != NULL)
  {
    return;
  }

  // if (strcmp(addon_data->closest, "tree") == 0) {
  //   addon_data->tm = registration_icp(
  //     addon_data->source,
  //     addon_data->target,
  //     &(addon_data->aligned),
  //     addon_data->th,
  //     addon_data->k,
  //     addon_data->max_dist,
  //     cloud_closest_points_tree);
  // }
  // else {
  addon_data->tm = registration_icp(
      addon_data->source,
      addon_data->target,
      &(addon_data->aligned),
      addon_data->th,
      addon_data->k,
      addon_data->max_dist,
      cloud_closest_points_bf);
  // }

  if (addon_data->tm == NULL || addon_data->aligned == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpExecute: Error performing alignment!");
  }
}

static void RegistrationIcpComplete(napi_env env, napi_status status, void *data)
{
  napi_value tm, aligned;
  RegistrationIcpData *addon_data = (RegistrationIcpData *)data;

  if (status != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpComplete: Asynchronous execution failure!");
  }

  if (addon_data->aligned != NULL && pontu_cloud_to_napi_object(env, addon_data->aligned, &aligned) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpComplete: Failed to convert point cloud!");
  }

  if (addon_data->tm != NULL && pontu_matrix_to_napi_array(env, addon_data->tm, &tm) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpComplete: Failed to convert transformation matrix!");
  }

  if (addon_data->tm == NULL || addon_data->aligned == NULL || addon_data->err_info != NULL)
  {
    napi_value err_info;

    if (napi_create_string_utf8(env,
                                addon_data->err_info,
                                NAPI_AUTO_LENGTH,
                                &(err_info)) != napi_ok)
    {
      create_fatal_error("RegistrationIcpComplete", "Failed to create error name!");
    }

    if (napi_reject_deferred(env, addon_data->deferred, err_info) != napi_ok)
    {
      create_fatal_error("RegistrationIcpComplete", "Failed to reject promise!");
    }
  }
  else
  {
    napi_value icp_result;

    if (napi_create_object(env, &icp_result) != napi_ok)
    {
      create_fatal_error("RegistrationIcpComplete", "Failed to create result object!");
    }

    if (napi_set_named_property(env, icp_result, "algnCloud", aligned) != napi_ok)
    {
      create_fatal_error("RegistrationIcpComplete", "Failed to set aligned cloud property!");
    }

    if (napi_set_named_property(env, icp_result, "tm", tm) != napi_ok)
    {
      create_fatal_error("RegistrationIcpComplete", "Failed to set tm property!");
    }

    if (napi_resolve_deferred(env, addon_data->deferred, icp_result) != napi_ok)
    {
      create_fatal_error("RegistrationIcpComplete", "Failed to resolve promise!");
    }
  }

  if (napi_delete_async_work(env, addon_data->work) != napi_ok)
  {
    create_fatal_error("RegistrationIcpComplete", "Failed to delete async work!");
  }
}

static RegistrationIcpData *NewRegistrationIcpData(void)
{
  RegistrationIcpData *addon_data = (RegistrationIcpData *)malloc(sizeof(*addon_data));
  if (addon_data == NULL)
  {
    return NULL;
  }
  addon_data->err_info = NULL;
  addon_data->work = NULL;
  addon_data->deferred = NULL;
  addon_data->source = NULL;
  addon_data->target = NULL;
  addon_data->th = 0;
  addon_data->k = 0;
  addon_data->max_dist = 0;
  addon_data->closest = NULL;
  addon_data->aligned = NULL;
  addon_data->tm = NULL;

  return addon_data;
}

static void RegistrationIcpFillData(napi_env env, napi_callback_info info, RegistrationIcpData *addon_data)
{
  size_t argc = 6;
  napi_value *args = (napi_value *)malloc(sizeof(napi_value) * argc);
  if (args == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Failed to allocate memory for input arguments!");
    return;
  }

  if (napi_get_cb_info(env, info, &argc, args, NULL, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Failed to parse input arguments!");
    free(args);
    return;
  }

  if (!check_type(env, args[0], napi_object))
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid value was passed as first argument!");
    free(args);
    return;
  }
  if (!check_type(env, args[1], napi_object))
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid value was passed as second argument!");
    free(args);
    return;
  }
  if (!check_type(env, args[2], napi_number))
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid value was passed as third argument!");
    free(args);
    return;
  }
  if (!check_type(env, args[3], napi_number))
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid value was passed as fourth argument!");
    free(args);
    return;
  }
  if (!check_type(env, args[4], napi_number))
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid value was passed as fifth argument!");
    free(args);
    return;
  }
  if (!check_type(env, args[5], napi_string))
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid value was passed as sixth argument!");
    free(args);
    return;
  }

  if (napi_object_to_pontu_cloud(env, args[0], &(addon_data->source)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Failed to get first argument!");
    free(args);
    return;
  }

  if (napi_object_to_pontu_cloud(env, args[1], &(addon_data->target)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Failed to get second argument!");
    free(args);
    return;
  }

  if (napi_get_value_double(env, args[2], &(addon_data->th)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid number was passed as third argument!");
    free(args);
    return;
  }

  if (napi_get_value_uint32(env, args[3], &(addon_data->k)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid number was passed as fourth argument!");
    free(args);
    return;
  }

  if (napi_get_value_double(env, args[4], &(addon_data->max_dist)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid number was passed as fifth argument!");
    free(args);
    return;
  }

  size_t str_size;
  if (napi_get_value_string_utf8(env, args[5], NULL, 0, &str_size) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Failed to get sixth argument size!");
    free(args);
    return;
  }

  str_size = str_size + 1;
  addon_data->closest = (char *)malloc(sizeof(char) * str_size);
  if (args == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Failed to allocate memory for sixth argument!");
    free(args);
    return;
  }

  if (napi_get_value_string_utf8(env, args[5], addon_data->closest, str_size, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "RegistrationIcpFillData: Invalid string was passed as sixth argument!");
    free(args);
    return;
  }

  free(args);
}

static void FreeRegistrationIcpData(RegistrationIcpData **data)
{
  if (*data == NULL)
  {
    return;
  }

  if ((*data)->err_info != NULL)
  {
    free((*data)->err_info);
  }

  (*data)->work = NULL;
  (*data)->deferred = NULL;

  cloud_free(&((*data)->source));
  cloud_free(&((*data)->target));
  cloud_free(&((*data)->aligned));
  matrix_free(&((*data)->tm));

  if ((*data)->closest != NULL)
  {
    free((*data)->closest);
  }

  free(*data);
  *data = NULL;
}

static void RegistrationIcpPromiseUnloaded(napi_env env, void *data, void *hint)
{
  RegistrationIcpData *addon_data = (RegistrationIcpData *)data;
  FreeRegistrationIcpData(&addon_data);
}

napi_value RegistrationIcpPromise(napi_env env, napi_callback_info info)
{
  RegistrationIcpData *addon_data = NewRegistrationIcpData();
  if (addon_data == NULL)
  {
    create_fatal_error("RegistrationIcpPromise", "Failed to allocate memory for addon data!");
  }
  RegistrationIcpFillData(env, info, addon_data);

  napi_value promise = create_promise(env,
                                      info,
                                      &(addon_data->deferred),
                                      &(addon_data->work),
                                      addon_data,
                                      RegistrationIcpExecute,
                                      RegistrationIcpComplete,
                                      "Node-API Deferred Promise - registration_icp");
  if (napi_wrap(env,
                promise,
                addon_data,
                RegistrationIcpPromiseUnloaded,
                NULL,
                NULL) != napi_ok)
  {
    create_fatal_error("RegistrationIcpPromise", "Failed to associate data to Promise!");
  }

  return promise;
}

napi_value RegistrationIcpSync(napi_env env, napi_callback_info info)
{
  RegistrationIcpData *addon_data = NewRegistrationIcpData();
  if (addon_data == NULL)
  {
    create_fatal_error("RegistrationIcpSync", "Failed to allocate memory for addon data!");
  }
  RegistrationIcpFillData(env, info, addon_data);
  RegistrationIcpExecute(env, addon_data);

  napi_value rtn, rtn_cloud, rtn_tm;
  if (addon_data->tm == NULL ||
      addon_data->aligned == NULL ||
      addon_data->err_info != NULL ||
      pontu_cloud_to_napi_object(env, addon_data->aligned, &rtn_cloud) != napi_ok ||
      pontu_matrix_to_napi_array(env, addon_data->tm, &rtn_tm) != napi_ok ||
      napi_create_object(env, &rtn) != napi_ok ||
      napi_set_named_property(env, rtn, "algnCloud", rtn_cloud) != napi_ok ||
      napi_set_named_property(env, rtn, "tm", rtn_tm) != napi_ok)
  {
    if (napi_get_null(env, &rtn) != napi_ok)
    {
      create_fatal_error("RegistrationIcpSync", "Failed get null value!");
    }
  }

  FreeRegistrationIcpData(&addon_data);

  return rtn;
}
