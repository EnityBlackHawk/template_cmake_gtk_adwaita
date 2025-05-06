#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define GREATER_TYPE_WINDOW (greater_window_get_type())

G_DECLARE_FINAL_TYPE (GreaterWindow, greater_window, GREATER_WINDOW, WINDOW, AdwApplicationWindow)

GreaterWindow* greater_window_new();

G_END_DECLS