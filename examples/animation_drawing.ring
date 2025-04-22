# Animation Drawing Example

rect_x = 10
rect_y = 40
rect_w = 30
rect_h = 20

speed_x = 2
speed_y = 1

s_width = screen_width()
s_height = screen_height()

for i = 1 to 500
    clear_screen()

    set_color(0, 0, 255)

    fill_rect(rect_x, rect_y, rect_w, rect_h)

    flush_screen()

    rect_x = rect_x + speed_x
    rect_y = rect_y + speed_y

    if rect_x + rect_w > s_width or rect_x < 0
        speed_x = -speed_x
        rect_x = rect_x + speed_x
    ok

    if rect_y + rect_h > s_height or rect_y < 0
        speed_y = -speed_y
        rect_y = rect_y + speed_y
    ok

    sleep(0.01)
next

clear_screen()
draw_text(10, 100, "Drawing animation complete!", 0, 255, 0)
flush_screen()