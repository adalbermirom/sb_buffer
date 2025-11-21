## SB_Buffer 🚀

SB_Buffer (Stack Buffer) is a lightweight, high-performance dynamic string library for C.

It features Small String Optimization (SSO), allowing strings up to 256 bytes to be allocated directly on the stack (zero heap allocation overhead). It automatically switches to heap allocation for larger strings when necessary.

## ✨ Features

- ⚡ Zero Allocation for Small Strings: Uses an internal stack buffer (defaults to 256 bytes) for maximum speed.

- 🛡️ Safe & Robust: Includes magic number signatures to prevent usage of uninitialized or invalid pointers.

- 📈 Automatic Growth: Seamlessly switches to malloc/realloc when string length exceeds the stack capacity.

- 🧹 Memory Management: Clear distinction between Soft Reset (reuse memory) and Hard Reset (free memory).

- 📋 Independent Copy: Safe deep-copy function (sb_buffer_copy) that handles allocation automatically.

## 📂 Project Structure

```text 
 SB_Buffer/ 
 ├── include/
 │   └── sb_buffer.h # Public API
 ├── src/
 │   └── sb_buffer.c # Implementation 
 └── samples/ # Examples and Tests
```

## 🛠️ Integration

To use SB_Buffer in your project, simply include the header and compile the source file.

Copy include/sb_buffer.h and src/sb_buffer.c to your project.

Include the header:

```c 
 #include "sb_buffer.h"
```

## 🚀 Quick Start

Here is a simple example of how to initialize, use, and finalize a buffer.

```c 
#include <stdio.h> 
#include "sb_buffer.h"

int main() { // 1. Declaration (Allocated on Stack - Fast!) SB_Buffer b;

// 2. Initialization (Sets up safety checks and internal pointers)
sb_buffer_init(&b);

// 3. Adding Data
sb_buffer_add_string(&b, "Hello");
sb_buffer_add_literal(&b, " World"); // Optimized for literals

// 4. Accessing Data
printf("Result: %s\n", sb_buffer_get_str(&b));
printf("Length: %zu\n", sb_buffer_get_len(&b));

// 5. Cleanup (Frees memory ONLY if heap was used)
sb_buffer_finalize(&b);

return 0;
}
```

## Compiling the Example

```bash 
  cd samples
  gcc -o my_app main.c ../src/sb_buffer.c -I../include -Wall -std=c99
  
```

## 📖 Key Concepts

 **Stack vs Heap (SSO)**

- < 256 bytes: The data lives inside the struct variable itself (Stack). malloc is never called.

- \> 256 bytes: The library automatically allocates memory on the Heap.

## Reset vs Clear

- sb_buffer_clear(&b): (Soft Reset) Sets length to 0 but keeps the allocated memory. Use this inside loops for maximum performance.

- sb_buffer_reset(&b): (Hard Reset) Frees the heap memory (if any) and points back to the internal stack buffer.

## 💡 Pro Tip: The One-Liner Macro

If you are using GCC/Clang and want a convenient way to declare and init inside functions:

```c 

int main(int argc, char **argv) { 

    SB_Buffer SB_INIT(my_buf); // Declares AND initializes!

    sb_buffer_add_string(&my_buf, "It works!");
    
    printf("Output: %s\n", my_buf.str);
    
    sb_buffer_finalize(&my_buf);

    return 0;
}
```

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.
