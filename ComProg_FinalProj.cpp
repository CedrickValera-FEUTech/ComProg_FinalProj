#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

const int MAX_INVENTORY = 10;
const int MAX_SALES = 100;

// Function to check if a string contains only alphanumeric characters and spaces.
bool isAlphanumeric(const string& str) {
    for (char c : str) {
        if (!isalnum(c) && !isspace(c)) return false;
    }
    return !str.empty();
}

// Add these declarations after the existing function declarations
double getValidPrice();
int getValidQuantity();
void pressEnter();
bool isStrictlyNumeric(const string& str);
bool hasInvalidPriceChars(const string& str);
int countChar(const string& str, char target);

class merchObject {
public:
    string merchID;
    double productPrice;
    int generatedID;
    int quantity;
    string supplier;
    string dateAdded;
    string lastUpdated;

    merchObject() : merchID(""), productPrice(0.0), generatedID(0), quantity(0), supplier(""), dateAdded(""), lastUpdated("") {}

    string getCurrentDateTime() {
        time_t now = time(0);
        tm localTime;
        localtime_s(&localTime, &now);
        char timeStr[32];
        strftime(timeStr, sizeof(timeStr), "Saved on: %Y-%m-%d %H:%M:%S", &localTime);
        return string(timeStr) + "\n";
    }

    int generateUniqueID() {
        return rand() % 90000 + 10000;
    }

    void addItem() {
        string tempName = "", tempSupplier = "";
        double tempPrice = 0;
        int tempQty = 0;
        bool nameValid = false, priceValid = false, qtyValid = false, supplierValid = false;
        string input;

        while (!nameValid || !priceValid || !qtyValid || !supplierValid) {
            system("cls");
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "                ADD MERCH PRODUCT              " << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

            // Display valid inputs
            if (nameValid) cout << "Item name: " << tempName << endl;
            if (priceValid) cout << "Price: $" << fixed << setprecision(2) << tempPrice << endl;
            if (qtyValid) cout << "Quantity: " << tempQty << endl;
            if (supplierValid) cout << "Supplier: " << tempSupplier << endl;

            // Get name if not valid
            if (!nameValid) {
                cout << "Enter item name: ";
                getline(cin, input);
                input.erase(0, input.find_first_not_of(" "));
                input.erase(input.find_last_not_of(" ") + 1);

                if (!input.empty() && isAlphanumeric(input)) {
                    tempName = input;
                    nameValid = true;
                }
                else {
                    cout << "Invalid name! Use only letters, numbers, and spaces.\n";
                    pressEnter();
                    continue;
                }
            }

            // Get price if not valid
            if (nameValid && !priceValid) {
                cout << "Enter item price: ";
                getline(cin, input);
                input.erase(0, input.find_first_not_of(" "));
                input.erase(input.find_last_not_of(" ") + 1);

                if (!hasInvalidPriceChars(input) && countChar(input, '.') <= 1) {
                    try {
                        double price = stod(input);
                        if (price > 0) {
                            tempPrice = price;
                            priceValid = true;
                        }
                        else {
                            cout << "Invalid price! Enter a positive number.\n";
                            pressEnter();
                            continue;
                        }
                    }
                    catch (...) {
                        cout << "Invalid price format!\n";
                        pressEnter();
                        continue;
                    }
                }
                else {
                    cout << "Invalid price! Enter a valid number.\n";
                    pressEnter();
                    continue;
                }
            }

            // Get quantity if not valid
            if (priceValid && !qtyValid) {
                cout << "Enter quantity: ";
                getline(cin, input);
                input.erase(0, input.find_first_not_of(" "));
                input.erase(input.find_last_not_of(" ") + 1);

                if (isStrictlyNumeric(input)) {
                    int qty = stoi(input);
                    if (qty >= 0) {
                        tempQty = qty;
                        qtyValid = true;
                    }
                    else {
                        cout << "Invalid quantity! Enter a non-negative number.\n";
                        pressEnter();
                        continue;
                    }
                }
                else {
                    cout << "Invalid quantity! Enter a valid number.\n";
                    pressEnter();
                    continue;
                }
            }

            // Get supplier if not valid
            if (qtyValid && !supplierValid) {
                cout << "Enter supplier name: ";
                getline(cin, input);
                input.erase(0, input.find_first_not_of(" "));
                input.erase(input.find_last_not_of(" ") + 1);

                if (!input.empty() && isAlphanumeric(input)) {
                    tempSupplier = input;
                    supplierValid = true;
                }
                else {
                    cout << "Invalid supplier name! Use only letters, numbers, and spaces.\n";
                    pressEnter();
                    continue;
                }
            }
        }

        // All inputs are valid, save to object
        merchID = tempName;
        productPrice = tempPrice;
        quantity = tempQty;
        supplier = tempSupplier;
        generatedID = generateUniqueID();
        dateAdded = getCurrentDateTime();
        lastUpdated = dateAdded;

        cout << "\nGenerated Product ID: " << generatedID << endl;
        cout << getCurrentDateTime();
        pressEnter();
    }
};

