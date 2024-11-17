#include <bits/stdc++.h>
using namespace std;

// Function to read all lines from a file and store them in a vector of strings
vector<string> readLines(const string& filename) {
    ifstream file(filename);
    vector<string> lines;
    string line;

    // Read each line from the file and add it to the vector
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream tokenStream(str);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
class Process {
public:
    // Get the macro definition from MNT based on the macro name
    static string getMacroDef(const string& name, const vector<string>& MNT) {
        for (const auto& entry : MNT) {
            if (split(entry, ' ')[0] == name) {
                return entry;  // Return the macro definition if found
            }
        }
        return "";  // Return an empty string if macro is not found
    }

    // Expand the macro using MDT and APTAB
    static void macroMDT(const string& macroMNT, const vector<string>& MDT, const vector<string>& aptab) {
        int mdtIndex = stoi(split(macroMNT, ' ')[3]) - 1;  // Get MDT index from macro definition
        cout << "Expanded Code:" << endl;

        // Loop through the MDT lines and expand the macro
        for (size_t i = mdtIndex; i < MDT.size(); ++i) {
            if (MDT[i] == "MEND") {
                break;  // Stop when we reach "MEND"
            } else {
                vector<string> mdtLine = split(MDT[i], ' ');
                string instruction = mdtLine[0];
                string expandedLine = instruction;

                // Replace positional parameters (P, 1) with values from APTAB
                for (size_t j = 1; j < mdtLine.size(); ++j) {
                    string param = mdtLine[j];
                    if (param.substr(0, 3) == "(P,") {
                        int paramIndex = stoi(param.substr(3, param.size() - 4)) - 1;  // Extract parameter index
                        expandedLine += " " + aptab[paramIndex];  // Add value from APTAB
                    } else {
                        expandedLine += " " + param;  // For non-positional parameters, just add them
                    }
                }
                cout << expandedLine << endl;  // Print the expanded line
            }
        }
        cout << "_________________________________" << endl;
    }

    // Prepare the Argument Table (APTAB) using macro call and default values from KPDTAB
    static vector<string> prepareAPTAB(const string& macroMNT, const vector<string>& KPDTAB, const string& macroCall) {
        vector<string> mntValues = split(macroMNT, ' ');
        int numPP = stoi(mntValues[1]);  // Number of positional parameters
        int numKP = stoi(mntValues[2]);  // Number of keyword parameters
        int kpdTabStart = stoi(mntValues[4]) - 1;  // Starting index for KPDTAB entries

        vector<string> aptab;
        vector<string> callParams = split(split(macroCall, ' ')[1], ',');  // Split macro call parameters

        // Add positional parameters to APTAB
        for (const auto& param : callParams) {
            if (param.find('=') == string::npos) {
                aptab.push_back(param);  // Add positional parameters without default values
            }
        }

        // Add default keyword parameters from KPDTAB
        for (size_t i = kpdTabStart; i < kpdTabStart + numKP; ++i) {
            vector<string> entry = split(KPDTAB[i], ' ');
            aptab.push_back(entry[1]);  // Add default keyword parameters to APTAB
        }

        // Override default keyword parameters with values from the macro call
        for (const auto& param : callParams) {
            if (param.find('=') != string::npos) {
                vector<string> kvPair = split(param, '=');
                aptab.push_back(kvPair[1]);  // Add overridden keyword parameters to APTAB
            }
        }

        return aptab;  // Return the completed APTAB
    }
};

int main() {
    // Read the macro calls, MNT, MDT, and KPDTAB from their respective files
    vector<string> macroCalls = readLines("CALLS2.txt");
    vector<string> MNT = readLines("MNT2.txt");
    vector<string> MDT = readLines("MDT2.txt");
    vector<string> KPDTAB = readLines("KPDTAB2.txt");

    // Process each macro call
    for (auto& macroCall : macroCalls) {
        // trim(macroCall);  // Trim spaces from the macro call
        string macroName = split(macroCall, ' ')[0];  // Get macro name
        string macroEntry = Process::getMacroDef(macroName, MNT);  // Get macro definition
        vector<string> aptab = Process::prepareAPTAB(macroEntry, KPDTAB, macroCall);  // Prepare the APTAB
        Process::macroMDT(macroEntry, MDT, aptab);  // Expand the macro using MDT and APTAB
    }

    return 0;
}

// // Function to remove leading and trailing spaces from a string
// void trim(string& str) {
//     str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
//         return !isspace(ch);  // Find first non-space character
//     }));
//     str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
//         return !isspace(ch);  // Find last non-space character
//     }).base(), str.end());
// }
