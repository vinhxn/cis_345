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
1.4. Output
File "part1out": sample content as below:
0x023C
0x02EC
0x00E0
0x02A0
0x036C
0x034C
0x0168
0x02DC
0x00D0
0x0284
0x0098
0x0378
0x02F8
0x02D0
0x0330
0x02B8
0x0314
0x02B4
0x0378
0x0118

File "p1result.txt":
d4b5efb200a7e6adfccc5202a6e8d7a3  part1out

2. Part 2:
2.1 Project Overview:
    Source Files: phyframe.h, phyframe.c, pagetable.h, pagetable.c, mempart2.c
    Input: A file containing a sequence of virtual addresses (binary format)
    Output: A file containing the corresponding physical addresses (text format) and a file with the MD5 checksum and page fault count
2.2. Components:
- Physical Frame Management (phyframe.c):
    Manage the allocation and deallocation of physical frames.
    Implement the LRU page replacement policy.
- Page Table Management (pagetable.c):
    Manage the page table entries for the process.
    Implement reverse mapping to quickly locate which page table entry is pointing to a specific physical frame.
- Main Program (mempart2.c):
    Read the input sequence of virtual addresses.
    Translate virtual addresses to physical addresses using the implemented page table.
    Handle input/output operations.
    Report the number of page faults.
2.3. Program Flow:
- Main Program (mempart2.c):
    Validate the number of command-line arguments.
    Open the input file for reading.
    Open the output file for writing.
    Initialize the page table and physical frame management.
    Read virtual addresses from the input file.
    For each virtual address, translate it to a physical address using the page table.
    Handle page faults and allocate/reallocate physical frames as needed.
    Write the translated physical address to the output file.
    Compute the MD5 checksum of the output file and store it in a result file.
    Report the number of page faults.

- Physical Frame Management (phyframe.c):
    Implement functions to allocate and deallocate physical frames.
    Implement the LRU page replacement policy.
    Update the LRU information upon each access.

- Page Table Management (pagetable.c):
    Implement functions to manage the page table entries.
    Implement reverse mapping to quickly locate page table entries.
    Update the page table and reverse mapping upon each access.