struct SaleRecord {
    int productId;      // ID of the product sold
    int quantitySold;   // Number of units sold
    double totalAmount; // Total sale amount for this transaction
    string date;        // Date and time of the sale

    SaleRecord() : productId(0), quantitySold(0), totalAmount(0.0), date("") {}
};

// Add after the SaleRecord structure
struct Supplier {
    int id;
    string name;
    string contact;
    string address;

    Supplier() : id(0), name(""), contact(""), address("") {}

    string toString() const {
        return name + " | " + contact + " | " + address;
    }
};

// Function to count the occurrences of a specified character in a string.
int countChar(const string& str, char target) {
    int count = 0;
    for (char c : str) {
        if (c == target) count++; // Increment count if the character matches the target.
    }
    return count; // Return the total count of the target character.
}

// Function to check if a string contains invalid characters for a price (non-digit and not a period).
bool hasInvalidPriceChars(const string& str) {
    for (char c : str) {
        if (!isdigit(c) && c != '.') return true; // If any character is not a digit or a period, return true.
    }
    return false; // Return false if all characters are valid for a price.
}

// Function to check if a string contains only digits.
bool isStrictlyNumeric(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false; // If any character is not a digit, return false.
    }
    return !str.empty(); // Return true if the string is not empty and all characters are digits.
}

// Function to pause the program until the user presses the Enter key.
void pressEnter() {
    cout << "\nPress Enter to continue...";
    while (_getch() != 13); // Wait for the Enter key (ASCII 13).
}

// Function to prompt the user to enter a valid price and return it as a double.
double getValidPrice() {
    string input;
    while (true) {
        cout << "Enter item price: ";
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" ")); // Remove leading spaces.
        input.erase(input.find_last_not_of(" ") + 1); // Remove trailing spaces.

        // Check if the input is valid for a price.
        if (!hasInvalidPriceChars(input) && countChar(input, '.') <= 1) {
            try {
                double price = stod(input); // Convert string to double.
                if (price > 0) return price; // Return the price if it's positive.
            }
            catch (...) {} // Catch any exceptions during conversion (invalid format).
        }

        system("cls"); // Clear the screen for error message.
        cout << "Invalid price! Enter a valid positive number.\n";
    }
}


// Function to prompt the user to enter a valid quantity and return it as an integer.
int getValidQuantity() {
    string input;
    while (true) {
        cout << "Enter quantity: ";
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" ")); // Remove leading spaces.
        input.erase(input.find_last_not_of(" ") + 1); // Remove trailing spaces.

        // Check if the input is strictly numeric.
        if (isStrictlyNumeric(input)) {
            int qty = stoi(input); // Convert string to integer.
            if (qty >= 0) return qty; // Return the quantity if it's non-negative.
        }

        system("cls"); // Clear the screen for error message.
        cout << "Invalid quantity! Enter a valid non-negative number.\n";
    }
}

// Function to prompt the user to enter a valid menu choice and return it as an integer.
int getValidMenuChoice() {
    string input;
    while (true) {
        cout << "Enter your choice (1-7): ";
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" ")); // Remove leading spaces.
        input.erase(input.find_last_not_of(" ") + 1); // Remove trailing spaces.

        // Check if the input is strictly numeric.
        if (isStrictlyNumeric(input)) {
            int choice = stoi(input); // Convert string to integer.
            if (choice >= 1 && choice <= 7) return choice; // Return the choice if it's within the valid range.
        }

        // Display the menu again with an error message.
        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "          MERCHANDISE MANAGEMENT          " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "          1. Add Merch Product           " << endl;
        cout << "          2. Update Merch Product          " << endl;
        cout << "          3. Track Sales Product           " << endl;
        cout << "          4. Display Merch Product         " << endl;
        cout << "          5. Search Merch Product          " << endl;
        cout << "          6. Delete Merch Product          " << endl;
        cout << "          7. Save and Exit                " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "Invalid choice! Enter a number between 1 and 7.\n";
    }
}


