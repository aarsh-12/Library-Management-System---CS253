#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <tuple>
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;

vector<string> readFromFile(string filename, int columns)
{
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open())
    {
        string line;
        vector<string> data;
        while (getline(file, line))
        {
            stringstream ss(line);
            string item;
            int count = 0;
            while (getline(ss, item, ','))
            {
                data.push_back(item);
                count++;
                if (count == columns)
                {
                    break;
                }
            }
        }
        file.close();
        return data;
    }
    else
    {
        cout << "Error: File not found!" << endl;
        return {};
    }
}

void writeToFile(string filename, vector<string> data, int columns)
{
    fstream file;
    file.open(filename, ios::out);
    if (file.is_open())
    {
        for (int i = 0; i < data.size(); i++)
        {
            file << data[i];
            if ((i + 1) % columns == 0)
            {
                file << "," << endl;
            }
            else
            {
                file << ",";
            }
        }
        file.close();
    }
    else
    {
        cout << "Error: File not found!" << endl;
    }
}

class Book
{
private:
    string isbn;
    string title;
    string author;
    string publisher;
    string publicationYear;
    string status;
    time_t borrowDate;
    time_t dueDate;

public:
    Book();
    Book(string isbn, string title, string author, string publisher, string publicationYear, string status);

    string getIsbn();
    void setIsbn(string isbn);
    string getTitle();
    void setTitle(string title);
    string getAuthor();
    void setAuthor(string author);
    string getPublisher();
    void setPublisher(string publisher);
    string getPublicationYear();
    void setPublicationYear(string publicationYear);
    string getStatus();
    void setStatus(string status);
    time_t getBorrowDate();
    void setBorrowDate(time_t borrowDate);
    time_t getDueDate();
    void setDueDate(time_t dueDate);
};

class Library
{
private:
    vector<Book *> books;

public:
    Library();
    void AddBook(Book *book);
    void RemoveBook(string isbn);
    void UpdateBook(Book *book);
    Book *SelectBook(string isbn);
    vector<Book *> ListBooks();
    void DisplayAllBooks();
    void UpdateBookStatus(string isbn, string status);
    ~Library();
};

Book::Book()
{
    this->isbn = "";
    this->title = "";
    this->author = "";
    this->publisher = "";
    this->publicationYear = "";
    this->status = "";
    this->borrowDate = 0;
    this->dueDate = 0;
}

Book::Book(string isbn, string title, string author, string publisher, string publicationYear, string status)
{
    this->isbn = isbn;
    this->title = title;
    this->author = author;
    this->publisher = publisher;
    this->publicationYear = publicationYear;
    this->status = status;
    this->borrowDate = 0;
    this->dueDate = 0;
}

string Book::getIsbn()
{
    return isbn;
}

void Book::setIsbn(string isbn)
{
    this->isbn = isbn;
}

string Book::getTitle()
{
    return title;
}

void Book::setTitle(string title)
{
    this->title = title;
}

string Book::getAuthor()
{
    return author;
}

void Book::setAuthor(string author)
{
    this->author = author;
}

string Book::getPublisher()
{
    return publisher;
}

void Book::setPublisher(string publisher)
{
    this->publisher = publisher;
}

string Book::getPublicationYear()
{
    return publicationYear;
}

void Book::setPublicationYear(string publicationYear)
{
    this->publicationYear = publicationYear;
}

string Book::getStatus()
{
    return status;
}

void Book::setStatus(string status)
{
    this->status = status;
}

time_t Book::getBorrowDate()
{
    return borrowDate;
}

void Book::setBorrowDate(time_t borrowDate)
{
    this->borrowDate = borrowDate;
}

time_t Book::getDueDate()
{
    return dueDate;
}

void Book::setDueDate(time_t dueDate)
{
    this->dueDate = dueDate;
}

class Account
{
private:
    vector<Book *> borrowedBooks;    // List of currently borrowed books
    vector<Book *> borrowingHistory; // List of all borrowed books (history)
    double finePerDay;
    int maxBooks;
    int maxDays;
    bool isFaculty;
    double fees;

public:
    Account(double finePerDay, int maxBooks, int maxDays, bool isFaculty);
    void addBorrowedBook(Book *book, time_t borrowDate, time_t dueDate);
    void returnBook(string isbn);
    double calculateFine();
    void displayBorrowingHistory();
    vector<Book *> displayCurrentlyBorrowedBooks();
    bool canBorrow();
    bool hasOverdueBooks();
    void payFees(double amount);
    double getFees();
    ~Account();
};

Account::Account(double finePerDay, int maxBooks, int maxDays, bool isFaculty)
{
    this->finePerDay = finePerDay;
    this->maxBooks = maxBooks;
    this->maxDays = maxDays;
    this->isFaculty = isFaculty;
    this->fees = 0.0;
}

void Account::addBorrowedBook(Book *book, time_t borrowDate, time_t dueDate)
{
    if (borrowedBooks.size() >= maxBooks)
    {
        cout << "Cannot borrow more than " << maxBooks << " books!" << endl;
        return;
    }
    if (calculateFine() > 0 || hasOverdueBooks())
    {
        cout << "Cannot borrow books until you pay the fees and return overdue books!" << endl;
        return;
    }
    if (book->getStatus() != "Available")
    {
        cout << "Book is not available for borrowing!" << endl;
        return;
    }
    book->setStatus("Borrowed");
    book->setBorrowDate(borrowDate);
    book->setDueDate(dueDate);
    borrowedBooks.push_back(book);
    Book *borrowedBookCopy = new Book(book->getIsbn(), book->getTitle(), book->getAuthor(), book->getPublisher(), book->getPublicationYear(), book->getStatus());
    borrowedBookCopy->setBorrowDate(borrowDate);
    borrowedBookCopy->setDueDate(dueDate);
    borrowingHistory.push_back(borrowedBookCopy);
    Library().UpdateBookStatus(book->getIsbn(), "Borrowed");
    cout << "Book borrowed successfully!" << endl;
}

Account::~Account()
{
    for (auto &book : borrowingHistory)
    {
        delete book;
    }
}

void Account::returnBook(string isbn)
{
    for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it)
    {
        if ((*it)->getIsbn() == isbn)
        {
            (*it)->setStatus("Available");
            Library().UpdateBookStatus(isbn, "Available");
            borrowedBooks.erase(it);
            cout << "Book returned successfully!" << endl;
            return;
        }
    }

    cout << "Book not found in currently borrowed books!" << endl;
}

