#include "gui/CalculatorWindow.h"
#include <sstream>
#include <iomanip>
#include <cstring>

namespace calc {

// ---------------------------------------------------------------------------
// CSS theme
// ---------------------------------------------------------------------------

static const char* CSS = R"(
window {
    background-color: #1e1e2e;
}

.main-box {
    padding: 24px;
}

.input-row entry {
    font-size: 18px;
    padding: 12px 16px;
    border-radius: 12px;
    background-color: #313244;
    color: #cdd6f4;
    border: 2px solid #45475a;
    min-height: 24px;
}

.input-row entry:focus {
    border-color: #89b4fa;
    box-shadow: 0 0 0 2px alpha(#89b4fa, 0.25);
}

.eval-button {
    font-size: 18px;
    font-weight: bold;
    padding: 12px 24px;
    border-radius: 12px;
    background-color: #89b4fa;
    color: #1e1e2e;
    border: none;
    min-height: 24px;
}

.eval-button:hover {
    background-color: #b4d0fb;
}

.result-frame {
    background-color: #181825;
    border-radius: 16px;
    padding: 20px 24px;
    border: 1px solid #313244;
}

.result-hint {
    font-size: 13px;
    color: #6c7086;
}

.result-value {
    font-size: 32px;
    font-weight: bold;
    color: #a6e3a1;
    font-family: monospace;
}

.result-error {
    font-size: 16px;
    color: #f38ba8;
}

.section-header {
    font-size: 13px;
    font-weight: bold;
    color: #6c7086;
    letter-spacing: 1px;
}

.section-frame {
    background-color: #181825;
    border-radius: 12px;
    padding: 8px;
    border: 1px solid #313244;
}

.history-expr {
    font-size: 14px;
    color: #bac2de;
    font-family: monospace;
}

.history-result {
    font-size: 14px;
    font-weight: bold;
    color: #89b4fa;
    font-family: monospace;
}

.var-name {
    font-size: 14px;
    color: #f9e2af;
    font-family: monospace;
}

.var-value {
    font-size: 14px;
    color: #cdd6f4;
    font-family: monospace;
}

.small-button {
    font-size: 11px;
    padding: 4px 12px;
    border-radius: 8px;
    background-color: #45475a;
    color: #bac2de;
    border: none;
}

.small-button:hover {
    background-color: #585b70;
}

.empty-hint {
    font-size: 13px;
    color: #585b70;
    font-style: italic;
    padding: 12px;
}

.history-row {
    padding: 8px 12px;
    border-radius: 8px;
}

.history-row:hover {
    background-color: #313244;
}
)";

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static std::string formatDouble(double val, int precision) {
    std::ostringstream ss;
    ss << std::setprecision(precision) << val;
    return ss.str();
}

// ---------------------------------------------------------------------------
// Signal callbacks (static trampolines)
// ---------------------------------------------------------------------------

void CalculatorWindow::onActivateEntry(GtkEntry*, gpointer data) {
    static_cast<CalculatorWindow*>(data)->evaluate();
}

void CalculatorWindow::onClickEvaluate(GtkButton*, gpointer data) {
    static_cast<CalculatorWindow*>(data)->evaluate();
}

void CalculatorWindow::onClickClearHistory(GtkButton*, gpointer data) {
    auto* self = static_cast<CalculatorWindow*>(data);
    GtkWidget* child = gtk_widget_get_first_child(self->history_box_);
    while (child) {
        GtkWidget* next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(self->history_box_), child);
        child = next;
    }

    auto* hint = gtk_label_new("No history yet");
    gtk_widget_add_css_class(hint, "empty-hint");
    gtk_box_append(GTK_BOX(self->history_box_), hint);
}

void CalculatorWindow::onClickClearVars(GtkButton*, gpointer data) {
    auto* self = static_cast<CalculatorWindow*>(data);
    self->calculator_.variables().clear();
    self->refreshVariables();
}

// ---------------------------------------------------------------------------
// Core logic
// ---------------------------------------------------------------------------

