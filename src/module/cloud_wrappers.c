#include "../../include/module/cloud_wrappers.h"

// Cloud save methods
static void CloudSaveExecute(napi_env env, void* data) {
  CloudSaveData* addon_data = (CloudSaveData*)data;
  const char * ext = file_ext(addon_data->filename);

  int result = 0;
  if (strcmp(ext, "csv") == 0) {
    result= cloud_save_csv(addon_data->cloud, addon_data->filename);
  }
  else if (strcmp(ext, "pcd") == 0) {
    result = cloud_save_pcd(addon_data->cloud, addon_data->filename);
  }
  else if (strcmp(ext, "ply") == 0) {
    result = cloud_save_ply(addon_data->cloud, addon_data->filename);
  }
  else if (strcmp(ext, "xyz") == 0) {
    result = cloud_save_xyz(addon_data->cloud, addon_data->filename);
  }

  if (result == 0) {
    addon_data->cloud = NULL;
  }
  //free((char*)ext);
}

static void CloudSaveComplete(napi_env env, napi_status status, void* data) {
  if (status != napi_ok) {
    return;
  }

  CloudSaveData* addon_data = (CloudSaveData*)data;
  if (addon_data->cloud == NULL) {
    napi_value undefined;
    status = napi_get_undefined(env, &undefined);
    check_status(env, status, "Failed get undefined value!");

    status = napi_reject_deferred(env, addon_data->deferred, undefined);
    check_status(env, status, "Failed to reject promise!");
  }
  else {
    napi_value cloud = pontu_cloud_to_napi_object(env, addon_data->cloud);
    status = napi_resolve_deferred(env, addon_data->deferred, cloud);
    check_status(env, status, "Failed to resolve promise!");
  }

  napi_delete_async_work(env, addon_data->work);
  check_status(env, status, "Failed to delete async work!");

  addon_data->work = NULL;
  addon_data->deferred = NULL;
  cloud_free(&(addon_data->cloud));
  free(addon_data->filename);
  free(addon_data);
}

napi_value CloudSavePromise(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value work_name, promise;

  CloudSaveData* addon_data = (CloudSaveData*)malloc(sizeof(*addon_data));
  addon_data->work = NULL;

  size_t argc = 2;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_object);
  throw_type_error(env, args[1], napi_string);

  addon_data->cloud = napi_object_to_pontu_cloud(env, args[0]);
  
  size_t str_size;
  status = napi_get_value_string_utf8(env, args[1], NULL, 0, &str_size);
  check_status(env, status, "Failed to get filename size!");
  str_size = str_size + 1;
  addon_data->filename = (char*)malloc(sizeof(char) * str_size);
  status = napi_get_value_string_utf8(env, args[1], addon_data->filename, str_size, NULL);
  check_status(env, status, "Failed to get filename!");

  status = napi_create_promise(env, &(addon_data->deferred), &promise);
  check_status(env, status, "Failed to create promise!");

  status = napi_create_string_utf8(env, 
    "Node-API Deferred Promise - cloud_save", 
    NAPI_AUTO_LENGTH, 
    &work_name);
  check_status(env, status, "Failed to create work name!");

  status = napi_create_async_work(env,
    NULL,
    work_name,
    CloudSaveExecute,
    CloudSaveComplete,
    addon_data,
    &(addon_data->work));
  check_status(env, status, "Failed to create async work!");

  status = napi_queue_async_work(env, addon_data->work);
  check_status(env, status, "Failed to queue async work!");

  return promise;
}

// Cloud load methods
static void CloudLoadExecute(napi_env env, void* data) {
  CloudLoadData* addon_data = (CloudLoadData*)data;
  const char * ext = file_ext(addon_data->filename);
  
  if (strcmp(ext, "csv") == 0 || strcmp(ext, "CSV") == 0) {
    addon_data->cloud = cloud_load_csv(addon_data->filename);
  }
  else if (strcmp(ext, "json") == 0 || strcmp(ext, "JSON") == 0) {
    addon_data->cloud = cloud_load_json(addon_data->filename);
  }
  else if (strcmp(ext, "obj") == 0 || strcmp(ext, "OBJ") == 0) {
    addon_data->cloud = cloud_load_obj(addon_data->filename);
  }
  else if (strcmp(ext, "pcd") == 0 || strcmp(ext, "PCD") == 0) {
    addon_data->cloud = cloud_load_pcd(addon_data->filename);
  }
  else if (strcmp(ext, "ply") == 0 || strcmp(ext, "PLY") == 0) {
    addon_data->cloud = cloud_load_ply(addon_data->filename);
  }
  else if (strcmp(ext, "xyz") == 0 || strcmp(ext, "XYZ") == 0) {
    addon_data->cloud = cloud_load_xyz(addon_data->filename);
  }
  else {
    printf("Erro, %s\n", ext);
    addon_data->cloud = NULL;
  }
  //free((char*)ext);
}

