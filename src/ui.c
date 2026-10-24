#include <gtk/gtk.h>

/* look for style.css under include/, next to the executable first, then
 * relative to the working directory, so the binary still finds its
 * stylesheet when launched from elsewhere */
static GFile* find_style_file(void) {
  GFile* file;
  gchar* exe;
  gchar* dir;
  gchar* path;

  exe = g_file_read_link("/proc/self/exe", NULL);
  if (exe != NULL) {
    dir = g_path_get_dirname(exe);
    path = g_build_filename(dir, "include", "style.css", NULL);
    file = g_file_new_for_path(path);

    if (g_file_query_exists(file, NULL)) {
      g_free(path);
      g_free(dir);
      g_free(exe);
      return file;
    }

    g_object_unref(file);
    g_free(path);
    g_free(dir);
    g_free(exe);
  }

  return g_file_new_for_path("include/style.css");
}

static gboolean on_escape(GtkEventControllerKey* controller, guint keyval, guint keycode, GdkModifierType state, GtkWindow* window) {
  if (keyval == GDK_KEY_Escape) {
    gtk_window_close(window);
    return TRUE;
  }
  return FALSE;
}

void startup(GtkApplication* app, gpointer user_data) {
  GtkCssProvider* provider;
  GFile* file;

  provider = gtk_css_provider_new();
  file = find_style_file();

  gtk_css_provider_load_from_file(provider, file);

  gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_object_unref(file);
  g_object_unref(provider);
}

void activate(GtkApplication* app, gpointer user_data) {
  GtkWidget* window;
  GtkWidget* handle;
  GtkWidget* card;
  GtkWidget* box;
  GtkWidget* search;
  GtkWidget* button;
  GtkEventController* keys;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 480, 120);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
  gtk_widget_add_css_class(window, "launcher");

  card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_add_css_class(card, "card");

  box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

  search = gtk_search_entry_new();
  gtk_widget_set_hexpand(search, TRUE);

  button = gtk_button_new_with_label("search");
  gtk_widget_add_css_class(button, "suggested-action");

  gtk_box_append(GTK_BOX(box), search);
  gtk_box_append(GTK_BOX(box), button);
  gtk_box_append(GTK_BOX(card), box);

  /* borderless windows have no title bar to drag, so let the card
   * itself act as the drag handle */
  handle = gtk_window_handle_new();
  gtk_window_handle_set_child(GTK_WINDOW_HANDLE(handle), card);
  gtk_window_set_child(GTK_WINDOW(window), handle);

  /* no title bar means no close button; Escape closes the window.
   * capture phase, because the focused GtkSearchEntry swallows Escape
   * to clear itself before it would reach the window in the bubble
   * phase */
  keys = gtk_event_controller_key_new();
  gtk_event_controller_set_propagation_phase(keys, GTK_PHASE_CAPTURE);
  g_signal_connect(keys, "key-pressed", G_CALLBACK(on_escape), window);
  gtk_widget_add_controller(window, keys);

  gtk_window_present(GTK_WINDOW(window));
}