double Account::calculateFine()
{
    double totalFine = 0.0;
    time_t currentTime = time(0);

    for (const auto &book : borrowedBooks)
    {
        if (currentTime > book->getDueDate())
        {
            double overdueDays = difftime(currentTime, book->getDueDate()) / (60 * 60 * 24);
            totalFine += overdueDays * finePerDay;
        }
    }

    fees = totalFine;
    return totalFine;
}
void Account::displayBorrowingHistory()
{
    cout << "Borrowing History:" << endl;
    for (const auto &book : borrowingHistory)
    {
        time_t borrowDate = book->getBorrowDate();
        time_t dueDate = book->getDueDate();
        cout << "-----------------------------------" << endl;
        cout << "ISBN: " << book->getIsbn() << endl;
        cout << "Title: " << book->getTitle() << endl;
        cout << "Author: " << book->getAuthor() << endl;
        cout << "Borrow Date: " << ctime(&borrowDate);
        cout << "Due Date: " << ctime(&dueDate);
        cout << "-----------------------------------" << endl;
    }
    cout << "Total Fine: $" << calculateFine() << endl;
}

vector<Book *> Account::displayCurrentlyBorrowedBooks()
{
    cout << "Currently Borrowed Books:" << endl;
    for (const auto &book : borrowedBooks)
    {
        time_t borrowDate = book->getBorrowDate();
        time_t dueDate = book->getDueDate();
        cout << "-----------------------------------" << endl;
        cout << "ISBN: " << book->getIsbn() << endl;
        cout << "Title: " << book->getTitle() << endl;
        cout << "Author: " << book->getAuthor() << endl;
        cout << "Borrow Date: " << ctime(&borrowDate);
        cout << "Due Date: " << ctime(&dueDate);
        cout << "Status: " << book->getStatus() << endl;
        cout << "-----------------------------------" << endl;
    }
    cout << "Total Fine: $" << calculateFine() << endl;
    return borrowedBooks;
}

bool Account::canBorrow()
{
    return fees == 0 && !hasOverdueBooks();
}

bool Account::hasOverdueBooks()
{
    time_t currentTime = time(0);
    for (const auto &book : borrowedBooks)
    {
        if (currentTime > book->getDueDate())
        {
            return true;
        }
    }
    return false;
}

void Account::payFees(double amount)
{
    if (amount < fees)
    {
        cout << "Insufficient amount to pay the fees" << endl;
    }
    else if (amount > fees)
    {
        cout << "Amount is greater than fees!" << endl;
    }
    else
    {
        fees -= amount;
        cout << "Fees paid successfully!" << endl;
    }
}

double Account::getFees()
{
    return fees;
}

class User
{
private:
    string userId;
    string password;
    string name;
    string role;
    Account *account;

public:
    User();
    User(string userId, string password, string name, string role, Account *account);

    string getUserId();
    void setUserId(string userId);
    string getPassword();
    void setPassword(string password);
    string getName();
    void setName(string name);
    string getRole();
    void setRole(string role);
    Account *getAccount();
    void setAccount(Account *account);
};

User::User()
{
    this->userId = "";
    this->password = "";
    this->name = "";
    this->role = "";
    this->account = new Account(10, 3, 15, false); // Default values for a student
}

User::User(string userId, string password, string name, string role, Account *account)
{
    this->userId = userId;
    this->password = password;
    this->name = name;
    this->role = role;
    this->account = account;
}

string User::getUserId()
{
    return userId;
}

void User::setUserId(string userId)
{
    this->userId = userId;
}

string User::getPassword()
{
    return password;
}

void User::setPassword(string password)
{
    this->password = password;
}

string User::getName()
{
    return name;
}

void User::setName(string name)
{
    this->name = name;
}

string User::getRole()
{
    return role;
}

void User::setRole(string role)
{
    this->role = role;
}

Account *User::getAccount()
{
    return account;
}

void User::setAccount(Account *account)
{
    this->account = account;
}

class Student : public User
{
public:
    Student();
    Student(string userId, string password, string name);
    string StudentLogin();
};

Student::Student() : User("", "", "", "Student", new Account(10, 3, 15, false)) {}

Student::Student(string userId, string password, string name) : User(userId, password, name, "Student", new Account(10, 3, 15, false)) {}

class Faculty : public User
{
public:
    Faculty();
    Faculty(string userId, string password, string name);
    string FacultyLogin();
};

class StudentDBM
{
private:
    std::vector<Student *> students;

public:
    StudentDBM();
    void AddStudent(Student *student);
    void DeleteStudent(string userid);
    void UpdateStudent(Student *student);
    Student *SelectStudent(string userid);
    void DisplayAllStudents();
    void UpdateStatus();
    std::vector<Student *> ListStudents();
    ~StudentDBM();
};

void StudentDBM::DisplayAllStudents()
{
    cout << "-----------------------------------" << endl;
    cout << "          ALL STUDENTS             " << endl;
    cout << "-----------------------------------" << endl;
    for (const auto &student : students)
    {
        cout << "UserID: " << student->getUserId() << endl;
        cout << "Name: " << student->getName() << endl;
        cout << "-----------------------------------" << endl;
    }
}

class FacultyDBM
{
private:
    std::vector<Faculty *> faculties;

public:
    FacultyDBM();
    void AddFaculty(Faculty *faculty);
    void DeleteFaculty(string userid);
    void UpdateFaculty(Faculty *faculty);
    Faculty *SelectFaculty(string userid);
    std::vector<Faculty *> ListFaculties();
    void DisplayAllFaculties();
    ~FacultyDBM();
};

void FacultyDBM::DisplayAllFaculties()
{
    cout << "-----------------------------------" << endl;
    cout << "          ALL FACULTY              " << endl;
    cout << "-----------------------------------" << endl;
    for (const auto &faculty : faculties)
    {
        cout << "UserID: " << faculty->getUserId() << endl;
        cout << "Name: " << faculty->getName() << endl;
        cout << "-----------------------------------" << endl;
    }
}

class Librarian;

