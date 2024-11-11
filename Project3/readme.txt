1. Part 1:
1.1 Project Overview:
The program reads a sequence of virtual addresses from an input file, translates them to physical addresses using a hardcoded page table, and writes the physical addresses to an output file. Additionally, it computes the MD5 checksum of the output file.

Input: A file containing a sequence of virtual addresses (binary format)
Output: A file containing the corresponding physical addresses (text format) and an MD5 checksum file

1.2. Components:
- Input Handling: Read the input sequence of virtual addresses from a file.
- Translation Logic: Use a hardcoded page table to translate virtual addresses to physical addresses.
- Output Handling:
    Write the translated physical addresses to an output file.
    Compute and store the MD5 checksum of the output file.

1.3. Program Flow:
Main Function:
    - Validate the number of command-line arguments.
    - Open the input file for reading.
    - Open the output file for writing.
    - Reading and Translation:
        Read virtual addresses from the input file.
        For each virtual address, call the translate_address function to convert it to a physical address.
    - Translation Function:
        Extract the page number and offset from the virtual address.
        Validate the page number.
        Use the page table to find the corresponding frame number.
        Calculate the physical address using the frame number and offset.
    - Output and Checksum: Write the physical address to the output file.
    - After processing all addresses, close the input and output files.
    - Compute the MD5 checksum of the output file and store it in a result file.

2. Part 2:
