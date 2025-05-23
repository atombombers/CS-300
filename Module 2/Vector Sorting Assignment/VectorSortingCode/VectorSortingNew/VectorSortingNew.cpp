//============================================================================
// Name        : VectorSorting.cpp
// Author      : Brooke Slampak
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Vector Sorting Algorithms
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
    string title; //title of the item
    string fund; //associated fund
    double amount; //bid amount

    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    std::cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information using console (std::in)
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    std::cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    std::cout << "Enter title: ";
    getline(cin, bid.title);

    std::cout << "Enter fund: ";
    cin >> bid.fund;

    std::cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    std::cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

// FIXME (2a): Implement the quick sort logic over bid.title

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    //set low and high equal to begin and end

    // pick the middle element as pivot point

    // while not done 

        // keep incrementing low index while bids[low].title < Pivot

        // keep decrementing high index while Pivot < bids[high].title

        /* If there are zero or one elements remaining,
            all bids are partitioned. Return high */
            // else swap the low and high bids (built in vector method)
                 // move low and high closer ++low, --high
         //return high;


    //local variable declaration
    int midPoint = begin + (end - begin) / 2;
    string pivot = bids[midPoint].title;
    Bid tempSwap;
    bool done = false;

    while (!done) {

        // Increment begin index until a bid >= pivot is found
        while (bids[begin].title < pivot) {

            begin++;
        }

        // Decrement end index until a bid <= pivot is found
        while (pivot < bids[end].title) {

            end--;
        }

        // If indices cross, partitioning is done
        if (begin >= end) {

            done = true;
        }

        else {
            // Swap values and move indices toward center
            tempSwap = bids[begin];
            bids[begin] = bids[end];
            bids[end] = tempSwap;

            begin++;
            end--;
        }
    }

    return end;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    //set mid equal to 0

    /* Base case: If there are 1 or zero bids to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/

     /* Partition bids into low and high such that
      midpoint is location of last element in low */

      // recursively sort low partition (begin to mid)

      // recursively sort high partition (mid+1 to end)

    int lowIndex;

    if (begin >= end) {

        return; // base case: 0 or 1 elements
    }

    // Partition the vector and get the midpoint
    lowIndex = partition(bids, begin, end);

    // Recursively sort the two partitions
    quickSort(bids, begin, lowIndex);
    quickSort(bids, lowIndex + 1, end);

}

// FIXME (1a): Implement the selection sort logic over bid.title

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    //define min as int (index of the current minimum bid)

    // check size of bids vector
    // set size_t platform-neutral result equal to bid.size()

    // pos is the position within bids that divides sorted/unsorted
    // for size_t pos = 0 and less than size -1 
        // set min = pos
        // loop over remaining elements to the right of position
            // if this element's title is less than minimum title
                // this element becomes the minimum
        // swap the current minimum with smaller one found
            // swap is a built in vector method

    //local variable declaration
    int indexSmallest = 0;
    Bid tempSwap;

    for (int i = 0; i < bids.size() - 1; i++) {

        indexSmallest = i;

        // Find the smallest item in remaining unsorted list
        for (int j = i + 1; j < bids.size(); j++) {

            if (bids[j].title < bids[indexSmallest].title) {
                indexSmallest = j;
            }
        }

        // Swap the found smallest with the current element
        tempSwap = bids[i];
        bids[i] = bids[indexSmallest];
        bids[indexSmallest] = tempSwap;

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
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;


    const int GLOBAL_SLEEP_VALUE = 5000;
    int choice = 0;
    string anyKey = " ";
    bool goodInput;

    while (choice != 9) {

        std::cout << "Menu:" << endl;
        std::cout << "  1. Load Bids" << endl;
        std::cout << "  2. Display All Bids" << endl;
        std::cout << "  3. Selection Sort All Bids" << endl;
        std::cout << "  4. Quick Sort All Bids" << endl;
        std::cout << "  9. Exit" << endl;
        std::cout << "Enter choice: ";

        try {

            cin >> choice;

            if ((choice > 0 && choice < 5) || (choice == 9)) {
                goodInput = true;
            }
            else {
                goodInput = false;
                throw 1;
            }

            switch (choice) { 

            case 1:
                // Initialize a timer variable before loading bids
                ticks = clock();

                // Complete the method call to load the bids
                bids = loadBids(csvPath);

                std::cout << bids.size() << " bids read" << endl;

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                std::cout << "time: " << ticks << " clock ticks" << endl;
                std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_VALUE);

                break;

            case 2:
                // Loop and display the bids read
                for (int i = 0; i < bids.size(); ++i) {
                    displayBid(bids[i]);
                }
                std::cout << "Press any key to continue...";

                cin >> anyKey;

                break;

            case 3:

                //Selection sort switch 

                ticks = clock();

                selectionSort(bids);

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                std::cout << "time: " << ticks << " clock ticks" << endl;
                std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_VALUE);

                break;

            case 4:

                //Quick sort switch 

                ticks = clock();

                quickSort(bids, 0, bids.size() - 1);

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                std::cout << "time: " << ticks << " clock ticks" << endl;
                std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_VALUE);

                break;

            case 9:
                //Exit
                break;

            default:
                throw 2;
            }
        }
        catch (int err) {
            std::cout << "\nInvalid input, try again." << endl;
            Sleep(GLOBAL_SLEEP_VALUE);
        }

        cin.clear(); // Reset input state
        cin.ignore(); // Discard leftover characters
        system("cls"); //Clear the screen
    }

    std::cout << "Goodbye." << endl;

    Sleep(GLOBAL_SLEEP_VALUE);

    return 0;
}
