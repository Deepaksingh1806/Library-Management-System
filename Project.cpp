#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Book {
    int bookID;
    string title, author;

public:
    void getData() {
        cout << "\nEnter Book ID: ";
        cin >> bookID;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author Name: ";
        getline(cin, author);
    }

    void showData() const {
        cout << "\nBook ID: " << bookID;
        cout << "\nTitle: " << title;
        cout << "\nAuthor: " << author << endl;
    }

    int getBookID() const { return bookID; }
    string getTitle() const { return title; }
};

// Function to add a new book record
void addBook() {
    Book b;
    ofstream outFile("library.txt", ios::binary | ios::app);
    if (!outFile) {
        cout << "\nError opening file for writing!\n";
        return;
    }
    b.getData();
    outFile.write(reinterpret_cast<char*>(&b), sizeof(b));
    outFile.close();
    cout << "\nBook record added successfully!\n";
}

// Function to display all book records
void displayBooks() {
    Book b;
    ifstream inFile("library.txt", ios::binary);
    if (!inFile) {
        cout << "\nNo records found!\n";
        return;
    }
    cout << "\n===== All Book Records =====\n";
    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        b.showData();
        cout << "-----------------------------\n";
    }
    inFile.close();
}

// Function to delete a book record
void deleteBook() {
    int id;
    cout << "\nEnter Book ID to delete: ";
    cin >> id;

    ifstream inFile("library.txt", ios::binary);
    if (!inFile) {
        cout << "\nNo records found!\n";
        return;
    }

    ofstream outFile("temp.txt", ios::binary);
    Book b;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.getBookID() != id) {
            outFile.write(reinterpret_cast<char*>(&b), sizeof(b));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();

    remove("library.txt");
    rename("temp.txt", "library.txt");

    if (found)
        cout << "\nBook record deleted successfully!\n";
    else
        cout << "\nBook with ID " << id << " not found!\n";
}

// ✅ New Function: Issue Book
void issueBook() {
    int id;
    string borrower;
    Book b;
    bool found = false;

    cout << "\nEnter Book ID to issue: ";
    cin >> id;
    cin.ignore();

    ifstream inFile("library.txt", ios::binary);
    if (!inFile) {
        cout << "\nNo book records found!\n";
        return;
    }

    ofstream issueFile("issued_books.txt", ios::app);
    if (!issueFile) {
        cout << "\nError opening issue file!\n";
        inFile.close();
        return;
    }

    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.getBookID() == id) {
            found = true;
            cout << "\nBook Found!";
            b.showData();
            cout << "\nEnter Borrower's Name: ";
            getline(cin, borrower);

            issueFile << "Book ID: " << b.getBookID() << "\n";
            issueFile << "Title: " << b.getTitle() << "\n";
            issueFile << "Borrower: " << borrower << "\n";
            issueFile << "-----------------------------\n";

            cout << "\nBook issued successfully to " << borrower << "!\n";
            break;
        }
    }

    if (!found)
        cout << "\nBook with ID " << id << " not found!\n";

    inFile.close();
    issueFile.close();
}

// Function to display issued books
void displayIssuedBooks() {
    ifstream issueFile("issued_books.txt");
    if (!issueFile) {
        cout << "\nNo issued books found!\n";
        return;
    }

    cout << "\n===== Issued Book Records =====\n";
    string line;
    while (getline(issueFile, line)) {
        cout << line << endl;
    }
    issueFile.close();
}

int main() {
    int choice;
    do {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====";
        cout << "\n1. Add Book";
        cout << "\n2. Display All Books";
        cout << "\n3. Delete Book";
        cout << "\n4. Issue Book";
        cout << "\n5. Display Issued Books";
        cout << "\n6. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                deleteBook();
                break;
            case 4:
                issueBook();
                break;
            case 5:
                displayIssuedBooks();
                break;
            case 6:
                cout << "\nThank you for using the system.\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}