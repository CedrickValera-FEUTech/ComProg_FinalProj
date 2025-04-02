#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

// Maximum number of items that can be stored in inventory.
const int MAX_INVENTORY = 100;

// Maximum number of sales records that can be stored.
const int MAX_SALES = 1000;

// Function to check if a string contains only alphanumeric characters and spaces.
bool isAlphanumeric(const string& str) {
    for (char c : str) {
        if (!isalnum(c) && !isspace(c)) return false; // If a character is not alphanumeric or space, return false.
    }
    return !str.empty(); // Return true if the string is not empty, otherwise false.
}

// Function to get a valid price from the user.
double getValidPrice();

// Function to get a valid quantity from the user.
int getValidQuantity();

// Function to pause the program until the user presses Enter.
void pressEnter();

// Class to represent a merchandise item.
class merchObject {
public:
    string merchID;        // Name of the merchandise item.
    double productPrice;   // Price of the merchandise item.
    int generatedID;       // Unique ID generated for the item.
    int quantity;          // Quantity of the item in stock.
    string supplier;       // Supplier of the item.
    string dateAdded;      // Date and time the item was added.
    string lastUpdated;    // Date and time the item was last updated.

    // Default constructor.
    merchObject() : merchID(""), productPrice(0.0), generatedID(0), quantity(0), supplier(""), dateAdded(""), lastUpdated("") {}

    // Function to get the current date and time as a string.
    string getCurrentDateTime() {
        time_t now = time(0);
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &now);
        return string(timeStr);
    }

    // Function to generate a unique ID for the item.
    int generateUniqueID() {
        return rand() % 90000 + 10000;
    }

    // Function to add a new item to the inventory.
    void addItem() {
        system("cls"); // Clear the console.
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "                ADD MERCH PRODUCT              " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

        // Loop to get a valid item name.
        while (true) {
            cout << "Enter item name (alphanumeric only): ";
            getline(cin, merchID);
            if (!merchID.empty() && isAlphanumeric(merchID)) break; // Break if the name is valid.
            system("cls");
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "                ADD MERCH PRODUCT              " << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "Invalid name! Use only letters, numbers, and spaces.\n";
        }

        productPrice = getValidPrice(); // Get a valid price from the user.
        quantity = getValidQuantity();   // Get a valid quantity from the user.

        // Loop to get a valid supplier name.
        while (true) {
            cout << "Enter supplier name (alphanumeric only): ";
            getline(cin, supplier);
            if (!supplier.empty() && isAlphanumeric(supplier)) break; // Break if the name is valid.
            system("cls");
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "                ADD MERCH PRODUCT              " << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "Invalid supplier name! Use only letters, numbers, and spaces.\n";
        }

        generatedID = generateUniqueID(); // Generate a unique ID.
        dateAdded = getCurrentDateTime();  // Get the current date and time.
        lastUpdated = dateAdded;           // Set the last updated time to the added time.

        cout << "Generated Product ID: " << generatedID << endl;
        cout << getCurrentDateTime();
        pressEnter(); // Pause the program.
    }
};

// Structure to represent a sales record.
struct SaleRecord {
    int productId;     // ID of the sold product.
    int quantitySold;  // Number of quantities sold.
    double totalAmount; // Total amount of the sale.
    string date;        // Date and time of the sale.

    // Default constructor.
    SaleRecord() : productId(0), quantitySold(0), totalAmount(0.0), date("") {}
};

// A Function to count the number of occurrences of a character in a string.
int countChar(const string& str, char target) {
    int count = 0;
    for (char c : str) {
        if (c == target) count++;
    }
    return count;
}

// A Function to check if a string contains invalid characters for a price.
bool hasInvalidPriceChars(const string& str) {
    for (char c : str) {
        if (!isdigit(c) && c != '.') return true;
    }
    return false;
}

// A Function to check if a string contains only digits.
bool isStrictlyNumeric(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return !str.empty();
}

// A Function to pause the program until the user presses Enter.
void pressEnter() {
    cout << "\nPress Enter to continue...";
    while (_getch() != 13);
}

