# Mini-Shell

This is a simple mini-shell written in C to immitate basics commands
of a Linux terminal or Bash. It serves as a practice to apply my 
knowledge learned in The C Programming Language book by K&R.

## Project Structure:
```bash
~/C_PRACTICE/
└── projects/
    └── mini-shell/
        ├── bin/
        ├── build/
        ├── src/
        │  ├── main.c
        │  ├── tests/
        │  └── mini-shell/
        │       ├── mini-shell.h
        │       └── mini-shell.c
        ├── docs.txt
        └── CMakeLists.txt
```

## How to build

### Build commands

From the build/ directory:
```bash
cd ~/C_PRACTICE/projects/mini-shell/build
cmake ..
make
```

### Result

After building, you'll get:

```bash
~/C_PRACTICE/projects/mini-shell/bin/mini-shell
```
