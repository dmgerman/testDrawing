/* gcc -o motion-snake motion-snake.c `pkg-config --cflags --libs gtk+-3.0` */
#include <gtk/gtk.h>
#include <math.h>

#define SNAKE_LENGTH 300
double cursor_x[SNAKE_LENGTH], cursor_y[SNAKE_LENGTH];
int snake_head = 0;

GtkAdjustment *adjustment;

static void
on_motion_notify (GtkWidget      *window,
                  GdkEventMotion *event)
{
  if (event->window == gtk_widget_get_window (window))
    {
      int i;
      cursor_x[snake_head] = event->x;
      cursor_y[snake_head] = event->y;
      snake_head = (snake_head + 1) % SNAKE_LENGTH;
      gtk_widget_queue_draw (window);
    }
}

static void
on_draw (GtkWidget *window,
         cairo_t   *cr)
{
  int i, j;
  float displaying_ms = gtk_adjustment_get_value (adjustment);
  g_usleep (displaying_ms * 1000);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_set_source_rgb (cr, 0, 0.5, 0.5);

  j = snake_head;
  cairo_move_to (cr, cursor_x[j], cursor_y[j]);
  double color = 0;
  double color2 = 0;
  for (i=0; i<SNAKE_LENGTH; i++)
    {
        color +=0.1;
        if (color > 0.8) color = 0;
        color2 += 0.2;
        if (color2 > 0.8) color2 = 0;
        if (i %2 == 0)
            cairo_set_source_rgba(cr, color, color2,0.9, 1.0);
        else
            cairo_set_source_rgba(cr, 0.9, 0.9,0.9, 1.0);
        if (j > 1)
            cairo_move_to (cr, cursor_x[j-1], cursor_y[j-1]);
            cairo_line_to (cr, cursor_x[j], cursor_y[j]);
        cairo_stroke (cr);

      j = (j + 1) % SNAKE_LENGTH;
    }
}

int
main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *label;
  GtkWidget *scale;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);
  gtk_widget_set_app_paintable (window, TRUE);
  gtk_widget_add_events (window, GDK_POINTER_MOTION_MASK);
  gtk_widget_set_app_paintable (window, TRUE);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  adjustment = gtk_adjustment_new (20, 0, 200, 1, 10, 0);
  scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, adjustment);
  gtk_box_pack_end (GTK_BOX (vbox), scale, FALSE, FALSE, 0);

  label = gtk_label_new ("Screen update time (ms):");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_box_pack_end (GTK_BOX (vbox), label, FALSE, FALSE, 0);

  g_signal_connect (window, "motion-notify-event",
                    G_CALLBACK (on_motion_notify), NULL);
  g_signal_connect (window, "draw",
                    G_CALLBACK (on_draw), NULL);
  g_signal_connect (window, "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);

  gtk_widget_show_all (window);
  
  GdkWindow *gdk_window = gtk_widget_get_window(window);
  gdk_window_set_event_compression (gdk_window, FALSE);
  
  gtk_main ();

  return 0;
}
