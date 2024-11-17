#include <iostream>
#include <vector>
#include <climits>
using namespace std;

// Function to display the allocation results
void displayAllocationResult(const vector<int>& allocation, const vector<int>& processSize, int numProcesses) {
    cout << "Process No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < numProcesses; i++) {
        cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i] + 1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

// Function for First Fit allocation
void firstFit(vector<int>& blockSize, int numBlocks, vector<int>& processSize, int numProcesses) {
    vector<int> allocation(numProcesses, -1);  // Store allocation result (-1 means not allocated)
    vector<bool> used(numBlocks, false);       // Track used blocks

    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numBlocks; j++) {
            if (!used[j] && blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                used[j] = true;                  // Mark block as used
                break;
            }
        }
    }

    cout << "\nFirst Fit Allocation Result:\n";
    displayAllocationResult(allocation, processSize, numProcesses);
}

// Function for Next Fit allocation
void nextFit(vector<int>& blockSize, int numBlocks, vector<int>& processSize, int numProcesses) {
    vector<int> allocation(numProcesses, -1);
    vector<bool> used(numBlocks, false);
    int lastAllocatedBlock = 0;

    for (int i = 0; i < numProcesses; i++) {
        int j = (lastAllocatedBlock+1)%numProcesses;
        while(j != lastAllocatedBlock ) {
            if (!used[j] && blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                used[j] = true;
                lastAllocatedBlock = j;
                break;
            }
             j = (j+1)%numProcesses;
        }
        cout<<i<<" "<<lastAllocatedBlock<<endl;
        
    }

    cout << "\nNext Fit Allocation Result:\n";
    displayAllocationResult(allocation, processSize, numProcesses);
}

// Function for Worst Fit allocation
void worstFit(vector<int>& blockSize, int numBlocks, vector<int>& processSize, int numProcesses) {
    vector<int> allocation(numProcesses, -1);
    vector<bool> used(numBlocks, false);

    for (int i = 0; i < numProcesses; i++) {
        int worstIndex = -1;
        for (int j = 0; j < numBlocks; j++) {
            if (!used[j] && blockSize[j] >= processSize[i]) {
                if (worstIndex == -1 || blockSize[j] > blockSize[worstIndex]) {
                    worstIndex = j;
                }
            }
        }
        if (worstIndex != -1) {
            allocation[i] = worstIndex;
            used[worstIndex] = true;  // Mark block as used
        }
    }

    cout << "\nWorst Fit Allocation Result:\n";
    displayAllocationResult(allocation, processSize, numProcesses);
}

// Function for Best Fit allocation
void bestFit(vector<int>& blockSize, int numBlocks, vector<int>& processSize, int numProcesses) {
    vector<int> allocation(numProcesses, -1);
    vector<bool> used(numBlocks, false);

    for (int i = 0; i < numProcesses; i++) {
        int bestIndex = -1;
        for (int j = 0; j < numBlocks; j++) {
            if (!used[j] && blockSize[j] >= processSize[i]) {
                if (bestIndex == -1 || blockSize[j] < blockSize[bestIndex]) {
                    bestIndex = j;
                }
            }
        }
        if (bestIndex != -1) {
            allocation[i] = bestIndex;
            used[bestIndex] = true;  // Mark block as used
        }
    }

    cout << "\nBest Fit Allocation Result:\n";
    displayAllocationResult(allocation, processSize, numProcesses);
}

int main() {
    int numBlocks, numProcesses, choice;

    do {
        cout << "\nMemory Allocation Strategies Menu\n";
        cout << "1. First Fit\n";
        cout << "2. Next Fit\n";
        cout << "3. Worst Fit\n";
        cout << "4. Best Fit\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 5) break;

        cout << "Enter number of memory blocks: ";
        cin >> numBlocks;
        vector<int> blockSize(numBlocks);

        cout << "Enter sizes of each block:\n";
        for (int i = 0; i < numBlocks; i++) {
            cout << "Block " << i + 1 << ": ";
            cin >> blockSize[i];
        }

        cout << "\nEnter number of processes: ";
        cin >> numProcesses;
        vector<int> processSize(numProcesses);

        cout << "Enter memory size required for each process:\n";
        for (int i = 0; i < numProcesses; i++) {
            cout << "Process " << i + 1 << ": ";
            cin >> processSize[i];
        }

        // Call the appropriate function based on user choice
        switch (choice) {
            case 1:
                firstFit(blockSize, numBlocks, processSize, numProcesses);
                break;
            case 2:
                nextFit(blockSize, numBlocks, processSize, numProcesses);
                break;
            case 3:
                worstFit(blockSize, numBlocks, processSize, numProcesses);
                break;
            case 4:
                bestFit(blockSize, numBlocks, processSize, numProcesses);
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
                break;
        }

    } while (choice != 5);

    cout << "Exiting the program...\n";
    return 0;
}
