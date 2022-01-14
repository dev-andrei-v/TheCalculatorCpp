#include "gui/CalculatorWindow.h"

static void onActivate(GtkApplication* app, gpointer) {
    // Prevent multiple windows on re-activation
    if (gtk_application_get_active_window(app)) {
        gtk_window_present(gtk_application_get_active_window(app));
        return;
    }
    // CalculatorWindow registers itself with the GtkApplication
    static calc::CalculatorWindow window(app);
}

int main(int argc, char* argv[]) {
    auto* app = gtk_application_new("com.thecalculator.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(onActivate), nullptr);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
