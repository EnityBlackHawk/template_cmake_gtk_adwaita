#include "MainWindow.h"
#include <glib-object.h>
#include "NotificationManager.h"

struct _MainWindow
{
    AdwApplicationWindow  parent_instance;

    /* Template widgets */
    GtkLabel *label;
    GtkButton *button;

};

G_DEFINE_FINAL_TYPE(MainWindow, main_window, ADW_TYPE_APPLICATION_WINDOW)

static void on_button_clicked()
{
    // Exibe a notificação
    NotificationManager::getInstance().showNotification("Hello", "This is a test notification");
    g_print("Button clicked!\n");
}

static void main_window_class_init(MainWindowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/org/gnome/Example/ui/window.ui");

    //Pega os widgets do template
    gtk_widget_class_bind_template_child(widget_class, MainWindow, label);
    gtk_widget_class_bind_template_child(widget_class, MainWindow, button);

}

static void main_window_init(MainWindow *self)
{
    // Inicializa o template
    gtk_widget_init_template(GTK_WIDGET(self));

    // Conecta o sinal de clique do botão
    g_signal_connect(self->button, "clicked", G_CALLBACK(on_button_clicked), NULL);
}

MainWindow* main_window_new() {
      return (MainWindow*) g_object_new(MAIN_TYPE_WINDOW, NULL);
}