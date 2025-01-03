# Project: DOS Shell and Text Editor

## Overview
This repository contains the implementation of a DOS shell system with foundational file and directory management functionalities, and a modular design for extending functionalities, including a text editor component. The project is divided into multiple header files for modularity and a main implementation file (`DOS.cpp`) to demonstrate the shell's capabilities.

### Key Components
1. **DOS Shell System**
   - Implements a virtual directory and file management system.
   - Provides functionalities to create, manage, and delete files and directories.

2. **Text Editor (Partially Implemented)**
   - The foundation for a text editor is provided through modular header files.
   - Full text editing functionality is planned for future updates.

### File Descriptions
- **TextEditor.h**: Placeholder for text editing functionalities.
- **Directory.h**: Defines the structure and operations for directories.
- **Document.h**: Manages document-related functionalities within the shell.
- **DOS.cpp**: Main program file containing the DOS shell interface.
- **File.h**: Provides file structure and related operations.
- **Line.h**: Handles line-based content, useful for text editing.
- **Node.h**: Defines the node structure for linked data management.

## How to Run

### Prerequisites
- A C++ compiler (e.g., GCC, Clang, or MSVC).
- A code editor or IDE supporting C++ (e.g., Visual Studio Code, CLion, or Code::Blocks).

### Steps
1. **Clone the Repository**
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. **Compile the Code**
   Use a C++ compiler to compile `DOS.cpp` and the associated header files. Example using `g++`:
   ```bash
   g++ DOS.cpp -o dos_shell
   ```

3. **Run the Program**
   Execute the compiled binary:
   ```bash
   ./dos_shell
   ```

### Notes
- Ensure all header files are in the same directory as `DOS.cpp` during compilation.
- This project is designed as a modular system; additional features can be added by creating new headers or extending existing ones.

## Future Enhancements
- Full implementation of the text editor functionalities.
- Integration of a more comprehensive file management system.
- Advanced shell commands for enhanced user experience.

## Contributing
Contributions are welcome! Feel free to open issues or submit pull requests for new features or bug fixes.

## License
This project is licensed under the [MIT License](LICENSE).

