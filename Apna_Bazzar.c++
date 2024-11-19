#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h> // For getch() function
#include <iomanip> // For std::setw and std::setprecision
#include <limits>
#include <windows.h>  // For Sleep function on Windows


#ifdef _WIN32
#include <cstdlib>  // For system("CLS")
#else
#include <cstdlib>  // For system("clear")
#endif

using namespace std;

// Global flag to control program restart
bool restartFlag = false;  

// Define ANSI color codes
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string BLUE = "\033[34m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string WHITE = "\033[97m";

// Function to clear the screen based on the OS
void clearScreen() {
    #ifdef _WIN32
        system("CLS");  // Windows
    #else
        system("clear");  // Linux/Mac
    #endif
}

// Function to print the welcome message
void printWelcomeMessage() {
    cout << endl;
    cout << CYAN << "*********************************************" << RESET << endl;
    cout << "*                                           *" << endl;
    cout << "*           " << BOLD << BLUE << "WELCOME TO APNA BAZAAR" << RESET << "          *" << endl;
    cout << "*                                           *" << endl;
    cout << CYAN << "*********************************************" << RESET << endl;
    cout << endl;
}

// Struct for CartItem
struct CartItem {
    string name;
    double price;
};

// Class for Item management
class Item {
public:
    string name;
    string description;
    double price;
    string category;


    
void addItem() {
        // Hardcoded existing categories (replace with your own list if needed)
        string existingCategories[] = {"Fashion", "Electronics", "Books", "Home & Kitchen"};

        cout << "Do you want to add to an existing category or create a new one? (1 for existing, 2 for new): ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            // Display all existing categories
            cout << "Choose a category from the existing ones:\n";
            for (int i = 0; i < 4; ++i) {
                cout << i + 1 << ". " << existingCategories[i] << endl;
            }

            cout << "Enter the number of the category you want to add to: ";
            int catChoice;
            cin >> catChoice;

            // Validate the input (category number should be in the valid range)
            if (catChoice > 0 && catChoice <= 4) {
                category = existingCategories[catChoice - 1];
            } else {
                cout << "Invalid choice. Returning to menu.\n";
                return;
            }
        }
        else if (choice == 2) {
            cout << "Enter new category name: ";
            cin >> category;

            // Create a new file for the new category
            ofstream newCategoryFile(category + ".txt");
            if (!newCategoryFile) {
                cout << "Error creating new category file!" << endl;
                return;
            }
            newCategoryFile.close();  // Close the file
            cout << "New category created: " << category << endl;
        } else {
            cout << "Invalid choice. Returning to main menu.\n";
            return;
        }

        // Collect item details
        cout << "Enter item name: ";
        cin.ignore();  // To ignore the newline left by previous input
        getline(cin, name);

        cout << "Enter item description: ";
        getline(cin, description);

        // Validate the price input to ensure it's a valid number
        cout << "Enter item price in Rupees: ";
        while (!(cin >> price) || price <= 0) {
            cout << "Invalid price. Please enter a valid price in Rupees: ";
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the incorrect input
        }

        // Save item to the respective category file
        ofstream categoryOutFile(category + ".txt", ios::app);
        categoryOutFile << name << " " << description << " " << price << endl;
        categoryOutFile.close();

        cout << "Item added successfully!" << endl;
    }



    
    void updateItem() {
        string itemName;
        cout << "Enter the name of the item to update: ";
        cin.ignore();
        getline(cin, itemName);

        string line;
        ifstream categoryFile;
        categoryFile.open("Fashion.txt"); // Open the appropriate category file
        if (!categoryFile.is_open()) {
            cout << "Could not open category file.\n";
            return;
        }

        vector<string> lines;
        bool found = false;

        while (getline(categoryFile, line)) {
            if (line.find(itemName) != string::npos) {
                found = true;
                cout << "Current details: " << line << endl;
                cout << "Enter new category (leave blank to keep current): ";
                string newCategory;
                getline(cin, newCategory);
                if (newCategory.empty()) newCategory = category;

                cout << "Enter new name (leave blank to keep current): ";
                string newName;
                getline(cin, newName);
                if (newName.empty()) newName = name;

                cout << "Enter new description (leave blank to keep current): ";
                string newDescription;
                getline(cin, newDescription);
                if (newDescription.empty()) newDescription = description;

                cout << "Enter new price (leave blank to keep current ): ";
                string newPriceStr;
                getline(cin, newPriceStr);
                double newPrice = price;
                if (!newPriceStr.empty()) newPrice = stod(newPriceStr);

                line = newName + " " + newDescription + " " + to_string(newPrice);
            }
            lines.push_back(line);
        }

        categoryFile.close();

        // Write back the updated lines to the file
        ofstream outFile("Fashion.txt");
        for (const auto& l : lines) {
            outFile << l << endl;
        }
        outFile.close();

        if (found) {
            cout << "Item updated successfully!\n";
        } else {
            cout << "Item not found.\n";
        }
    }

    void showItemsInCategory(vector<CartItem>& cart) {
    cout << "Available Categories:\n1. Fashion\n2. Electronics\n3. Books\n4. Home & Kitchen\n";
    int categoryChoice;
    cout << "Select a category (1-4): ";
    cin >> categoryChoice;

    string selectedCategory;
    switch (categoryChoice) {
        case 1: selectedCategory = "Fashion"; break;
        case 2: selectedCategory = "Electronics"; break;
        case 3: selectedCategory = "Books"; break;
        case 4: selectedCategory = "Home & Kitchen"; break;
        default: cout << "Invalid choice!\n"; return;
    }

    ifstream categoryFile(selectedCategory + ".txt");
    if (!categoryFile) {
        cout << "No items found in this category or file could not be opened.\n";
        return;
    }

    cout << "\nItems in " << selectedCategory << " category:\n";
    vector<Item> items; // Temporary storage for items
    string line;
    int itemIndex = 1;

    while (getline(categoryFile, line)) {
        if (line.empty()) continue;

        Item tempItem;
        size_t lastSpace = line.find_last_of(' ');
        if (lastSpace == string::npos) continue;

        tempItem.price = stod(line.substr(lastSpace + 1));
        string itemDetails = line.substr(0, lastSpace);
        size_t descSpace = itemDetails.find_last_of(' ');

        if (descSpace == string::npos) continue;

        tempItem.description = itemDetails.substr(descSpace + 1);
        tempItem.name = itemDetails.substr(0, descSpace);
        tempItem.category = selectedCategory;

        cout << itemIndex << ". Name: " << tempItem.name << ", Description: " << tempItem.description << ", Price: $" << tempItem.price << endl;

        items.push_back(tempItem);
        itemIndex++;
    }
    categoryFile.close();

    int itemChoice;
    cout << "\nEnter the number of the item you want to take action on (0 to cancel): ";
    cin >> itemChoice;

    if (itemChoice > 0 && itemChoice <= items.size()) {
        int action;
        cout << "1. Add to Cart\n2. Buy Now\nChoose an action (1-2): ";
        cin >> action;

        if (action == 1) {
            CartItem cartItem = {items[itemChoice - 1].name, items[itemChoice - 1].price};
            cart.push_back(cartItem);

            // Save to cart file
            ofstream cartFile("cart.txt", ios::app);
            if (cartFile) {
                cartFile << cartItem.name << "," << cartItem.price << endl;
                cartFile.close();
                cout << "Item added to cart!\n";
            } else {
                cout << "Error: Unable to save cart data.\n";
            }
        } else if (action == 2) {
            cout << "\nBuying the item...\n";
            CartItem cartItem = {items[itemChoice - 1].name, items[itemChoice - 1].price};
            cout << "Item purchased successfully!\n";
            generateBill({cartItem}); // Generate bill for the purchased item
        } else {
            cout << "Invalid action selected.\n";
        }
    } else if (itemChoice == 0) {
        cout << "Action canceled.\n";
    } else {
        cout << "Invalid item selection.\n";
    }
}

void generateBill(const vector<CartItem>& cart) {
    double total = 0;
    string phoneNumber, address;

    cout << "Please enter your phone number: ";
    cin >> phoneNumber;
    cout << "Please enter your address: ";
    cin.ignore();
    getline(cin, address);

    ofstream billFile("bills.txt", ios::app);
    if (!billFile) {
        cerr << "Error: Unable to save the bill.\n";
        return;
    }

    cout << "===============================================\n";
    cout << "                  APNA BAZAAR Invoice         \n";
    cout << "===============================================\n";

    cout << "Customer Details:\n";
    cout << "Phone Number: " << phoneNumber << endl;
    cout << "Address     : " << address << endl;

    billFile << phoneNumber << endl; // Save phone number in the bill file

    cout << "-----------------------------------------------\n";
    cout << "Items Purchased:\n";

    cout << left << setw(25) << "Item" << right << setw(10) << "Price ($)" << endl;
    cout << "-----------------------------------------------\n";

    for (const auto& item : cart) {
        cout << left << setw(25) << item.name << right << setw(10) << fixed << setprecision(2) << item.price << endl;
        total += item.price;

        // Save item details in the bill file
        billFile << item.name << "," << fixed << setprecision(2) << item.price << endl;
    }

    cout << "-----------------------------------------------\n";
    cout << "Total Amount Due: $" << fixed << setprecision(2) << total << endl;
    cout << "===============================================\n";
    cout << "          Thank you for shopping with us!       \n";
    cout << "===============================================\n";

    billFile << "Total:" << fixed << setprecision(2) << total << endl;
    billFile << "---" << endl; // Separator between bills
    billFile.close();

    cout << "Press Enter to go back to the main menu...";
    cin.ignore();
    cin.get();
}


};