class LibrarianDBM
{
private:
    std::vector<Librarian *> librarians;

public:
    LibrarianDBM();
    void AddLibrarian(Librarian *librarian);
    void DeleteLibrarian(string userid);
    void UpdateLibrarian(Librarian *librarian, string userid);
    Librarian *SelectLibrarian(string userid);
    std::vector<Librarian *> ListLibrarians();
    void DisplayAllLibrarians();
    ~LibrarianDBM();
};

Faculty::Faculty() : User("", "", "", "Faculty", new Account(0, 5, 30, true)) {}

Faculty::Faculty(string userId, string password, string name) : User(userId, password, name, "Faculty", new Account(0, 5, 30, true)) {}

class Librarian : public User
{
public:
    Librarian();
    Librarian(string userid, string password, string name);

    string LibrarianLogin();
    void AddUser(int role, StudentDBM &studentDBM, FacultyDBM &facultyDBM, LibrarianDBM &librarianDBM);
    void RemoveUser(int role, StudentDBM &studentDBM, FacultyDBM &facultyDBM, LibrarianDBM &librarianDBM);
    void UpdateCredentials(LibrarianDBM &librarianDBM);
    void AddBook(Library &library);
    void RemoveBook(Library &library);
    void UpdateBook(Library &library);
    void DisplayAllBooks(Library &library);
};
void LibrarianDBM::DisplayAllLibrarians()
{
    cout << "-----------------------------------" << endl;
    cout << "          ALL LIBRARIANS           " << endl;
    cout << "-----------------------------------" << endl;
    for (const auto &librarian : librarians)
    {
        cout << "UserID: " << librarian->getUserId() << endl;
        cout << "Name: " << librarian->getName() << endl;
        cout << "-----------------------------------" << endl;
    }
}

string Student ::StudentLogin()
{
    string userid, password;
    Student student;

    for (int i = 0; i < 3; i++)
    {
        cout << "-----------------------------------" << endl;
        cout << "           LOGIN WINDOW            " << endl;
        cout << "-----------------------------------" << endl;

        cout << "[Input] Enter UserID: ";
        cin >> userid;
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "[Input] Enter Password: ";
        cin >> password;
        cin.clear();
        cin.ignore(10000, '\n');

        student.setUserId(userid);
        student.setPassword(password);

        Student *foundStudent = StudentDBM().SelectStudent(userid);
        if (foundStudent && foundStudent->getPassword() == password)
        {
            cout << "Login Successful!" << endl;
            return userid;
        }
        else
        {
            cout << "Login Failed!" << endl;
            if (i == 2)
            {
                cout << "Maximum attempts reached!" << endl;
                return "";
            }
        }
    }
    return "";
}

string Faculty ::FacultyLogin()
{
    string userid, password;
    Faculty faculty;

    for (int i = 0; i < 3; i++)
    {
        cout << "-----------------------------------" << endl;
        cout << "           LOGIN WINDOW            " << endl;
        cout << "-----------------------------------" << endl;

        cout << "[Input] Enter UserID: ";
        cin >> userid;
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "[Input] Enter Password: ";
        cin >> password;
        cin.clear();
        cin.ignore(10000, '\n');

        faculty.setUserId(userid);
        faculty.setPassword(password);

        Faculty *foundFaculty = FacultyDBM().SelectFaculty(userid);
        if (foundFaculty && foundFaculty->getPassword() == password)
        {
            cout << "Login Successful!" << endl;
            return userid;
        }
        else
        {
            cout << "Login Failed!" << endl;
            if (i == 2)
            {
                cout << "Maximum attempts reached!" << endl;
                return "";
            }
        }
    }
    return "";
}

Library::Library()
{
    vector<string> data = readFromFile("./DB/books.csv", 6);
    vector<Book *> books = {};

    if (data.size() == 1)
        return;

    for (int i = 0; i < data.size(); i += 6)
    {
        Book *book = new Book(data[i], data[i + 1], data[i + 2], data[i + 3], data[i + 4], data[i + 5]);
        books.push_back(book);
    }

    this->books = books;
}

Library::~Library()
{
    vector<string> data = {};

    for (int i = 0; i < books.size(); i++)
    {
        data.push_back(books[i]->getIsbn());
        data.push_back(books[i]->getTitle());
        data.push_back(books[i]->getAuthor());
        data.push_back(books[i]->getPublisher());
        data.push_back(books[i]->getPublicationYear());
        data.push_back(books[i]->getStatus());
    }

    writeToFile("./DB/books.csv", data, 6);

    for (Book *book : books)
    {
        delete book;
    }
}

void Library::UpdateBookStatus(string isbn, string status)
{
    for (Book *book : books)
    {
        if (book->getIsbn() == isbn)
        {
            book->setStatus(status);
            // Write the updated status to the file
            vector<string> data = {};
            for (Book *b : books)
            {
                data.push_back(b->getIsbn());
                data.push_back(b->getTitle());
                data.push_back(b->getAuthor());
                data.push_back(b->getPublisher());
                data.push_back(b->getPublicationYear());
                data.push_back(b->getStatus());
            }
            writeToFile("./DB/books.csv", data, 6);
            return;
        }
    }
}

void Library::AddBook(Book *book)
{
    Book *temp = SelectBook(book->getIsbn());
    if (temp != NULL)
    {
        cout << "ISBN already exists!" << endl;
        return;
    }
    books.push_back(book);
    cout << "Book Added!" << endl;
}

void Library::RemoveBook(string isbn)
{
    for (int i = 0; i < books.size(); i++)
    {
        if (books[i]->getIsbn() == isbn)
        {
            books.erase(books.begin() + i);
            cout << "Book Removed!" << endl;
            return;
        }
    }
    cout << "ISBN does not exist!" << endl;
}

void Library::UpdateBook(Book *book)
{
    for (int i = 0; i < books.size(); i++)
    {
        if (books[i]->getIsbn() == book->getIsbn())
        {
            books[i] = book;
            cout << "Book Updated!" << endl;
            return;
        }
    }
    cout << "ISBN does not exist!" << endl;
}

Book *Library::SelectBook(string isbn)
{
    for (int i = 0; i < books.size(); i++)
    {
        if (books[i]->getIsbn() == isbn)
        {
            return books[i];
        }
    }
    return NULL;
}

vector<Book *> Library::ListBooks()
{
    return books;
}

