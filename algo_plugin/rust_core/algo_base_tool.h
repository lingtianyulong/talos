#ifndef ALGO_BASE_TOOL_H
#define ALGO_BASE_TOOL_H

#ifdef __cplusplus
extern "C" {
#endif

struct ToolObject;

using ToolNameFunc = const char *(*)(ToolObject *);
using ToolInitFunc = bool (*)(ToolObject *);
using ToolRunFunc = int (*)(ToolObject *);
using ToolDestroyFunc = void (*)(ToolObject *);

struct ToolVTable {
  ToolNameFunc name;
  ToolInitFunc init;
  ToolRunFunc run;
  ToolDestroyFunc destroy;
};

struct Tool {
  ToolObject *obj;
  ToolVTable *vtable;
};

#ifdef __cplusplus
}
#endif

#endif ALGO_BASE_TOOL_H
