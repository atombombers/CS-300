//============================================================================
// Name        : LinkedList.cpp
// Author      : Brooke Slampak
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include <Windows.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node* next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // FIXME (1): Initialize housekeeping variables
    // set head and tail equal to null
    head = NULL;
    tail = NULL;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // FIXME (2): Implement append logic
    //Create new node
    Node* newNode = new Node(bid);

    //if there is nothing at the head...
    if (head == NULL) {
        // new node becomes the head and the tail
        head = newNode;
        tail = newNode;
    }
    else {
        // make current tail node point to the new node
        tail->next = newNode;
        // and tail becomes the new node
        tail = newNode;
    }
    //increase size count
    size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // FIXME (3): Implement prepend logic
    
    // Create new node
    Node* newNode = new Node(bid);

    // if there is already something at the head...
    if (head->next != nullptr) {
        // new node points to current head as its next node
        newNode->next = head;
        // head now becomes the new node
        head = newNode;
        //increase size coun
        size++;
    }
    else {
        // If the list is empty (head is null), use Append to add the bid as the first node
        Append(bid);
    }
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // FIXME (4): Implement print logic
    
    // start at the head
    Node* currentNode = head;

    //loop over each node looking for a match
    for (int i = 0; i < size; i++) {
        //output current bidID, title, amount and fund
        cout << currentNode->bid.bidId << ": " << currentNode->bid.title << " | " << currentNode->bid.amount
            << " | " << currentNode->bid.fund << endl;
        //set current equal to next
        currentNode = currentNode->next;
    }

}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    // FIXME (5): Implement remove logic

    // Start at the head
    Node* cursor = head;
    Node* tempNode;

    // Loop through the list up to size - 1 nodes
    for (int i = 0; i < size - 1; i++) {
        // The node to remove is the head
        if ((cursor->bid.bidId == bidId) && (i == 0)) {
            // Make head point to the next node in the list
            head = cursor->next;
            // Free memory occupied by the current head node
            free(cursor);
            // Decrease size count
            size--;
            break;
        }
        // The node to remove is the tail
        else if ((cursor->next->bid.bidId == bidId) && (cursor->next->next == nullptr)) {//match the tail
            // hold onto the next node temporarily
            tempNode = cursor->next;
            // Set the tail pointer to the current node (new tail)
            tail = cursor;
            // Disconnect the last node
            cursor->next = nullptr;
            // Free up memory held by temp
            free(tempNode);
            // Decrease size count
            size--;
            break;

        }
        //The node to remove is in the middle of the list
        else if (cursor->bid.bidId == bidId) {
            // Temporarily store the node after the current node
            tempNode = cursor->next;
            // Copy the data from the next node into the current one 
            cursor->bid = tempNode->bid;
            // Skip over the next node
            cursor->next = tempNode->next;
            // Free up memory held by temp
            free(tempNode);
            // Decrease size count
            size--;
            break;
        }

        // Move to the next node in the list
        cursor = cursor->next;

    }
}



/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {

    // Default Bid object to return if no match is found
    Bid bid;

    // FIXME (6): Implement search logic

    // Start at the head of the list
    Node* cursor = head;

    // Search the list until the end is reached
    while (cursor != NULL) {
        // if the current node bidId matches the search bidId, return it
        if (cursor->bid.bidId == bidId) {
 
            return cursor->bid;

        }

        // Move to the next node in the list
        cursor = cursor->next;

    }

    // If no match is found, return the default Bid object
    return bid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
        << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList* list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
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
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv"; 
        bidKey = "98109"; 
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;
    // Defines a global sleep duration of 5000 milliseconds
    const int GLOBAL_SLEEP_VALUE = 5000;
    // Variable for user menu choice
    int choice = 0;
    // Variable for input pause
    string anyKey = " ";
    // Boolean for validating user input
    bool goodInput;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";


        try {
            // Get user input for menu choice
            cin >> choice;
            // Validate choice; only 1–5 and 9 are acceptable choices
            if ((choice > 0 && choice < 6) || (choice == 9)) {
                goodInput = true;
            }
            else {
                // Invalid input throws an exception
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:
                bid = getBid();
                //bidList.Append(bid);
                bidList.Prepend(bid);
                displayBid(bid);

                break;

            case 2:
                ticks = clock();

                loadBids(csvPath, &bidList);

                cout << bidList.Size() << " bids read" << endl;

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " milliseconds" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_VALUE);

                break;

            case 3:
                bidList.PrintList();
                // Pause to allow user to read output
                cout << "\n Enter any key to continue." << endl;

                cin >> anyKey;

                break;

            case 4:
                ticks = clock();

                bid = bidList.Search(bidKey);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                if (!bid.bidId.empty()) {
                    displayBid(bid);
                }
                else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_VALUE);

                break;

            case 5:
                bidList.Remove(bidKey);

                Sleep(GLOBAL_SLEEP_VALUE);

                break;

            case 9:
                //Exit program
                break;

            default:

                throw 2;
            }

        }
        // Catch and handle invalid input
        catch (int err) {
            std::cout << "\nInvalid input. Try again." << endl;
            Sleep(GLOBAL_SLEEP_VALUE);
        }

        //clears the cin operator of input
        cin.clear();
        cin.ignore();

        //clears the console screen
        system("cls");

    }

    cout << "Goodbye." << endl;

    Sleep(GLOBAL_SLEEP_VALUE);

    return 0;
}