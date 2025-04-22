# Ring MRE Extension Documentation

This document outlines the functions provided by the Ring MRE extension, enabling interaction with the MRE platform features.

## Graphics Functions

### `draw_text(x, y, text_string, [r, g, b])`
Draws the specified `text_string` at coordinates (`x`, `y`).
- **Parameters:**
    - `x` (Number): The x-coordinate for the text.
    - `y` (Number): The y-coordinate for the text.
    - `text_string` (String): The text to display.
    - `r` (Number, Optional): Red component of the text color (0-255). Defaults to white if not provided.
    - `g` (Number, Optional): Green component of the text color (0-255). Defaults to white if not provided.
    - `b` (Number, Optional): Blue component of the text color (0-255). Defaults to white if not provided.
- **Returns:** None

### `flush_screen()`
Updates the display to show all drawing operations performed since the last flush.
- **Parameters:** None
- **Returns:** None

### `clear_screen()`
Clears the entire screen to black.
- **Parameters:** None
- **Returns:** None

### `draw_rect(x, y, width, height)`
Draws the outline of a rectangle using the current color set by `set_color()`.
- **Parameters:**
    - `x` (Number): The x-coordinate of the top-left corner.
    - `y` (Number): The y-coordinate of the top-left corner.
    - `width` (Number): The width of the rectangle.
    - `height` (Number): The height of the rectangle.
- **Returns:** None

### `fill_rect(x, y, width, height)`
Draws a filled rectangle using the current color set by `set_color()`.
- **Parameters:**
    - `x` (Number): The x-coordinate of the top-left corner.
    - `y` (Number): The y-coordinate of the top-left corner.
    - `width` (Number): The width of the rectangle.
    - `height` (Number): The height of the rectangle.
- **Returns:** None

### `draw_line(x1, y1, x2, y2)`
Draws a line between two points (`x1`, `y1`) and (`x2`, `y2`) using the current color.
- **Parameters:**
    - `x1` (Number): The x-coordinate of the starting point.
    - `y1` (Number): The y-coordinate of the starting point.
    - `x2` (Number): The x-coordinate of the ending point.
    - `y2` (Number): The y-coordinate of the ending point.
- **Returns:** None

### `draw_pixel(x, y)`
Draws a single pixel at coordinates (`x`, `y`) using the current color.
- **Parameters:**
    - `x` (Number): The x-coordinate of the pixel.
    - `y` (Number): The y-coordinate of the pixel.
- **Returns:** None

### `set_color(r, g, b)`
Sets the current drawing color used by subsequent drawing functions (rectangles, lines, pixels).
- **Parameters:**
    - `r` (Number): Red component (0-255).
    - `g` (Number): Green component (0-255).
    - `b` (Number): Blue component (0-255).
- **Returns:** None

### `background_color(r, g, b)`
Sets the background color and immediately fills the entire screen with it.
- **Parameters:**
    - `r` (Number): Red component (0-255).
    - `g` (Number): Green component (0-255).
    - `b` (Number): Blue component (0-255).
- **Returns:** None

### `screen_width()`
Returns the width of the device screen in pixels.
- **Parameters:** None
- **Returns:** (Number) The screen width.

### `screen_height()`
Returns the height of the device screen in pixels.
- **Parameters:** None
- **Returns:** (Number) The screen height.

## System Functions

### `sleep(milliseconds)`
Pauses the execution of the script for the specified duration.
- **Parameters:**
    - `milliseconds` (Number): The duration to sleep in milliseconds.
- **Returns:** None

### `memory_size()`
Returns the total memory available to the MRE application in bytes.
- **Parameters:** None
- **Returns:** (Number) The total memory size.

## Time Functions

### `clock()`
Returns the number of milliseconds that have elapsed since the application started.
- **Parameters:** None
- **Returns:** (Number) Milliseconds since start.

### `clockspersecond()`
Returns the number of clock ticks per second. For MRE, this is typically fixed at 1000.
- **Parameters:** None
- **Returns:** (Number) Always 1000.0.

### `time()`
Returns the current system time as a string.
- **Parameters:** None
- **Returns:** (String) The current time in "HH:MM:SS" format.

### `date()`
Returns the current system date as a string.
- **Parameters:** None
- **Returns:** (String) The current date in "DD/MM/YYYY" format.
