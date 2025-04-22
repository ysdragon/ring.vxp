# Draw text example

clear_screen()

background_color(36, 39, 58)

lines = [
	"Ring Programming Language",
	"Version: " + version(),
	"Running on MRE",
	"Features:",
	"- Simple",
	"- Cross-platform",
	"- Embeddable"
]

x_pos = 10
y_pos = 10
line_height = 18

for i = 1 to len(lines)
	if i > 4
		x = x_pos + 15
	else
		x = x_pos
	ok
	
	if i = 1
		draw_text(x, y_pos, lines[i], 150, 200, 255)
	elseif i <= 3
		draw_text(x, y_pos, lines[i], 255, 255, 255)
	else
		draw_text(x, y_pos, lines[i], 200, 255, 200)
	ok
	
	y_pos += line_height
	
	if i = 3
		y_pos += (line_height / 2)
	ok
next

flush_screen()