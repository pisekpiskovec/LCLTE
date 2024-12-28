#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

string command, filePath = "";
vector<string> fileContent;

string formatNumber(int current, int places)
{
    string retString = "";
    for (uint i = 0; i < to_string(places).length() - to_string(current).length(); i++)
    {
        retString += "0";
    }
    retString += to_string(current);
    return retString;
}

bool fileExists(string name){
    ifstream f(name.c_str());
    return f.good();
}

void loadBuffer(string path){
    if(path == "" || !fileExists(path)){ cout << "No file loaded." << endl; }
    ifstream file(path);
    if(!file.is_open()) {cout << "Failed to open file." << endl; return;}
    string line;
    for(int i = 0; getline(file, line); i++){ line += "\n"; fileContent.push_back(line); }
    file.close();
}

void openFile(){

    cout << "Location: " << endl;
    cin.ignore();
    getline(cin, filePath);
    loadBuffer(filePath);
}

void printBuffer(){
    for(uint row = 1; row <= fileContent.size(); row++){
        cout << formatNumber(row, fileContent.size()) << " | " << fileContent[row-1];
    }
}

void readFile(){ if(!fileContent.empty()) printBuffer(); }

void newFile(){
    string inputEnd, line;
    cout << "Enter End Of File flag [def: EOF]: ";
    cin.ignore();
    getline(cin, inputEnd);
    if (inputEnd == "") inputEnd = "EOF";
    cout << endl << endl;
    fileContent.clear();
    while(true){
        getline(cin, line);
        if(line == inputEnd) break;
        line += "\n";
        fileContent.push_back(line);
    }
}

void saveFile(){
    if(filePath == ""){ cout << "Invalid location. Please enter new location." << endl; openFile(); }
    else if(fileExists(filePath)) {
        cout << "File already exists. Rewrite? [def: Y] [y/n] ";
        string rewrite;
        cin.ignore();
        getline(cin, rewrite);
        if (!(rewrite == "" || rewrite == to_string(tolower('Y'))))
            { cout << "Please enter new location." << endl; openFile(); }
    }

    ofstream file(filePath);
    if(!file.is_open()) return;
    for(uint i = 0; i < fileContent.size(); i++){
        file << fileContent[i];
    }
    file.close();
}

void editLine(){
    string selectedLineStr = "";
    if(filePath == ""){ cout << "Invalid location. Please enter new location." << endl; openFile(); }
    lineSelecting:
        selectedLineStr = "";
        cout << "Enter line number (max " << fileContent.size() << "): ";
        cin.ignore();
        getline(cin, selectedLineStr);
        cout << "Debug: slectedLineStr: " << selectedLineStr << endl;
        uint selectedLine = stoi(selectedLineStr);
    if(selectedLine > fileContent.size()+1 || selectedLine == 0) goto lineSelecting;
    string inputEnd, line;
    cout << "Enter End Of File flag [def: EOF]: ";
    cin.ignore();
    getline(cin, inputEnd);
    if (inputEnd == "") inputEnd = "EOF";
    cout << formatNumber(selectedLine, fileContent.size()) << " | " << fileContent[selectedLine-1];
    cout << endl << endl;
    while(true){
        getline(cin, line);
        if(line == inputEnd) break;
        line += "\n";
        fileContent.push_back(line);
    }
}

int main()
{
    shellBegin:
        cout << "Enter command: " << endl;
        cin >> command;
        if(command == "open" || command == "load") openFile();
        else if(command == "read" || command == "print") readFile();
        else if(command == "new") newFile();
        else if(command == "save") saveFile();
        else if(command == "edit") editLine();
        else if(command == "exit" || command == "quit" || command == "q") goto shellEnd;
        else { cout << "Command not found." << endl; }
    goto shellBegin;
    shellEnd:
        return 0;
}
