#include <node_api.h>
#include "../../include/module/common.h"
#include "../../include/module/cloud_wrappers.h"
#include "../../include/module/registration_wrappers.h"

napi_value Init(napi_env env, napi_value exports) {
  // Cloud
  export_function(env, exports, "cloud_save", CloudSavePromise);
  export_function(env, exports, "cloud_load", CloudLoadPromise);
  export_function(env, exports, "cloud_rmse", CloudRmsePromise);
  export_function(env, exports, "cloud_transform", CloudTransformPromise);
  // Registration
  export_function(env, exports, "registration_icp", RegistrationIcpPromise);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);