#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;
//Limits
const int MAX_INVENTORY = 10;
const int MAX_SALES = 100;
//Alphanum checking
bool isAlNumSpec(const string& str) {
    if (str.empty()) return false;

    for (char c : str) {
        if (!isalnum(c) && !isspace(c) &&
            c != '-' && c != '_' && c != '&' && c != '@' &&
            c != '.' && c != '!' && c != '#' && c != '$' &&
            c != '%' && c != '+' && c != '=' && c != '(' &&
            c != ')' && c != '[' && c != ']' && c != '*') {
            return false;
        }
    }
    return true;
}
//Classes
class merchObject;
struct SaleRecord;
struct Supplier;

//Function Prototypes
bool isAlNumSpec(const string& str);
double getValidPrice();
int getValidQuantity();
void pressEnter();
bool isStrictlyNumeric(const string& str);
bool hasInvalidPriceChars(const string& str);
int countChar(const string& str, char target);
void updateItem(merchObject* items[], int count);
void displayMerch(merchObject* items[], int count);
void searchItem(merchObject* items[], int count);
void deleteItem(merchObject* items[], int& count);
void saveInventory(merchObject* items[], int count, SaleRecord sales[], int salesCount);
void loadInventory(merchObject* items[], int& count);
void loadSales(SaleRecord sales[], int& salesCount);
void customerPurchaseMenu(merchObject* items[], int count, SaleRecord sales[], int& salesCount);
void recordSale(merchObject* items[], int count, SaleRecord sales[], int& salesCount);
void displaySales(merchObject* items[], int count, SaleRecord sales[], int salesCount);
bool containsIgnoreCase(const string& str, const string& search);
int getValidMenuChoice();


//First class for Identification
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
    //Getting date and time
    string getCurrentDateTime() {
        time_t now = time(0);
        tm localTime;
        localtime_s(&localTime, &now);
        char timeStr[32];
        strftime(timeStr, sizeof(timeStr), "Saved on: %Y-%m-%d %H:%M:%S", &localTime);
        return string(timeStr) + "\n";
    }
    //Random ID Generator
    int generateUniqueID() {
        return rand() % 90000 + 10000;
    }

    //Add Item Menu (using $)
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

                if (!input.empty() && isAlNumSpec(input)) {
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

                if (!input.empty() && isAlNumSpec(input)) {
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
//Sale Record Structure for Identification
struct SaleRecord {
    int productId;
    int quantitySold;
    double totalAmount;
    string date;

    SaleRecord() : productId(0), quantitySold(0), totalAmount(0.0), date("") {}
};

//character counting
int countChar(const string& str, char target) {
    int count = 0;
    for (char c : str) {
        if (c == target) count++;
    }
    return count;
}
//Invalid Price Validation
bool hasInvalidPriceChars(const string& str) {
    for (char c : str) {
        if (!isdigit(c) && c != '.') return true;
    }
    return false;
}
//Strict Numbering Validation
bool isStrictlyNumeric(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return !str.empty();
}
//Enter Function using ASCII Code 13
void pressEnter() {
    cout << "\nPress Enter to continue...";
    while (_getch() != 13);
}
//Price Validation
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
//Quantity Validation
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

// Valid Menu Function
int getValidMenuChoice() {
    string input;
    while (true) {
        cout << "Enter your choice (1-8): ";  // Updated to 8 choices
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" "));
        input.erase(input.find_last_not_of(" ") + 1);

        if (isStrictlyNumeric(input)) {
            int choice = stoi(input);
            if (choice >= 1 && choice <= 8) return choice;  // Updated to 8 choices
        }

        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "              MERCHANDISE MANAGEMENT           " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "             1. Add Merch Product             " << endl;
        cout << "             2. Update Merch Product          " << endl;
        cout << "             3. Track Sales Product           " << endl;
        cout << "             4. Display Merch Product         " << endl;
        cout << "             5. Search Merch Product          " << endl;
        cout << "             6. Delete Merch Product          " << endl;
        cout << "             7. Customer Purchase Menu        " << endl;
        cout << "             8. Save and Exit                 " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "Invalid choice! Enter a number between 1 and 8.\n";
    }
}