void Library::DisplayAllBooks()
{
    cout << "All Books:" << endl;
    for (const auto &book : books)
    {
        cout << "-----------------------------------" << endl;
        cout << "ISBN: " << book->getIsbn() << endl;
        cout << "Title: " << book->getTitle() << endl;
        cout << "Author: " << book->getAuthor() << endl;
        cout << "Publisher: " << book->getPublisher() << endl;
        cout << "Publication Year: " << book->getPublicationYear() << endl;
        cout << "Status: " << book->getStatus() << endl;
        cout << "-----------------------------------" << endl;
    }
}

Librarian::Librarian() : User("", "", "", "Librarian", new Account(0, 0, 0, false)) {}

Librarian::Librarian(string userid, string password, string name) : User(userid, password, name, "Librarian", new Account(0, 0, 0, false)) {}

string Librarian::LibrarianLogin()
{
    string userid, password;
    Librarian librarian;

    for (int i = 0; i < 3; i++)
    {
        cout << "-----------------------------------" << endl;
        cout << "           LOGIN WINDOW            " << endl;
        cout << "-----------------------------------" << endl;

        cout << "[Input] Enter UserID: ";
        cin >> userid;
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "[Input] Enter Password: ";
        cin >> password;
        cin.clear();
        cin.ignore(10000, '\n');

        librarian.setUserId(userid);
        librarian.setPassword(password);

        Librarian *foundLibrarian = LibrarianDBM().SelectLibrarian(userid);
        if (foundLibrarian && foundLibrarian->getPassword() == password)
        {
            cout << "Login Successful!" << endl;
            return userid;
        }
        else
        {
            cout << "Login Failed!" << endl;
            if (i == 2)
            {
                cout << "Maximum attempts reached!" << endl;
                return "";
            }
        }
    }
    return "";
}

void Librarian::AddUser(int role, StudentDBM &studentDBM, FacultyDBM &facultyDBM, LibrarianDBM &librarianDBM)
{
    string userid, password, name;
    cout << "-----------------------------------" << endl;
    cout << "           ADD USER WINDOW          " << endl;
    cout << "-----------------------------------" << endl;
    cout << "[Input] Enter UserID: ";
    cin >> userid;
    cout << "[Input] Enter Password: ";
    cin >> password;
    cin.ignore(); // To clear newline before getline
    cout << "[Input] Enter Name: ";
    getline(cin, name);

    if (role == 1)
    {
        Student *student = new Student(userid, password, name);
        studentDBM.AddStudent(student);
        vector<string> data = {};
        for (Student *s : studentDBM.ListStudents())
        {
            data.push_back(s->getUserId());
            data.push_back(s->getPassword());
            data.push_back(s->getName());
        }
        writeToFile("./DB/student.csv", data, 3);
    }
    else if (role == 2)
    {
        Faculty *faculty = new Faculty(userid, password, name);
        facultyDBM.AddFaculty(faculty);
        vector<string> data = {};
        for (Faculty *f : facultyDBM.ListFaculties())
        {
            data.push_back(f->getUserId());
            data.push_back(f->getPassword());
            data.push_back(f->getName());
        }
        writeToFile("./DB/faculty.csv", data, 3);
    }
    else if (role == 3)
    {
        Librarian *librarian = new Librarian(userid, password, name);
        librarianDBM.AddLibrarian(librarian);
        vector<string> data = {};
        for (Librarian *l : librarianDBM.ListLibrarians())
        {
            data.push_back(l->getUserId());
            data.push_back(l->getPassword());
            data.push_back(l->getName());
        }
        writeToFile("./DB/librarian.csv", data, 3);
    }
    else
    {
        cout << "Invalid Role!" << endl;
    }
}

void Librarian::RemoveUser(int role, StudentDBM &studentDBM, FacultyDBM &facultyDBM, LibrarianDBM &librarianDBM)
{
    string userid;
    cout << "-----------------------------------" << endl;
    cout << "           REMOVE USER WINDOW       " << endl;
    cout << "-----------------------------------" << endl;
    cout << "[Input] Enter UserID: ";
    cin >> userid;
    cin.clear();
    cin.ignore(10000, '\n');
    if (userid == this->getUserId())
    {
        cout << "Cannot delete yourself!" << endl;
        return;
    }
    if (role == 1)
    {
        studentDBM.DeleteStudent(userid);
        vector<string> data = {};
        for (Student *s : studentDBM.ListStudents())
        {
            data.push_back(s->getUserId());
            data.push_back(s->getPassword());
            data.push_back(s->getName());
        }
        writeToFile("./DB/student.csv", data, 3);
    }
    else if (role == 2)
    {
        facultyDBM.DeleteFaculty(userid);
        vector<string> data = {};
        for (Faculty *f : facultyDBM.ListFaculties())
        {
            data.push_back(f->getUserId());
            data.push_back(f->getPassword());
            data.push_back(f->getName());
        }
        writeToFile("./DB/faculty.csv", data, 3);
    }
    else if (role == 3)
    {
        librarianDBM.DeleteLibrarian(userid);
        vector<string> data = {};
        for (Librarian *l : librarianDBM.ListLibrarians())
        {
            data.push_back(l->getUserId());
            data.push_back(l->getPassword());
            data.push_back(l->getName());
        }
        writeToFile("./DB/librarian.csv", data, 3);
    }
    else
    {
        cout << "Invalid Role!" << endl;
    }
}

void Librarian::UpdateCredentials(LibrarianDBM &librarianDBM)
{
    string userid, password, newId;
    cout << "-----------------------------------" << endl;
    cout << "           UPDATE CREDENTIALS       " << endl;
    cout << "-----------------------------------" << endl;
    cout << "UserID: ";
    userid = this->getUserId();
    cout << userid << endl;
    cout << "-----------------------------------" << endl;
    cout << "[Input] Enter New UserID: ";
    cin >> newId;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter new Password: ";
    cin >> password;
    cin.clear();
    cin.ignore(10000, '\n');
    Librarian *librarian = new Librarian(newId, password, this->getName());
    librarianDBM.UpdateLibrarian(librarian, userid);
    cout << "Credentials updated successfully!" << endl;
    librarianDBM.DeleteLibrarian(userid);
    vector<string> data = {};
    for (Librarian *l : librarianDBM.ListLibrarians())
    {
        data.push_back(l->getUserId());
        data.push_back(l->getPassword());
        data.push_back(l->getName());
    }
    writeToFile("./DB/librarian.csv", data, 3);
}

