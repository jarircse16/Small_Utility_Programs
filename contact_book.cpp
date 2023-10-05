#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

// Define a structure to represent a contact
struct Contact {
    string name;
    string phoneNumber;

    // Constructor with default parameter values
    Contact(const string& n = "", const string& num = "") : name(n), phoneNumber(num) {}
};

// Function to display all contacts sorted by name
void viewAllContacts(const unordered_map<string, Contact>& contactBook) {
    vector<string> sortedNames;
    for (const auto& entry : contactBook) {
        sortedNames.push_back(entry.first);
    }

    sort(sortedNames.begin(), sortedNames.end());

    cout << "All Contacts (Sorted by Name):" << endl;
    for (const string& name : sortedNames) {
        const Contact& contact = contactBook.at(name);
        cout << "Name: " << contact.name << endl;
        cout << "Phone Number: " << contact.phoneNumber << endl;
        cout << "------------------------" << endl;
    }
}

// Function to save contacts to a file
void saveContactsToFile(const unordered_map<string, Contact>& contactBook, const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Unable to open the file for saving contacts." << endl;
        return;
    }

    for (const auto& entry : contactBook) {
        const Contact& contact = entry.second;
        file << contact.name << "," << contact.phoneNumber << endl;
    }

    file.close();
    cout << "Contacts saved to " << filename << " successfully!" << endl;
}

// Function to load contacts from a file
void loadContactsFromFile(unordered_map<string, Contact>& contactBook, const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Unable to open the file for loading contacts." << endl;
        return;
    }

    contactBook.clear(); // Clear existing contacts

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(",");
        if (pos != string::npos) {
            string name = line.substr(0, pos);
            string phoneNumber = line.substr(pos + 1);
            contactBook[name] = Contact(name, phoneNumber);
        }
    }

    file.close();
    cout << "Contacts loaded from " << filename << " successfully!" << endl;
}

int main() {
    // Create a hashmap to store contacts, where the name is the key and Contact is the value
    unordered_map<string, Contact> contactBook;

    // Specify the filename to save and load contacts
    const string filename = "contacts.txt";

    // Load contacts from the file (if the file exists)
    loadContactsFromFile(contactBook, filename);

    while (true) {
        cout << "Contact Book Menu:" << endl;
        cout << "1. Add Contact" << endl;
        cout << "2. Edit Contact" << endl;
        cout << "3. Delete Contact" << endl;
        cout << "4. Read Contact" << endl;
        cout << "5. Search Contact" << endl;
        cout << "6. View All Contacts (Sorted)" << endl;
        cout << "7. Save Contacts to File" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();  // Clear the newline character from the input buffer

        switch (choice) {
            case 1: {
                // Add Contact
                string name, phoneNumber;
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Phone Number: ";
                getline(cin, phoneNumber);
                contactBook[name] = Contact(name, phoneNumber);
                cout << "Contact added successfully!" << endl;
                break;
            }
            case 2: {
                // Edit Contact
                string name;
                cout << "Enter Name to Edit: ";
                getline(cin, name);
                if (contactBook.find(name) != contactBook.end()) {
                    string newPhoneNumber;
                    cout << "Enter New Phone Number: ";
                    getline(cin, newPhoneNumber);
                    contactBook[name].phoneNumber = newPhoneNumber;
                    cout << "Contact edited successfully!" << endl;
                } else {
                    cout << "Contact not found!" << endl;
                }
                break;
            }
            case 3: {
                // Delete Contact
                string name;
                cout << "Enter Name to Delete: ";
                getline(cin, name);
                if (contactBook.find(name) != contactBook.end()) {
                    contactBook.erase(name);
                    cout << "Contact deleted successfully!" << endl;
                } else {
                    cout << "Contact not found!" << endl;
                }
                break;
            }
            case 4: {
                // Read Contact
                string name;
                cout << "Enter Name to Read: ";
                getline(cin, name);
                if (contactBook.find(name) != contactBook.end()) {
                    Contact& contact = contactBook[name];
                    cout << "Name: " << contact.name << endl;
                    cout << "Phone Number: " << contact.phoneNumber << endl;
                } else {
                    cout << "Contact not found!" << endl;
                }
                break;
            }
            case 5: {
                // Search Contact
                string name;
                cout << "Enter Name to Search: ";
                getline(cin, name);
                if (contactBook.find(name) != contactBook.end()) {
                    Contact& contact = contactBook[name];
                    cout << "Contact found:" << endl;
                    cout << "Name: " << contact.name << endl;
                    cout << "Phone Number: " << contact.phoneNumber << endl;
                } else {
                    cout << "Contact not found!" << endl;
                }
                break;
            }
            case 6:
                // View All Contacts (Sorted by Name)
                viewAllContacts(contactBook);
                break;
            case 7:
                // Save Contacts to File
                saveContactsToFile(contactBook, filename);
                break;
            case 8:
                // Exit
                cout << "Exiting Contact Book. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
