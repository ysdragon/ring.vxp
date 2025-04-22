# Spinner Animation Example

cx = screen_width() / 2
cy = screen_height() / 2
radius = 25
angle = 0
speed_angle = 15

for i = 1 to 200
    clear_screen()

    rad = angle * 3.14159 / 180
    end_x = floor(cx + radius * cos(rad))
    end_y = floor(cy + radius * sin(rad))

    set_color(255, 255, 255)

    draw_line(cx, cy, end_x, end_y)

    fill_rect(cx-1, cy-1, 3, 3)

    flush_screen()

    angle = (angle + speed_angle) % 360

    sleep(50)
next

# Final message
clear_screen()
draw_text(10, cy - 10, "Spinner animation complete!", 0, 255, 0)
flush_screen()