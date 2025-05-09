#include "GreaterWindow.h"

#include <config.h>

#include "NotificationManager.h"


struct _GreaterWindow
{
    AdwApplicationWindow  parent_instance;

    /* Template widgets */
    GtkLabel *label;
    GtkButton *button;

};

G_DEFINE_FINAL_TYPE(GreaterWindow, greater_window, ADW_TYPE_APPLICATION_WINDOW)


static void on_button_clicked()
{
    // Exibe a notificação
    NotificationManager::getInstance().showNotification("Hello", "This is a test notification");
    g_print("Button clicked!\n");
}



static void greater_window_class_init(GreaterWindowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    const char* resource_path = GET_RESOURCE("/ui/greater.ui");
    gtk_widget_class_set_template_from_resource(widget_class, resource_path);

    //Pega os widgets do template
    gtk_widget_class_bind_template_child(widget_class, GreaterWindow, label);
    gtk_widget_class_bind_template_child(widget_class, GreaterWindow, button);

}

static void greater_window_init(GreaterWindow *self)
{
    // Inicializa o template
    gtk_widget_init_template(GTK_WIDGET(self));

    g_signal_connect(self->button, "clicked", on_button_clicked, NULL);
}

GreaterWindow* greater_window_new() {
  return (GreaterWindow*) g_object_new(GREATER_TYPE_WINDOW, NULL);
}