// User class for Login and Registration
class User {
private:
    string username;
    string password;

    // Helper function to mask password input
    string getPasswordInput() {
        string password;
        char ch;
        cout << "Enter password: ";
        while ((ch = getch()) != '\r') { // '\r' is the Enter key
            if (ch == '\b' && !password.empty()) { // Handle backspace
                password.pop_back();
                cout << "\b \b"; // Remove last '*' from display
            } else if (ch != '\b') { // Add character to password
                password += ch;
                cout << '*'; // Display '*'
            }
        }
        cout << endl; // Move to the next line after Enter key
        return password;
    }

public:
    // Registration
    bool registerUser() {
        cout << "Enter a username for registration: ";
        cin >> username;
        password = getPasswordInput(); // Get masked password input

        ofstream file("users.txt", ios::app);
        if (!file) {
            cerr << "Error opening file for registration.\n";
            return false;
        }

        file << username << " " << password << endl;
        file.close();
        cout << "Registration successful!\n";
        return true;
    }

    // Login
    bool login() {
        string inputUsername, inputPassword;

        while (true) { // Loop until successful login or user exits
            cout << "Enter username: ";
            cin >> inputUsername;
            inputPassword = getPasswordInput(); // Get masked password input

            ifstream file("users.txt");
            if (!file) {
                cerr << "Error opening file for login.\n";
                return false;
            }

            string fileUsername, filePassword;
            bool loginSuccess = false;
            while (file >> fileUsername >> filePassword) {
                if (fileUsername == inputUsername && filePassword == inputPassword) {
                    cout << "Login successful! Welcome back, " << inputUsername << "!\n";
                    loginSuccess = true;
                    break;
                }
            }
            file.close();

            if (loginSuccess) {
                return true;
            } else {
                cout << "Login failed. Incorrect username or password. Try again.\n";
            }
        }
    }


void deleteAccount() {
    cout << "Are you sure you want to delete your account? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        ifstream userFile("users.txt");
        ofstream tempFile("temp.txt");

        string u, p;
        bool accountFound = false;

        while (userFile >> u >> p) {
            // If the username matches the current user, skip writing this line
            if (u != username) {
                tempFile << u << " " << p << endl; // Write the other users to the temp file
            } else {
                accountFound = true; // Mark that the account was found and removed
            }
        }

        userFile.close();
        tempFile.close();

        if (accountFound) {
            // Remove the original users.txt file
            remove("users.txt");

            // Rename the temp file to users.txt
            rename("temp.txt", "users.txt");

            cout << "Account deleted successfully! Please register again." << endl;
        } else {
            cout << "Account not found!" << endl;
        }

        // Wait for user to press Enter before exiting
        cout << "Press Enter to exit the program...";
        cin.ignore();  // To discard any leftover newline character
        cin.get();     // Wait for Enter key to be pressed

        exit(0); // Exit the program after deleting the account
    }
}
};





