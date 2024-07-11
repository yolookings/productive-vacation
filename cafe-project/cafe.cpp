#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

// Function to log activities
void log_activity(const string& activity) {
    ofstream log_file("activity.log", ios_base::app);
    time_t now = time(0);
    tm* ltm = localtime(&now);
    log_file << "[" << 1900 + ltm->tm_year << "-"
             << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-"
             << setfill('0') << setw(2) << ltm->tm_mday << " "
             << setfill('0') << setw(2) << ltm->tm_hour << ":"
             << setfill('0') << setw(2) << ltm->tm_min << ":"
             << setfill('0') << setw(2) << ltm->tm_sec << "] "
             << activity << endl;
    log_file.close();
}

// Item object class
class Item {
private:
    int quantity;

public:
    string name;

    // Constructor
    Item(string name, int quantity) {
        this->name = name;
        this->quantity = quantity;
    }

    // Add quantity
    void add(int q) {
        quantity += q;
    }

    // Remove quantity
    void remove(int q) {
        quantity -= q;
    }

    // Get quantity
    int get_quantity() {
        return quantity;
    }
};

// Recipe object class
class Recipe {
protected:
    vector<pair<string, int>> ingredients;

public:
    string name;

    // Constructor
    Recipe(string name, vector<pair<string, int>> ingredients) {
        this->name = name;
        this->ingredients = ingredients;
    }

    // Update recipe
    void update(vector<pair<string, int>> ingredients) {
        this->ingredients = ingredients;
    }

    // Get recipe name
    string get_name() {
        return name;
    }

    // Get recipe size
    int get_recipe_size() {
        return ingredients.size();
    }

    // Get recipe
    vector<pair<string, int>> get_recipe() {
        return ingredients;
    }

    // Show recipe
    virtual void show_recipe() {
        cout << "\nRECIPE: " << name << endl;
        for (auto& ingredient : ingredients) {
            cout << ingredient.first << ": " << ingredient.second << endl;
        }
    }
};

// GroupRecipe object class inheriting from Recipe
class GroupRecipe : public Recipe {
private:
    int servings;
public:
    // Constructor
    GroupRecipe(string name, vector<pair<string, int>> ingredients, int servings)
        : Recipe(name, ingredients), servings(servings) {}

    // Show recipe
    void show_recipe() override {
        cout << "\nGROUP RECIPE: "
                << name 
                << " (" << servings << " servings)" <<  endl;
        for (auto& ingredient : get_recipe()) {
            cout << ingredient.first << ": " << ingredient.second << endl;
        }
    }
};

// Main inventory tracking class
class Inventory {
private:
    vector<Item> items;
    vector<Recipe*> recipes;

    int find_item_index(string item) {
        for (int i = 0; i < items.size(); i++)
            if (items[i].name == item)
                return i;
        return -1;
    }

    int find_recipe_index(string recipe) {
        for (int i = 0; i < recipes.size(); i++)
            if (recipes[i]->name == recipe)
                return i;
        return -1;
    }

public:
    //=================//
    // ITEM OPERATIONS //
    //=================//

    // Add item (single)
    void add_item(string name, int quantity) {
        if (find_item_index(name) != -1) {
            items[find_item_index(name)].add(quantity);
            log_activity("Added " + to_string(quantity) + " of " + name);
            return;
        }

        items.push_back(Item(name, quantity));
        log_activity("Added new item " + name + " with quantity " + to_string(quantity));
    }

    // Add item (multiple)
    void add_item(vector<pair<string, int>> input) {
        for (int i = 0; i < input.size(); i++) {
            if (find_item_index(input[i].first) != -1)
                items[find_item_index(input[i].first)].add(input[i].second);
            else
                items.push_back(Item(input[i].first, input[i].second));
        }

        log_activity("Added multiple items to inventory");
    }

    // Check item
    bool check_item(string name, int quantity) {
        if (find_item_index(name) != -1) {
            if (items[find_item_index(name)].get_quantity() < quantity)
                return false;
            return true;
        }
        return false;
    }

    // Remove item (single)
    void remove_item(string item, int quantity) {
        if (find_item_index(item) != -1) {
            if (items[find_item_index(item)].get_quantity() < quantity) {
                cout << "Not enough quantity" << endl;
                log_activity("Failed to remove " + to_string(quantity) + " of " + item + ": Not enough quantity");
                return;
            }
            if (items[find_item_index(item)].get_quantity() == quantity) {
                items.erase(items.begin() + find_item_index(item));
                log_activity("Removed all of " + item);
                return;
            }
            items[find_item_index(item)].remove(quantity);
            log_activity("Removed " + to_string(quantity) + " of " + item);
            return;
        }
    }