void Librarian::AddBook(Library &library)
{
    string isbn, title, author, publisher, publicationYear, status;
    cout << "-----------------------------------" << endl;
    cout << "           ADD BOOK WINDOW          " << endl;
    cout << "-----------------------------------" << endl;
    cout << "[Input] Enter ISBN: ";
    cin >> isbn;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Title: ";
    cin >> title;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Author: ";
    cin >> author;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Publisher: ";
    cin >> publisher;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Publication Year: ";
    cin >> publicationYear;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Status: ";
    cin >> status;
    cin.clear();
    cin.ignore(10000, '\n');
    Book *book = new Book(isbn, title, author, publisher, publicationYear, status);
    library.AddBook(book);
    vector<string> data = {};
    for (Book *b : library.ListBooks())
    {
        data.push_back(b->getIsbn());
        data.push_back(b->getTitle());
        data.push_back(b->getAuthor());
        data.push_back(b->getPublisher());
        data.push_back(b->getPublicationYear());
        data.push_back(b->getStatus());
    }
    writeToFile("./DB/books.csv", data, 6);
}
void Librarian::RemoveBook(Library &library)
{
    string isbn;
    cout << "-----------------------------------" << endl;
    cout << "           REMOVE BOOK WINDOW       " << endl;
    cout << "-----------------------------------" << endl;
    cout << "[Input] Enter ISBN: ";
    cin >> isbn;
    cin.clear();
    cin.ignore(10000, '\n');
    library.RemoveBook(isbn);
    vector<string> data = {};
    for (Book *b : library.ListBooks())
    {
        data.push_back(b->getIsbn());
        data.push_back(b->getTitle());
        data.push_back(b->getAuthor());
        data.push_back(b->getPublisher());
        data.push_back(b->getPublicationYear());
        data.push_back(b->getStatus());
    }
    writeToFile("./DB/books.csv", data, 6);
}

void Librarian::UpdateBook(Library &library)
{
    string isbn, title, author, publisher, publicationYear, status;
    cout << "-----------------------------------" << endl;
    cout << "           UPDATE BOOK WINDOW       " << endl;
    cout << "-----------------------------------" << endl;
    cout << "[Input] Enter ISBN: ";
    cin >> isbn;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Title: ";
    cin >> title;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Author: ";
    cin >> author;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Publisher: ";
    cin >> publisher;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Publication Year: ";
    cin >> publicationYear;
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[Input] Enter Status: ";
    cin >> status;
    cin.clear();
    cin.ignore(10000, '\n');
    Book *book = new Book(isbn, title, author, publisher, publicationYear, status);
    library.UpdateBook(book);
    vector<string> data = {};
    for (Book *b : library.ListBooks())
    {
        data.push_back(b->getIsbn());
        data.push_back(b->getTitle());
        data.push_back(b->getAuthor());
        data.push_back(b->getPublisher());
        data.push_back(b->getPublicationYear());
        data.push_back(b->getStatus());
    }
    writeToFile("./DB/books.csv", data, 6);
}

void Librarian::DisplayAllBooks(Library &library)
{
    library.DisplayAllBooks();
}

LibrarianDBM::LibrarianDBM()
{
    vector<string> data = readFromFile("./DB/librarian.csv", 3);
    vector<Librarian *> librarians = {};

    if (data.size() == 1)
        return;

    for (int i = 0; i < data.size(); i += 3)
    {
        Librarian *librarian = new Librarian(data[i], data[i + 1], data[i + 2]);
        librarians.push_back(librarian);
    }

    this->librarians = librarians;
}

LibrarianDBM::~LibrarianDBM()
{
    vector<string> data = {};

    for (int i = 0; i < librarians.size(); i++)
    {
        data.push_back(librarians[i]->getUserId());
        data.push_back(librarians[i]->getPassword());
        data.push_back(librarians[i]->getName());
    }

    writeToFile("./DB/librarian.csv", data, 3);

    for (Librarian *librarian : librarians)
    {
        delete librarian;
    }
}

void LibrarianDBM::UpdateLibrarian(Librarian *librarian, string userid)
{
    for (int i = 0; i < librarians.size(); i++)
    {
        if (librarians[i]->getUserId() == userid)
        {
            librarians[i] = librarian;
            return;
        }
    }
}

Librarian *LibrarianDBM::SelectLibrarian(string userid)
{
    for (int i = 0; i < librarians.size(); i++)
    {
        if (librarians[i]->getUserId() == userid)
        {
            return librarians[i];
        }
    }
    return NULL;
}

vector<Librarian *> LibrarianDBM::ListLibrarians()
{
    return librarians;
}

void LibrarianDBM::AddLibrarian(Librarian *librarian)
{
    Librarian *temp = SelectLibrarian(librarian->getUserId());
    if (temp != NULL)
    {
        cout << "UserID already exists!" << endl;
        return;
    }
    librarians.push_back(librarian);
    cout << "Librarian Added!" << endl;
}

void LibrarianDBM::DeleteLibrarian(string userid)
{
    for (int i = 0; i < librarians.size(); i++)
    {
        if (librarians[i]->getUserId() == userid)
        {
            librarians.erase(librarians.begin() + i);
            cout << "Librarian Deleted!" << endl;
            return;
        }
    }
    cout << "UserID does not exist!" << endl;
}

StudentDBM::StudentDBM()
{
    vector<string> data = readFromFile("./DB/student.csv", 3);
    vector<Student *> students = {};

    if (data.size() == 1)
        return;

    for (int i = 0; i < data.size(); i += 3)
    {
        Student *student = new Student(data[i], data[i + 1], data[i + 2]);
        students.push_back(student);
    }

    this->students = students;
}

StudentDBM::~StudentDBM()
{
    vector<string> data = {};

    for (int i = 0; i < students.size(); i++)
    {
        data.push_back(students[i]->getUserId());
        data.push_back(students[i]->getPassword());
        data.push_back(students[i]->getName());
    }

    writeToFile("./DB/student.csv", data, 3);

    for (Student *student : students)
    {
        delete student;
    }
}

void StudentDBM::UpdateStudent(Student *student)
{
    for (int i = 0; i < students.size(); i++)
    {
        if (students[i]->getUserId() == student->getUserId())
        {
            students[i] = student;
            return;
        }
    }
}

