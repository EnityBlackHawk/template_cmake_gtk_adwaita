#include <adwaita.h>
#include "MainWindow.h"
#include "GreaterWindow.h"
#include "resources.h"
#include "NotificationManager.h"

// Callback for application "activate"
static void on_activate(AdwApplication* app, gpointer user_data) {
    GError* error = nullptr;

    GtkWindow* window = gtk_application_get_active_window(GTK_APPLICATION(app));
    if (window == nullptr) {
        window = GTK_WINDOW( greater_window_new() );
    }

    // Set the application for the window
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char* argv[]) {

    g_resources_register(resources_get_resource());
    g_set_application_name("My UI Adwaita App");

    AdwApplication* app = adw_application_new("org.ebh.Example", G_APPLICATION_DEFAULT_FLAGS);
    NotificationManager::getInstance().setApplication( reinterpret_cast<GApplication*>(app));
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), nullptr);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
