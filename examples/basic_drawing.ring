# Basic Drawing Functions Example

clear_screen()

set_color(255, 255, 255)
draw_text(10, 10, "Hello World!")

draw_text(10, 30, "Colored Text", 255, 0, 0)

set_color(0, 255, 0)
draw_rect(10, 50, 100, 50)

set_color(0, 0, 255)
fill_rect(120, 50, 100, 50)

set_color(255, 255, 0)
draw_line(10, 120, 220, 120)

set_color(255, 0, 255)
for i = 1 to 50
    draw_pixel(i*5, 150)
next

flush_screen()