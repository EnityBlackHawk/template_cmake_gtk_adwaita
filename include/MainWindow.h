#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define MAIN_TYPE_WINDOW (main_window_get_type())

G_DECLARE_FINAL_TYPE (MainWindow, main_window, MAIN_WINDOW, WINDOW, AdwApplicationWindow)

MainWindow* main_window_new(void);

G_END_DECLS