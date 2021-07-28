#include "../../include/module/cloud_wrappers.h"

// Cloud load methods
static void CloudLoadExecute(napi_env env, void *data)
{
  CloudLoadData *addon_data = (CloudLoadData *)data;

  if (addon_data->err_info != NULL)
  {
    return;
  }

  const char *ext = file_ext(addon_data->filename);

  if (strcmp(ext, "csv") == 0 || strcmp(ext, "CSV") == 0)
  {
    addon_data->cloud = cloud_load_csv(addon_data->filename);
  }
  else if (strcmp(ext, "json") == 0 || strcmp(ext, "JSON") == 0)
  {
    addon_data->cloud = cloud_load_json(addon_data->filename);
  }
  else if (strcmp(ext, "obj") == 0 || strcmp(ext, "OBJ") == 0)
  {
    addon_data->cloud = cloud_load_obj(addon_data->filename);
  }
  else if (strcmp(ext, "pcd") == 0 || strcmp(ext, "PCD") == 0)
  {
    addon_data->cloud = cloud_load_pcd(addon_data->filename);
  }
  else if (strcmp(ext, "ply") == 0 || strcmp(ext, "PLY") == 0)
  {
    addon_data->cloud = cloud_load_ply(addon_data->filename);
  }
  else if (strcmp(ext, "xyz") == 0 || strcmp(ext, "XYZ") == 0)
  {
    addon_data->cloud = cloud_load_xyz(addon_data->filename);
  }

  if (addon_data->cloud == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadExecute: No files or empty point cloud!");
  }
}

static void CloudLoadComplete(napi_env env, napi_status status, void *data)
{
  napi_value cloud;
  CloudLoadData *addon_data = (CloudLoadData *)data;

  if (status != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadComplete: Asynchronous execution failure!");
  }

  if (addon_data->cloud != NULL && pontu_cloud_to_napi_object(env, addon_data->cloud, &cloud) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadComplete: Failed to convert point cloud!");
  }

  if (addon_data->cloud == NULL || addon_data->err_info != NULL)
  {
    napi_value err_info;

    if (napi_create_string_utf8(env,
                                addon_data->err_info,
                                NAPI_AUTO_LENGTH,
                                &(err_info)) != napi_ok)
    {
      create_fatal_error("CloudLoadComplete", "Failed to create error name!");
    }

    if (napi_reject_deferred(env, addon_data->deferred, err_info) != napi_ok)
    {
      create_fatal_error("CloudLoadComplete", "Failed to reject promise!");
    }
  }
  else
  {
    if (napi_resolve_deferred(env, addon_data->deferred, cloud) != napi_ok)
    {
      create_fatal_error("CloudLoadComplete", "Failed to resolve promise!");
    }
  }

  if (napi_delete_async_work(env, addon_data->work) != napi_ok)
  {
    create_fatal_error("CloudLoadComplete", "Failed to delete async work!");
  }
}

static CloudLoadData *NewCloudLoadData(void)
{
  CloudLoadData *addon_data = (CloudLoadData *)malloc(sizeof(*addon_data));
  if (addon_data == NULL)
  {
    return NULL;
  }
  addon_data->err_info = NULL;
  addon_data->work = NULL;
  addon_data->deferred = NULL;
  addon_data->cloud = NULL;
  addon_data->filename = NULL;

  return addon_data;
}

static void CloudLoadFillData(napi_env env, napi_callback_info info, CloudLoadData *addon_data)
{
  size_t argc = 1;
  napi_value *args = (napi_value *)malloc(sizeof(napi_value) * argc);
  if (args == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadFillData: Failed to allocate memory for input arguments!");
    return;
  }

  if (napi_get_cb_info(env, info, &argc, args, NULL, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadFillData: Failed to parse input arguments!");
    free(args);
    return;
  }

  if (!check_type(env, args[0], napi_string))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadFillData: Invalid value was passed as first argument!");
    free(args);
    return;
  }

  size_t str_size;
  if (napi_get_value_string_utf8(env, args[0], NULL, 0, &str_size) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadFillData: Failed to get first argument size!");
    free(args);
    return;
  }

  str_size = str_size + 1;
  addon_data->filename = (char *)malloc(sizeof(char) * str_size);
  if (addon_data->filename == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadFillData: Failed to allocate memory for first argument!");
    free(args);
    return;
  }

  if (napi_get_value_string_utf8(env, args[0], addon_data->filename, str_size, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudLoadFillData: Invalid string was passed as first argument!");
    free(args);
    return;
  }

  free(args);
}

static void FreeCloudLoadData(CloudLoadData **data)
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

  cloud_free(&((*data)->cloud));

  if ((*data)->filename != NULL)
  {
    free((*data)->filename);
  }

  free(*data);
  *data = NULL;
}