void saveInventory(merchObject* items[], int count, SaleRecord sales[], int salesCount);
void customerPurchaseMenu(merchObject* items[], int count, SaleRecord sales[], int& salesCount);
void recordSale(merchObject* items[], int count, SaleRecord sales[], int& salesCount);
void displaySales(merchObject* items[], int count, SaleRecord sales[], int salesCount);

//Main Function
int main() {
    srand(static_cast<unsigned int>(time(0)));

    merchObject** inventory = new merchObject * [MAX_INVENTORY]();
    SaleRecord* sales = new SaleRecord[MAX_SALES]();
    int inventoryCount = 0;
    int salesCount = 0;

    // Load existing data
    loadInventory(inventory, inventoryCount);
    loadSales(sales, salesCount);

    int choice;

    do {
        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "              MERCHANDISE MANAGEMENT           " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "             1. Add Merch Product             " << endl;
        cout << "             2. Update Merch Product          " << endl;
        cout << "             3. Track Sales Product           " << endl;
        cout << "             4. Display Merch Product         " << endl;
        cout << "             5. Search Merch Product          " << endl;
        cout << "             6. Delete Merch Product          " << endl;
        cout << "             7. Customer Purchase Menu        " << endl;
        cout << "             8. Save and Exit                 " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

        choice = getValidMenuChoice();

        switch (choice) {
            //Make sure it reads the limits
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
            customerPurchaseMenu(inventory, inventoryCount, sales, salesCount);
            break;
        case 8:
            saveInventory(inventory, inventoryCount, sales, salesCount);
            cout << "Records saved. Exiting...\n";
            break;
        }
    } while (choice != 8);

    // Cleanup for memory allocation
    for (int i = 0; i < inventoryCount; i++) {
        delete inventory[i];
    }
    delete[] inventory;
    delete[] sales;

    return 0;
}
//File Handling (2 Files: Inventory and Sales)
void saveInventory(merchObject* items[], int count, SaleRecord sales[], int salesCount) {
    if (items == nullptr) {
        cout << "Error: Invalid inventory data!\n";
        return;
    }

    // Save inventory records
    ofstream invFile("inventory_records.txt");
    if (!invFile) {
        cout << "Error saving inventory!\n";
        return;
    }

    invFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    invFile << "                                              INVENTORY RECORDS                                               " << endl;
    invFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    invFile << left << setw(10) << "ID"
        << setw(25) << "Name"
        << setw(15) << "Price"
        << setw(10) << "Quantity"
        << setw(25) << "Supplier"
        << "Last Updated" << endl;
    invFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

    for (int i = 0; i < count; i++) {
        if (items[i] != nullptr) {
            invFile << left
                << setw(10) << items[i]->generatedID
                << setw(25) << items[i]->merchID
                << "$" << setw(14) << fixed << setprecision(2) << items[i]->productPrice
                << setw(10) << items[i]->quantity
                << setw(25) << items[i]->supplier
                << items[i]->lastUpdated;
        }
    }
    invFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    invFile.close();

    // Save sales records
    ofstream salesFile("sales_records.txt");
    if (!salesFile) {
        cout << "Error saving sales records!\n";
        return;
    }

    salesFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    salesFile << "                                               SALES RECORDS                                                 " << endl;
    salesFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    salesFile << left << setw(10) << "ID"
        << setw(25) << "Product Name"
        << setw(10) << "Quantity"
        << setw(15) << "Total Amount"
        << "Date" << endl;
    salesFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

    double totalSales = 0;
    for (int i = 0; i < salesCount; i++) {
        string productName = "[Deleted Product]";
        for (int j = 0; j < count; j++) {
            if (items[j] != nullptr && items[j]->generatedID == sales[i].productId) {
                productName = items[j]->merchID;
                break;
            }
        }

        salesFile << left
            << setw(10) << sales[i].productId
            << setw(25) << productName
            << setw(10) << sales[i].quantitySold
            << "$" << setw(14) << fixed << setprecision(2) << sales[i].totalAmount
            << sales[i].date;
        totalSales += sales[i].totalAmount;
    }

    salesFile << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    salesFile << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
    salesFile.close();

    cout << "Records saved successfully!\n";
}