void saveInventory(merchObject* items[], int count) {
    ofstream outFile("inventory.txt");
    if (!outFile) {
        cout << "Error saving inventory!\n";
        return;
    }

    outFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    outFile << left << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(10) << "Price"
        << setw(10) << "Quantity"
        << setw(20) << "Supplier"
        << "Date" << endl;
    outFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

    for (int i = 0; i < count; i++) {
        outFile << left << setw(10) << items[i]->generatedID
            << setw(20) << items[i]->merchID
            << setw(10) << fixed << setprecision(2) << items[i]->productPrice
            << setw(10) << items[i]->quantity
            << setw(20) << items[i]->supplier
            << items[i]->lastUpdated;
    }

    outFile.close();
    cout << "Inventory saved successfully!\n";
}

void displayMerch(merchObject* items[], int count) {
    system("cls");
    if (count == 0) {
        cout << "No items in inventory.\n";
        pressEnter();
        return;
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "                                                         INVENTORY                                                       " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << left << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(10) << "Price"
        << setw(10) << "Qty"
        << setw(20) << "Supplier"
        << "Added/Updated" << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

    for (int i = 0; i < count; i++) {
        cout << left << setw(10) << items[i]->generatedID
            << setw(20) << items[i]->merchID
            << "$" << setw(9) << fixed << setprecision(2) << items[i]->productPrice
            << setw(10) << items[i]->quantity
            << setw(20) << items[i]->supplier
            << items[i]->lastUpdated;
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    pressEnter();
}

// Add this helper function after the other utility functions
// Function to check if a string contains another string, ignoring case.
bool containsIgnoreCase(const string& str, const string& search) {
    if (search.length() > str.length()) return false; // If search string is longer, it can't be contained.

    for (size_t i = 0; i <= str.length() - search.length(); i++) {
        bool found = true;
        for (size_t j = 0; j < search.length(); j++) {
            if (tolower(str[i + j]) != tolower(search[j])) { // Compare characters ignoring case.
                found = false;
                break;
            }
        }
        if (found) return true; // Return true if the search string is found.
    }
    return false; // Return false if the search string is not found.
}

// Update the searchItem function
void searchItem(merchObject* items[], int count) {
    system("cls");
    if (count == 0) {
        cout << "No items available to search.\n";
        pressEnter();
        return;
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "               SEARCH PRODUCT                 " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "Enter item name or ID to search: ";
    string query;
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
        if (containsIgnoreCase(items[i]->merchID, query) ||
            (isStrictlyNumeric(query) && stoi(query) == items[i]->generatedID)) {
            if (!found) {
                cout << "\nSearch Results:\n";
                cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            }
            cout << "ID: " << items[i]->generatedID << endl
                << "Name: " << items[i]->merchID << endl
                << "Price: $" << fixed << setprecision(2) << items[i]->productPrice << endl
                << "Quantity: " << items[i]->quantity << endl
                << "Supplier: " << items[i]->supplier << endl
                << "Added: " << items[i]->dateAdded
                << "Last Updated: " << items[i]->lastUpdated
                << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
            found = true;
        }
    }

    if (!found) {
        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "               SEARCH PRODUCT                 " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "No items found matching '" << query << "'\n";
    }
    pressEnter();
}

// Update the updateItem function
void updateItem(merchObject* items[], int count) {
    system("cls");
    if (count == 0) {
        cout << "No items available to update.\n";
        pressEnter();
        return;
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "               UPDATE PRODUCT                 " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "Enter product name or ID to update: ";
    string input;
    getline(cin, input);

    input.erase(0, input.find_first_not_of(" "));
    input.erase(input.find_last_not_of(" ") + 1);

    if (input.empty()) {
        cout << "Invalid input! Search query cannot be empty.\n";
        pressEnter();
        return;
    }

    merchObject* item = nullptr;
    int matchCount = 0;
    int lastMatchIndex = -1;

    // Search for matches
    for (int i = 0; i < count; i++) {
        if ((isStrictlyNumeric(input) && items[i]->generatedID == stoi(input)) ||
            containsIgnoreCase(items[i]->merchID, input)) {
            matchCount++;
            lastMatchIndex = i;
            if (matchCount == 1) {
                cout << "\nMatching products:\n";
                cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            }
            cout << matchCount << ". ID: " << items[i]->generatedID
                << " | Name: " << items[i]->merchID
                << " | Price: $" << fixed << setprecision(2) << items[i]->productPrice << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        }
    }

    // Handle matches
    if (matchCount == 1) {
        item = items[lastMatchIndex];
    }
    else if (matchCount > 1) {
        cout << "\nMultiple matches found. Please enter the ID of the product you want to update: ";
        string idInput;
        getline(cin, idInput);

        if (isStrictlyNumeric(idInput)) {
            int id = stoi(idInput);
            for (int i = 0; i < count; i++) {
                if (items[i]->generatedID == id) {
                    item = items[i];
                    break;
                }
            }
        }
    }

    if (!item) {
        cout << "Product not found!\n";
        pressEnter();
        return;
    }

    cout << "\nCurrent details:\n";
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "ID: " << item->generatedID << endl
        << "Name: " << item->merchID << endl
        << "Price: $" << fixed << setprecision(2) << item->productPrice << endl
        << "Quantity: " << item->quantity << endl
        << "Supplier: " << item->supplier << endl
        << "Date Added: " << item->dateAdded
        << "Last Updated: " << item->lastUpdated;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

    cout << "Enter new details (press Enter to keep current value):\n";
    string input_str;

    cout << "\nNew name (current: " << item->merchID << "): ";
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

    cout << "New price (current: $" << fixed << setprecision(2) << item->productPrice << "): ";
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

    cout << "New supplier (current: " << item->supplier << "): ";
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

    cout << "\nItem updated successfully!\n";
    cout << "Last Updated: " << item->lastUpdated;
    pressEnter();
}

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

void displaySales(merchObject* items[], int count, SaleRecord sales[], int salesCount) {
    system("cls");
    if (salesCount == 0) {
        cout << "No sales records available.\n";
        pressEnter();
        return;
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "                                                     SALES RECORDS                                                     " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << left << setw(10) << "ID"
        << setw(15) << "Quantity"
        << setw(15) << "Total"
        << "Date" << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

    double totalSales = 0;
    for (int i = 0; i < salesCount; i++) {
        cout << left << setw(10) << sales[i].productId
            << setw(15) << sales[i].quantitySold
            << "$" << setw(14) << fixed << setprecision(2) << sales[i].totalAmount
            << sales[i].date;
        totalSales += sales[i].totalAmount;
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
    pressEnter();
}

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

// Add before the main function
void loadInventory(merchObject* items[], int& count) {
    ifstream inFile("inventory.txt");
    if (!inFile) {
        return;
    }

    string line;
    getline(inFile, line); // Skip separator
    getline(inFile, line); // Skip header
    getline(inFile, line); // Skip separator

    while (getline(inFile, line) && count < MAX_INVENTORY) {
        if (line == "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=") {
            items[count] = new merchObject();

            getline(inFile, line);
            items[count]->generatedID = stoi(line);

            getline(inFile, line);
            items[count]->merchID = line;

            getline(inFile, line);
            items[count]->productPrice = stod(line);

            getline(inFile, line);
            items[count]->quantity = stoi(line);

            getline(inFile, line);
            items[count]->supplier = line;

            getline(inFile, items[count]->dateAdded);
            getline(inFile, items[count]->lastUpdated);

            getline(inFile, line); // Read the closing separator
            count++;
        }
    }
    inFile.close();
}


class Node {
public:
    merchObject* data;
    Node* next;

    Node(merchObject* obj) : data(obj), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    void addProduct(merchObject* product) {
        Node* newNode = new Node(product);
        if (!head) {
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    merchObject* getProduct(int id) {
        Node* current = head;
        while (current) {
            if (current->data->generatedID == id) {
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool removeProduct(int id) {
        if (!head) return false;

        if (head->data->generatedID == id) {
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            return true;
        }

        Node* current = head;
        while (current->next) {
            if (current->next->data->generatedID == id) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                size--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp->data;
            delete temp;
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));

    merchObject** inventory = new merchObject * [MAX_INVENTORY]();
    SaleRecord* sales = new SaleRecord[MAX_SALES]();
    int inventoryCount = 0;
    int salesCount = 0;

    loadInventory(inventory, inventoryCount);

    int choice;

    do {
        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "              MERCHANDISE MANAGEMENT           " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "             1. Add Merch Product              " << endl;
        cout << "             2. Update Merch Product           " << endl;
        cout << "             3. Track Sales Product            " << endl;
        cout << "             4. Display Merch Product          " << endl;
        cout << "             5. Search Merch Product           " << endl;
        cout << "             6. Delete Merch Product           " << endl;
        cout << "             7. Save and Exit                  " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

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
    delete[] inventory;
    delete[] sales;

    return 0;
}