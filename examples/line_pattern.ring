# Line Pattern Example

width = screen_width()
height = screen_height()

pattern_step = 10
offset = 0
frame = 0

for i = 1 to 100
    frame++
    
    clear_screen()
    
    set_color(0, 255, 0)
    
    for j = 0 to width step pattern_step
        draw_line(j, 0, width-j, height)
    next
    
    for j = 0 to height step pattern_step
        draw_line(0, j, width, height-j)
    next
    
    set_color(255, 255, 255)
    draw_text(5, 5, "Frame: " + frame)
    
    flush_screen()
    
    offset += 1
    if offset >= pattern_step
        offset = 0
    ok
    
    sleep(50)
next

clear_screen()
set_color(255, 255, 255)
draw_text(10, 40, "Line Pattern Animation")
draw_text(10, 60, "Complete!", 0, 255, 0)

flush_screen()