// A Function to get a valid price from the user.
double getValidPrice() {
    string input;
    while (true) {
        cout << "Enter item price: ";
        getline(cin, input);
        input.erase(0, input.find_first_not_of(" "));
        input.erase(input.find_last_not_of(" ") + 1);
        if (!hasInvalidPriceChars(input) && countChar(input, '.') <= 1) {
            try {
                double price = stod(input);
                if (price > 0) return price;
            }
            catch (...) {}
        }
        system("cls");
        cout << "Invalid price! Enter a valid positive number.\n";
    }
}

// A Function to get a valid quantity from the user.
int getValidQuantity() {
    string input;
    while (true) {
        cout << "Enter quantity: ";
        getline(cin, input);
        input.erase(0, input.find_first_not_of(" "));
        input.erase(input.find_last_not_of(" ") + 1);
        if (isStrictlyNumeric(input)) {
            int qty = stoi(input);
            if (qty >= 0) return qty;
        }
        system("cls");
        cout << "Invalid quantity! Enter a valid non-negative number.\n";
    }
}

// A Function to get a valid menu choice from the user.
int getValidMenuChoice() {
    string input;
    while (true) {
        cout << "Enter your choice (1-7): ";
        getline(cin, input);
        input.erase(0, input.find_first_not_of(" "));
        input.erase(input.find_last_not_of(" ") + 1);
        if (isStrictlyNumeric(input)) {
            int choice = stoi(input);
            if (choice >= 1 && choice <= 7) return choice;
        }
        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "                MINI MERCH                     " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "1. Add Merch\n2. Update Merch\n3. Track Sales\n";
        cout << "4. Display Merch\n5. Search Item\n6. Delete Item\n7. Save & Exit\n";
        cout << "Invalid choice! Enter a number between 1 and 7.\n";
    }
}

// A Function to save the inventory to a file.
void saveInventory(merchObject* items[], int count) {
    ofstream outFile("inventory.txt");
    if (!outFile) {
        cout << "Error saving inventory!\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        outFile << "-------------------------------------------" << endl
            << items[i]->generatedID << endl
            << items[i]->merchID << endl
            << items[i]->productPrice << endl
            << items[i]->quantity << endl
            << items[i]->supplier << endl
            << items[i]->dateAdded
            << items[i]->lastUpdated
            << "-------------------------------------------" << endl;
    }
    outFile.close();
    cout << "Inventory saved successfully!\n";
}

// A Function to display the inventory.
void displayMerch(merchObject* items[], int count) {
    system("cls");
    if (count == 0) {
        cout << "No items in inventory.\n";
        pressEnter();
        return;
    }
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "                 INVENTORY                     " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << left << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(10) << "Price"
        << setw(10) << "Qty"
        << setw(20) << "Supplier"
        << "Added/Updated" << endl;
    cout << "------------------------------------------------------------------------------\n";
    for (int i = 0; i < count; i++) {
        cout << left << setw(10) << items[i]->generatedID
            << setw(20) << items[i]->merchID
            << "$" << setw(9) << fixed << setprecision(2) << items[i]->productPrice
            << setw(10) << items[i]->quantity
            << setw(20) << items[i]->supplier
            << items[i]->lastUpdated;
    }
    cout << "------------------------------------------------------------------------------\n";
    pressEnter();
}

// A Function to search for an item in the inventory.
void searchItem(merchObject* items[], int count) {
    system("cls");
    if (count == 0) {
        cout << "No items available to search.\n";
        pressEnter();
        return;
    }
    string query;
    cout << "Enter item name or numeric ID to search: ";
    getline(cin, query);
    query.erase(0, query.find_first_not_of(" "));
    query.erase(query.find_last_not_of(" ") + 1);
    if (query.empty()) {
        system("cls");
        cout << "Invalid input! Search query cannot be empty.\n";
        pressEnter();
        return;
    }
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (items[i]->merchID == query || (isStrictlyNumeric(query) && stoi(query) == items[i]->generatedID)) {
            cout << "\n" << items[i]->generatedID << endl
                << items[i]->merchID << endl
                << "$" << fixed << setprecision(2) << items[i]->productPrice << endl
                << items[i]->quantity << endl
                << items[i]->supplier << endl
                << items[i]->dateAdded
                << items[i]->lastUpdated;
            found = true;
        }
    }
    if (!found) {
        system("cls");
        cout << "Item not found!\n";
    }
    pressEnter();
}