Student *StudentDBM::SelectStudent(string userid)
{
    for (int i = 0; i < students.size(); i++)
    {
        if (students[i]->getUserId() == userid)
        {
            return students[i];
        }
    }
    return NULL;
}

vector<Student *> StudentDBM::ListStudents()
{
    return students;
}

void StudentDBM::AddStudent(Student *student)
{
    Student *temp = SelectStudent(student->getUserId());
    if (temp != NULL)
    {
        cout << "UserID already exists!" << endl;
        return;
    }
    students.push_back(student);
    cout << "Student Added!" << endl;
}

void StudentDBM::DeleteStudent(string userid)
{
    for (int i = 0; i < students.size(); i++)
    {
        if (students[i]->getUserId() == userid)
        {
            students.erase(students.begin() + i);
            cout << "Student Deleted!" << endl;
            return;
        }
    }
    cout << "UserID does not exist!" << endl;
}

FacultyDBM::FacultyDBM()
{
    vector<string> data = readFromFile("./DB/faculty.csv", 3);
    vector<Faculty *> faculties = {};

    if (data.size() == 1)
        return;

    for (int i = 0; i < data.size(); i += 3)
    {
        Faculty *faculty = new Faculty(data[i], data[i + 1], data[i + 2]);
        faculties.push_back(faculty);
    }

    this->faculties = faculties;
}

FacultyDBM::~FacultyDBM()
{
    vector<string> data = {};

    for (int i = 0; i < faculties.size(); i++)
    {
        data.push_back(faculties[i]->getUserId());
        data.push_back(faculties[i]->getPassword());
        data.push_back(faculties[i]->getName());
    }

    writeToFile("./DB/faculty.csv", data, 3);

    for (Faculty *faculty : faculties)
    {
        delete faculty;
    }
}

void FacultyDBM::UpdateFaculty(Faculty *faculty)
{
    for (int i = 0; i < faculties.size(); i++)
    {
        if (faculties[i]->getUserId() == faculty->getUserId())
        {
            faculties[i] = faculty;
            return;
        }
    }
}

Faculty *FacultyDBM::SelectFaculty(string userid)
{
    for (int i = 0; i < faculties.size(); i++)
    {
        if (faculties[i]->getUserId() == userid)
        {
            return faculties[i];
        }
    }
    return NULL;
}

vector<Faculty *> FacultyDBM::ListFaculties()
{
    return faculties;
}

void FacultyDBM::AddFaculty(Faculty *faculty)
{
    Faculty *temp = SelectFaculty(faculty->getUserId());
    if (temp != NULL)
    {
        cout << "UserID already exists!" << endl;
        return;
    }
    faculties.push_back(faculty);
    cout << "Faculty Added!" << endl;
}

void FacultyDBM::DeleteFaculty(string userid)
{
    for (int i = 0; i < faculties.size(); i++)
    {
        if (faculties[i]->getUserId() == userid)
        {
            faculties.erase(faculties.begin() + i);
            cout << "Faculty Deleted!" << endl;
            return;
        }
    }
    cout << "UserID does not exist!" << endl;
}
void StudentMenu(StudentDBM &studentDBM, Library &library)
{
    Student stud;
    string userid = stud.StudentLogin();
    if (userid == "")
        return;
    Student *student = studentDBM.SelectStudent(userid);
    if (!student)
    {
        cout << "Error: Student not found!" << endl;
        return;
    }
    int option = 0;
    while (option != 7)
    {

        cout << "-----------------------------------" << endl;
        cout << "          STUDENT MENU             " << endl;
        cout << "-----------------------------------" << endl;
        cout << "|| 1 - BORROW BOOK               ||" << endl;
        cout << "|| 2 - RETURN BOOK               ||" << endl;
        cout << "|| 3 - DISPLAY BORROWING HISTORY ||" << endl;
        cout << "|| 4 - DISPLAY CURRENTLY BORROWED||" << endl;
        cout << "|| 5 - PAY FEES                  ||" << endl;
        cout << "|| 6 - SHOW FEES                 ||" << endl;
        cout << "|| 7 - LOGOUT                    ||" << endl;
        cout << "-----------------------------------" << endl;
        cout << "[Input] Enter Option: ";
        cin >> option;
        cin.clear();
        cin.ignore(10000, '\n');

        if (option == 1)
        {
            cout << "-----------------------------------" << endl;
            cout << "          AVAILABLE BOOKS          " << endl;
            cout << "-----------------------------------" << endl;
            for (const auto &book : library.ListBooks())
            {
                if (book->getStatus() == "Available")
                {
                    cout << "ISBN: " << book->getIsbn() << endl;
                    cout << "Title: " << book->getTitle() << endl;
                    cout << "Author: " << book->getAuthor() << endl;
                    cout << "Publisher: " << book->getPublisher() << endl;
                    cout << "Publication Year: " << book->getPublicationYear() << endl;
                    cout << "-----------------------------------" << endl;
                }
            }
            string isbn;
            cout << "[Input] Enter ISBN of the book to borrow: ";
            cin >> isbn;
            cin.clear();
            cin.ignore(10000, '\n');
            Book *book = library.SelectBook(isbn);
            if (book)
            {
                time_t now = time(0);
                time_t dueDate = now + (15 * 24 * 60 * 60); // 15 days from now
                student->getAccount()->addBorrowedBook(book, now, dueDate);
            }
            else
            {
                cout << "Book not found!" << endl;
            }
        }
        else if (option == 2)
        {
            cout << "-----------------------------------" << endl;
            cout << "       CURRENTLY BORROWED BOOKS      " << endl;
            cout << "-----------------------------------" << endl;
            vector<Book *> borrowedBooks = student->getAccount()->displayCurrentlyBorrowedBooks();
            if (borrowedBooks.empty())
            {
                cout << "No books currently borrowed." << endl;
                continue;
            }
            // else
            // {
            //     for (const auto &book : borrowedBooks)
            //     {
            //         cout << "ISBN: " << book->getIsbn() << endl;
            //         cout << "Title: " << book->getTitle() << endl;
            //         cout << "Author: " << book->getAuthor() << endl;
            //         cout << "-----------------------------------" << endl;
            //     }
            // }
            string isbn;
            cout << "[Input] Enter ISBN of the book to return: ";
            cin >> isbn;
            cin.clear();
            cin.ignore(10000, '\n');
            student->getAccount()->returnBook(isbn);
        }
        else if (option == 3)
        {
            student->getAccount()->displayBorrowingHistory();
        }
        else if (option == 4)
        {
            student->getAccount()->displayCurrentlyBorrowedBooks();
        }
        else if (option == 5)
        {
            double amount;
            cout << "[Input] Enter amount to pay: ";
            cin >> amount;
            cin.clear();
            cin.ignore(10000, '\n');
            student->getAccount()->payFees(amount);
        }
        else if (option == 6)
        {
            cout << "Current Fees: $" << student->getAccount()->getFees() << endl;
        }
        else if (option == 7)
        {
            cout << "Logging out!" << endl;
            return;
        }
        else
        {
            cout << "Invalid Option!" << endl;
        }
    }
}