static void CloudLoadComplete(napi_env env, napi_status status, void* data) {
  if (status != napi_ok) {
    return;
  }

  CloudLoadData* addon_data = (CloudLoadData*)data;
  if (addon_data->cloud == NULL) {
    napi_value undefined;
    status = napi_get_undefined(env, &undefined);
    check_status(env, status, "Failed get undefined value!");

    status = napi_reject_deferred(env, addon_data->deferred, undefined);
    check_status(env, status, "Failed to reject promise!");
  }
  else {
    napi_value cloud = pontu_cloud_to_napi_object(env, addon_data->cloud);

    status = napi_resolve_deferred(env, addon_data->deferred, cloud);
    check_status(env, status, "Failed to resolve promise!");
  }

  napi_delete_async_work(env, addon_data->work);
  check_status(env, status, "Failed to delete async work!");

  addon_data->work = NULL;
  addon_data->deferred = NULL;
  cloud_free(&(addon_data->cloud));
  free(addon_data->filename);
  free(addon_data);
}

napi_value CloudLoadPromise(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value work_name, promise;

  CloudLoadData* addon_data = (CloudLoadData*)malloc(sizeof(*addon_data));
  addon_data->work = NULL;

  size_t argc = 1;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_string);
  size_t str_size;
  status = napi_get_value_string_utf8(env, args[0], NULL, 0, &str_size);
  check_status(env, status, "Failed to get filename size!");
  str_size = str_size + 1;
  addon_data->filename = (char*)malloc(sizeof(char) * str_size);
  status = napi_get_value_string_utf8(env, args[0], addon_data->filename, str_size, NULL);
  check_status(env, status, "Failed to get filename!");

  status = napi_create_promise(env, &(addon_data->deferred), &promise);
  check_status(env, status, "Failed to create promise!");

  status = napi_create_string_utf8(env, 
    "Node-API Deferred Promise - cloud_load", 
    NAPI_AUTO_LENGTH, 
    &work_name);
  check_status(env, status, "Failed to create work name!");

  status = napi_create_async_work(env,
    NULL,
    work_name,
    CloudLoadExecute,
    CloudLoadComplete,
    addon_data,
    &(addon_data->work));
  check_status(env, status, "Failed to create async work!");

  status = napi_queue_async_work(env, addon_data->work);
  check_status(env, status, "Failed to queue async work!");

  return promise;
}

// Cloud rmse methods
static void CloudRmseExecute(napi_env env, void* data) {
  CloudRmseData* addon_data = (CloudRmseData*)data;
  if (strcmp(addon_data->closest, "tree") == 0) {
    addon_data->rmse = cloud_rmse(
      addon_data->source,
      addon_data->target,
      addon_data->max_dist,
      cloud_closest_points_tree);
  }
  else {
    addon_data->rmse = cloud_rmse(
      addon_data->source,
      addon_data->target,
      addon_data->max_dist,
      cloud_closest_points_bf);
  }
}

static void CloudRmseComplete(napi_env env, napi_status status, void* data) {
  if (status != napi_ok) {
    return;
  }

  CloudRmseData* addon_data = (CloudRmseData*)data;

  napi_value rmse;
  status = napi_create_double(env, addon_data->rmse, &rmse);
  check_status(env, status, "Failed to convert rmse value!");

  status = napi_resolve_deferred(env, addon_data->deferred, rmse);
  check_status(env, status, "Failed to resolve promise!");

  napi_delete_async_work(env, addon_data->work);
  check_status(env, status, "Failed to delete async work!");

  addon_data->work = NULL;
  addon_data->deferred = NULL;
  cloud_free(&(addon_data->source));
  cloud_free(&(addon_data->target));
  free(addon_data);
}

