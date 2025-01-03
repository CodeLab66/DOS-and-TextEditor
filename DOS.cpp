#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include "Node.h"
#include "File.h"
#include "Directory.h"
#include "TextEditor.h"
using namespace std;

struct PriorityPrint
{
    string name;
    string content;
    int priority;

    bool operator<(const PriorityPrint& temp) const
    {
        return priority < temp.priority;
    }
};
priority_queue<PriorityPrint> priorityPrintQueue;
struct Print
{
    string name;
    string content;
};
queue<Print> printQueue;
struct PriorityFilePrint
{
    string name;
    string content;
    int priority;
    int timeToPrint;

    bool operator<(const PriorityFilePrint& other) const
    {
        return priority < other.priority;
    }
};
priority_queue<PriorityFilePrint> priorityFileQueue;

void displayAuthorInformation()
{
    cout << "Author: Muhammad Bilal" << endl;
    cout << "Registeration Number: BSCS23146" << endl;
    cout << "Course Name: Data Structures" << endl;
}



void findFileRecursive(TreeNode* currentDir, const string& fileName, bool& foundFile, string currentPath)
{
    if (Directory* dir = dynamic_cast<Directory*>(currentDir))
    {
        for (TreeNode* i : dir->children)
        {
            string newPath = currentPath + "/" + i->name;
            if (i->name == fileName)
            {
                cout << "File found at path: " << newPath << endl;
                i->displayInformation();
                foundFile = true;
                return;
            }
            if (Directory* subDir = dynamic_cast<Directory*>(i))
            {
                findFileRecursive(subDir, fileName, foundFile, newPath);
                if (foundFile)
                {
                    return;
                }
            }
        }
    }
}
void findTextInFile(const string& filePath, const string& searchString)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cout << "Cannot open file: " << filePath << endl;
        return;
    }

    string line;
    int lineNumber = 1;
    bool found = false;

    while (getline(file, line))
    {
        if (line.find(searchString) != string::npos)
        {
            cout << "Found \"" << searchString << "\" in " << filePath << " at line " << lineNumber << ": " << line << endl;
            found = true;
        }
        lineNumber++;
    }

    if (!found)
    {
        cout << "\"" << searchString << "\" not found in " << filePath << endl;
    }

    file.close();
}
void formatDirectory(Directory* dir)
{
    for (TreeNode* child : dir->children)
    {
        if (Directory* subDir = dynamic_cast<Directory*>(child))
        {
            formatDirectory(subDir);
        }
        delete child;
    }
    dir->children.clear();
}
Directory* findDirectory(TreeNode* node, const string& dirName)
{
    if (Directory* dir = dynamic_cast<Directory*>(node))
    {
        if (dir->name == dirName)
        {
            return dir;
        }
        for (TreeNode* child : dir->children)
        {
            Directory* foundDir = findDirectory(child, dirName);
            if (foundDir)
            {
                return foundDir;
            }
        }
    }
    return nullptr;
}
void removeDirectory(Directory* parentDir, const string& dirName)
{
    for (vector<TreeNode*>::iterator i = parentDir->children.begin(); i != parentDir->children.end(); ++i)
    {
        Directory* tempDir = dynamic_cast<Directory*>(*i);
        if (tempDir != nullptr && tempDir->name == dirName)
        {
            for (TreeNode* child : tempDir->children)
            {
                if (Directory* subDir = dynamic_cast<Directory*>(child))
                {
                    removeDirectory(tempDir, subDir->name);
                }
                else
                {
                    delete child;
                }
            }
            tempDir->children.clear();
            delete tempDir;
            parentDir->children.erase(i);
            cout << "Directory " << dirName << " removed." << endl;
            return;
        }
    }
    cout << "Directory " << dirName << " not found in the current directory." << endl;
}


