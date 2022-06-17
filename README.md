# COSC4316-Compiler-Language-Translator
Semester project for compiling a program language into assembly.

The compiler is a Deterministic Finite-State Automaton (DFSA) utilizing the bottom-up parse scanner for reading input files[^DFSA]. The syntax analyzer utilizes a Deterministic Pushdown-Automaton (PDA) with a Precedence Table and an Operator Precedence Table that is automatically generated from a grammar file (much like Flex & Bison)[^PDA]. Flex and Bison are general-purpose parser generator tools that reads a grammar file and parses any string, based on said grammar file. This project's precedence table generation utilizes this same approach. Using Floyd's approach to generate the precedence tables, a small number of terminal character combinations will hold more than one precedence relations[^Floyd]. The analyzer is capable of handling such special cases because the code is tailored to handle the grammar.

### Notes
- The compiler does not inform the user of any syntax errors and fails silently. 
- The assembly code is NASM for Linux and is not optimized.
- Linux terminal command: *nasm -felf64 Java0.asm && ld -o Java0 Java0.o && ./Java0*
- Basic read and write to terminal, prompt only asks for 2 Byte integer.
- No negative values; Everything is treated as unsigned values.


##### Footnotes
[^DFSA]: Deterministic Finite-State Automaton: Each derivation of a string is correct if it exists, otherwise the string is rejected.
[^PDA]: Deterministic Push-Down-Automaton: An automaton that employs a stack. Much like DFSA such that the string is correct if it exists, otherwise it is rejected.
[^Floyd]: Robert W. Floyd. 1963. *Syntactic Analysis and Operator Precedence*. J. ACM 10, 3 (July 1963), 316-333. (https://doi.org/10.1145/321172.321179)