class ShoppingCart {
public:
   
   void showCart() {
    vector<CartItem> cart;

    // Load cart data from file
    ifstream cartFile("cart.txt");
    if (cartFile) {
        string line;
        while (getline(cartFile, line)) {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                string name = line.substr(0, commaPos);
                double price = stod(line.substr(commaPos + 1));
                cart.push_back({name, price});
            }
        }
        cartFile.close();
    }

    if (cart.empty()) {
        cout << "Your cart is empty!\n";
        return;
    }

    cout << "Items in your cart:\n";
    for (const auto& item : cart) {
        cout << "Item: " << item.name << ", Price: $" << item.price << endl;
    }
    cout << "Press Enter to go back to the main menu...";
    cin.ignore();
    cin.get();
}


void showBill() {
        string phoneNumber;
        cout << "Enter your phone number to view past bills: ";
        cin >> phoneNumber;

        ifstream billFile("bills.txt");
        if (!billFile) {
            cerr << "Error: Unable to open bill file.\n";
            return;
        }

        string line;
        bool billFound = false;
        double total = 0;

        cout << "===============================================\n";
        cout << "                  AP NA BAZAAR Invoice         \n";
        cout << "===============================================\n";

        while (getline(billFile, line)) {
            if (line == phoneNumber) { // Found a matching phone number
                billFound = true;
                cout << "Customer Phone Number: " << phoneNumber << endl;

                cout << "-----------------------------------------------\n";
                cout << "Items Purchased:\n";

                cout << left << setw(25) << "Item" << right << setw(10) << "Price ($)" << endl;
                cout << "-----------------------------------------------\n";

                while (getline(billFile, line) && line != "---") {
                    if (line.find("Total:") != string::npos) {
                        total = stod(line.substr(6)); // Extract total amount
                        continue;
                    }

                    size_t commaPos = line.find(',');
                    if (commaPos != string::npos) {
                        string itemName = line.substr(0, commaPos);
                        double itemPrice = stod(line.substr(commaPos + 1));

                        cout << left << setw(25) << itemName << right << setw(10) << fixed << setprecision(2) << itemPrice << endl;
                    }
                }

                cout << "-----------------------------------------------\n";
                cout << "Total Amount Paid: $" << fixed << setprecision(2) << total << endl;
                cout << "===============================================\n";
            }
        }

        if (!billFound) {
            cout << "No bill exists for the phone number: " << phoneNumber << endl;
        }

        billFile.close();
        cout << "Press Enter to go back to the main menu...";
        cin.ignore();
        cin.get();
    }

};

