#include <stdio.h>
#include <string.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_opengl.h>
#include <GTK/GTK.h>

char *altura;
char *ancho;

GtkWidget *ventana;
GtkWidget *ventana1;
GtkWidget *label;
GtkWidget *label1;
GtkWidget *label2;
GtkWidget *label3;
GtkWidget *entry;
GtkWidget *entry1;
GtkWidget *entry2;
GtkWidget *entry3;
GtkWidget *box;
GtkWidget *button;
GtkWidget *button1;
GtkWidget *button2;
GtkWidget *button3;


ALLEGRO_DISPLAY *display;
ALLEGRO_DISPLAY *display1;
ALLEGRO_BITMAP  *image;
ALLEGRO_FILECHOOSER *filechooser;

void cargarImagen()
{
    int alt = 0;
    int anch = 0;
    int cont = 1;
    int i;
    int n;

    char *fin1 = altura;
    char *fin2 = ancho;

    while (*fin1 != '\0')
    {
        fin1++;
    }
    fin1--;

    while (*fin2 != '\0')
    {
        fin2++;
    }
    fin2--;

    for(i = strlen(altura) - 1; i >= 0 ; i--){
        char n1 = *((char*)fin1);
        n = ((int)n1-'0') * cont;
        alt = alt+n;
        cont = cont*10;
        fin1--;
    }
    cont = 1;
    for(i = strlen(ancho) - 1; i >= 0 ; i--){
        char n1 = *((char*)fin2);
        n = ((int)n1-'0') * cont;
        anch = anch+n;
        cont = cont*10;
        fin2--;
    }
    char *dirImagen;

    if(!al_init())
        al_show_native_message_box(NULL, NULL, NULL, "Could not initialize Allegro 5", NULL, NULL);

    if(!al_init_image_addon()) {
      al_show_native_message_box(display, "Error", "Error", "Error al inicializar imagen!",NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
    }


    filechooser = al_create_native_file_dialog("C:", "Choose a file.", "*.png;*.jpg;*.tif;*.bmp;", 1);
    al_show_native_file_dialog (display, filechooser);

    dirImagen =  al_get_native_file_dialog_path(filechooser, 0);
    image = al_load_bitmap(dirImagen);
    int alt1 = al_get_bitmap_height (image);
    int anch1 = al_get_bitmap_width (image);
    display = al_create_display(anch1, alt1);

    if(!display)
        al_show_native_message_box(NULL, NULL, NULL, "Could not create Allegro 5 display", NULL, NULL);

    al_draw_scaled_bitmap (image, 0, 0, al_get_bitmap_width (image), al_get_bitmap_height (image), 0, 0, anch1, alt1, 0);

    if(!image) {
        al_show_native_message_box(display, "Error", "Error", "Error al Cargar Imagen!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);

        al_destroy_display(display);
        return 0;
    }
    al_flip_display();

}

GtkWidget* cargarImagenAux(gpointer data){

    altura = gtk_entry_get_text(GTK_ENTRY(entry));
    ancho = gtk_entry_get_text(GTK_ENTRY(entry1));
    gtk_widget_hide(button);
    gtk_widget_hide(button1);
    gtk_widget_hide(button2);
    gtk_widget_show (label);
    gtk_widget_show (label1);
    gtk_widget_show (entry);
    gtk_widget_show (entry1);
    gtk_widget_show(button1);
    cargarImagen();
}


GtkWidget* CambiarTamanoImagenAux(gpointer data){

    gtk_widget_hide (label);
    gtk_widget_hide (label1);
    gtk_widget_hide (entry);
    gtk_widget_hide (label2);
    gtk_widget_hide (entry1);
    gtk_widget_hide (entry2);
    gtk_widget_hide(button1);
    gtk_widget_show (label2);
    gtk_widget_show (entry2);
    gtk_widget_show(button2);
    al_destroy_display(display);
    char *alto = gtk_entry_get_text(GTK_ENTRY(entry));
    char *ancho = gtk_entry_get_text(GTK_ENTRY(entry1));
    int x = atoi(alto);
    int y = atoi(ancho);

    if(x==0 || y==0){
        display1 = al_create_display(al_get_bitmap_width (image), al_get_bitmap_height (image));
        al_draw_scaled_bitmap (image, 0, 0, al_get_bitmap_width (image), al_get_bitmap_height (image), 0, 0,
                               al_get_bitmap_width (image), al_get_bitmap_height (image), 0);
    }
    else{
        display1 = al_create_display(x,y);
        al_draw_scaled_bitmap (image, 0, 0, al_get_bitmap_width (image), al_get_bitmap_height (image), 0, 0, x, y, 0);
    }
    al_flip_display();
}

GtkWidget* agregarTextoImagenAux(gpointer data){
    gtk_widget_hide (label2);
    gtk_widget_hide (entry2);
    gtk_widget_hide (button2);
    gtk_widget_show (label3);
    gtk_widget_show (entry3);
    gtk_widget_show (button3);
    char *alto = gtk_entry_get_text(GTK_ENTRY(entry));
    char *ancho = gtk_entry_get_text(GTK_ENTRY(entry1));
    int x = atoi(alto);
    int y = atoi(ancho);
    al_init_font_addon();
    al_init_ttf_addon();



    ALLEGRO_FONT *texto1 = al_load_font("TequilaSunset-Regular.ttf", 15, NULL);
    al_draw_text(texto1, al_map_rgb(255, 255, 255), ((al_get_display_width(display1))/4), ((al_get_display_height(display1))/4), 0, gtk_entry_get_text(GTK_ENTRY(entry2)));
    al_flip_display();
}

GtkWidget* guardar(gpointer data){
    char *nombre = gtk_entry_get_text(GTK_ENTRY(entry3));
    al_save_bitmap(nombre, al_get_backbuffer(display));
    exit(0);
}


int main(int argc, char *argv[]){

    gtk_init (&argc, &argv);

    ventana = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(ventana), 0, 0);
    label = gtk_label_new_with_mnemonic("  Insertar la Altura: ");
    label1 = gtk_label_new_with_mnemonic("  Insertar el Ancho:  ");
    label2 = gtk_label_new_with_mnemonic("  Insertar el Texto:  ");
    label3 = gtk_label_new_with_mnemonic("  Escriba el nombre con la extension  ");
    entry = gtk_entry_new();
    entry1 = gtk_entry_new();
    entry2 = gtk_entry_new();
    entry3 = gtk_entry_new();

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    button = gtk_button_new_with_label("Seleccionar Imagen");
    button1 = gtk_button_new_with_label("Cambiar de tamanno");
    button2 = gtk_button_new_with_label("Agregar texto");
    button3 = gtk_button_new_with_label("Guardar");
    gtk_widget_set_size_request(button, 150, 1);
    gtk_widget_set_size_request(button1, 150, 1);
    gtk_widget_set_size_request(button2, 150, 1);

    gtk_container_add (GTK_CONTAINER (ventana), box);
    gtk_container_add (GTK_CONTAINER (box), label);
    gtk_container_add (GTK_CONTAINER (box), entry);
    gtk_container_add (GTK_CONTAINER (box), label1);
    gtk_container_add (GTK_CONTAINER (box), entry1);
    gtk_container_add (GTK_CONTAINER (box), label2);
    gtk_container_add (GTK_CONTAINER (box), label3);
    gtk_container_add (GTK_CONTAINER (box), entry2);
    gtk_container_add (GTK_CONTAINER (box), entry3);
    gtk_container_add (GTK_CONTAINER (box), button1);
    gtk_container_add (GTK_CONTAINER (box), button2);
    gtk_container_add (GTK_CONTAINER (box), button);
    gtk_container_add (GTK_CONTAINER (box), button3);

    g_signal_connect (button, "clicked", G_CALLBACK(cargarImagenAux), NULL);
    g_signal_connect (button1, "clicked", G_CALLBACK(CambiarTamanoImagenAux), NULL);
    g_signal_connect (button2, "clicked", G_CALLBACK(agregarTextoImagenAux), NULL);
    g_signal_connect (button3, "clicked", G_CALLBACK(guardar), NULL);

    gtk_widget_show (ventana);
    gtk_widget_show (box);
    gtk_widget_show (button);
    gtk_main ();

    return 0;
}