void processCommands(Directory& root, Directory*& currentDir, const string& command)
{
    cout << endl;
    if (command.substr(0, 6) == "ATTRIB")
    {
        if (command.length() <= 7) {
            cout << "Usage: ATTRIB <filename>" << endl;
            return;
        }
        string fileName = command.substr(7);
        bool foundFile = false;
        for (TreeNode* i : currentDir->children)
        {
            if (i->name == fileName)
            {
                i->displayInformation();
                foundFile = true;
                break;
            }
        }
        if (!foundFile)
        {
            cout << "Couldn't find the File in Current Directory";
        }
    }
    else if (command.substr(0, 2) == "CD")
    {
        if (command == "CD.")
        {
            cout << "Current Directory: " << currentDir->getPath() << endl;
        }
        else if (command == "CD..")
        {
            if (currentDir->parent != nullptr)
            {
                currentDir = dynamic_cast<Directory*>(currentDir->parent);
            }
            else
            {
                cout << "Already at the root directory." << endl;
            }
        }
        else if (command == "CD/")
        {
            currentDir = &root;
        }
        else if (command == "CD")
        {
            cout << "Current Directory: " << currentDir->name << endl;
            cout << "Contents:" << endl;
            for (TreeNode* child : currentDir->children)
            {
                cout << " - " << child->name;
                if (dynamic_cast<Directory*>(child))
                {
                    cout << " (Directory) ";
                }
                else
                {
                    cout << " (File) ";
                }
                cout << endl;
            }
        }
        else if (command.length() > 3 && command.substr(0, 3) == "CD ")
        {
            string dirName = command.substr(3);
            bool found = false;
            for (TreeNode* child : currentDir->children)
            {
                Directory* dir = dynamic_cast<Directory*>(child);
                if (dir && dir->name == dirName)
                {
                    currentDir = dir;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                cout << "Directory " << dirName << " not found in the current directory." << endl;
            }
        }
        else
        {
            cout << "Invalid CD command." << endl;
        }
    }
    else if (command.substr(0, 7) == "CONVERT")
    {
        int space = command.find(' ', 8);
        if (space == string::npos)
        {
            cout << "Usage: CONVERT (source_extension) (target_extension)" << endl;
            return;
        }

        string sourceExt = command.substr(8, space - 8);
        string targetExt = command.substr(space + 1);

        bool converted = false;
        for (TreeNode* child : currentDir->children)
        {
            File* file = dynamic_cast<File*>(child);
            if (file && file->fileType == sourceExt)
            {
                int lastdot = file->name.find_last_of('.');
                string newName = file->name.substr(0, lastdot) + "." + targetExt;
                file->renameFile(newName);
                file->fileType = targetExt;
                converted = true;
                cout << "Converted: " << file->name << " to ." << targetExt << endl;
            }
        }
        if (!converted)
        {
            cout << "No files with the extension ." << sourceExt << " were found in the current directory." << endl;
        }
    }
    else if (command.substr(0, 4) == "COPY")
    {
        if (command.length() <= 5)
        {
            cout << "Usage: COPY <filename> <destination_directory>" << endl;
            return;
        }
        int space = command.find(' ', 5);
        string fileName = command.substr(5, space - 5);
        string destDirName = command.substr(space + 1);
        bool foundFile = false;
        File* copyOfFile = nullptr;
        for (TreeNode* i : currentDir->children)
        {
            File* file = dynamic_cast<File*>(i);
            if (file && file->name == fileName)
            {
                copyOfFile = file;
                foundFile = true;
                break;
            }
        }
        if (!foundFile)
        {
            cout << "File " << fileName << " not found in current directory." << endl;
            return;
        }
        Directory* destDir = findDirectory(&root, destDirName);
        if (destDir == nullptr)
        {
            cout << "Destination directory " << destDirName << " not found." << endl;
            return;
        }
        File* newFile = new File(*copyOfFile);
        newFile->parent = destDir;
        destDir->children.push_back(newFile);
        cout << "File " << fileName << " copied to " << destDirName << "." << endl;
    }
    else if (command.substr(0, 6) == "CREATE")
    {
        if (command.length() <= 7)
        {
            cout << "Usage: CREATE <filename>" << endl;
            return;
        }
        string fileName = command.substr(7);
        cout << "Enter the contents of the file (end with an empty line):" << endl;
        string content;
        string line;
        while (getline(cin, line) && !line.empty())
        {
            content += line + '\n';
        }
        File* newFile = new File(fileName, "owner", false, false, "txt", content, currentDir);
        currentDir->children.push_back(newFile);
        ofstream write(fileName);
        write << content;
        write.close();
        cout << "File " << fileName << " created." << endl;
    }
    else if (command.substr(0, 3) == "DEL")
    {
        if (command.length() <= 4)
        {
            cout << "Usage: DEL <filename>" << endl;
            return;
        }
        string fileName = command.substr(4);
        vector<TreeNode*>::iterator it = remove_if(currentDir->children.begin(), currentDir->children.end(), [&fileName](TreeNode* child)
            {
                File* file = dynamic_cast<File*>(child);
                if (file && file->name == fileName)
                {
                    remove(fileName.c_str());
                    delete file;
                    return true;
                }
                return false;
            });
        if (it != currentDir->children.end())
        {
            currentDir->children.erase(it, currentDir->children.end());
            cout << "File " << fileName << " deleted." << endl;
        }
        else
        {
            cout << "File " << fileName << " not found." << endl;
        }
    }
    else if (command == "DIR")
    {
        cout << "Directory : " << currentDir->name << endl;
        cout << "Content : " << endl;
        for (TreeNode* i : currentDir->children)
        {
            cout << " - " << i->name;
            if (dynamic_cast<Directory*>(i))
            {
                cout << " (Directory)";
            }
            else
            {
                cout << " (File)";
            }
            cout << endl;
        }
    }
    else if (command == "EXIT")
    {
        cout << "Exiting the Program..." << endl;
        exit(0);
    }
    else if (command.substr(0, 4) == "FIND")
    {
        if (command.length() <= 5)
        {
            cout << "Usage : Find <filename>" << endl;
            return;
        }
        string fileName = command.substr(5);
        bool found = false;
        findFileRecursive(currentDir, fileName, found, currentDir->getPath());
        if (found == false)
        {
            cout << "File " << fileName << " was not found.";
        }
    }
    else if (command.substr(0, 5) == "FINDF")
    {
        if (command.length() <= 6)
        {
            cout << "Usage: FINDF <search_string> <filename>" << endl;
            return;
        }

        string searchString = command.substr(6);
        size_t spaceIndex = searchString.find(' ');
        if (spaceIndex == string::npos)
        {
            cout << "Usage: FINDF <search_string> <filename>" << endl;
            return;
        }

        string search = searchString.substr(0, spaceIndex);
        string filename = searchString.substr(spaceIndex + 1);

        bool found = false;
        for (TreeNode* i : currentDir->children)
        {
            File* file = dynamic_cast<File*>(i);
            if (file && file->name == filename)
            {
                if (file->content.find(search) != string::npos)
                {
                    cout << "Search string found in file: " << file->name << endl;
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            cout << "File not found or search string not found in the file!" << endl;
        }
    }
    else if (command == "FORMAT")
    {
        formatDirectory(currentDir);
        cout << "Formatted the current directory and all subdirectories." << endl;
    }
    else if (command == "HELP")
    {
        cout << "Available commands:" << endl;
        cout << "ATTRIB: Displays file (name provided as input) attributes." << endl;
        cout << "Usage: ATTRIB <filename>" << endl;
        cout << endl;
        cout << "CD: Displays the name of or changes the current directory." << endl;
        cout << "Usage: CD <directory_name>" << endl;
        cout << "Special Cases:" << endl;
        cout << "  CD. : Prints working directory (i.e. your current directory in your tree structure)." << endl;
        cout << "  CD.. : Change directory to previous directory." << endl;
        cout << "  CD\\ : Changes directory to root directory (i.e. V:\\>)." << endl;
        cout << endl;
        cout << "CONVERT: Asks two types and converts extension of all files of one type to another type." << endl;
        cout << "Usage: CONVERT <source_extension> <target_extension>" << endl;
        cout << endl;
        cout << "COPY: Copies one file in the current directory to another location (directory)." << endl;
        cout << "Usage: COPY <filename> <destination_directory>" << endl;
        cout << endl;
        cout << "CREATE: Creates a file with the name provided and allows the user to enter contents of the file." << endl;
        cout << "Usage: CREATE <filename>" << endl;
        cout << endl;
        cout << "DEL: Deletes a file whose name is provided in input." << endl;
        cout << "Usage: DEL <filename>" << endl;
        cout << endl;
        cout << "DIR: Displays a list of files and subdirectories in a directory." << endl;
        cout << "Usage: DIR" << endl;
        cout << endl;
        cout << "EDIT: Opens a file (loads into a linked list) and allows the user to edit and save the contents of the file." << endl;
        cout << "Usage: EDIT <filename>" << endl;
        cout << endl;
        cout << "EXIT: Quits the program." << endl;
        cout << "Usage: EXIT" << endl;
        cout << endl;
        cout << "FIND: Searches for a file in your current virtual directory whose name is provided as input." << endl;
        cout << "Usage: FIND <filename>" << endl;
        cout << endl;
        cout << "FORMAT: Formats the current virtual directory i.e. empties the current directory and all subdirectories." << endl;
        cout << "Usage: FORMAT" << endl;
        cout << endl;
        cout << "HELP: Provides Help information for all these commands." << endl;
        cout << "Usage: HELP" << endl;
        cout << endl;
        cout << "LOADTREE: Load a given tree in your tree structure. Given tree is in a file named tree.txt." << endl;
        cout << "Usage: LOADTREE <tree_file>" << endl;
        cout << endl;
        cout << "MKDIR: Creates a virtual directory." << endl;
        cout << "Usage: MKDIR <directory_name>" << endl;
        cout << endl;
        cout << "MOVE: Moves one file (whose name is provided as input) from one directory to another directory." << endl;
        cout << "Usage: MOVE <filename> <destination_directory>" << endl;
        cout << endl;
        cout << "PPRINT: Adds a text file to the priority based print queue, and displays the current priority queue." << endl;
        cout << "Usage: PPRINT <filename>" << endl;
        cout << endl;
        cout << "PROMPT: Changes the Windows command prompt (for example from V:\\> to V$)." << endl;
        cout << "Usage: PROMPT <new_prompt>" << endl;
        cout << endl;
        cout << "PRINT: Adds a text file to the print queue, and displays the current queue." << endl;
        cout << "Usage: PRINT <filename>" << endl;
        cout << endl;
        cout << "PQUEUE: Shows current state of the priority based print queue , with time left for each element." << endl;
        cout << "Usage: PQUEUE" << endl;
        cout << endl;
        cout << "PWD: Prints working directory." << endl;
        cout << "Usage: PWD" << endl;
        cout << endl;
        cout << "QUEUE: Shows current state of the print queue, with time left for each element." << endl;
        cout << "Usage: QUEUE" << endl;
        cout << endl;
        cout << "RENAME: Renames a file whose current and new name is provided as input." << endl;
        cout << "Usage: RENAME <current_name> <new_name>" << endl;
        cout << endl;
        cout << "RMDIR: Removes a directory along with its contents. Consider setting the respective sibling links." << endl;
        cout << "Usage: RMDIR <directory_name>" << endl;
        cout << endl;
        cout << "TREE: Displays the complete directory structure." << endl;
        cout << "Usage: TREE" << endl;
        cout << endl;
        cout << "VER: Displays the version of your directory." << endl;
        cout << "Usage: VER" << endl;
        cout << endl;
    }
    else if (command.substr(0, 5) == "MKDIR")
    {
        if (command.length() <= 6)
        {
            cout << "Usage: MKDIR <directory_name>" << endl;
            return;
        }
        string dirName = command.substr(6);
        Directory* newDir = new Directory(dirName, "owner", false, currentDir);
        currentDir->children.push_back(newDir);
        cout << "Directory " << dirName << " created." << endl;
    }
    else if (command.substr(0, 4) == "MOVE")
    {
        if (command.length() <= 5)
        {
            cout << "Usage: MOVE <filename> <destination_directory>" << endl;
            return;
        }
        int space = command.find(' ', 5);
        if (space == string::npos)
        {
            cout << "Usage: MOVE <filename> <destination_directory>" << endl;
            return;
        }
        string fileName = command.substr(5, space - 5);
        string destDirName = command.substr(space + 1);
        bool foundFile = false;
        File* fileToMove = nullptr;
        for (TreeNode* i : currentDir->children)
        {
            File* file = dynamic_cast<File*>(i);
            if (file && file->name == fileName)
            {
                fileToMove = file;
                foundFile = true;
                break;
            }
        }
        if (!foundFile)
        {
            cout << "File " << fileName << " not found in current directory." << endl;
            return;
        }

        Directory* destDir = findDirectory(&root, destDirName);
        if (destDir == nullptr)
        {
            cout << "Destination directory " << destDirName << " not found." << endl;
            return;
        }

        currentDir->children.erase(remove(currentDir->children.begin(), currentDir->children.end(), fileToMove), currentDir->children.end());
        fileToMove->parent = destDir;
        destDir->children.push_back(fileToMove);
        cout << "File " << fileName << " moved to " << destDirName << "." << endl;
    }
    else if (command.substr(0, 6) == "PROMPT")
    {
        if (command.length() <= 7)
        {
            cout << "Usage: PROMPT <new_prompt>" << endl;
            return;
        }
        string prompt = command.substr(7);
        cout << "Prompt changed to: " << prompt << endl;
    }
    else if (command == "PWD")
    {
        string path = "";
        Directory* dir = currentDir;
        while (dir != nullptr)
        {
            path = dir->name + "/" + path;
            dir = dynamic_cast<Directory*>(dir->parent);
        }
        cout << "Current working directory: " << path << endl;
    }
    else if (command.substr(0, 6) == "RENAME")
    {
        if (command.length() <= 7)
        {
            cout << "Usage: RENAME <current_name> <new_name>" << endl;
            return;
        }
        int space = command.find(' ', 7);
        string currentName = command.substr(7, space - 7);
        string newName = command.substr(space + 1);
        bool foundFile = false;

        for (TreeNode* i : currentDir->children)
        {
            File* file = dynamic_cast<File*>(i);
            if (file && file->name == currentName)
            {
                file->name = newName;
                foundFile = true;
                cout << "File " << currentName << " renamed to " << newName << "." << endl;
                break;
            }
        }

        if (!foundFile)
        {
            cout << "File " << currentName << " not found in the current directory." << endl;
        }
    }
    else if (command.substr(0, 5) == "RMDIR")
    {
        if (command.length() <= 6)
        {
            cout << "Usage: RMDIR <directory_name>" << endl;
            return;
        }
        string dirName = command.substr(6);
        removeDirectory(currentDir, dirName);
    }
    else if (command.substr(0, 5) == "PRINT")
    {
        if (command.length() <= 6)
        {
            cout << "Usage: PRINT <file_name>" << endl;
            return;
        }
        string fileName = command.substr(6);
        bool fileFound = false;
        for (TreeNode* child : currentDir->children)
        {
            File* file = dynamic_cast<File*>(child);
            if (file && file->name == fileName)
            {
                Print printFile = { file->name, file->content };
                printQueue.push(printFile);
                fileFound = true;
                break;
            }
        }
        if (!fileFound)
        {
            cout << "File " << fileName << " not found in the current directory." << endl;
            return;
        }
        cout << "Current Print Queue:" << endl;
        queue<Print> tempQueue = printQueue;
        while (!tempQueue.empty())
        {
            Print printFile = tempQueue.front();
            tempQueue.pop();
            cout << "File: " << printFile.name << endl;
        }
    }
    else if (command.substr(0, 6) == "PPRINT")
    {
        if (command.length() <= 7)
        {
            cout << "Usage: PPRINT <file_name>" << endl;
            return;
        }
        string fileName = command.substr(7);
        bool fileFound = false;
        for (TreeNode* child : currentDir->children)
        {
            File* file = dynamic_cast<File*>(child);
            if (file && file->name == fileName)
            {
                PriorityFilePrint printFile = { file->name, file->content, file->priority, file->timeToPrint };
                priorityFileQueue.push(printFile);
                fileFound = true;
                break;
            }
        }
        if (!fileFound)
        {
            cout << "File " << fileName << " not found in the current directory." << endl;
            return;
        }
        cout << "Current Print Queue:" << endl;
        priority_queue<PriorityFilePrint> tempQueue = priorityFileQueue;
        while (!tempQueue.empty())
        {
            PriorityFilePrint printFile = tempQueue.top();
            tempQueue.pop();
            cout << "File: " << printFile.name << " | Priority: " << printFile.priority << endl;
        }
    }
    else if (command.substr(0, 6) == "PQUEUE")
    {
        cout << "Priority Print Queue:" << endl;
        priority_queue<PriorityFilePrint> tempQueue = priorityFileQueue;
        while (!tempQueue.empty())
        {
            PriorityFilePrint printFile = tempQueue.top();
            tempQueue.pop();
            cout << "File: " << printFile.name << " | Priority: " << printFile.priority << " | Time to Print: " << printFile.timeToPrint << " seconds" << endl;
        }
    }
    else if (command == "TREE")
    {
        currentDir->displayInformation();
        cout << endl;
    }
    else if (command.substr(0, 4) == "EDIT")
    {
        if (command.length() <= 5)
        {
            cout << "Usage: EDIT <file_name>" << endl;
            return;
        }
        string fileName = command.substr(5);
        bool fileFound = false;
        for (TreeNode* child : currentDir->children)
        {
            File* file = dynamic_cast<File*>(child);
            if (file && file->name == fileName)
            {
                if (file->isLocked == true)
                {
                    string password;
                    cout << "File is locked , Enter the password to access the file : ";
                    cin >> password;
                    if (password != "1234")
                    {
                        cout << "Incorrect Password" << endl;
                        return;
                    }
                    else
                    {
                        system("cls");
                        OpenTextEditor(file->name);
                        fileFound = true;
                        break;
                    }
                }
                else
                {
                    system("cls");
                    OpenTextEditor(file->name);
                    fileFound = true;
                    break;
                }
            }
        }
        if (!fileFound)
        {
            cout << "File " << fileName << " not found in the current directory." << endl;
            return;
        }
    }
    else
    {
        cout << "Invalid command." << endl;
    }
}

int main()
{
    displayAuthorInformation();
    cout << endl;
    Directory root("V", "owner", false);

    Directory* dir1 = new Directory("University", "owner", false, &root);
    Directory* dir2 = new Directory("WebDevelopment", "owner", false, &root);

    root.addDirectory(dir1);
    root.addDirectory(dir2);

    Directory* subDir1 = new Directory("DSA", "owner", false, dir1);
    File* subFile1 = new File("LinkedList.txt", "owner", false, false, "txt", "", subDir1);
    dir1->addDirectory(subDir1);
    subDir1->children.push_back(subFile1);

    Directory* subDir4 = new Directory("Database", "owner", false, dir1);
    File* subFile4 = new File("Joins.txt", "owner", false, true, "txt", "", subDir4);
    dir1->addDirectory(subDir4);
    subDir4->children.push_back(subFile4);

    Directory* subDir2 = new Directory("HTML", "owner", false, dir2);
    File* subFile2 = new File("Tags.txt", "owner", false, false, "txt", "", subDir2);
    dir2->addDirectory(subDir2);
    subDir2->children.push_back(subFile2);

    Directory* subDir3 = new Directory("JavaScript", "owner", false, dir2);
    File* subFile3 = new File("DOM.txt", "owner", false, false, "txt", "", subDir3);
    dir2->addDirectory(subDir3);
    subDir3->children.push_back(subFile3);

    Directory* currentDir = &root;

    string command;
    while (true) 
    {
        cout << currentDir->getPath() << "> ";
        getline(cin, command);
        processCommands(root, currentDir, command);
        cout << endl;
    }

    return 0;
}