napi_value CloudRmsePromise(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value work_name, promise;

  CloudRmseData* addon_data = (CloudRmseData*)malloc(sizeof(*addon_data));
  addon_data->work = NULL;

  size_t argc = 4;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_object);
  throw_type_error(env, args[1], napi_object);
  throw_type_error(env, args[2], napi_number);
  throw_type_error(env, args[3], napi_string);

  addon_data->source = napi_object_to_pontu_cloud(env, args[0]);
  addon_data->target = napi_object_to_pontu_cloud(env, args[1]);

  status = napi_get_value_double(env, args[2], &(addon_data->max_dist));
  check_status(env, status, "Invalid number was passed as thrid argument!");
  
  size_t str_size;
  status = napi_get_value_string_utf8(env, args[3], NULL, 0, &str_size);
  check_status(env, status, "Failed to get fiveth argument size!");
  str_size = str_size + 1;
  addon_data->closest = (char*)malloc(sizeof(char) * str_size);
  status = napi_get_value_string_utf8(env, args[3], addon_data->closest, str_size, NULL);
  check_status(env, status, "Invalid string was passed as fourth argument!");

  status = napi_create_promise(env, &(addon_data->deferred), &promise);
  check_status(env, status, "Failed to create promise!");

  status = napi_create_string_utf8(env, 
    "Node-API Deferred Promise - cloud_save", 
    NAPI_AUTO_LENGTH, 
    &work_name);
  check_status(env, status, "Failed to create work name!");

  status = napi_create_async_work(env,
    NULL,
    work_name,
    CloudRmseExecute,
    CloudRmseComplete,
    addon_data,
    &(addon_data->work));
  check_status(env, status, "Failed to create async work!");

  status = napi_queue_async_work(env, addon_data->work);
  check_status(env, status, "Failed to queue async work!");

  return promise;
}

// Cloud transform methods
static void CloudTransformExecute(napi_env env, void* data) {
  CloudTransformData* addon_data = (CloudTransformData*)data;
  cloud_transform(addon_data->cloud, addon_data->rt);
}

static void CloudTransformComplete(napi_env env, napi_status status, void* data) {
  if (status != napi_ok) {
    return;
  }

  CloudTransformData* addon_data = (CloudTransformData*)data;
  if (addon_data->cloud == NULL) {
    napi_value undefined;
    status = napi_get_undefined(env, &undefined);
    check_status(env, status, "Failed get undefined value!");

    status = napi_reject_deferred(env, addon_data->deferred, undefined);
    check_status(env, status, "Failed to reject promise!");
  }
  else {
    napi_value cloud = pontu_cloud_to_napi_object(env, addon_data->cloud);

    status = napi_resolve_deferred(env, addon_data->deferred, cloud);
    check_status(env, status, "Failed to resolve promise!");
  }

  napi_delete_async_work(env, addon_data->work);
  check_status(env, status, "Failed to delete async work!");

  addon_data->work = NULL;
  addon_data->deferred = NULL;
  cloud_free(&(addon_data->cloud));
  matrix_free(&(addon_data->rt));
  free(addon_data);
}

napi_value CloudTransformPromise(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value work_name, promise;

  CloudTransformData* addon_data = (CloudTransformData*)malloc(sizeof(*addon_data));
  addon_data->work = NULL;

  size_t argc = 2;
  napi_value args[argc];

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  check_status(env, status, "Failed to parse arguments!");

  throw_type_error(env, args[0], napi_object);
  throw_type_error(env, args[1], napi_object);

  addon_data->cloud = napi_object_to_pontu_cloud(env, args[0]);
  addon_data->rt = napi_array_to_pontu_matrix(env, args[1]);

  status = napi_create_promise(env, &(addon_data->deferred), &promise);
  check_status(env, status, "Failed to create promise!");

  status = napi_create_string_utf8(env, 
    "Node-API Deferred Promise - cloud_save", 
    NAPI_AUTO_LENGTH, 
    &work_name);
  check_status(env, status, "Failed to create work name!");

  status = napi_create_async_work(env,
    NULL,
    work_name,
    CloudTransformExecute,
    CloudTransformComplete,
    addon_data,
    &(addon_data->work));
  check_status(env, status, "Failed to create async work!");

  status = napi_queue_async_work(env, addon_data->work);
  check_status(env, status, "Failed to queue async work!");

  return promise;
}