void FacultyMenu(FacultyDBM &facultyDBM, Library &library)
{
    Faculty fact;
    string userid = fact.FacultyLogin();
    if (userid == "")
        return;

    int option = 0;
    Faculty *faculty = facultyDBM.SelectFaculty(userid);
    if (!faculty)
    {
        cout << "Error: Faculty not found!" << endl;
        return;
    }
    while (option != 6)
    {

        cout << "-----------------------------------" << endl;
        cout << "          FACULTY MENU             " << endl;
        cout << "-----------------------------------" << endl;
        cout << "|| 1 - BORROW BOOK               ||" << endl;
        cout << "|| 2 - RETURN BOOK               ||" << endl;
        cout << "|| 3 - DISPLAY BORROWING HISTORY ||" << endl;
        cout << "|| 4 - DISPLAY CURRENTLY BORROWED||" << endl;
        cout << "|| 5 - LOGOUT                    ||" << endl;
        cout << "-----------------------------------" << endl;
        cout << "[Input] Enter Option: ";
        cin >> option;
        cin.clear();
        cin.ignore(10000, '\n');

        if (option == 1)
        {
            cout << "-----------------------------------" << endl;
            cout << "          AVAILABLE BOOKS          " << endl;
            cout << "-----------------------------------" << endl;
            for (const auto &book : library.ListBooks())
            {
                if (book->getStatus() == "Available")
                {
                    cout << "ISBN: " << book->getIsbn() << endl;
                    cout << "Title: " << book->getTitle() << endl;
                    cout << "Author: " << book->getAuthor() << endl;
                    cout << "Publisher: " << book->getPublisher() << endl;
                    cout << "Publication Year: " << book->getPublicationYear() << endl;
                    cout << "-----------------------------------" << endl;
                }
            }
            string isbn;
            cout << "[Input] Enter ISBN of the book to borrow: ";
            cin >> isbn;
            cin.clear();
            cin.ignore(10000, '\n');
            Book *book = library.SelectBook(isbn);
            if (book)
            {
                time_t now = time(0);
                time_t dueDate = now + (30 * 24 * 60 * 60); // 30 days from now
                faculty->getAccount()->addBorrowedBook(book, now, dueDate);
            }
            else
            {
                cout << "Book not found!" << endl;
            }
        }
        else if (option == 2)
        {
            cout << "-----------------------------------" << endl;
            cout << "       CURRENTLY BORROWED BOOKS      " << endl;
            cout << "-----------------------------------" << endl;
            vector<Book *> borrowedBooks = faculty->getAccount()->displayCurrentlyBorrowedBooks();
            if (borrowedBooks.empty())
            {
                cout << "No books currently borrowed." << endl;
                continue;
            }
            // else
            // {
            //     for (const auto &book : borrowedBooks)
            //     {
            //         cout << "ISBN: " << book->getIsbn() << endl;
            //         cout << "Title: " << book->getTitle() << endl;
            //         cout << "Author: " << book->getAuthor() << endl;
            //         cout << "-----------------------------------" << endl;
            //     }
            // }
            string isbn;
            cout << "[Input] Enter ISBN of the book to return: ";
            cin >> isbn;
            cin.clear();
            cin.ignore(10000, '\n');
            faculty->getAccount()->returnBook(isbn);
        }
        else if (option == 3)
        {
            faculty->getAccount()->displayBorrowingHistory();
        }
        else if (option == 4)
        {
            faculty->getAccount()->displayCurrentlyBorrowedBooks();
        }
        else if (option == 5)
        {
            cout << "Logging out!" << endl;
            return;
        }
        else
        {
            cout << "Invalid Option!" << endl;
        }
    }
}
void LibrarianMenu(LibrarianDBM &librarianDBM, Library &library, StudentDBM &studentDBM, FacultyDBM &facultyDBM)
{
    Librarian libb;
    string userid = libb.LibrarianLogin();
    if (userid == "")
        return;

    int option = 0;
    Librarian *librarian = librarianDBM.SelectLibrarian(userid);
    if (!librarian)
    {
        cout << "Error: Librarian not found!" << endl;
        return;
    }

    while (option != 16)
    {
        cout << "-----------------------------------" << endl;
        cout << "          LIBRARIAN MENU           " << endl;
        cout << "-----------------------------------" << endl;
        cout << "|| 1 - ADD STUDENT               ||" << endl;
        cout << "|| 2 - ADD FACULTY               ||" << endl;
        cout << "|| 3 - ADD LIBRARIAN             ||" << endl;
        cout << "|| 4 - REMOVE STUDENT            ||" << endl;
        cout << "|| 5 - REMOVE FACULTY            ||" << endl;
        cout << "|| 6 - REMOVE LIBRARIAN          ||" << endl;
        cout << "|| 7 - UPDATE CREDENTIALS        ||" << endl;
        cout << "|| 8 - ADD BOOK                  ||" << endl;
        cout << "|| 9 - REMOVE BOOK               ||" << endl;
        cout << "|| 10 - UPDATE BOOK DETAILS      ||" << endl;
        cout << "|| 11 - DISPLAY ALL BOOKS        ||" << endl;
        cout << "|| 12 - DISPLAY ALL STUDENTS     ||" << endl;
        cout << "|| 13 - DISPLAY ALL FACULTY      ||" << endl;
        cout << "|| 14 - DISPLAY ALL LIBRARIANS   ||" << endl;
        cout << "|| 15 - RESERVE A BOOK           ||" << endl;
        cout << "|| 16 - LOGOUT                   ||" << endl;
        cout << "-----------------------------------" << endl;
        cout << "[Input] Enter Option: ";
        cin >> option;
        cin.clear();
        cin.ignore(10000, '\n');

        if (option == 1)
            librarian->AddUser(1, studentDBM, facultyDBM, librarianDBM);
        else if (option == 2)
            librarian->AddUser(2, studentDBM, facultyDBM, librarianDBM);
        else if (option == 3)
            librarian->AddUser(3, studentDBM, facultyDBM, librarianDBM);
        else if (option == 4)
        {
            cout << "-----------------------------------" << endl;
            cout << "          ALL STUDENTS             " << endl;
            cout << "-----------------------------------" << endl;
            studentDBM.DisplayAllStudents();
            librarian->RemoveUser(1, studentDBM, facultyDBM, librarianDBM);
        }
        else if (option == 5)
        {
            cout << "-----------------------------------" << endl;
            cout << "          ALL FACULTY              " << endl;
            cout << "-----------------------------------" << endl;
            facultyDBM.DisplayAllFaculties();
            librarian->RemoveUser(2, studentDBM, facultyDBM, librarianDBM);
        }
        else if (option == 6)
        {
            cout << "-----------------------------------" << endl;
            cout << "          ALL LIBRARIANS           " << endl;
            cout << "-----------------------------------" << endl;
            librarianDBM.DisplayAllLibrarians();
            librarian->RemoveUser(3, studentDBM, facultyDBM, librarianDBM);
        }
        else if (option == 7)
            librarian->UpdateCredentials(librarianDBM);
        else if (option == 8)
            librarian->AddBook(library);
        else if (option == 9)
        {
            cout << "-----------------------------------" << endl;
            cout << "          ALL BOOKS                " << endl;
            cout << "-----------------------------------" << endl;
            library.DisplayAllBooks();
            librarian->RemoveBook(library);
        }
        else if (option == 10)
        {
            cout << "-----------------------------------" << endl;
            cout << "          ALL BOOKS                " << endl;
            cout << "-----------------------------------" << endl;
            library.DisplayAllBooks();
            librarian->UpdateBook(library);
        }
        else if (option == 11)
            librarian->DisplayAllBooks(library);
        else if (option == 12)
        {
            cout << "-----------------------------------" << endl;
            cout << "          ALL STUDENTS             " << endl;
            cout << "-----------------------------------" << endl;
            studentDBM.DisplayAllStudents();
        }
        else if (option == 13)
        {
            cout << "-----------------------------------" << endl;
            cout << "          ALL FACULTY              " << endl;
            cout << "-----------------------------------" << endl;
            facultyDBM.DisplayAllFaculties();
        }
        else if (option == 14)
        {
            cout << "-----------------------------------" << endl;
            cout << "          ALL LIBRARIANS           " << endl;
            cout << "-----------------------------------" << endl;
            librarianDBM.DisplayAllLibrarians();
        }
        else if (option == 15)
        {
            cout << "-----------------------------------" << endl;
            cout << "          AVAILABLE BOOKS          " << endl;
            cout << "-----------------------------------" << endl;
            for (const auto &book : library.ListBooks())
            {
                if (book->getStatus() == "Available")
                {
                    cout << "ISBN: " << book->getIsbn() << endl;
                    cout << "Title: " << book->getTitle() << endl;
                    cout << "Author: " << book->getAuthor() << endl;
                    cout << "Publisher: " << book->getPublisher() << endl;
                    cout << "Publication Year: " << book->getPublicationYear() << endl;
                    cout << "-----------------------------------" << endl;
                }
            }
            string isbn;
            cout << "[Input] Enter ISBN of the book to reserve: ";
            cin >> isbn;
            cin.clear();
            cin.ignore(10000, '\n');
            Book *book = library.SelectBook(isbn);
            if (book)
            {
                if (book->getStatus() == "Available")
                {
                    library.UpdateBookStatus(isbn, "Reserved");
                    cout << "Book reserved successfully!" << endl;
                }
                else
                {
                    cout << "Book is not available for reservation!" << endl;
                }
            }
            else
            {
                cout << "Book not found!" << endl;
            }
        }
        else if (option == 16)
        {
            cout << "Logging out!" << endl;
            return;
        }
        else
            cout << "Invalid Option!" << endl;
    }
}
void LoginMenu()
{
    Library library;           // Create the library object once and pass it to the menus
    StudentDBM studentDBM;     // Create the StudentDBM object once and pass it to the menus
    FacultyDBM facultyDBM;     // Create the FacultyDBM object once and pass it to the menus
    LibrarianDBM librarianDBM; // Create the LibrarianDBM object once and pass it to the menus

    int option = 0;
    while (option != 4)
    {
        system("cls");
        cout << "===================================" << endl;
        cout << "||                               ||" << endl;
        cout << "||     LIBRARY MANAGEMENT        ||" << endl;
        cout << "||          SYSTEM               ||" << endl;
        cout << "||                               ||" << endl;
        cout << "===================================" << endl;
        cout << "-----------------------------------" << endl;
        cout << "           LOGIN OPTIONS           " << endl;
        cout << "-----------------------------------" << endl;
        cout << "||   1 - STUDENT                 ||" << endl;
        cout << "||   2 - FACULTY                 ||" << endl;
        cout << "||   3 - LIBRARIAN               ||" << endl;
        cout << "||   4 - EXIT                    ||" << endl;
        cout << "-----------------------------------" << endl;
        cout << "[Input] Enter Login Option: ";
        cin >> option;
        cin.clear();
        cin.ignore(10000, '\n');

        if (option == 1)
        {
            system("cls");
            StudentMenu(studentDBM, library);
        }
        else if (option == 2)
        {
            system("cls");
            FacultyMenu(facultyDBM, library);
        }
        else if (option == 3)
        {
            system("cls");
            LibrarianMenu(librarianDBM, library, studentDBM, facultyDBM);
        }
        else if (option == 4)
        {
            cout << "Exiting..." << endl;
            cout << "Thank you for using the Library Management System!" << endl;
        }
        else
        {
            cout << "Invalid option! Please try again.\n";
            system("pause");
        }
    }
}

int main()
{
    LoginMenu();
    return 0;
}
