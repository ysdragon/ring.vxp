# Memory Info Example

memory = memory_size()

memory = "Memory Size: " + format(memory)

draw_text(10, 40, memory, 51, 153, 255)

flush_screen()


func format(memory)
    if memory >= 1024 * 1024
        formatted = "" + (memory / (1024 * 1024)) + " MB"
    elseif memory >= 1024
        formatted = "" + (memory / 1024) + " KB"
    else
        formatted = "" + memory + " bytes"
    ok

    return formatted
end