int main() {
    vector<CartItem> cart;         // To store cart items
    User user;                     // For user login and account management
    Item item;                     // For item management
    ShoppingCart shoppingCart;     // For shopping cart operations (assuming this exists)

    int choice;

    do {
        clearScreen(); // Clear the screen for the main menu
        printWelcomeMessage(); // Print the welcome message

        cout << "\nWelcome:\n1. Register\n2. Login\n3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            if (user.registerUser()) {
                cout << "Registration successful! Please log in to access the system.\n";
            }
            break;

        case 2:
            if (user.login()) {
                int menuChoice;
                do {
                    clearScreen(); // Clear the screen for the main menu
                    printWelcomeMessage(); // Print the welcome message again

                    cout << "\nMenu:\n1. Manage Account\n2. Show Items\n3. Show Cart\n4. Show Bill\n5. Exit\n";
                    cout << "Enter your choice: ";
                    cin >> menuChoice;

                    switch (menuChoice) {
                    case 1: { // Manage account submenu
                        int manageChoice;
                        do {
                            clearScreen(); // Clear the screen for account management
                            printWelcomeMessage(); // Print the welcome message again

                            cout << "\nManage Account:\n1. Add Item\n2. Update Item\n3. Delete Account\n4. Back to Main Menu\n";
                            cout << "Enter your choice: ";
                            cin >> manageChoice;

                            switch (manageChoice) {
                            case 1:
                                item.addItem();
                                break;
                            case 2:
                                item.updateItem();
                                break;
                            case 3:
                                user.deleteAccount();  // Deletes account and exits the program
                                break;
                            case 4:
                                cout << "Returning to main menu...\n";
                                break;
                            default:
                                cout << "Invalid choice! Please try again.\n";
                            }
                        } while (manageChoice != 4);
                        break;
                    }
                    case 2:
                        item.showItemsInCategory(cart); // Show items and allow adding to cart
                        break;

                    case 3:
                        shoppingCart.showCart(); // Show items in the cart
                        break;

                    case 4:
                        shoppingCart.showBill(); // Generate bill for items in the cart
                        break;

                    case 5:
                        cout << "Exiting the program. Goodbye!\n";
                        menuChoice = 5; // Exit the program
                        break;

                    default:
                        cout << "Invalid choice! Please try again.\n";
                    }
                } while (menuChoice != 5);
            }
            break;

        case 3:
            cout << "Exiting the program. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}
