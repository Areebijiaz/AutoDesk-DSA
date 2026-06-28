AutoDesk DSA

A console-based car dealership management system built in C++ using core data structures — linked list for inventory, stack for soft deletes, and dual Binary Search Trees for fast search.

Data Structures Used

StructurePurposeLinked ListStores and manages full car inventoryStackRecycle bin for soft-deleted recordsBinary Search Tree (by ID)O(log n) search by vehicle IDBinary Search Tree (by Price)O(log n) search by vehicle priceBinary File I/OPersistent storage across sessions

Features


Role-based login (Admin / User)
Add, update, delete car records
Soft delete with recycle bin (restore or permanently delete)
Search by ID or price in O(log n) time
Persistent data storage using binary files
Clean console UI with input validation


Tech Stack


Language: C++
Paradigm: Object-Oriented Programming
Storage: Binary file persistence (.dat)
IDE: Visual Studio / Code::Blocks


How to Run

bash# Clone the repo
git clone https://github.com/Areebijiaz/AutoDesk-DSA.git
cd AutoDesk-DSA

# Compile
g++ dsa2.cpp -o autoDesk

# Run
./autoDesk

Project Structure

AutoDesk-DSA/
├── dsa2.cpp              # Main source file
├── recycle.dat           # Binary data file (auto-generated)
├── DSA PROJECT REPORT.docx
├── LICENSE
└── README.md

What I Learned


Implementing linked lists, stacks, and BSTs from scratch in C++
Balancing multiple data structures for different search strategies
Binary file I/O for persistent storage without a database
Role-based access control logic in a console app



Made by Areeb Ejaz