static void CloudLoadPromiseUnloader(napi_env env, void *data, void *hint)
{
  CloudLoadData *addon_data = (CloudLoadData *)data;
  FreeCloudLoadData(&addon_data);
}

napi_value CloudLoadPromise(napi_env env, napi_callback_info info)
{
  CloudLoadData *addon_data = NewCloudLoadData();
  if (addon_data == NULL)
  {
    create_fatal_error("CloudLoadPromise", "Failed to allocate memory for addon data!");
  }
  CloudLoadFillData(env, info, addon_data);

  napi_value promise = create_promise(env,
                                      info,
                                      &(addon_data->deferred),
                                      &(addon_data->work),
                                      addon_data,
                                      CloudLoadExecute,
                                      CloudLoadComplete,
                                      "Node-API Deferred Promise - cloud_load");

  if (napi_wrap(env,
                promise,
                addon_data,
                CloudLoadPromiseUnloader,
                NULL,
                NULL) != napi_ok)
  {
    create_fatal_error("CloudLoadPromise", "Failed to associate data to Promise!");
  }

  return promise;
}

napi_value CloudLoadSync(napi_env env, napi_callback_info info)
{
  CloudLoadData *addon_data = NewCloudLoadData();
  if (addon_data == NULL)
  {
    create_fatal_error("CloudLoadSync", "Failed to allocate memory for addon data!");
  }
  CloudLoadFillData(env, info, addon_data);
  CloudLoadExecute(env, addon_data);

  napi_value rtn;
  if (addon_data->cloud == NULL ||
      addon_data->err_info != NULL ||
      pontu_cloud_to_napi_object(env, addon_data->cloud, &rtn) != napi_ok)
  {
    if (napi_get_null(env, &rtn) != napi_ok)
    {
      create_fatal_error("CloudLoadSync", "Failed get null value!");
    }
  }

  FreeCloudLoadData(&addon_data);

  return rtn;
}

// Cloud save methods
static void CloudSaveExecute(napi_env env, void *data)
{
  CloudSaveData *addon_data = (CloudSaveData *)data;

  if (addon_data->err_info != NULL)
  {
    return;
  }

  const char *ext = file_ext(addon_data->filename);

  int result = 0;
  if (strcmp(ext, "csv") == 0 || strcmp(ext, "CSV") == 0)
  {
    result = cloud_save_csv(addon_data->cloud, addon_data->filename);
  }
  else if (strcmp(ext, "pcd") == 0 || strcmp(ext, "PCD") == 0)
  {
    result = cloud_save_pcd(addon_data->cloud, addon_data->filename);
  }
  else if (strcmp(ext, "ply") == 0 || strcmp(ext, "PLY") == 0)
  {
    result = cloud_save_ply(addon_data->cloud, addon_data->filename);
  }
  else if (strcmp(ext, "xyz") == 0 || strcmp(ext, "XYZ") == 0)
  {
    result = cloud_save_xyz(addon_data->cloud, addon_data->filename);
  }

  if (result == 0 || addon_data->cloud == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveExecute: Failed to save point cloud!");
  }
}