void loadInventory(merchObject* items[], int& count) {
    ifstream invFile("inventory_records.txt");
    if (!invFile) return;

    string line;
    // Skip header lines
    for (int i = 0; i < 5; i++) getline(invFile, line);

    while (getline(invFile, line) && count < MAX_INVENTORY) {
        if (line.find("=-=") != string::npos) break;

        items[count] = new merchObject();

        // Parse line using string operations
        size_t pos = 0;
        string token;

        // Get ID
        pos = line.find_first_not_of(" ");
        token = line.substr(pos, 10);
        items[count]->generatedID = stoi(token);

        // Get Name
        pos = line.find_first_not_of(" ", pos + 10);
        token = line.substr(pos, 25);
        items[count]->merchID = token.substr(0, token.find_last_not_of(" ") + 1);

        // Get Price (skip $ symbol)
        pos = line.find("$", pos) + 1;
        token = line.substr(pos, 14);
        items[count]->productPrice = stod(token);

        // Get Quantity
        pos = line.find_first_not_of(" ", pos + 14);
        token = line.substr(pos, 10);
        items[count]->quantity = stoi(token);

        // Get Supplier
        pos = line.find_first_not_of(" ", pos + 10);
        token = line.substr(pos, 25);
        items[count]->supplier = token.substr(0, token.find_last_not_of(" ") + 1);

        // Get DateTime
        pos = line.find("Saved on:", pos);
        items[count]->lastUpdated = line.substr(pos) + "\n";
        items[count]->dateAdded = items[count]->lastUpdated;

        count++;
    }
    invFile.close();
}

void loadSales(SaleRecord sales[], int& salesCount) {
    ifstream salesFile("sales_records.txt");
    if (!salesFile) return;

    string line;
    // Skip header lines
    for (int i = 0; i < 5; i++) getline(salesFile, line);

    while (getline(salesFile, line) && salesCount < MAX_SALES) {
        if (line.find("=-=") != string::npos) break;

        size_t pos = 0;
        string token;

        // Get Product ID
        pos = line.find_first_not_of(" ");
        token = line.substr(pos, 10);
        sales[salesCount].productId = stoi(token);

        // Skip product name (25 chars)
        pos += 25;

        // Get Quantity
        pos = line.find_first_not_of(" ", pos);
        token = line.substr(pos, 10);
        sales[salesCount].quantitySold = stoi(token);

        // Get Total Amount (skip $ symbol)
        pos = line.find("$", pos) + 1;
        token = line.substr(pos, 14);
        sales[salesCount].totalAmount = stod(token);

        // Get DateTime
        pos = line.find("Saved on:", pos);
        sales[salesCount].date = line.substr(pos) + "\n";

        salesCount++;
    }
    salesFile.close();
}