void CalculatorWindow::evaluate() {
    GtkEntryBuffer* buf = gtk_entry_get_buffer(GTK_ENTRY(input_entry_));
    const char* text = gtk_entry_buffer_get_text(buf);
    if (!text || std::strlen(text) == 0) return;

    std::string expr(text);

    // Check for "let" assignment
    bool isLet = false;
    std::string varName;
    std::string evalExpr = expr;

    if (expr.size() > 4 && expr.substr(0, 4) == "let ") {
        auto eq = expr.find('=', 4);
        if (eq != std::string::npos) {
            isLet = true;
            varName = expr.substr(4, eq - 4);
            // trim varName
            auto s = varName.find_first_not_of(" \t");
            auto e = varName.find_last_not_of(" \t");
            if (s != std::string::npos) varName = varName.substr(s, e - s + 1);
            evalExpr = expr.substr(eq + 1);
            auto s2 = evalExpr.find_first_not_of(" \t");
            auto e2 = evalExpr.find_last_not_of(" \t");
            if (s2 != std::string::npos) evalExpr = evalExpr.substr(s2, e2 - s2 + 1);
        }
    }

    try {
        double result = calculator_.evaluate(evalExpr);
        calculator_.variables().set("ans", result);

        if (isLet) {
            calculator_.variables().set(varName, result);
        }

        std::string resultStr = formatDouble(result, precision_);
        gtk_label_set_text(GTK_LABEL(result_label_), resultStr.c_str());
        gtk_widget_remove_css_class(result_label_, "result-error");
        gtk_widget_add_css_class(result_label_, "result-value");

        addHistoryRow(expr, result);
        refreshVariables();
    } catch (const std::exception& ex) {
        gtk_label_set_text(GTK_LABEL(result_label_), ex.what());
        gtk_widget_remove_css_class(result_label_, "result-value");
        gtk_widget_add_css_class(result_label_, "result-error");
    }

    gtk_entry_buffer_set_text(buf, "", 0);
    gtk_widget_grab_focus(input_entry_);
}

void CalculatorWindow::addHistoryRow(const std::string& expr, double result) {
    // Remove "No history yet" hint if present
    GtkWidget* first = gtk_widget_get_first_child(history_box_);
    if (first && GTK_IS_LABEL(first)) {
        const char* txt = gtk_label_get_text(GTK_LABEL(first));
        if (txt && std::strcmp(txt, "No history yet") == 0) {
            gtk_box_remove(GTK_BOX(history_box_), first);
        }
    }

    auto* row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_add_css_class(row, "history-row");

    auto* exprLabel = gtk_label_new(expr.c_str());
    gtk_widget_add_css_class(exprLabel, "history-expr");
    gtk_label_set_xalign(GTK_LABEL(exprLabel), 0);
    gtk_widget_set_hexpand(exprLabel, TRUE);
    gtk_label_set_ellipsize(GTK_LABEL(exprLabel), PANGO_ELLIPSIZE_END);

    auto* eqLabel = gtk_label_new("=");
    gtk_widget_add_css_class(eqLabel, "history-expr");

    std::string resultStr = formatDouble(result, precision_);
    auto* resultLabel = gtk_label_new(resultStr.c_str());
    gtk_widget_add_css_class(resultLabel, "history-result");

    gtk_box_append(GTK_BOX(row), exprLabel);
    gtk_box_append(GTK_BOX(row), eqLabel);
    gtk_box_append(GTK_BOX(row), resultLabel);

    // Prepend (newest first)
    gtk_box_prepend(GTK_BOX(history_box_), row);
}

void CalculatorWindow::refreshVariables() {
    // Clear existing children
    GtkWidget* child = gtk_widget_get_first_child(variables_box_);
    while (child) {
        GtkWidget* next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(variables_box_), child);
        child = next;
    }

    const auto& vars = calculator_.variables().all();
    if (vars.empty()) {
        auto* hint = gtk_label_new("No variables defined");
        gtk_widget_add_css_class(hint, "empty-hint");
        gtk_box_append(GTK_BOX(variables_box_), hint);
        return;
    }

    for (const auto& [name, value] : vars) {
        auto* row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        gtk_widget_set_margin_start(row, 12);
        gtk_widget_set_margin_end(row, 12);
        gtk_widget_set_margin_top(row, 4);
        gtk_widget_set_margin_bottom(row, 4);

        auto* nameLabel = gtk_label_new(name.c_str());
        gtk_widget_add_css_class(nameLabel, "var-name");
        gtk_label_set_xalign(GTK_LABEL(nameLabel), 0);

        auto* eq = gtk_label_new("=");
        gtk_widget_add_css_class(eq, "var-value");

        std::string valStr = formatDouble(value, precision_);
        auto* valLabel = gtk_label_new(valStr.c_str());
        gtk_widget_add_css_class(valLabel, "var-value");
        gtk_widget_set_hexpand(valLabel, TRUE);
        gtk_label_set_xalign(GTK_LABEL(valLabel), 0);

        gtk_box_append(GTK_BOX(row), nameLabel);
        gtk_box_append(GTK_BOX(row), eq);
        gtk_box_append(GTK_BOX(row), valLabel);
        gtk_box_append(GTK_BOX(variables_box_), row);
    }
}

// ---------------------------------------------------------------------------
// CSS application
// ---------------------------------------------------------------------------

void CalculatorWindow::applyCSS() {
    auto* provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, CSS);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    g_object_unref(provider);
}

// ---------------------------------------------------------------------------
// UI construction
// ---------------------------------------------------------------------------

CalculatorWindow::CalculatorWindow(GtkApplication* app) {
    applyCSS();
    buildUI(app);
}