static void CloudSaveComplete(napi_env env, napi_status status, void *data)
{
  napi_value cloud;
  CloudLoadData *addon_data = (CloudLoadData *)data;

  if (status != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveComplete: Asynchronous execution failure!");
  }

  if (addon_data->cloud != NULL && pontu_cloud_to_napi_object(env, addon_data->cloud, &cloud) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveComplete: Failed to convert point cloud!");
  }

  if (addon_data->cloud == NULL || addon_data->err_info != NULL)
  {
    napi_value err_info;

    if (napi_create_string_utf8(env,
                                addon_data->err_info,
                                NAPI_AUTO_LENGTH,
                                &(err_info)) != napi_ok)
    {
      create_fatal_error("CloudLoadComplete", "Failed to create error name!");
    }

    if (napi_reject_deferred(env, addon_data->deferred, err_info) != napi_ok)
    {
      create_fatal_error("CloudLoadComplete", "Failed to reject promise!");
    }
  }
  else
  {
    if (napi_resolve_deferred(env, addon_data->deferred, cloud) != napi_ok)
    {
      create_fatal_error("CloudLoadComplete", "Failed to resolve promise!");
    }
  }

  if (napi_delete_async_work(env, addon_data->work) != napi_ok)
  {
    create_fatal_error("CloudLoadComplete", "Failed to delete async work!");
  }
}

static CloudSaveData *NewCloudSaveData(void)
{
  CloudSaveData *addon_data = (CloudSaveData *)malloc(sizeof(*addon_data));
  if (addon_data == NULL)
  {
    return NULL;
  }
  addon_data->err_info = NULL;
  addon_data->work = NULL;
  addon_data->deferred = NULL;
  addon_data->cloud = NULL;
  addon_data->filename = NULL;

  return addon_data;
}

static void CloudSaveFillData(napi_env env, napi_callback_info info, CloudSaveData *addon_data)
{
  size_t argc = 2;
  napi_value *args = (napi_value *)malloc(sizeof(napi_value) * argc);
  if (args == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to allocate memory for input arguments!");
    return;
  }

  if (napi_get_cb_info(env, info, &argc, args, NULL, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to parse input arguments!");
    free(args);
    return;
  }

  if (!check_type(env, args[0], napi_object))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Invalid value was passed as first argument!");
    free(args);
    return;
  }

  if (!check_type(env, args[1], napi_string))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Invalid value was passed as second argument!");
    free(args);
    return;
  }

  if (napi_object_to_pontu_cloud(env, args[0], &(addon_data->cloud)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to get first argument!");
    free(args);
    return;
  }

  size_t str_size;
  if (napi_get_value_string_utf8(env, args[1], NULL, 0, &str_size) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to get second argument size!");
    free(args);
    return;
  }

  str_size = str_size + 1;
  addon_data->filename = (char *)malloc(sizeof(char) * str_size);
  if (addon_data->filename == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to allocate memory for second argument!");
    free(args);
    return;
  }

  if (napi_get_value_string_utf8(env, args[1], addon_data->filename, str_size, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Invalid string was passed as second argument!");
    free(args);
    return;
  }

  free(args);
}

static void FreeCloudSaveData(CloudSaveData **data)
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

  cloud_free(&((*data)->cloud));

  if ((*data)->filename != NULL)
  {
    free((*data)->filename);
  }

  free(*data);
  *data = NULL;
}

static void CloudSavePromiseUnloaded(napi_env env, void *data, void *hint)
{
  CloudSaveData *addon_data = (CloudSaveData *)data;
  FreeCloudSaveData(&addon_data);
}

napi_value CloudSavePromise(napi_env env, napi_callback_info info)
{
  CloudSaveData *addon_data = NewCloudSaveData();
  if (addon_data == NULL)
  {
    create_fatal_error("CloudSavePromise", "Failed to allocate memory for addon data!");
  }
  CloudSaveFillData(env, info, addon_data);

  napi_value promise = create_promise(env,
                                      info,
                                      &(addon_data->deferred),
                                      &(addon_data->work),
                                      addon_data,
                                      CloudSaveExecute,
                                      CloudSaveComplete,
                                      "Node-API Deferred Promise - cloud_save");

  if (napi_wrap(env,
                promise,
                addon_data,
                CloudSavePromiseUnloaded,
                NULL,
                NULL) != napi_ok)
  {
    create_fatal_error("CloudLoadPromise", "Failed to associate data to Promise!");
  }
  return promise;
}

