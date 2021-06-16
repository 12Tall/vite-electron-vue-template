#include <assert.h>
#include <node_api.h>
#include <windows.h>
#include <winuser.h>

#define DECLARE_NAPI_METHOD(name, func) \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value setWindowTransparent(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value res;

  size_t argc = 1;
  napi_value argv[1];
  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  assert(status == napi_ok);
  if (argc < 1) {
    napi_throw_type_error(env, NULL, "FindWoindow takes only 2 parameters");
    return NULL;
  }

  int32_t handle = 0;
  napi_valuetype valType;
  status = napi_typeof(env, argv[0], &valType);
  assert(status == napi_ok);
  if (valType == napi_number) {
    status = napi_get_value_int32(env, argv[0], &handle);
    assert(status == napi_ok);
  }

  // https://blog.csdn.net/u011822516/article/details/41946631
  // 一种实现窗口透明的方式
  bool result = SetWindowLongA((HWND)handle, GWL_EXSTYLE,
                               GetWindowLongA((HWND)handle, GWL_EXSTYLE) |
                                   WS_EX_LAYERED) != 0;

  status = napi_get_boolean(env, result, &res);
  assert(status == napi_ok);
  return res;
}
static napi_value updateWindow(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value res;

  size_t argc = 1;
  napi_value argv[1];
  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  assert(status == napi_ok);
  if (argc < 1) {
    napi_throw_type_error(env, NULL, "FindWoindow takes only 2 parameters");
    return NULL;
  }

  int32_t handle = 0;
  napi_valuetype valType;
  status = napi_typeof(env, argv[0], &valType);
  assert(status == napi_ok);
  if (valType == napi_number) {
    status = napi_get_value_int32(env, argv[0], &handle);
    assert(status == napi_ok);
  }

  bool result = UpdateWindow((HWND)handle);

  status = napi_get_boolean(env, result, &res);
  assert(status == napi_ok);

  return res;
}

static napi_value setParent(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value res;

  size_t argc = 2;
  napi_value argv[2];
  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  assert(status == napi_ok);
  if (argc < 2) {
    napi_throw_type_error(env, NULL, "FindWoindow takes only 2 parameters");
    return NULL;
  }

  int32_t child = 0, parent = 0;
  napi_valuetype valType;
  status = napi_typeof(env, argv[0], &valType);
  assert(status == napi_ok);
  if (valType == napi_number) {
    status = napi_get_value_int32(env, argv[0], &child);
    assert(status == napi_ok);
  }

  status = napi_typeof(env, argv[1], &valType);
  assert(status == napi_ok);
  if (valType == napi_number) {
    status = napi_get_value_int32(env, argv[1], &parent);
    assert(status == napi_ok);
  }

  HWND hwnd = SetParent((HWND)child, (HWND)parent);

  if (hwnd == NULL) {
    status = napi_get_null(env, &res);
  } else {
    status = napi_create_int32(env, (int32_t)hwnd, &res);
  }
  assert(status == napi_ok);

  return res;
}

/**
  *  HWND FindWindowExA(
  *  HWND   hWndParent,      // 父窗口，NULL 表示从Desktop 窗口开始查找
  *  HWND   hWndChildAfter,  // 起始子窗口，NULL 表示从第一个子窗口开始
  *  LPCSTR lpszClass,
  *  LPCSTR lpszWindow
  );
  */
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  // 1. 查找包含'SHELLDLL_DefView' 子窗口的WorkerW 窗口
  HWND p = FindWindowExA(hwnd, NULL, "SHELLDLL_DefView", NULL);
  HWND* ret = (HWND*)lParam;  // 传入的wallpaper_hwnd 指针

  if (p) {
    // 将指针指向下一个WorkerW
    // 还有一种判断方法，就是下一个WorkerW 的Handle
    // 要比包含`SHELLDLL_DefView`的WorkerW 的Handle 大2
    *ret = FindWindowExA(NULL, hwnd, "WorkerW", NULL);
  }
  return true;
}

/**
 * https://stackoverflow.com/questions/56132584/draw-on-windows-10-wallpaper-in-c
 */
static napi_value getWallpaperWindow(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value res;
  // 1. 查找最底层的窗口
  HWND progman = FindWindowA("ProgMan", NULL);

  // 2. 向最底层窗口创建两个WorkerW 窗口，如果已存在则什么都不会发生
  // 网络上大部分代码发送的消息参数都是0x00,0x00。但这会造成一旦WorkerW
  // 窗口被不小心销毁，就再也找不到了
  // https://gitlab.com/kelteseth/ScreenPlay/-/blob/master/ScreenPlayWallpaper/src/winwindow.cpp#L387
  // 而相应的，在监视窗口消息时，应使用对应版本的spy++，否则是拦截不到消息的
  // 需要注意的是，在程序异常退出时会有边框遗留在壁纸上
  SendMessageTimeout(progman, 0x052C, 0x0D, 0x01, SMTO_BLOCK, 1000, nullptr);
  // SendMessage(progman, 0x052C, 0x0D, 0x01);

  // 3. 枚举所有窗口，直到发现包含'SHELLDLL_DefView'
  // 子窗口的WorkerW，他下面的WorkerW 就是我们需要的WorkerW
  HWND wallpaper_hwnd = nullptr;
  EnumWindows(EnumWindowsProc,
              (LPARAM)&wallpaper_hwnd  // 被传入回调函数的参数
  );

  status = napi_create_int32(env, (int32_t)wallpaper_hwnd, &res);
  assert(status == napi_ok);
  return res;
}

// string|NULL, string|NULL
static napi_value findWindow(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value res;

  size_t argc = 2;
  napi_value argv[2];
  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  assert(status == napi_ok);
  if (argc < 2) {
    napi_throw_type_error(env, NULL, "FindWoindow takes only 2 parameters");
    return NULL;
  }

  size_t len = 0;
  char *className = NULL, *windowName = NULL;
  napi_valuetype valType;
  status = napi_typeof(env, argv[0], &valType);
  assert(status == napi_ok);
  if (valType == napi_string) {
    // 获取字符串长度
    status = napi_get_value_string_utf8(env, argv[0], NULL, 0, &len);
    assert(status == napi_ok);
    className = (char*)calloc(++len, sizeof(char));
    status = napi_get_value_string_utf8(env, argv[0], className, len, &len);
    assert(status == napi_ok);
  }

  status = napi_typeof(env, argv[1], &valType);
  assert(status == napi_ok);
  if (valType == napi_string) {
    status = napi_get_value_string_utf8(env, argv[1], NULL, 0, &len);
    assert(status == napi_ok);
    windowName = (char*)calloc(++len, sizeof(char));
    status = napi_get_value_string_utf8(env, argv[1], windowName, len, &len);
    assert(status == napi_ok);
  }

  HWND hWnd = FindWindowA((LPCSTR)className, (LPCSTR)windowName);
  status = napi_create_int32(env, (int32_t)hWnd, &res);
  assert(status == napi_ok);
  free(className);
  free(windowName);
  return res;
}

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc[] = {
      DECLARE_NAPI_METHOD("findWindow", findWindow),
      DECLARE_NAPI_METHOD("getWallpaperWindow", getWallpaperWindow),
      DECLARE_NAPI_METHOD("setParent", setParent),
      DECLARE_NAPI_METHOD("updateWindow", updateWindow),
      DECLARE_NAPI_METHOD("setWindowTransparent", setWindowTransparent),
  };
  status = napi_define_properties(env, exports, 5, desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)