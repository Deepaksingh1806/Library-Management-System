#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
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
    string getAuthor() const { return author; }
};

//  Add Book (text version)
void addBook() {
    Book b;
    ofstream outFile("library.txt", ios::app); // text mode
    if (!outFile) {
        cout << "\nError opening file for writing!\n";
        return;
    }
    b.getData();
    outFile << b.getBookID() << "|" << b.getTitle() << "|" << b.getAuthor() << "\n";
    outFile.close();
    cout << "\nBook record added successfully!\n";
}

//  Display Books in Table Format
void displayBooks() {
    ifstream inFile("library.txt");
    if (!inFile) {
        cout << "\nNo records found!\n";
        return;
    }

    cout << "\n===========================================================\n";
    cout << left << setw(10) << "| Book ID"
         << setw(30) << "| Title"
         << setw(25) << "| Author" << "|\n";
    cout << "===========================================================\n";

    string line;
    while (getline(inFile, line)) {
        int id;
        string title, author;
        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');

        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            id = stoi(line.substr(0, pos1));
            title = line.substr(pos1 + 1, pos2 - pos1 - 1);
            author = line.substr(pos2 + 1);

            cout << left
                 << "| " << setw(8) << id
                 << "| " << setw(28) << title
                 << "| " << setw(23) << author << "|\n";
        }
    }

    cout << "===========================================================\n";
    inFile.close();
}

//  Delete Book (text version)
void deleteBook() {
    int id;
    cout << "\nEnter Book ID to delete: ";
    cin >> id;

    ifstream inFile("library.txt");
    if (!inFile) {
        cout << "\nNo records found!\n";
        return;
    }

    ofstream outFile("temp.txt");
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        size_t pos = line.find('|');
        if (pos != string::npos) {
            int currentID = stoi(line.substr(0, pos));
            if (currentID != id)
                outFile << line << "\n";
            else
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

//  Issue Book (records in readable text)
void issueBook() {
    int id;
    string borrower;
    bool found = false;

    cout << "\nEnter Book ID to issue: ";
    cin >> id;
    cin.ignore();

    ifstream inFile("library.txt");
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

    string line;
    while (getline(inFile, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');

        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            int currentID = stoi(line.substr(0, pos1));
            string title = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string author = line.substr(pos2 + 1);

            if (currentID == id) {
                found = true;
                cout << "\nBook Found!";
                cout << "\nBook ID: " << currentID;
                cout << "\nTitle: " << title;
                cout << "\nAuthor: " << author;
                cout << "\nEnter Borrower's Name: ";
                getline(cin, borrower);

                issueFile << currentID << "|" << title << "|" << author << "|" << borrower << "\n";
                cout << "\nBook issued successfully to " << borrower << "!\n";
                break;
            }
        }
    }

    if (!found)
        cout << "\nBook with ID " << id << " not found!\n";

    inFile.close();
    issueFile.close();
}

//  Display Issued Books
void displayIssuedBooks() {
    ifstream issueFile("issued_books.txt");
    if (!issueFile) {
        cout << "\nNo issued books found!\n";
        return;
    }

    cout << "\n==========================================================================\n";
    cout << left << setw(10) << "| Book ID"
         << setw(25) << "| Title"
         << setw(20) << "| Author"
         << setw(20) << "| Borrower" << "|\n";
    cout << "==========================================================================\n";

    string line;
    while (getline(issueFile, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);

        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
            int id = stoi(line.substr(0, pos1));
            string title = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string author = line.substr(pos2 + 1, pos3 - pos2 - 1);
            string borrower = line.substr(pos3 + 1);

            cout << left
                 << "| " << setw(8) << id
                 << "| " << setw(23) << title
                 << "| " << setw(18) << author
                 << "| " << setw(18) << borrower << "|\n";
        }
    }

    cout << "==========================================================================\n";
    issueFile.close();
}

//  Main Menu
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
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: deleteBook(); break;
            case 4: issueBook(); break;
            case 5: displayIssuedBooks(); break;
            case 6: cout << "\nThank you for using the system.\n"; break;
            default: cout << "\nInvalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}
