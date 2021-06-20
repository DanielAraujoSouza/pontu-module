#ifndef REGISTRATION_SYNC_WRAPPERS_H
#define REGISTRATION_SYNC_WRAPPERS_H

#include <node_api.h>
#include "./converters.h"
#include "../registration.h"

napi_value RegistrationIcpSync(napi_env env, napi_callback_info info);

#endif // REGISTRATION_SYNC_WRAPPERS_H