# Debug documentation
This is the documentation for the "Debug2" header file (DBG). This overwrites the standard malloc, realloc and free and adds in a list of all the allocated memory which at the end of the program can be retrieved to check for memory leaks and overflow errors. It is not guarenteed to catch overflow errors.

## Setup
---
To use debugging just include the debugging library before any other library which should be debugged. Then add the last line of the main function before returning run DBG_MemoryLog(File) to print the log to a file or DBG_MemoryPrint() to print the log to stdout. Only one of them can be used.

---
## Functions:
---

**void DBG_MemoryLog(FILE *File)**:
- Writes a list of all still allocated memory to a file, cannot be run twice
- **Return**: Void
- **Arguments**:
  - **File**: The file to write to.

**void DBG_MemoryPrint()**:
- Prints a list of all still allocated memory, cannot be run twice
- **Return**: Void

---