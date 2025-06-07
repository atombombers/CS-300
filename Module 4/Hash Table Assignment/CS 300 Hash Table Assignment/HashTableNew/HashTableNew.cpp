//============================================================================
// Name        : HashTable.cpp
// Author      : Brooke Slampak
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <Windows.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;
const int GLOBAL_SLEEP_TIME = 5000;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (1): Initialize the structures used to hold bids

    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize);


}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {

    // invoke local tableSize to size with this->
    this->tableSize = size;

    // resize nodes size
    nodes.resize(size);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    
    // erase nodes beginning
    nodes.erase(nodes.begin());

}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {

    // return key tableSize
    return key % tableSize;

}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {

    //create the key for the given bid
    unsigned int tempKey = hash(stoi(bid.bidId));


    // if no entry found for the key
    if (nodes.at(tempKey).key == UINT_MAX) {

        //make the new Node
        Node newNode = Node(bid, tempKey);

        //add the node
        nodes.at(tempKey) = newNode;
    }

    else {

        //gets a pointer to node existing at tempKey, which contains a key already
        Node* currNode = &nodes.at(tempKey);

        //make the new node pointer
        Node* newNode = new Node(bid, tempKey);

        //goes through linked list starting at currNode until the last node in the chain
        while (currNode->next != NULL) {

            currNode = currNode->next;

        }

        //appends the new node to the end of the chain
        currNode->next = newNode;

    }

}

/**
 * Print all bids
 */
void HashTable::PrintAll() {

    int j = 0;

    //loop that iterates over each index of the hash table, where tableSize is the total # of slots in the table
    for (unsigned int i = 0; i < tableSize; i++) {

        //gets the node stored at index i in the nodes vector
        Node* currNode = &nodes.at(i);

        //checks that the current node is not empty
        if (currNode->key != UINT_MAX) {

            //increments the count
            j++;

            //Prints the bid info for each chained node
            cout << currNode->key << ": " << currNode->bid.bidId << " | " << currNode->bid.amount
                << " | " << currNode->bid.fund << endl;

            //while the current node has a next pointer, print all chained nodes
            while (currNode->next != nullptr) {

                //Increments the counter for each node in the collision chain
                j++;

                //Moves the pointer to the next node in the chain
                currNode = currNode->next;

                //Prints the bid info for each chained node
                cout << currNode->key << ": " << currNode->bid.bidId << " | " << currNode->bid.amount
                    << " | " << currNode->bid.fund << endl;

            }
        }
    }

    return;

}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    
    //converts bidId to an integer with stoi function
    //applies the hash function to get an index in the hash table
    //gets the node at that index using nodes.at
    //takes the address of the node and stores it in pointer "cursor"
    Node* cursor = &nodes.at(hash(stoi(bidId)));

    //initializes a temp pointer to null for search and deletion logic
    Node* tempNode = NULL;

    //if there is a chain of nodes...
    if (cursor->next != nullptr) { 

        //if the first node in the chain matches bidId...
        if (cursor->bid.bidId == bidId) {

            //copy the next node's data into the current node (cursor) to simulate removing the first node
            tempNode = cursor->next;
            //update pointers
            cursor->key = tempNode->key;
            cursor->bid = tempNode->bid;
            cursor->next = tempNode->next;
            //free the next node's memory
            free(tempNode);
            return;
        }

        //the first node didn’t match; we loop through the chain looking for bidId
        else { 

            //if the end of the list hasn't been reached, but bidId has not been found
            while (cursor != NULL && cursor->bid.bidId != bidId) {

                //set temp to the current cursor
                tempNode = cursor; 
                //move cursor ahead
                cursor = cursor->next; 
            }

            //the list was searched and didn't find the key
            if (cursor == NULL) {

                cout << " Key not found." << endl;
                return;
            }

            //update tempNode->next to skip over the matched node, unlinking it from the chain
            tempNode->next = cursor->next;

            delete cursor;

        }

    }
    //there’s no chain (only one node at this bucket)
    else {

        //this single node matches bidId
        if (cursor->bid.bidId == bidId) {

            //reset the node to a blank node, clearing it
            Node newNode = Node();

            cursor->key = newNode.key;
            cursor->bid = newNode.bid;
            cursor->next = nullptr;

            return;

        }

        //no chain and no match at the head
        else {

            cout << "Bid not found." << endl;
        }
    }

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    //converts bidId to an integer with stoi function
    //applies the hash function to get an index in the hash table
    //gets the node at that index using nodes.at
    //takes the address of the node and stores it in pointer "cursor"
    Node* cursor = &nodes.at(hash(stoi(bidId)));

    while (cursor != NULL) {

        //if the current node's bidId matches the one being searched for..
        if (cursor->bid.bidId == bidId) {

            //return the matching bid
            return cursor->bid;

        }

        //otherwise move to the next mode in the chain
        cursor = cursor->next;

    }

    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    //for (auto const& c : header) {
    //    cout << c << " | ";
    //}
    //cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();

    int choice = 0;
    string anyKey = " ";
    bool goodInput;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        try {
            cin >> choice;

            // limit the user menu inputs to good values
            if ((choice > 0 && choice < 5) || (choice == 9)) {
                goodInput = true;
            }
            //throw error for catch
            else {
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:

                // Initialize a timer variable before loading bids
                ticks = clock();

                // Complete the method call to load the bids
                loadBids(csvPath, bidTable);

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 2:
                bidTable->PrintAll();

                cout << "\n Enter any key to continue..." << endl;

                cin >> anyKey;

                break;

            case 3:
                ticks = clock();

                bid = bidTable->Search(bidKey);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                if (!bid.bidId.empty()) {
                    displayBid(bid);
                }
                else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 4:
                bidTable->Remove(bidKey);

                cout << "Bid removed." << endl;

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 9:

                break;

            default:

                throw 2;
            }
        }

        catch (int err) {

            std::cout << "\nPlease check your input." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        //clear the cin operator of extra input
        cin.clear();
        cin.ignore();

        //clear the console
        system("cls");
    }

    cout << "Goodbye." << endl;

    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}
