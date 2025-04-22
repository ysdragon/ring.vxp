# List Index Example

myList = [
	"one",
	"two",
	"three",
	"four",
	"five"
]

clear_screen()
set_color(255, 255, 255)
draw_text(10, 10, "List Index Example")

for i = 1 to len(myList)
	item = myList[i]
	draw_text(10, 40 + ((i-1) * 20), item + " (index: " + i + ")")
next

flush_screen()