    // Remove item (multiple)
    void remove_item(vector<pair<string, int>> items) {
        for (int i = 0; i < items.size(); i++) {
            if (find_item_index(items[i].first) != -1) {
                if (this->items[find_item_index(items[i].first)].get_quantity() < items[i].second) {
                    cout << "Not enough quantity" << endl;
                    log_activity("Failed to remove multiple items: Not enough quantity");
                    return;
                } else if (this->items[find_item_index(items[i].first)].get_quantity() == items[i].second) {
                    this->items.erase(this->items.begin() + find_item_index(items[i].first));
                } else {
                    this->items[find_item_index(items[i].first)].remove(items[i].second);
                }
            }
        }
        log_activity("Removed multiple items from inventory");
    }

    // Show item
    void show_item(string item) {
        if (find_item_index(item) != -1) {
            cout << "\nITEM: " << item << endl
                << "Quantity: " << items[find_item_index(item)].get_quantity() << endl;
            log_activity("Displayed item " + item);
            return;
        }

        cout << "Item not found" << endl;
        log_activity("Failed to display item " + item + ": Item not found");
    }

    // Print items
    void print_items() {
        cout << "\nINVENTORY:" << endl;
        for (int i = 0; i < items.size(); i++)
            cout << items[i].name << ": "
            << items[i].get_quantity() << endl;
        cout << endl;
        log_activity("Displayed all items in inventory");
    }

    //===================//
    // RECIPE OPERATIONS //
    //===================//

    // Create recipe
    void create_recipe(string name, vector<pair<string, int>> ingredients) {
        if (find_recipe_index(name) != -1) {
            cout << "Recipe already exists" << endl;
            log_activity("Failed to create recipe " + name + ": Recipe already exists");
            return;
        }

        recipes.push_back(new Recipe(name, ingredients));
        log_activity("Created recipe " + name);
    }

    // Create group recipe
    void create_group_recipe(string name, vector<pair<string, int>> ingredients, int servings) {
        if (find_recipe_index(name) != -1) {
            cout << "Recipe already exists" << endl;
            log_activity("Failed to create group recipe " + name + ": Recipe already exists");
            return;
        }

        recipes.push_back(new GroupRecipe(name, ingredients, servings));
        log_activity("Created group recipe " + name);
    }

    // Check recipe
    bool check_recipe(string name) {
        if (find_recipe_index(name) != -1) return true;
        return false;
    }

    // Update recipe
    void update_recipe(string name, vector<pair<string, int>> ingredients) {
        if (find_recipe_index(name) != -1) {
            recipes[find_recipe_index(name)]->update(ingredients);
            log_activity("Updated recipe " + name);
            return;
        }

        cout << "Recipe not found" << endl;
        log_activity("Failed to update recipe " + name + ": Recipe not found");
    }

    // Remove recipe
    void remove_recipe(string name) {
        if (find_recipe_index(name) != -1) {
            recipes.erase(recipes.begin() + find_recipe_index(name));
            log_activity("Removed recipe " + name);
            return;
        }

        cout << "Recipe not found" << endl;
        log_activity("Failed to remove recipe " + name + ": Recipe not found");
    }

    // Show recipe
    void show_recipe(string name) {
        if (find_recipe_index(name) != -1) {
            recipes[find_recipe_index(name)]->show_recipe();
            log_activity("Displayed recipe " + name);
            return;
        }

        cout << "Recipe not found" << endl;
        log_activity("Failed to display recipe " + name + ": Recipe not found");
    }

    // Print recipes
    void print_recipes() {
        cout << "\nRECIPES:" << endl;
        for (int i = 0; i < recipes.size(); i++){
            cout << recipes[i]->get_name()
                 << " (" << recipes[i]->get_recipe_size() << " ingredients)" << endl;
        }

        cout << endl;
        log_activity("Displayed all recipes");
    }

    // Use recipe
    void use_recipe(string name) {
        if (find_recipe_index(name) != -1) {
            for (auto& ingredient : recipes[find_recipe_index(name)]->get_recipe()) {
                if (!check_item(ingredient.first, ingredient.second)) {
                    cout << "Not enough ingredients" << endl;
                    log_activity("Failed to use recipe " + name + ": Not enough ingredients");
                    return;
                }
            }

            for (auto& ingredient : recipes[find_recipe_index(name)]->get_recipe()) {
                remove_item(ingredient.first, ingredient.second);
            }

            log_activity("Used recipe " + name);
            return;
        }

        cout << "Recipe not found" << endl;
        log_activity("Failed to use recipe " + name + ": Recipe not found");
    }