// A Function to update an item in the inventory.
void updateItem(merchObject* items[], int count) {
    system("cls");
    if (count == 0) {
        cout << "No items available to update.\n";
        pressEnter();
        return;
    }
    cout << "Enter product ID to update: ";
    string input;
    getline(cin, input);
    if (!isStrictlyNumeric(input)) {
        cout << "Invalid ID format!\n";
        pressEnter();
        return;
    }
    int id = stoi(input);
    merchObject* item = nullptr;
    for (int i = 0; i < count; i++) {
        if (items[i]->generatedID == id) {
            item = items[i];
            break;
        }
    }
    if (!item) {
        cout << "Product not found!\n";
        pressEnter();
        return;
    }
    cout << "\nCurrent details:\n";
    cout << item->merchID << endl
        << "$" << fixed << setprecision(2) << item->productPrice << endl
        << item->quantity << endl
        << item->supplier << endl
        << item->dateAdded
        << item->lastUpdated << endl;
    cout << "Enter new details (press Enter to keep current value):\n";
    string input_str;
    cout << "New name (alphanumeric only): ";
    getline(cin, input_str);
    if (!input_str.empty()) {
        if (isAlphanumeric(input_str)) {
            item->merchID = input_str;
            item->lastUpdated = item->getCurrentDateTime();
        }
        else {
            cout << "Invalid name format, keeping current name.\n";
        }
    }
    cout << "New price (current: $" << item->productPrice << "): ";
    getline(cin, input_str);
    if (!input_str.empty()) {
        try {
            double newPrice = stod(input_str);
            if (newPrice > 0) {
                item->productPrice = newPrice;
                item->lastUpdated = item->getCurrentDateTime();
            }
            else cout << "Invalid price, keeping current price.\n";
        }
        catch (...) {
            cout << "Invalid price format, keeping current price.\n";
        }
    }
    cout << "New quantity (current: " << item->quantity << "): ";
    getline(cin, input_str);
    if (!input_str.empty() && isStrictlyNumeric(input_str)) {
        int newQty = stoi(input_str);
        if (newQty >= 0) {
            item->quantity = newQty;
            item->lastUpdated = item->getCurrentDateTime();
        }
        else cout << "Invalid quantity, keeping current quantity.\n";
    }
    cout << "New supplier (alphanumeric only): ";
    getline(cin, input_str);
    if (!input_str.empty()) {
        if (isAlphanumeric(input_str)) {
            item->supplier = input_str;
            item->lastUpdated = item->getCurrentDateTime();
        }
        else {
            cout << "Invalid supplier format, keeping current supplier.\n";
        }
    }
    cout << "Item updated successfully!\n";
    cout << item->lastUpdated;
    pressEnter();
}

