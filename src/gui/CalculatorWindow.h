#pragma once

#include <gtk/gtk.h>
#include "engine/Calculator.h"
#include <string>

namespace calc {

class CalculatorWindow {
public:
    explicit CalculatorWindow(GtkApplication* app);

private:
    GtkWidget* window_ = nullptr;
    GtkWidget* input_entry_ = nullptr;
    GtkWidget* result_label_ = nullptr;
    GtkWidget* history_box_ = nullptr;
    GtkWidget* variables_box_ = nullptr;

    Calculator calculator_;
    int precision_ = 10;

    void buildUI(GtkApplication* app);
    void evaluate();
    void refreshVariables();
    void addHistoryRow(const std::string& expr, double result);
    void applyCSS();

    static void onActivateEntry(GtkEntry* entry, gpointer data);
    static void onClickEvaluate(GtkButton* button, gpointer data);
    static void onClickClearHistory(GtkButton* button, gpointer data);
    static void onClickClearVars(GtkButton* button, gpointer data);
};

} // namespace calc