    // Export items to CSV
    void export_items_to_csv(const string& filename) {
        ofstream csv_file(filename);
        if (!csv_file.is_open()) {
            cout << "Failed to open CSV file for writing." << endl;
            return;
        }

        csv_file << "Item Name,Quantity" << endl;
        for (auto& item : items) {
            csv_file << item.name << "," << item.get_quantity() << endl;
        }

        csv_file.close();
        cout << "Items exported to CSV successfully." << endl;
        log_activity("Exported items to CSV file: " + filename);
    }

    // Import items from CSV
    void import_items_from_csv(const string& filename) {
        ifstream csv_file(filename);
        if (!csv_file.is_open()) {
            cout << "Failed to open CSV file for reading." << endl;
            return;
        }

        items.clear(); // Clear existing items
        string line;
        getline(csv_file, line); // Skip header line
        while (getline(csv_file, line)) {
            stringstream ss(line);
            string item_name;
            int quantity;
            if (getline(ss, item_name, ',') && ss >> quantity) {
                items.push_back(Item(item_name, quantity));
            }
        }

        csv_file.close();
        cout << "Items imported from CSV successfully." << endl;
        log_activity("Imported items from CSV file: " + filename);
    }
};

//==============//
// MENU DISPLAY //
//==============//

void display_menu_item() {
    cout << "\nITEM DASHBOARD"
        << "\n1. Add Item"
        << "\n2. Multi-Add Item"
        << "\n3. Remove Item"
        << "\n4. Multi-Remove Item"
        << "\n5. Find Item"
        << "\n6. Print Items"
        << "\n7. Export Items to CSV"
        << "\n8. Import Items from CSV"
        << "\n0. Exit" << endl;
}

void display_menu_recipe() {
    cout << "\nRECIPE DASHBOARD"
        << "\n1. Create Recipe"
        << "\n2. Create Group Recipe"
        << "\n3. Update Recipe"
        << "\n4. Remove Recipe"
        << "\n5. Find Recipe"
        << "\n6. Print Recipes"
        << "\n7. Use Recipe"
        << "\n0. Exit" << endl;
}

void display_menu() {
    cout << "\nCAFE MANAGEMENT SYSTEM"
        << "\n1. Item Dashboard"
        << "\n2. Recipe Dashboard"
        << "\n0. Exit" << endl;
}

//===========================//
// USER INTERFACE OPERATIONS //
//===========================//

// Loops for item operations
void loop_item(Inventory& tracker) {
    int choice;
    string item;
    int quantity;
    vector<pair<string, int>> items;

    display_menu_item();

    cout << "> ";
    cin >> choice;
    cout << endl;

    switch (choice) {
    case 1: {
        // Add item
        cout << "[!] Enter item name & quanity" << endl
                << "  Syntax: <item> <quantity>" << endl
                << "> ";
        cin >> item >> quantity;
        tracker.add_item(item, quantity);
        break;
    }
    case 2:
        // Multi-Add item
        cout << "[!] Enter item name & quanity" << endl
                << "  Syntax: <item> <quantity>" << endl
                << "  Enter 'exit' to stop" << endl
                << "Example: " << endl
                << "  sugar 50" << endl
                << "  exit" << endl;

        // Creates vector for items
        while(1) {
            // Loops until user enters 'exit'
            cout << "> ";
            cin >> item;
            if (item == "exit") break;
            cin >> quantity;

            // Pushes item to vector
            items.push_back(make_pair(item, quantity));
        }
        tracker.add_item(items);
        break;

    case 3:
        // Remove item
        cout << "[!] Enter item name & quanity" << endl
                << "  Syntax: <item> <quantity>" << endl
                << "> ";
        cin >> item >> quantity;
        tracker.remove_item(item, quantity);
        break;

    case 4:
        // Multi-Remove item
        cout << "[!] Enter item name & quanity" << endl
                << "  Syntax: <item> <quantity>" << endl
                << "  Enter 'exit' to stop" << endl
                << "Example: " << endl
                << "  sugar 50" << endl
                << "  exit" << endl;

        // Creates vector for items
        while(1) {
            // Loops until user enters 'exit'
            cout << "> ";
            cin >> item;
            if (item == "exit") break;
            cin >> quantity;

            // Pushes item to vector
            items.push_back(make_pair(item, quantity));
        }
        tracker.remove_item(items);
        break;

    case 5:
        // Find item
        cout << "[!] Enter item name" << endl
                << "> ";
        cin >> item;
        tracker.show_item(item);
        break;

    case 6:
        // Print items
        tracker.print_items();
        break;

    case 7:
        // Export items to CSV
        cout << "[!] Enter CSV filename" << endl
             << "> ";
        cin >> item;
        tracker.export_items_to_csv(item);
        break;

    case 8:
        // Import items from CSV
        cout << "[!] Enter CSV filename" << endl
             << "> ";
        cin >> item;
        tracker.import_items_from_csv(item);
        break;

    case 0:
        cout << "Exiting..." << endl;
        return;

    default:
        cout << "Invalid choice. Please try again." << endl;
    }
    loop_item(tracker);
}

