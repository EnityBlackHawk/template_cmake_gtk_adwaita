#include "GreaterWindow.h"


struct _GreaterWindow
{
    AdwApplicationWindow  parent_instance;

    /* Template widgets */
    GtkLabel *label;
    GtkButton *button;

};

G_DEFINE_FINAL_TYPE(GreaterWindow, greater_window, ADW_TYPE_APPLICATION_WINDOW)



static void greater_window_class_init(GreaterWindowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/org/gnome/Example/ui/greater.ui");

    //Pega os widgets do template
    gtk_widget_class_bind_template_child(widget_class, GreaterWindow, label);
    gtk_widget_class_bind_template_child(widget_class, GreaterWindow, button);

}

static void greater_window_init(GreaterWindow *self)
{
    // Inicializa o template
    gtk_widget_init_template(GTK_WIDGET(self));
}

GreaterWindow* greater_window_new() {
  return (GreaterWindow*) g_object_new(GREATER_TYPE_WINDOW, NULL);
}