napi_value CloudSaveSync(napi_env env, napi_callback_info info)
{
  CloudSaveData *addon_data = NewCloudSaveData();
  if (addon_data == NULL)
  {
    create_fatal_error("CloudSaveSync", "Failed to allocate memory for addon data!");
  }
  CloudSaveFillData(env, info, addon_data);
  CloudSaveExecute(env, addon_data);

  napi_value rtn;
  if (addon_data->cloud == NULL ||
      addon_data->err_info != NULL ||
      pontu_cloud_to_napi_object(env, addon_data->cloud, &rtn) != napi_ok)
  {
    if (napi_get_null(env, &rtn) != napi_ok)
    {
      create_fatal_error("CloudSaveSync", "Failed get null value!");
    }
  }

  FreeCloudSaveData(&addon_data);

  return rtn;
}

// Cloud rmse methods
static void CloudRmseExecute(napi_env env, void *data)
{
  CloudRmseData *addon_data = (CloudRmseData *)data;

  if (addon_data->err_info != NULL)
  {
    return;
  }

  // if (strcmp(addon_data->closest, "tree") == 0) {
  //   addon_data->rmse = cloud_rmse(
  //     addon_data->source,
  //     addon_data->target,
  //     addon_data->max_dist,
  //     cloud_closest_points_tree);
  // }
  // else {
  addon_data->rmse = cloud_rmse(
      addon_data->source,
      addon_data->target,
      addon_data->max_dist,
      cloud_closest_points_bf);
  // }
}

static void CloudRmseComplete(napi_env env, napi_status status, void *data)
{
  CloudRmseData *addon_data = (CloudRmseData *)data;

  if (status != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudRmseExecute: Asynchronous execution failure!");
  }

  if (addon_data->err_info != NULL)
  {
    napi_value err_info;

    if (napi_create_string_utf8(env,
                                addon_data->err_info,
                                NAPI_AUTO_LENGTH,
                                &(err_info)) != napi_ok)
    {
      create_fatal_error("CloudRmseExecute", "Failed to create error name!");
    }

    if (napi_reject_deferred(env, addon_data->deferred, err_info) != napi_ok)
    {
      create_fatal_error("CloudRmseExecute", "Failed to reject promise!");
    }
  }
  else
  {
    napi_value rmse;
    if (napi_create_double(env, addon_data->rmse, &rmse) != napi_ok)
    {
      create_fatal_error("CloudRmseExecute", "Failed to convert RMSE value!");
    }

    if (napi_resolve_deferred(env, addon_data->deferred, rmse) != napi_ok)
    {
      create_fatal_error("CloudRmseExecute", "Failed to resolve promise!");
    }
  }

  if (napi_delete_async_work(env, addon_data->work) != napi_ok)
  {
    create_fatal_error("CloudRmseExecute", "Failed to delete async work!");
  }
}

static CloudRmseData *NewCloudRmseData(void)
{
  CloudRmseData *addon_data = (CloudRmseData *)malloc(sizeof(*addon_data));
  if (addon_data == NULL)
  {
    return NULL;
  }
  addon_data->err_info = NULL;
  addon_data->work = NULL;
  addon_data->deferred = NULL;
  addon_data->source = NULL;
  addon_data->target = NULL;
  addon_data->max_dist = 0;
  addon_data->closest = NULL;
  addon_data->rmse = 0;

  return addon_data;
}