// A Function to delete an item from the inventory.
void deleteItem(merchObject* items[], int& count) {
    system("cls");
    if (count == 0) {
        cout << "No items available to delete.\n";
        pressEnter();
        return;
    }
    cout << "Enter product ID to delete: ";
    string input;
    getline(cin, input);
    if (!isStrictlyNumeric(input)) {
        cout << "Invalid ID format!\n";
        pressEnter();
        return;
    }
    int id = stoi(input);
    int index = -1;
    for (int i = 0; i < count; i++) {
        if (items[i]->generatedID == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "Product not found!\n";
        pressEnter();
        return;
    }
    cout << items[index]->lastUpdated;
    delete items[index];
    for (int i = index; i < count - 1; i++) {
        items[i] = items[i + 1];
    }
    count--;
    cout << "Item deleted successfully!\n";
    pressEnter();
}

// A Function to display the sales records.
void displaySales(merchObject* items[], int count, SaleRecord sales[], int salesCount) {
    system("cls");
    if (salesCount == 0) {
        cout << "No sales records available.\n";
        pressEnter();
        return;
    }
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "               SALES RECORDS                   " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << left << setw(10) << "ID"
        << setw(15) << "Quantity"
        << setw(15) << "Total"
        << "Date" << endl;
    cout << "------------------------------------------------------------------------------\n";
    double totalSales = 0;
    for (int i = 0; i < salesCount; i++) {
        cout << left << setw(10) << sales[i].productId
            << setw(15) << sales[i].quantitySold
            << "$" << setw(14) << fixed << setprecision(2) << sales[i].totalAmount
            << sales[i].date;
        totalSales += sales[i].totalAmount;
    }
    cout << "------------------------------------------------------------------------------\n";
    cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
    pressEnter();
}

// A Function to record a sale.
void recordSale(merchObject* items[], int count, SaleRecord sales[], int& salesCount) {
    system("cls");
    if (salesCount >= MAX_SALES) {
        cout << "Sales record is full!\n";
        pressEnter();
        return;
    }
    if (count == 0) {
        cout << "No items available for sale.\n";
        pressEnter();
        return;
    }
    cout << "Enter product ID: ";
    string input;
    getline(cin, input);
    if (!isStrictlyNumeric(input)) {
        cout << "Invalid ID format!\n";
        pressEnter();
        return;
    }
    int id = stoi(input);
    merchObject* item = nullptr;
    for (int i = 0; i < count; i++) {
        if (items[i]->generatedID == id) {
            item = items[i];
            break;
        }
    }
    if (!item) {
        cout << "Product not found!\n";
        pressEnter();
        return;
    }
    int quantity = getValidQuantity();
    if (quantity > item->quantity) {
        cout << "Insufficient stock!\n";
        pressEnter();
        return;
    }
    sales[salesCount].productId = id;
    sales[salesCount].quantitySold = quantity;
    sales[salesCount].totalAmount = quantity * item->productPrice;
    sales[salesCount].date = item->getCurrentDateTime();
    salesCount++;
    item->quantity -= quantity;
    item->lastUpdated = item->getCurrentDateTime();
    cout << "Sale recorded successfully!\n";
    cout << item->lastUpdated;
    pressEnter();
}

// Main function of the Merch System
int main() {
    srand(static_cast<unsigned int>(time(0)));
    merchObject* inventory[MAX_INVENTORY];
    SaleRecord sales[MAX_SALES];
    int inventoryCount = 0;
    int salesCount = 0;
    int choice;
    do {
        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "                MINI MERCH                     " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "1. Add Merch\n2. Update Merch\n3. Track Sales\n";
        cout << "4. Display Merch\n5. Search Item\n6. Delete Item\n7. Save & Exit\n";
        choice = getValidMenuChoice();
        switch (choice) {
        case 1:
            if (inventoryCount < MAX_INVENTORY) {
                inventory[inventoryCount] = new merchObject();
                inventory[inventoryCount]->addItem();
                inventoryCount++;
            }
            else {
                cout << "Inventory is full!\n";
                pressEnter();
            }
            break;
        case 2:
            updateItem(inventory, inventoryCount);
            break;
        case 3:
            recordSale(inventory, inventoryCount, sales, salesCount);
            displaySales(inventory, inventoryCount, sales, salesCount);
            break;
        case 4:
            displayMerch(inventory, inventoryCount);
            break;
        case 5:
            searchItem(inventory, inventoryCount);
            break;
        case 6:
            deleteItem(inventory, inventoryCount);
            break;
        case 7:
            saveInventory(inventory, inventoryCount);
            cout << "Inventory saved. Exiting...\n";
            break;
        }
    } while (choice != 7);
    for (int i = 0; i < inventoryCount; i++) {
        delete inventory[i];
    }
    return 0;
}