// Merch Display
void displayMerch(merchObject* items[], int count) {
    system("cls");
    if (count == 0 || items == nullptr) {
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
        if (items[i] != nullptr) {  // Add null check for each item
            cout << left << setw(10) << items[i]->generatedID
                << setw(20) << items[i]->merchID
                << "$" << setw(9) << fixed << setprecision(2) << items[i]->productPrice
                << setw(10) << items[i]->quantity
                << setw(20) << items[i]->supplier
                << items[i]->lastUpdated;
        }
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    pressEnter();
}

//Ignore Case Function
bool containsIgnoreCase(const string& str, const string& search) {
    if (search.length() > str.length()) return false;

    for (size_t i = 0; i <= str.length() - search.length(); i++) {
        bool found = true;
        for (size_t j = 0; j < search.length(); j++) {
            if (tolower(str[i + j]) != tolower(search[j])) {
                found = false;
                break;
            }
        }
        if (found) return true;
    }
    return false;
}

//Search Item 
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

// Update Item
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
        if (isAlNumSpec(input_str)) {
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
        if (isAlNumSpec(input_str)) {
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
    while (true) {
        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "               DELETE PRODUCT                 " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

        if (count == 0 || items == nullptr) {
            cout << "No items available to delete.\n";
            pressEnter();
            return;
        }

        cout << "Enter product ID to delete (or 'x' to exit): ";
        string input;
        getline(cin, input);

        // Check for exit condition
        if (input == "x" || input == "X") {
            return;
        }

        // Remove leading and trailing spaces
        input.erase(0, input.find_first_not_of(" "));
        input.erase(input.find_last_not_of(" ") + 1);

        // Check for empty input
        if (input.empty()) {
            system("cls");
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "               DELETE PRODUCT                 " << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "Error: Input cannot be empty!\n";
            pressEnter();
            continue;
        }

        // Validate numeric input
        if (!isStrictlyNumeric(input)) {
            system("cls");
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "               DELETE PRODUCT                 " << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "Error: ID must contain only numbers!\n";
            pressEnter();
            continue;
        }

        int id = stoi(input);
        int index = -1;
        for (int i = 0; i < count; i++) {
            if (items[i] != nullptr && items[i]->generatedID == id) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            system("cls");
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "               DELETE PRODUCT                 " << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "Product not found!\n";
            pressEnter();
            continue;
        }

        while (true) {
            system("cls");
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "               DELETE PRODUCT                 " << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "Product to delete:" << endl;
            cout << "ID: " << items[index]->generatedID << endl;
            cout << "Name: " << items[index]->merchID << endl;
            cout << "Price: $" << fixed << setprecision(2) << items[index]->productPrice << endl;
            cout << "Quantity: " << items[index]->quantity << endl;
            cout << "Supplier: " << items[index]->supplier << endl;
            cout << "Last Updated: " << items[index]->lastUpdated;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

            cout << "\nAre you sure you want to delete this item?" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cout << "Enter choice: ";

            string confirm;
            getline(cin, confirm);

            // Remove leading and trailing spaces
            confirm.erase(0, confirm.find_first_not_of(" "));
            confirm.erase(confirm.find_last_not_of(" ") + 1);

            if (!isStrictlyNumeric(confirm)) {
                system("cls");
                cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
                cout << "               DELETE PRODUCT                 " << endl;
                cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
                cout << "Invalid input! Please enter 1 or 2.\n";
                pressEnter();
                continue;
            }

            int choice = stoi(confirm);
            if (choice != 1 && choice != 2) {
                system("cls");
                cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
                cout << "               DELETE PRODUCT                 " << endl;
                cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
                cout << "Invalid choice! Please enter 1 or 2.\n";
                pressEnter();
                continue;
            }

            if (choice == 2) {
                system("cls");
                cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
                cout << "               DELETE PRODUCT                 " << endl;
                cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
                cout << "Deletion cancelled.\n";
                pressEnter();
                return;
            }

            delete items[index];
            items[index] = nullptr;

            for (int i = index; i < count - 1; i++) {
                items[i] = items[i + 1];
            }
            items[count - 1] = nullptr;
            count--;

            system("cls");
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "               DELETE PRODUCT                 " << endl;
            cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
            cout << "Item deleted successfully!\n";
            pressEnter();
            return;
        }
    }
}
//Sales Display
void displaySales(merchObject* items[], int count, SaleRecord sales[], int salesCount) {
    system("cls");
    if (salesCount == 0) {
        cout << "No sales records available.\n";
        pressEnter();
        return;
    }

    // Add null check for items array
    if (items == nullptr) {
        cout << "Error: Invalid inventory data!\n";
        pressEnter();
        return;
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "                                                     SALES RECORDS                                                     " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << left << setw(10) << "ID"
        << setw(20) << "Product Name"
        << setw(15) << "Quantity"
        << setw(15) << "Total"
        << "Date" << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

    double totalSales = 0;
    for (int i = 0; i < salesCount; i++) {
        string productName = "[Deleted Product]";

        // Safer item lookup with bounds and null checking
        if (count > 0) {
            for (int j = 0; j < count && items[j] != nullptr; j++) {
                if (items[j]->generatedID == sales[i].productId) {
                    productName = items[j]->merchID;
                    break;
                }
            }
        }

        cout << left << setw(10) << sales[i].productId
            << setw(20) << productName
            << setw(15) << sales[i].quantitySold
            << "$" << setw(14) << fixed << setprecision(2) << sales[i].totalAmount
            << sales[i].date;
        totalSales += sales[i].totalAmount;
    }

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
    pressEnter();
}
//Sales Record
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

// Customer Purchase Menu
void customerPurchaseMenu(merchObject* items[], int count, SaleRecord sales[], int& salesCount) {
    int choice;
    while (true) {
        system("cls");
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "                                  CUSTOMER PURCHASE MENU                                      " << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "Available Products:" << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << left << setw(10) << "ID"
            << setw(20) << "Name"
            << setw(10) << "Price"
            << setw(10) << "Stock" << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;

        // Display available products
        for (int i = 0; i < count; i++) {
            if (items[i] != nullptr && items[i]->quantity > 0) {
                cout << left << setw(10) << items[i]->generatedID
                    << setw(20) << items[i]->merchID
                    << "$" << setw(9) << fixed << setprecision(2) << items[i]->productPrice
                    << items[i]->quantity << endl;
            }
        }


        cout << "\n1. Purchase Product" << endl;
        cout << "2. View Receipt" << endl;
        cout << "3. Return to Main Menu" << endl;
        cout << "Enter choice (1-3): ";

        string input;
        getline(cin, input);

        if (!isStrictlyNumeric(input)) {
            cout << "Invalid choice! Please enter a number.\n";
            pressEnter();
            continue;
        }

        choice = stoi(input);
        if (choice < 1 || choice > 3) {
            cout << "Invalid choice! Please enter a number between 1 and 3.\n";
            pressEnter();
            continue;
        }

        if (choice == 1) {
            if (salesCount >= MAX_SALES) {
                cout << "Sales record is full!\n";
                pressEnter();
                continue;
            }

            cout << "\nEnter product ID: ";
            string idInput;
            getline(cin, idInput);

            if (!isStrictlyNumeric(idInput)) {
                cout << "Invalid ID format!\n";
                pressEnter();
                continue;
            }

            int id = stoi(idInput);
            merchObject* item = nullptr;
            for (int i = 0; i < count; i++) {
                if (items[i] != nullptr && items[i]->generatedID == id) {
                    item = items[i];
                    break;
                }
            }

            if (!item) {
                cout << "Product not found!\n";
                pressEnter();
                continue;
            }

            cout << "Enter quantity: ";
            string qtyInput;
            getline(cin, qtyInput);

            if (!isStrictlyNumeric(qtyInput)) {
                cout << "Invalid quantity format!\n";
                pressEnter();
                continue;
            }

            int quantity = stoi(qtyInput);
            if (quantity > item->quantity) {
                cout << "Insufficient stock!\n";
                pressEnter();
                continue;
            }

            sales[salesCount].productId = id;
            sales[salesCount].quantitySold = quantity;
            sales[salesCount].totalAmount = quantity * item->productPrice;
            sales[salesCount].date = item->getCurrentDateTime();

            salesCount++;
            item->quantity -= quantity;
            item->lastUpdated = item->getCurrentDateTime();

            cout << "\nPurchase successful!\n";
            cout << "Total amount: $" << fixed << setprecision(2) << sales[salesCount - 1].totalAmount << endl;
            pressEnter();
        }
        else if (choice == 2) {
            displaySales(items, count, sales, salesCount);
        }
        else if (choice == 3) {
            return;
        }
    }
}