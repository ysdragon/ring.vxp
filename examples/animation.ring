# Text Animation Example

text_to_animate = "Ring " + version() + " on MRE!"
text_x = 10
text_y = 100

speed_x = 3

s_width = screen_width()
s_height = screen_height()
text_width_estimate = 135

for i = 1 to 500
    clear_screen()

    set_color(255, 255, 0)

    draw_text(text_x, text_y, text_to_animate)

    flush_screen()

    text_x = text_x + speed_x
    if (text_x + text_width_estimate) > s_width or text_x < 0
        speed_x = -speed_x
        text_x = text_x + speed_x
    ok

    sleep(5)
next

clear_screen()
draw_text(10, 100, "Text animation complete!", 0, 255, 0)
flush_screen()