#include <gtk/gtk.h>

void closeApp(GtkWidget* window ,gpointer data)
{
    printf("closeApp");
    gtk_main_quit();
}

gboolean delete_event(GtkWidget* window,GdkEvent*event,gpointer data)
{
    printf("In delete_event");
    return false;
}

void hehe_button_clicked(GtkWidget* button,gpointer data)
{
    printf("hehe_button_clicked");
}

int main(int argc,char*argv[])
{
    GtkWidget *window,*button;
    gtk_init(&argc,&argv);
    window =gtk_window_new(GTK_WINDOW_TOPLEVEL);
    button=gtk_button_new_with_label("hehe");
    gtk_container_add(GTK_CONTAINER(window),button);
    //bind event
    g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(closeApp),NULL);
    g_signal_connect(GTK_OBJECT(window),"delete_event",GTK_SIGNAL_FUNC(delete_event),NULL); 
    g_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(hehe_button_clicked),NULL);
    
    gtk_widget_show(window);
    gtk_widget_show(button);
    gtk_main();

    return 0;
}
