# Device Information Example

clear_screen()

width = screen_width()
height = screen_height()
memory = memory_size() / 1024 / 1024

lines = [
	"Screen Information",
	"- Width: " + width,
	"- Height: " + height,
	"Device Information",
	"- Architecture: " + getarch(),
	"- Memory Size: " + memory + " MB",
]

x_pos = 10
y_pos = 10
line_height = 18
indent = 15

for i = 1 to len(lines)
	current_x = x_pos
	if left(lines[i], 1) = "-"
		current_x += indent
	ok
	
	if left(lines[i], 1) != "-"
		draw_text(current_x, y_pos, lines[i], 150, 200, 255)
	else
		draw_text(current_x, y_pos, lines[i], 200, 255, 200)
	ok
	
	y_pos += line_height
	
	if i = 3 or i = len(lines)
		y_pos += (line_height / 2)
	ok
next

flush_screen()