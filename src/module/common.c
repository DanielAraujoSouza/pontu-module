#include "../../include/module/common.h"

void check_status(napi_env env, napi_status status, char *msg) {
  if (status != napi_ok) {
    napi_throw_error(env, NULL, msg);
  }
}

void throw_type_error(napi_env env, napi_value value, napi_valuetype expected_type) {
  napi_status status;
  napi_valuetype value_type;

  status = napi_typeof(env, value, &value_type);
  check_status(env, status, "Failed to check type of value!");

  if(value_type != expected_type) {
    napi_throw_error(env, NULL, "Value type does not match expected!");
  }
}

void export_function(napi_env env, napi_value exports, char *name, void *method) {
  napi_status status;
  napi_value fn;
  
  status = napi_create_function(env, "exports", NAPI_AUTO_LENGTH, method, NULL, &fn);
  check_status(env, status, "Unable to wrap native function!");

  status = napi_set_named_property(env, exports, name, fn);
  check_status(env, status, "Unable to populate exports!");
}

char *to_lower(char *str) {
    size_t len = strlen(str);
    char *str_l = malloc(sizeof(char)*(len+1));

    for (size_t i = 0; i < len; ++i) {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    return str_l;
}

char *file_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return to_lower(dot + 1);
}