// Loops for recipe operations
void loop_recipe(Inventory& tracker) {
    int choice;
    string name, item;
    int quantity, servings;
    vector<pair<string, int>> ingredients;

    display_menu_recipe();

    cout << "> ";
    cin >> choice;
    cout << endl;

    switch (choice) {
    case 1:
        // Get recipe name
        cout << "[!] Enter recipe name" << endl
                << "> ";
        cin >> name;

        // Instructions
        cout << "[!] Enter ingredient & quantity" << endl
                << "  Syntax: <item> <quantity>" << endl
                << "  Enter 'exit' to stop" << endl
                << "Example: " << endl
                << "  sugar 50" << endl
                << "  exit" << endl;

        // Creates vector for ingredients
        while(1) {
            // Loops until user enters 'exit'
            cout << "> ";
            cin >> item;
            if (item == "exit") break;
            cin >> quantity;

            // Pushes item to vector
            ingredients.push_back(make_pair(item, quantity));
        }
        tracker.create_recipe(name, ingredients);
        break;

    case 2:
        // Get recipe name
        cout << "[!] Enter group recipe name" << endl
                << "> ";
        cin >> name;

        // Get servings
        cout << "[!] Enter servings" << endl
                << "> ";
        cin >> servings;

        // Instructions
        cout << "[!] Enter ingredient & quantity" << endl
                << "  Syntax: <item> <quantity>" << endl
                << "  Enter 'exit' to stop" << endl
                << "Example: " << endl
                << "  sugar 50" << endl
                << "  exit" << endl;

        // Creates vector for ingredients
        while(1) {
            // Loops until user enters 'exit'
            cout << "> ";
            cin >> item;
            if (item == "exit") break;
            cin >> quantity;

            // Pushes item to vector
            ingredients.push_back(make_pair(item, quantity));
        }
        tracker.create_group_recipe(name, ingredients, servings);
        break;

    case 3:
        // Get recipe name
        cout << "[!] Enter recipe name" << endl
                << "> ";
        cin >> name;

        // Check if recipe exists
        if (!tracker.check_recipe(name)) {
            cout << "Recipe not found" << endl;
            break;
        }

        // Instructions
        cout << "[!] Enter ingredient & quantity" << endl
                << "  Syntax: <item> <quantity>" << endl
                << "  Enter 'exit' to stop" << endl
                << "Example: " << endl
                << "  sugar 50" << endl
                << "  exit" << endl;
        
        // Creates vector for ingredients
        while(1) {
            // Loops until user enters 'exit'
            cout << "> ";
            cin >> item;
            if (item == "exit") break;
            cin >> quantity;

            // Pushes item to vector
            ingredients.push_back(make_pair(item, quantity));
        }
        tracker.update_recipe(name, ingredients);

    case 4:
        // Remove recipe
        cout << "[!] Enter recipe name" << endl
                << "> ";
        cin >> name;
        tracker.remove_recipe(name);
        break;

    case 5:
        // Show recipe
        cout << "[!] Enter recipe name" << endl
                << "> ";
        cin >> name;
        tracker.show_recipe(name);
        break;

    case 6:
        // Print recipes
        tracker.print_recipes();
        break;

    case 7:
        // Use recipe
        cout << "[!] Enter recipe name" << endl
                << "> ";
        cin >> name;
        tracker.use_recipe(name);
        break;

    case 0:
        cout << "Exiting..." << endl;
        return;

    default:
        cout << "Invalid choice. Please try again." << endl;
        break;
    }

    loop_recipe(tracker);
}

// Main loop
void loop_main(Inventory& tracker) {
    int choice;

    display_menu();

    cout << "> ";
    cin >> choice;
    cout << endl;

    switch (choice) {
    case 1:
        loop_item(tracker);
        break;

    case 2:
        loop_recipe(tracker);
        break;

    case 0:
        cout << "Exiting..." << endl;
        return;

    default:
        cout << "Invalid choice. Please try again." << endl;
        break;
    }

    loop_main(tracker);
}

// Main function
int main() {

    Inventory tracker;
    loop_main(tracker);

    return 0;
}