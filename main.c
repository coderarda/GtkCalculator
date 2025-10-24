#include <gtk/gtk.h>

// Define a data structure to hold multiple pieces of information
typedef struct {
    GtkWidget* result_label;
    GtkWidget* operator_label;  // For showing current operation
    double operand1;
    double operand2;
    char operation;
    gboolean has_operand1;
} CalculatorData;

static void center_widget(GtkWidget* widget) {
    gtk_widget_set_halign(widget, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(widget, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(widget, TRUE);
    gtk_widget_set_vexpand(widget, TRUE);
}

// Ensure the box is blank before writing new output.
static void clear_box(GtkWidget* box) {
    GtkWidget *child = gtk_widget_get_first_child(box);
    while (child != NULL) {
        GtkWidget *next_child = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(box), child);
        child = next_child;
    }
}

static void clear_button_clicked_cb(GtkWidget* widget, gpointer data) {
    CalculatorData* calc_data = (CalculatorData*)data;
    gtk_label_set_text(GTK_LABEL(calc_data->result_label), "0");
    calc_data->operand1 = 0;
    calc_data->operand2 = 0;
    calc_data->operation = '\0';
    calc_data->has_operand1 = FALSE;
}

static void equals_button_clicked_cb(GtkWidget* widget, gpointer data) {
    CalculatorData* calc_data = (CalculatorData*)data;
    const char* current_text = gtk_label_get_text(GTK_LABEL(calc_data->result_label));
    
    if (calc_data->has_operand1) {
        calc_data->operand2 = g_strtod(current_text, NULL);
        double result = 0.0;

        switch (calc_data->operation) {
            case '+':
                result = calc_data->operand1 + calc_data->operand2;
                break;
            case '-':
                result = calc_data->operand1 - calc_data->operand2;
                break;
            case '*':
                result = calc_data->operand1 * calc_data->operand2;
                break;
            case '/':
                if (calc_data->operand2 != 0) {
                    result = calc_data->operand1 / calc_data->operand2;
                } else {
                    gtk_label_set_text(GTK_LABEL(calc_data->result_label), "Error");
                    return;
                }
                break;
            default:
                return;
        }

        char result_str[64];
        g_snprintf(result_str, sizeof(result_str), "%g", result);
        gtk_label_set_text(GTK_LABEL(calc_data->result_label), result_str);
        calc_data->has_operand1 = FALSE; // Reset for next operation
    }
}

static void button_clicked_cb(GtkWidget* widget, gpointer data) {
    CalculatorData* calc_data = (CalculatorData*)data;
    const char* existing = gtk_label_get_text(GTK_LABEL(calc_data->result_label));
    const char* fragment = gtk_button_get_label(GTK_BUTTON(widget));

    if (existing && g_strcmp0(existing, "0") == 0) {
        existing = "";
    }
    char* combined = g_strconcat(existing ? existing : "", fragment, NULL);
    gtk_label_set_text(GTK_LABEL(calc_data->result_label), combined);
    g_free(combined);
}

static void operator_button_clicked_cb(GtkWidget* widget, gpointer data) {
    CalculatorData* calc_data = (CalculatorData*)data;
    const char* current_text = gtk_label_get_text(GTK_LABEL(calc_data->result_label));
    
    // Store the first operand
    calc_data->operand1 = g_strtod(current_text, NULL);
    calc_data->operation = gtk_button_get_label(GTK_BUTTON(widget))[0];
    calc_data->has_operand1 = TRUE;
    
    // Clear display for next number
    gtk_label_set_text(GTK_LABEL(calc_data->result_label), "0");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* box;
    GtkWidget* result_box;
    GtkWidget* vbox;
    
    // Allocate calculator data first
    CalculatorData* calc_data = g_new0(CalculatorData, 1);
    
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    center_widget(vbox);
    // Wrap this box with another box that is horizontal.
    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    center_widget(hbox);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    center_widget(box);
    result_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    center_widget(result_box);

    GtkWidget* result_label = gtk_label_new("0");
    center_widget(result_label);
    calc_data->result_label = result_label;
    gtk_box_append(GTK_BOX(result_box), result_label);
    gtk_box_append(GTK_BOX(vbox), result_box);
    for (int i = 0; i < 3; i++) { 
        GtkWidget* box_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        center_widget(box_row);
        for (int j = 1; j <= 3; j++) {
            char* label = g_strdup_printf("%d", i * 3 + j);
            GtkWidget* button = gtk_button_new_with_label(label);
            g_signal_connect(button, "clicked", G_CALLBACK(button_clicked_cb), calc_data);
            g_free(label);
            center_widget(button);
            gtk_box_append(GTK_BOX(box_row), button);
        }
        gtk_box_append(GTK_BOX(box), box_row);
    }
    GtkWidget* new_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* col1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget* col2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    center_widget(new_row);
    center_widget(col2);
    center_widget(col1);
    GtkWidget* btn_plus = gtk_button_new_with_label("+");
    GtkWidget* btn_minus = gtk_button_new_with_label("-");
    GtkWidget* btn_multiply = gtk_button_new_with_label("*");
    GtkWidget* btn_divide = gtk_button_new_with_label("/");
    gtk_box_append(GTK_BOX(col1), btn_multiply);
    gtk_box_append(GTK_BOX(col1), btn_divide);
    gtk_box_append(GTK_BOX(col1), btn_plus);
    gtk_box_append(GTK_BOX(col1), btn_minus);

    g_signal_connect(btn_plus, "clicked", G_CALLBACK(operator_button_clicked_cb), calc_data);
    
    GtkWidget* btn_equals = gtk_button_new_with_label("=");
    GtkWidget* btn_clear = gtk_button_new_with_label("C");
    GtkWidget* button0 = gtk_button_new_with_label("0");
    gtk_box_append(GTK_BOX(new_row), btn_clear);
    gtk_box_append(GTK_BOX(new_row), button0);
    gtk_box_append(GTK_BOX(new_row), btn_equals);
    
    g_signal_connect(btn_clear, "clicked", G_CALLBACK(clear_button_clicked_cb), calc_data);
    g_signal_connect(button0, "clicked", G_CALLBACK(button_clicked_cb), calc_data);
    g_signal_connect(btn_equals, "clicked", G_CALLBACK(equals_button_clicked_cb), calc_data);
    g_signal_connect(btn_minus, "clicked", G_CALLBACK(operator_button_clicked_cb), calc_data);
    g_signal_connect(btn_multiply, "clicked", G_CALLBACK(operator_button_clicked_cb), calc_data);
    g_signal_connect(btn_divide, "clicked", G_CALLBACK(operator_button_clicked_cb), calc_data);
    g_signal_connect(btn_equals, "clicked", G_CALLBACK(equals_button_clicked_cb), calc_data);

    gtk_box_append(GTK_BOX(box), new_row);
    gtk_box_append(GTK_BOX(hbox), box);
    gtk_box_append(GTK_BOX(hbox), col1);
    gtk_box_append(GTK_BOX(hbox), col2);
    gtk_box_append(GTK_BOX(vbox), hbox);
    gtk_window_set_child(GTK_WINDOW(window), vbox);
    
    // Store calc_data with window so it can be freed later
    g_object_set_data_full(G_OBJECT(window), "calc_data", calc_data, g_free);
    
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char const *argv[])
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.coderarda.Calculator", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, (char **)argv);
    g_object_unref(app);

    return status;
}