void CalculatorWindow::buildUI(GtkApplication* app) {
    // Window
    window_ = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window_), "TheCalculator");
    gtk_window_set_default_size(GTK_WINDOW(window_), 520, 680);

    // Header bar
    auto* header = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(window_), header);

    auto* titleLabel = gtk_label_new("TheCalculator");
    gtk_widget_add_css_class(titleLabel, "title");
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), titleLabel);

    // Main vertical box
    auto* mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_add_css_class(mainBox, "main-box");
    gtk_window_set_child(GTK_WINDOW(window_), mainBox);

    // --- Input row ---
    auto* inputRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_add_css_class(inputRow, "input-row");

    input_entry_ = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(input_entry_), "Enter expression... e.g. 2 + 3 * 4");
    gtk_widget_set_hexpand(input_entry_, TRUE);
    g_signal_connect(input_entry_, "activate", G_CALLBACK(onActivateEntry), this);

    auto* evalBtn = gtk_button_new_with_label("=");
    gtk_widget_add_css_class(evalBtn, "eval-button");
    g_signal_connect(evalBtn, "clicked", G_CALLBACK(onClickEvaluate), this);

    gtk_box_append(GTK_BOX(inputRow), input_entry_);
    gtk_box_append(GTK_BOX(inputRow), evalBtn);
    gtk_box_append(GTK_BOX(mainBox), inputRow);

    // --- Result display ---
    auto* resultFrame = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_add_css_class(resultFrame, "result-frame");

    auto* resultHint = gtk_label_new("RESULT");
    gtk_widget_add_css_class(resultHint, "result-hint");
    gtk_label_set_xalign(GTK_LABEL(resultHint), 0);

    result_label_ = gtk_label_new("0");
    gtk_widget_add_css_class(result_label_, "result-value");
    gtk_label_set_xalign(GTK_LABEL(result_label_), 0);
    gtk_label_set_selectable(GTK_LABEL(result_label_), TRUE);

    gtk_box_append(GTK_BOX(resultFrame), resultHint);
    gtk_box_append(GTK_BOX(resultFrame), result_label_);
    gtk_box_append(GTK_BOX(mainBox), resultFrame);

    // --- History section ---
    auto* historyHeader = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    auto* historyTitle = gtk_label_new("HISTORY");
    gtk_widget_add_css_class(historyTitle, "section-header");
    gtk_label_set_xalign(GTK_LABEL(historyTitle), 0);
    gtk_widget_set_hexpand(historyTitle, TRUE);

    auto* clearHistBtn = gtk_button_new_with_label("Clear");
    gtk_widget_add_css_class(clearHistBtn, "small-button");
    g_signal_connect(clearHistBtn, "clicked", G_CALLBACK(onClickClearHistory), this);

    gtk_box_append(GTK_BOX(historyHeader), historyTitle);
    gtk_box_append(GTK_BOX(historyHeader), clearHistBtn);
    gtk_box_append(GTK_BOX(mainBox), historyHeader);

    auto* historyScroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(historyScroll),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(historyScroll, TRUE);
    gtk_widget_add_css_class(historyScroll, "section-frame");

    history_box_ = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    auto* histHint = gtk_label_new("No history yet");
    gtk_widget_add_css_class(histHint, "empty-hint");
    gtk_box_append(GTK_BOX(history_box_), histHint);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(historyScroll), history_box_);
    gtk_box_append(GTK_BOX(mainBox), historyScroll);

    // --- Variables section ---
    auto* varsHeader = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    auto* varsTitle = gtk_label_new("VARIABLES");
    gtk_widget_add_css_class(varsTitle, "section-header");
    gtk_label_set_xalign(GTK_LABEL(varsTitle), 0);
    gtk_widget_set_hexpand(varsTitle, TRUE);

    auto* clearVarsBtn = gtk_button_new_with_label("Clear");
    gtk_widget_add_css_class(clearVarsBtn, "small-button");
    g_signal_connect(clearVarsBtn, "clicked", G_CALLBACK(onClickClearVars), this);

    gtk_box_append(GTK_BOX(varsHeader), varsTitle);
    gtk_box_append(GTK_BOX(varsHeader), clearVarsBtn);
    gtk_box_append(GTK_BOX(mainBox), varsHeader);

    auto* varsScroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(varsScroll),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(varsScroll, -1, 120);
    gtk_widget_add_css_class(varsScroll, "section-frame");

    variables_box_ = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    auto* varsHint = gtk_label_new("No variables defined");
    gtk_widget_add_css_class(varsHint, "empty-hint");
    gtk_box_append(GTK_BOX(variables_box_), varsHint);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(varsScroll), variables_box_);
    gtk_box_append(GTK_BOX(mainBox), varsScroll);

    // Show
    gtk_window_present(GTK_WINDOW(window_));
    gtk_widget_grab_focus(input_entry_);
}

} // namespace calc
