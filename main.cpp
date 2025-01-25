/****************************************
 *
 *  InventoryTracker was programmed by Ali Durmusoglu, UTD CS Freshman, in late November 2024.
 *
 *  InventoryTracker allows the user to record and view the quantities, names, and prices of up to 10 items.
 *  The program uses a text file to record the data of the inventory for persistence between runtimes.
 *  The user can add, remove, and calculate the total value of the inventory, and can also view all the
 *  items in a tabular format.
 *
****************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

void loadInventoryData(ifstream&, string [], double [], int [], int&);
void addItem(string [], double [], int [], int&, const int);
void removeItem(string [], double [], int [], int&);
void displayInventory(string [], int [], double [], int);
double calculateInventoryValue(double [], int [], int);
void updateInventoryData(ofstream&, string [], double [], int [], int);

int main()
{
    const int MAX_ITEMS = 10;
    string itemNames[MAX_ITEMS];
    double itemPrices[MAX_ITEMS] = {0};
    int itemQuantities[MAX_ITEMS] = {0};
    ifstream inventoryData;
    ofstream inventoryChart;
    int numItems = 0;
    short int menuOption;
    double inventoryValue = 0;
    bool exitProgram = false;

    // Loads data of items from file into the arrays

    inventoryData.open("inventory.txt");

    if (!inventoryData.fail())
    {
        loadInventoryData(inventoryData, itemNames, itemPrices, itemQuantities, numItems);
    }
    else
    {
        cout << "There is no pre-existing data in your inventory.\n";
    }

    // Opens the output file

    inventoryChart.open("inventory.txt");

    if (!inventoryChart.fail())
    {

        do
        {

            // Presents the user with a menu option

            cout << "Choose what you would like to do with your inventory.\n"
                << "\t(1) Add a new item to the inventory.\n\t(2) Remove an item in the inventory."
                << "\n\t(3) View all inventory items in a tabular format.\n"
                << "\t(4) Calculate the total value of the inventory.\n\t(5) Exit the program.\n"
                <<"\nEnter your preferred action: ";
            cin >> menuOption;

            // Validates the input for menu choice

            while (!(menuOption >= 1 && menuOption <= 5))
            {
                cout << "Error! Enter a valid choice input (Must be 1 through 5): ";
                cin >> menuOption;
            }

            // Executes the action chosen by the user from the menu

            switch (menuOption)
            {
                case 1: // Adds new item to the inventory
                    addItem(itemNames, itemPrices, itemQuantities, numItems, MAX_ITEMS);
                    break;

                case 2: // Removes an item from the inventory
                    removeItem(itemNames, itemPrices, itemQuantities, numItems);
                    break;

                case 3: // Displays the item already in the inventory
                    displayInventory(itemNames, itemQuantities, itemPrices, numItems);
                    break;

                case 4: // Calculate and displays the total value of the items in inventory
                    inventoryValue = calculateInventoryValue(itemPrices, itemQuantities, numItems);
                    cout << "\nThe total value of the current items in the inventory is $"
                         << fixed << setprecision(2) << inventoryValue << ".\n";

                    // Pauses for a second to let user read the value until ENTER is pressed

                    cout << "Press ENTER to continue.\n";
                    cin.ignore(32, '\n');
                    cin.get();
                    break;

                case 5: // Exits the program
                    updateInventoryData(inventoryChart, itemNames, itemPrices, itemQuantities, numItems);
                    exitProgram = true;
                    break;

                default:
                    cout << "Error! Something went wrong with the menu system. Please restart the program.\n";
                    break;
            }

        } while (!exitProgram);

    }
    else
    {
        cout << "The data writer failed to open. Please run the program again.\n";
    }

    return 0;
}

// loadInventoryData reads from a text file "inventory.txt" containing the names, quantities, and
// Items of all current items in inventory. Data file allows for data to persist between runtimes.
// PARAMS:
//      ifstream inventoryData - the input text file "inventory.txt" containing data of inventory
//      string itemNames[] - All item names in file loads into itemNames[] in order
//      double itemPrices[] - All item prices in file loads into itemPrices[] in order
//      int itemQuantities[] - All item quantities in file loads into itemQuantities[] in order
//      int numItems& - keeps track of how many items are currently in inventory

void loadInventoryData(ifstream& inventoryData, string itemNames[], double itemPrices[],
                    int itemQuantities[], int& numItems)
{

    while (inventoryData >> itemNames[numItems]  >> itemQuantities[numItems] >> itemPrices[numItems])
    {
        numItems++;
    }

}

// addItems adds an item to the end of the list of items currently in inventory.
// PARAMS:
//      string itemNames[] - all names of items in inventory
//      double itemPrices[] - all prices of items in inventory
//      int itemQuantities[] - all quantities of items in inventory
//      int numItems - the current number of items in inventory

void addItem(string itemNames[], double itemPrices[], int itemQuantities[], int& numItems, const int MAX_ITEMS)
{

    if (numItems >= MAX_ITEMS)
    {
        cout << "\nYou have reached tha maximum number of items to be stored in the tracker."
            << endl << "Remove an item to be able to add to inventory.\n\n";
    }
    else
    {
        // Prompts user for item name, price, and quantities and validates the input

        cout << "\nEnter the name of the item (No spaces, use underscores): ";
        cin >> itemNames[numItems];
        cin.ignore(32, '\n');

        while (itemNames[numItems].length() > 18)
        {
            cout << "Error! Enter a shorter name for the item: ";
            cin >> itemNames[numItems];
        }

        cout << "Enter the quantity of the item (integer from 1-1000): ";
        cin >> itemQuantities[numItems];

        while (itemQuantities[numItems] < 1 || itemQuantities[numItems] > 1000)
        {
            cout << "Error! Enter a quantity value that is in the valid range of 1 through 1000: ";
            cin >> itemQuantities[numItems];
        }

        cout << "Enter the price of the item ($0.01-$10,000.00): $";
        cin >> itemPrices[numItems];

        while (itemPrices[numItems] < 0 || itemPrices[numItems] > 10000)
        {
            cout << "Error! Enter a price value that is in the valid range of 1 through 10000: $";
            cin >> itemPrices[numItems];
        }

        cout << "Successfully added " << itemNames[numItems] << " to inventory.\n\n";
        numItems++;

    }

}

// removeItem removes an item that is currently in the inventory.
// PARAMS:
//      string itemNames[] - all names of items in inventory
//      double itemPrices[] - all prices of items in inventory
//      int itemQuantities[] - all quantities of items in inventory
//      int numItems - the current number of items in inventory

void removeItem(string itemNames[], double itemPrices[], int itemQuantities[], int& numItems)
{
    int itemIndex;

    // Displays current items in inventory

    cout << "\nYour current items in your inventory are:\n";

    for (int i = 0; i < numItems; i++)
    {
        cout << "(" << i + 1 << ") " << itemNames[i];

        if (i < numItems - 1)
        {
            cout << ", ";
        }

    }

    // Prompts user for item to be removed

    cout << "\nEnter the number of the item you would like to remove from inventory: ";
    cin >> itemIndex;
    itemIndex--;

    // Removes the item from inventory

    itemNames[itemIndex] = "";
    itemPrices[itemIndex] = 0;
    itemQuantities[itemIndex] = 0;

    for (int i = itemIndex; i < numItems; i++)
    {
        itemNames[i] = itemNames[i + 1];
        itemQuantities[i] = itemQuantities[i + 1];
        itemPrices[i] = itemPrices[i + 1];
    }

    cout << endl;
    numItems--;
}

// displayInventory displays the current items in inventory in a tabular format.
// PARAMS:
//      string itemNames[] - all names of items in inventory
//      double itemPrices[] - all prices of items in inventory
//      int itemQuantities[] - all quantities of items in inventory
//      int numItems - the current number of items in inventory

void displayInventory(string itemNames[], int itemQuantities[], double itemPrices[], int numItems)
{

    if (numItems == 0) // Displays there are no items in inventory if empty
    {
        cout << "\nThere are no items in your inventory.\n";
    }
    else // Displays items in inventory in tabular format
    {
        cout << left << endl << setw(18) << "ITEM NAME" << setw(12) << "QUANTITY" << setw(12) << "PRICE" << endl;

        for (int i = 0; i < numItems; i++)
        {
            cout << left << fixed << setprecision(2) << setw(18) << itemNames[i] << setw(12)
                << itemQuantities[i] << "$" << setw(11) << itemPrices[i] << endl;
        }

    }

    // Pauses for a second to let user read the chart until ENTER is pressed

    cout << "\nPress ENTER to continue.\n";
    cin.ignore(32, '\n');
    cin.get();
}

// calculateInventoryValue takes the prices and quantities of all items in current inventory
// And calculates the total value of the all items in inventory.
// PARAMS:
//      double itemPrices[] - all prices of items in inventory
//      int itemQuantities[] - all quantities of items in inventory
//      int numItems - the current number of items in inventory
// RETURNS:
//      double inventoryValue - the total value of all items in inventory

double calculateInventoryValue(double itemPrices[], int itemQuantities[], int numItems)
{
    double inventoryValue = 0;

    for (int i = 0; i < numItems; i++)
    {
        inventoryValue += itemPrices[i] * itemQuantities[i];
    }

    return inventoryValue;
}

// updateInventoryData loads all data of inventory into output file "inventory.txt" at the end
// Of runtime to be accessed in the future. Loaded in order of "Name" "Quantity" "Price".
// PARAMS:
//      ofstream inventoryChart - inventory data is loaded into the output text file "inventory.txt"
//      string itemNames[] - All item names loads from itemNames[] in order to file
//      double itemPrices[] - All item prices loads from itemPrices[] in order to file
//      int itemQuantities[] - All item quantities loads from itemQuantities[] in order to file
//      int numItems - number of items that are currently in inventory

void updateInventoryData(ofstream& inventoryChart, string itemNames[], double itemPrices[],
                    int itemQuantities[], int numItems)
{

    for (int i = 0; i < numItems; i++)
    {
        inventoryChart << fixed << setprecision(2)
            << itemNames[i] << " " << itemQuantities[i] << " " << itemPrices[i] << endl;
    }

    cout << "\nThe inventory data was successfully saved.\n";
}