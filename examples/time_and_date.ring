# Time and Date Example

clear_screen()
background_color(0, 0, 50)

set_color(255, 255, 255)
draw_text(10, 20, "Current Date and Time:")

date = date()
time = time()

draw_text(10, 50, "Date: " + date)
draw_text(10, 70, "Time: " + time)

start_time = clock()
draw_text(10, 100, "Clock ticks since start: " + start_time)
draw_text(10, 120, "Ticks per second: " + clockspersecond())

draw_text(10, 150, "Performing 1 second delay...")
flush_screen()

sleep(1000)

end_time = clock()
elapsed = end_time - start_time

draw_text(10, 170, "Time after delay: " + time())
draw_text(10, 190, "Elapsed milliseconds: " + elapsed)

flush_screen()