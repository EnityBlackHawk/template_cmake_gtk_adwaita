#pragma once

#include <string>

#include <gio/gio.h>

class NotificationManager {

public:

  static NotificationManager& getInstance() {
    static NotificationManager instance;
    return instance;
  }

  void showNotification(std::string title, std::string message) {

    if(_application == nullptr) {
      g_printerr("Application is not set. Cannot show notification.\n");
      return;
    }

    GNotification* notification = g_notification_new(title.c_str());
    g_notification_set_body(notification, message.c_str());
    g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_NORMAL);

    g_application_send_notification(_application, "notification", notification);

    g_object_unref(notification);
  }

  void setApplication(GApplication* application) {
    _application = application;
  }

private:
  GApplication*  _application;


};
