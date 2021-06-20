#ifndef CLOUD_SYNC_WRAPPERS_H
#define CLOUD_SYNC_WRAPPERS_H

#include <node_api.h>
#include "./converters.h"
#include "../cloud.h"
#include "../matrix.h"

napi_value CloudSaveSync(napi_env env, napi_callback_info info);

napi_value CloudLoadSync(napi_env env, napi_callback_info info);

napi_value CloudRmseSync(napi_env env, napi_callback_info info);

napi_value CloudTransformSync(napi_env env, napi_callback_info info);

#endif // CLOUD_SYNC_WRAPPERS_H