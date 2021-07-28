#include "../../include/module/common.h"

void export_function(napi_env env, napi_value exports, const char *name, napi_value (*method)(napi_env, napi_callback_info))
{
  napi_value fn;

  if (napi_create_function(env, "exports", NAPI_AUTO_LENGTH, method, NULL, &fn) != napi_ok)
  {
    create_fatal_error("export_function", "Unable to wrap native function!");
  }

  if (napi_set_named_property(env, exports, name, fn) != napi_ok)
  {
    create_fatal_error("export_function", "Unable to populate exports!");
  }
}

void to_lower(char *str)
{
  for (int i = 0; str[i]; i++)
  {
    str[i] = (char)tolower(str[i]);
  }
}

const char *file_ext(const char *filename)
{
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return NULL;
  return dot + 1;
}

napi_value create_promise(napi_env env,
                          napi_callback_info info,
                          napi_deferred *deferred,
                          napi_async_work *work,
                          void *addon_data,
                          void (*fn_execute)(napi_env, void *),
                          void (*fn_complete)(napi_env, napi_status, void *),
                          const char *promise_name)
{
  napi_value work_name, promise;

  if (napi_create_promise(env, deferred, &promise) != napi_ok)
  {
    create_fatal_error(promise_name, "Failed to create promise!");
  }

  if (napi_create_string_utf8(env,
                              promise_name,
                              NAPI_AUTO_LENGTH,
                              &work_name) != napi_ok)
  {
    create_fatal_error(promise_name, "Failed to create work name!");
  }

  if (napi_create_async_work(env,
                             NULL,
                             work_name,
                             fn_execute,
                             fn_complete,
                             addon_data,
                             work) != napi_ok)
  {
    create_fatal_error(promise_name, "Failed to create async work!");
  }

  if (napi_queue_async_work(env, *work) != napi_ok)
  {
    create_fatal_error(promise_name, "Failed to queue async work!");
  }

  return promise;
}

void create_fatal_error(const char *local, const char *message)
{
  napi_fatal_error(local, NAPI_AUTO_LENGTH, message, NAPI_AUTO_LENGTH);
}

void addon_data_write_err(char **err_info, const char *txt)
{
  *err_info = (char *)malloc(strlen(txt) + 1);
  if (*err_info == NULL)
  {
    create_fatal_error("addon_data_write_err", "Failed to allocate memory!");
  }

  strcpy(*err_info, txt);
}

int check_type(napi_env env, napi_value value, napi_valuetype expected_type)
{
  napi_valuetype value_type;

  if (napi_typeof(env, value, &value_type) != napi_ok)
  {
    return 0;
  }

  return value_type == expected_type;
}