static void CloudRmseFillData(napi_env env, napi_callback_info info, CloudRmseData *addon_data)
{
  size_t argc = 4;
  napi_value *args = (napi_value *)malloc(sizeof(napi_value) * argc);
  if (args == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudRmseFillData: Failed to allocate memory for input arguments!");
    return;
  }

  if (napi_get_cb_info(env, info, &argc, args, NULL, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudRmseFillData: Failed to parse input arguments!");
    free(args);
    return;
  }

  if (!check_type(env, args[0], napi_object))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudRmseFillData: Invalid value was passed as first argument!");
    free(args);
    return;
  }

  if (!check_type(env, args[1], napi_object))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudRmseFillData: Invalid value was passed as second argument!");
    free(args);
    return;
  }

  if (!check_type(env, args[2], napi_number))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudRmseFillData: Invalid value was passed as third argument!");
    free(args);
    return;
  }

  if (!check_type(env, args[3], napi_string))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudRmseFillData: Invalid value was passed as fourth argument!");
    free(args);
    return;
  }

  if (napi_object_to_pontu_cloud(env, args[0], &(addon_data->source)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to get first argument!");
    free(args);
    return;
  }

  if (napi_object_to_pontu_cloud(env, args[1], &(addon_data->target)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to get second argument!");
    free(args);
    return;
  }

  if (napi_get_value_double(env, args[2], &(addon_data->max_dist)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to get thrid argument!");
    free(args);
    return;
  }

  size_t str_size;
  if (napi_get_value_string_utf8(env, args[3], NULL, 0, &str_size) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to get fourth argument size!");
    free(args);
    return;
  }

  str_size = str_size + 1;
  addon_data->closest = (char *)malloc(sizeof(char) * str_size);
  if (addon_data->closest == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Failed to allocate memory for fourth argument!");
    free(args);
    return;
  }

  if (napi_get_value_string_utf8(env, args[3], addon_data->closest, str_size, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudSaveFillData: Invalid string was passed as fourth argument!");
    free(args);
    return;
  }

  free(args);
}

static void FreeCloudRmseData(CloudRmseData **data)
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

  if ((*data)->closest != NULL)
  {
    free((*data)->closest);
  }

  free(*data);
  *data = NULL;
}

static void CloudRmsePromiseUnloaded(napi_env env, void *data, void *hint)
{
  CloudRmseData *addon_data = (CloudRmseData *)data;
  FreeCloudRmseData(&addon_data);
}

napi_value CloudRmsePromise(napi_env env, napi_callback_info info)
{
  CloudRmseData *addon_data = NewCloudRmseData();
  if (addon_data == NULL)
  {
    create_fatal_error("CloudRmsePromise", "Failed to allocate memory for addon data!");
  }
  CloudRmseFillData(env, info, addon_data);

  napi_value promise = create_promise(env,
                                      info,
                                      &(addon_data->deferred),
                                      &(addon_data->work),
                                      addon_data,
                                      CloudRmseExecute,
                                      CloudRmseComplete,
                                      "Node-API Deferred Promise - cloud_rmse");
  if (napi_wrap(env,
                promise,
                addon_data,
                CloudRmsePromiseUnloaded,
                NULL,
                NULL) != napi_ok)
  {
    create_fatal_error("CloudRmsePromise", "Failed to associate data to Promise!");
  }

  return promise;
}

napi_value CloudRmseSync(napi_env env, napi_callback_info info)
{
  CloudRmseData *addon_data = NewCloudRmseData();
  if (addon_data == NULL)
  {
    create_fatal_error("CloudRmseSync", "Failed to allocate memory for addon data!");
  }
  CloudRmseFillData(env, info, addon_data);
  CloudRmseExecute(env, addon_data);

  napi_value rtn;
  if (addon_data->err_info != NULL ||
      napi_create_double(env, addon_data->rmse, &rtn) != napi_ok)
  {
    if (napi_get_null(env, &rtn) != napi_ok)
    {
      create_fatal_error("CloudSaveSync", "Failed get null value!");
    }
  }

  FreeCloudRmseData(&addon_data);

  return rtn;
}

// Cloud transform methods
static void CloudTransformExecute(napi_env env, void *data)
{
  CloudTransformData *addon_data = (CloudTransformData *)data;

  if (addon_data->err_info != NULL)
  {
    return;
  }

  cloud_transform(addon_data->cloud, addon_data->rt);
}

static void CloudTransformComplete(napi_env env, napi_status status, void *data)
{
  napi_value cloud;
  CloudTransformData *addon_data = (CloudTransformData *)data;

  if (status != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudTransformComplete: Asynchronous execution failure!");
  }

  if (addon_data->cloud != NULL && pontu_cloud_to_napi_object(env, addon_data->cloud, &cloud) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudTransformComplete: Failed to convert point cloud!");
  }

  if (addon_data->err_info != NULL)
  {
    napi_value err_info;

    if (napi_create_string_utf8(env,
                                addon_data->err_info,
                                NAPI_AUTO_LENGTH,
                                &(err_info)) != napi_ok)
    {
      create_fatal_error("CloudTransformComplete", "Failed to create error name!");
    }

    if (napi_reject_deferred(env, addon_data->deferred, err_info) != napi_ok)
    {
      create_fatal_error("CloudTransformComplete", "Failed to reject promise!");
    }
  }
  else
  {
    if (napi_resolve_deferred(env, addon_data->deferred, cloud) != napi_ok)
    {
      create_fatal_error("CloudTransformComplete", "Failed to resolve promise!");
    }
  }

  if (napi_delete_async_work(env, addon_data->work) != napi_ok)
  {
    create_fatal_error("CloudTransformComplete", "Failed to delete async work!");
  }
}

static CloudTransformData *NewCloudTransformData(void)
{
  CloudTransformData *addon_data = (CloudTransformData *)malloc(sizeof(*addon_data));
  if (addon_data == NULL)
  {
    return NULL;
  }
  addon_data->err_info = NULL;
  addon_data->work = NULL;
  addon_data->deferred = NULL;
  addon_data->cloud = NULL;
  addon_data->rt = NULL;

  return addon_data;
}

static void CloudTransformFillData(napi_env env, napi_callback_info info, CloudTransformData *addon_data)
{
  size_t argc = 4;
  napi_value *args = (napi_value *)malloc(sizeof(napi_value) * argc);
  if (args == NULL)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudTransformFillData: Failed to allocate memory for input arguments!");
    return;
  }

  if (napi_get_cb_info(env, info, &argc, args, NULL, NULL) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudTransformFillData: Failed to parse input arguments!");
    free(args);
    return;
  }

  if (!check_type(env, args[0], napi_object))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudTransformFillData: Invalid value was passed as first argument!");
    free(args);
    return;
  }

  if (!check_type(env, args[1], napi_object))
  {
    addon_data_write_err(&(addon_data->err_info), "CloudTransformFillData: Invalid value was passed as second argument!");
    free(args);
    return;
  }

  if (napi_object_to_pontu_cloud(env, args[0], &(addon_data->cloud)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudTransformFillData: Failed to get first argument!");
    free(args);
    return;
  }

  if (napi_array_to_pontu_matrix(env, args[1], &(addon_data->rt)) != napi_ok)
  {
    addon_data_write_err(&(addon_data->err_info), "CloudTransformFillData: Failed to get second argument!");
    free(args);
    return;
  }

  free(args);
}

static void FreeCloudTransfoData(CloudTransformData **data)
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

  cloud_free(&((*data)->cloud));
  matrix_free(&((*data)->rt));

  free(*data);
  *data = NULL;
}

