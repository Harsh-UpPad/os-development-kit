#ifndef OSDK_WIN_CPP_PRINT16_HPP
#define OSDK_WIN_CPP_PRINT16_HPP

namespace os {
    namespace win16 {
        inline void print_string(const char* str) {
            // Loop through each character until it hits the null terminator '\0'
            for (int i = 0; str[i] != '\0'; i++) {
                char c = str[i];

                // Check for a newline character to handle line breaks smoothly
                if (c == '\n') {
                    // Inject Carriage Return (\r) and Line Feed (\n) via BIOS
                    asm volatile (
                        "mov $0x0E, %%ah\n\t"
                        "mov $0x0D, %%al\n\t" // Carriage Return
                        "int $0x10\n\t"
                        "mov $0x0E, %%ah\n\t"
                        "mov $0x0A, %%al\n\t" // Line Feed
                        "int $0x10"
                        : : : "ah", "al"
                    );
                    continue;
                }

                // Call BIOS Interrupt 0x10 Teletype to draw the character on screen
                asm volatile (
                    "mov $0x0E, %%ah\n\t"  // BIOS Teletype function code
                    "mov %0, %%al\n\t"     // Move our character byte into AL register
                    "mov $0x00, %%bh\n\t"  // Display page number 0
                    "mov $0x07, %%bl\n\t"  // Text attribute: Light Grey on Black background
                    "int $0x10"            // Call Video BIOS Interrupt
                    :
                    : "r"(c)               // Pass the character variable to the assembly block
                    : "ah", "al", "bh", "bl" // Tell the compiler these registers are altered
                );
            }
        }
    }
}

#endif // OSDK_WIN_CPP_PRINT16_HPP
