#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>

using namespace std;

class Product {
public:
    int id{};
    string name;
    string category;
    int quantity{};
    double price{};

    Product() = default;

    Product(int id, string name, string category, int quantity, double price)
        : id(id), name(std::move(name)), category(std::move(category)),
          quantity(quantity), price(price) {}

    void display() const {
        cout << left << setw(8) << id
             << setw(24) << name.substr(0, 22)
             << setw(18) << category.substr(0, 16)
             << right << setw(10) << quantity
             << setw(14) << fixed << setprecision(2) << price << '\n';
    }

    string serialize() const {
        return to_string(id) + "|" + name + "|" + category + "|" +
               to_string(quantity) + "|" + to_string(price);
    }

    static bool deserialize(const string& line, Product& product) {
        stringstream ss(line);
        string idStr, quantityStr, priceStr;

        if (!getline(ss, idStr, '|') ||
            !getline(ss, product.name, '|') ||
            !getline(ss, product.category, '|') ||
            !getline(ss, quantityStr, '|') ||
            !getline(ss, priceStr)) {
            return false;
        }

        try {
            product.id = stoi(idStr);
            product.quantity = stoi(quantityStr);
            product.price = stod(priceStr);
        } catch (...) {
            return false;
        }
        return true;
    }
};

class InventoryManager {
private:
    vector<Product> products;
    const string filename = "products.txt";

    void save() const {
        ofstream file(filename);
        for (const auto& p : products)
            file << p.serialize() << '\n';
    }

    void load() {
        products.clear();
        ifstream file(filename);
        string line;
        Product p;

        while (getline(file, line)) {
            if (!line.empty() && Product::deserialize(line, p))
                products.push_back(p);
        }
    }

    int nextId() const {
        int maxId = 0;
        for (const auto& p : products)
            maxId = max(maxId, p.id);
        return maxId + 1;
    }

    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    static int readInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= 0) {
                clearInput();
                return value;
            }
            cout << "Invalid input. Please enter a non-negative integer.\n";
            clearInput();
        }
    }

    static double readDouble(const string& prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= 0) {
                clearInput();
                return value;
            }
            cout << "Invalid input. Please enter a non-negative number.\n";
            clearInput();
        }
    }

    Product* findById(int id) {
        for (auto& p : products)
            if (p.id == id)
                return &p;
        return nullptr;
    }

    void printHeader() const {
        cout << "\n" << left
             << setw(8) << "ID"
             << setw(24) << "Product"
             << setw(18) << "Category"
             << right << setw(10) << "Qty"
             << setw(14) << "Price" << '\n';
        cout << string(74, '-') << '\n';
    }

public:
    InventoryManager() {
        load();
    }

    void addProduct() {
        string name, category;
        cout << "\nProduct name: ";
        getline(cin, name);
        cout << "Category: ";
        getline(cin, category);

        if (name.empty() || category.empty()) {
            cout << "Name and category cannot be empty.\n";
            return;
        }

        int quantity = readInt("Quantity: ");
        double price = readDouble("Unit price: ");

        products.emplace_back(nextId(), name, category, quantity, price);
        save();
        cout << "Product added successfully.\n";
    }

    void viewProducts() const {
        if (products.empty()) {
            cout << "\nInventory is empty.\n";
            return;
        }

        printHeader();
        for (const auto& p : products)
            p.display();
    }

    void searchProduct() const {
        string keyword;
        cout << "\nSearch by product name/category: ";
        getline(cin, keyword);

        if (keyword.empty()) return;

        string lowerKeyword = keyword;
        transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(),
                  [](unsigned char c) { return static_cast<char>(tolower(c)); });

        bool found = false;
        for (const auto& p : products) {
            string text = p.name + " " + p.category;
            transform(text.begin(), text.end(), text.begin(),
                      [](unsigned char c) { return static_cast<char>(tolower(c)); });

            if (text.find(lowerKeyword) != string::npos) {
                if (!found) printHeader();
                p.display();
                found = true;
            }
        }

        if (!found)
            cout << "No matching product found.\n";
    }

    void updateProduct() {
        int id = readInt("\nEnter product ID to update: ");
        Product* p = findById(id);

        if (!p) {
            cout << "Product not found.\n";
            return;
        }

        string name, category;
        cout << "New product name [" << p->name << "]: ";
        getline(cin, name);
        cout << "New category [" << p->category << "]: ";
        getline(cin, category);

        if (!name.empty()) p->name = name;
        if (!category.empty()) p->category = category;

        p->quantity = readInt("New quantity: ");
        p->price = readDouble("New unit price: ");

        save();
        cout << "Product updated successfully.\n";
    }

    void deleteProduct() {
        int id = readInt("\nEnter product ID to delete: ");

        auto it = remove_if(products.begin(), products.end(),
                            [id](const Product& p) { return p.id == id; });

        if (it == products.end()) {
            cout << "Product not found.\n";
            return;
        }

        products.erase(it, products.end());
        save();
        cout << "Product deleted successfully.\n";
    }

    void stockReport() const {
        if (products.empty()) {
            cout << "\nInventory is empty.\n";
            return;
        }

        int totalUnits = 0;
        double inventoryValue = 0;
        int lowStockCount = 0;

        for (const auto& p : products) {
            totalUnits += p.quantity;
            inventoryValue += p.quantity * p.price;
            if (p.quantity <= 5) lowStockCount++;
        }

        cout << "\n========== INVENTORY REPORT ==========\n";
        cout << "Unique products : " << products.size() << '\n';
        cout << "Total units     : " << totalUnits << '\n';
        cout << "Inventory value : " << fixed << setprecision(2)
             << inventoryValue << '\n';
        cout << "Low-stock items : " << lowStockCount << " (<= 5 units)\n";

        cout << "\nLow-stock products:\n";
        bool found = false;
        for (const auto& p : products) {
            if (p.quantity <= 5) {
                if (!found) printHeader();
                p.display();
                found = true;
            }
        }
        if (!found) cout << "None.\n";
    }

    void run() {
        while (true) {
            cout << "\n========================================\n";
            cout << "       C++ INVENTORY MANAGEMENT\n";
            cout << "========================================\n";
            cout << "1. Add product\n";
            cout << "2. View all products\n";
            cout << "3. Search product\n";
            cout << "4. Update product\n";
            cout << "5. Delete product\n";
            cout << "6. Inventory report\n";
            cout << "0. Exit\n";

            int choice = readInt("Choose an option: ");

            switch (choice) {
                case 1: addProduct(); break;
                case 2: viewProducts(); break;
                case 3: searchProduct(); break;
                case 4: updateProduct(); break;
                case 5: deleteProduct(); break;
                case 6: stockReport(); break;
                case 0:
                    cout << "Thank you for using the system.\n";
                    return;
                default:
                    cout << "Invalid choice.\n";
            }
        }
    }
};

int main() {
    InventoryManager manager;
    manager.run();
    return 0;
}
