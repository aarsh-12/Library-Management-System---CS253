# Library Management System

## How to Run the Program

### 1. Extract the ZIP file  
Unzip the provided file to a directory of your choice.

### 2. Compile the program  
Use the following command to compile the C++ source file:  
```sh
g++ 1.cpp -o library_system
```

### 3. Run the program  
After successful compilation, run the program using:  
```sh
./library_system
```

### 4. Ensure database files are in place  
Make sure the `DB` folder is in the same directory as the executable. The `DB` folder should contain the following files:  
- `books.csv`
- `student.csv`
- `faculty.csv`
- `librarian.csv`

These files store system data and are required for proper functionality.

## Project Structure
The repository is structured as follows:

### **DB Folder**
This folder serves as the database and contains four CSV files to store system information:

- **books.csv** – Stores book details with columns: `ISBN`, `Title`, `Author`, `Publisher`, `Publication Year`, and `Status`.
- **student.csv** – Stores student details with columns: `UserID`, `Password`, and `Name`.
- **faculty.csv** – Stores faculty details with columns: `UserID`, `Password`, and `Name`.
- **librarian.csv** – Stores librarian details with columns: `UserID`, `Password`, and `Name`.

### **Main Program**
- **main.cpp** – The core file managing the program’s control flow. It implements various classes and functions required for system operations.
