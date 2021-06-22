#include <node_api.h>
#include "../../include/module/common.h"
#include "../../include/module/cloud_wrappers.h"
#include "../../include/module/registration_wrappers.h"

napi_value Init(napi_env env, napi_value exports)
{
  // Cloud Async
  export_function(env, exports, "cloud_save", CloudSavePromise);
  export_function(env, exports, "cloud_load", CloudLoadPromise);
  export_function(env, exports, "cloud_rmse", CloudRmsePromise);
  export_function(env, exports, "cloud_transform", CloudTransformPromise);
  // Cloud Sync
  export_function(env, exports, "cloud_save_sync", CloudSaveSync);
  export_function(env, exports, "cloud_load_sync", CloudLoadSync);
  export_function(env, exports, "cloud_rmse_sync", CloudRmseSync);
  export_function(env, exports, "cloud_transform_sync", CloudTransformSync);
  // Registration
  export_function(env, exports, "registration_icp", RegistrationIcpPromise);
  // Registration Sync
  export_function(env, exports, "registration_icp_sync", RegistrationIcpSync);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);