static void CloudTransformPromiseUnloaded(napi_env env, void *data, void *hint)
{
  CloudTransformData *addon_data = (CloudTransformData *)data;
  FreeCloudTransfoData(&addon_data);
}

napi_value CloudTransformPromise(napi_env env, napi_callback_info info)
{
  CloudTransformData *addon_data = NewCloudTransformData();
  if (addon_data == NULL)
  {
    create_fatal_error("CloudTransformPromise", "Failed to allocate memory for addon data!");
  }
  CloudTransformFillData(env, info, addon_data);

  napi_value promise = create_promise(env,
                                      info,
                                      &(addon_data->deferred),
                                      &(addon_data->work),
                                      addon_data,
                                      CloudTransformExecute,
                                      CloudTransformComplete,
                                      "Node-API Deferred Promise - cloud_transform");

  if (napi_wrap(env,
                promise,
                addon_data,
                CloudTransformPromiseUnloaded,
                NULL,
                NULL) != napi_ok)
  {
    create_fatal_error("CloudRmsePromise", "Failed to associate data to Promise!");
  }

  return promise;
}

napi_value CloudTransformSync(napi_env env, napi_callback_info info)
{
  CloudTransformData *addon_data = NewCloudTransformData();
  if (addon_data == NULL)
  {
    create_fatal_error("CloudTransformSync", "Failed to allocate memory for addon data!");
  }
  CloudTransformFillData(env, info, addon_data);
  CloudTransformExecute(env, addon_data);

  napi_value rtn;
  if (addon_data->err_info != NULL ||
      pontu_cloud_to_napi_object(env, addon_data->cloud, &rtn) != napi_ok)
  {
    if (napi_get_null(env, &rtn) != napi_ok)
    {
      create_fatal_error("CloudTransformSync", "Failed get null value!");
    }
  }

  FreeCloudTransfoData(&addon_data);

  return rtn;
}
