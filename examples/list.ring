# List Example

myList = [
	"one",
	"two",
	"three",
	"four",
	"five"
]

clear_screen()
set_color(255, 255, 255) 
draw_text(10, 10, "List Example")

y_position = 40
for item in myList
	draw_text(10, y_position, item)
	y_position = y_position + 20
next

flush_screen()
