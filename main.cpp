//Program to manage and manipulate arcade game records using a linked list

#include <cctype> 
#include <fstream> 
#include <iomanip> 
#include <iostream> 
#include <sstream>
#include <string> 

using namespace std;

//structure to hold different pieces of data for the game record
struct GameData
{
    string name;        //name of game
    string highScore;   //highest score for game
    string initials;    //initials of plater with highest score
    string plays;       //number of times game has been played
    string revenue;     //total revenue made from game
    GameData* next;     //pointer to next GameData node in linked list
};

//forward declarations for functions:
string toLowercase(string original);
string cutLeadingZeroes(string original);
void createLinkedList(GameData*& head, fstream& datfile);
void addRecord(GameData*& head, const string& commandLine);
void searchRecord(GameData*& head, const string& searchTerm);
void editRecord(GameData*& head, const string& commandLine);
void deleteRecord(GameData*& head, const string& recordToDelete);
void sortRecords(GameData*& head, const string& sortMethod);
void printList(ofstream& outFile, GameData* head);
void writeRecordsToFile(GameData* head, const string& filename);

int main()
{
    string database;  //variable for database filename
    string batch; //variable for batch filename

    //prompt user to enter name of database file, and store it in database variable
    cout << "Enter Database Name: ";
    cin >> database;

    //prompt the user to enter name of batch file, and store it in batch variable
    cout << "\nEnter batch file name: ";
    cin >> batch;
    cout << '\n';

    //create an input filestream object for the batch file (reading)
    ifstream batchfile(batch, ios::in);

    //check if batch file can be opened; if it cannot, print an error to the console and exit
    if (!batchfile)
    {
        cerr << "batchfile could not be opened for reading.\n";
        return 1;
    }

    //create the database file for reading, writing, and appending in binary mode
    //'ios::app' ensures that if file does not exist, it is created
    fstream datfile(database, ios::in | ios::out | ios::binary | ios::app);

    //check if data file can be opened; if it cannot, print an error to the console and exit
    if (!datfile)
    {
        cerr << "datafile could not be opened for reading.\n";
        return 1;
    }

    //close file to reset its state and prepare for reopening with different modes
    datfile.close();

    //reopen the file for reading and writing in binary mode without appending, ready for data operations
    datfile.open(database, ios::in | ios::out | ios::binary);

    //check if data file can be opened; if it cannot, print an error to the console and exit
    if (!datfile)
    {
        cerr << "datafile failed to be reopened.\n";
        return 1;
    }

    //initialize head(first node) of linked list to nullptr, meaning list is currently empty
    GameData* head = nullptr;

    //call function to fill linked list with records from database file
    createLinkedList(head, datfile);

    //create a string object to store our batch file command lines in
    string commandLine{};

    //keep reading in lines from our batch file until we get to end of file
    while (!batchfile.eof())
    {
        //while there are still lines to process, store each batchfile line in our commandLine string object
        while (getline(batchfile, commandLine))
        {
            //if first character is the number '1', call function to add record to linked list
            if (commandLine[0] == '1')
            {
                addRecord(head, commandLine);   //call add record function
            }
            //if first character is the number '2', parse our command line for search term, and call function to search for record in linked list
            if (commandLine[0] == '2')
            {
                string searchTerm{ commandLine.substr(2) };   //get portion of command line starting from position 2 (after space)   
                searchRecord(head, searchTerm);               //call search record function
            }
            //if first character is the number '3', call function to edit our record in linked list
            if (commandLine[0] == '3')
            {
                editRecord(head, commandLine);  //call edit record function
            }
            //if first character is the number '4', parse command line for record to delete, and call function to delete record from linked list
            if (commandLine[0] == '4')
            {
                string recordToDelete{ commandLine.substr(2) }; //get portion of command line starting from position 2 (after space)
                deleteRecord(head, recordToDelete);             //call delete record function
            }
            //if first character is the number '5', parse command line for sort method, and call function to sort records in linked list
            if (commandLine[0] == '5')
            {
                string sortMethod{ commandLine.substr(2) };  //get portion of command line starting from position 2 (after space)
                sortRecords(head, sortMethod); //call sort records function
            }
        }
    }

    //after processing all commands, write modified records to 'freeplay.dat' file
    writeRecordsToFile(head, "freeplay.dat");

    //close both files after all operations are completed
    batchfile.close();
    datfile.close();

    //return 0 to indicate successful execution of program
    return 0;
}

//function to convert string to lowercase
string toLowercase(string original)
{
    //loop through each character in string
    for (size_t i = 0; i < original.size(); ++i)
    {
        //convert current character to lowercase equivalent,
        //converting to 'char' type for compatibility with 'string' type
        original[i] = static_cast<char>(tolower(original[i]));
    }
    //return modified string with lowercase letters
    return original;
}

//function to remove leading zeros from string
string cutLeadingZeroes(string original) 
{
    //loop through each charactter in string
    for (size_t i{ 0 }; i < original.size(); ++i)
    {
        //if current character isn't '0'
        if (original[i] != '0')
        {
            //create new string starting from this nonzero character to end of original string
            string withoutLeadingZeros{ original.substr(i) };

            //return modified string without leading zeros
            return withoutLeadingZeros;
        }
    }
    //if whole string is '0's or empty, return only one '0' to indicate value is zero
    return "0";
}

//function to read data from database file and create linked list of GameData structures
void createLinkedList(GameData*& head, fstream& datfile)
{
    GameData* tail = nullptr; //keeps track of last node in linked list
    string datfileLine;       //string variable to store each line form database file

    //keep reading lines from database file until we get to end of file
    while (getline(datfile, datfileLine))
    {        
       //create stringstream from line so we can extract different fields of game record
        stringstream line(datfileLine);

        //create variables to store each field of game record
        string datfileName, highScore, initials, plays, revenue;

        //Extract fields: use commas to parse since each field is separated by a comma in the database file       
        getline(line, datfileName, ',');    //extract game name
       
        getline(line, highScore, ',');      //extract high score 
        highScore = highScore.substr(1);  //remove leading space so high score only contains the numbers

        getline(line, initials, ',');       //extract initials
        initials = initials.substr(1);    //remove leading space so initials only contains the letter

        getline(line, plays, ',');          //extract plays
        plays = plays.substr(1);          //remove leading space so plays only contains the numbers

        getline(line, revenue, ',');        //extract revenue
        revenue = revenue.substr(2);      //remove leading space and $ so revenue only contains numbers and decimal point
           
        //format highScore and plays to remove leading zeroes
        highScore = cutLeadingZeroes(highScore);
        plays = cutLeadingZeroes(plays);

        //create stringstream object to format revenue:
        stringstream formattedRevenue; 
        double revenueValue = stod(revenue);                             //convert revenue from string to double value
        formattedRevenue << fixed << setprecision(2) << revenueValue;    //ensure revenue is a double with 2 decimal places
        revenue = formattedRevenue.str();                                //convert back to string

        //create new GameData node with extracted data
        GameData* newGame = new GameData
        {
            datfileName,
            highScore,
            initials,
            plays,
            revenue,
            nullptr //next node pointer (currently set to nullptr since this is last node)
        };
        //if list is currently empty, this new node becomes head and tail
        if (head == nullptr) 
        {
            head = newGame; 
            tail = newGame; 
        }
        //otherwise: 
        else 
        {
            tail->next = newGame;  //append new node to end of list
            tail = newGame;        //update tail to new last node
        }
    }
}

//function to add new record to end of linked list
void addRecord(GameData*& head, const string& commandLine)
{
    //parse command line to extract game data:

    //find the first and second double quote to isolate name field
    //use .find() to get position of first double quote in command line and store in variable
    size_t doubleQuote1{ commandLine.find('\"') };
    //create variable for position of 2nd double quote(.find() searches for first double quote after doubleQuote1)
    size_t doubleQuote2{ commandLine.find('\"', doubleQuote1 + 1) };

    /*use spaces as dilimiters for our fields
      start searching at double quote instead of beginning of command line, because our name
      can include spaces and punctuation(which would mistake our in-field comma/space for delimiter) */
    size_t space1{ commandLine.find(' ', doubleQuote2) }; //get position of first space after 2nd double quote
    size_t space2{ commandLine.find(' ', space1 + 1) };   //get position of first space after space1
    size_t space3{ commandLine.find(' ', space2 + 1) };   //get position of first space after space2
    size_t space4{ commandLine.find(' ', space3 + 1) };   //get position of first space after space3

    //parse commandline string to extract fields (based on positions of quotes and spaces after quotes):

    //extract name starting from right after first double quote all the way to before second double quote
    //doubleQuote2 - (doubleQuote1 + 1) calculates the correct number of characters to extract for name
    string name{ commandLine.substr(doubleQuote1 + 1, doubleQuote2 - (doubleQuote1 + 1)) };

    //extract high score from commandline by getting portion starting after space1 all the way to before space2
    //space2 - (space1 + 1) calculates the exact number of characters to extract for the highscore
    string highScore{ commandLine.substr(space1 + 1, space2 - (space1 + 1)) };

    //extract initials by getting portion of commandline starting after space2 all the way to before space3
    //space3 - (space2 + 1) calculates the exact number of characters to extract for the initials
    string initials{ commandLine.substr(space2 + 1, space3 - (space2 + 1)) };

    //extract plays by getting portion of commandline starting after space3 all the way to before space4
    //space4 - (space3 + 1) calculates the exact number of characters to extract for plays
    string plays{ commandLine.substr(space3 + 1, space4 - (space3 + 1)) };

    //extract revenue by getting portion from after the $
    string revenue{ commandLine.substr(space4 + 2) };

    //create new GameData node with extracted information from command line
    GameData* newGame = new GameData
    {
        name,
        highScore,
        initials,
        plays,
        revenue,
        nullptr  //next node in list (nullptr since this will be last node)
    };

    //if list is empty, new node becomes head of linked list
    if (head == nullptr)
    {
        head = newGame;  //'head' is assigned address of 'newGame', making 'newGame' first node (head) of list
    }
    //otherwise, if list is not empty, find end of list and add new node there
    else
    {
        //start searching at head of list
        GameData* currentNode = head;

        //traverse list starting from head until reaching last node
        //last node is identified by its 'next' pointer being nullptr
        //'current' traverses through nodes, moving to next as long as 'current->next' is not null
        while (currentNode->next != nullptr)
        {
            currentNode = currentNode->next; //move to next node in list
        }
        //when we reach end of list(when current->next is null), append 'newGame' to list by setting current->next to 'newGame'
        //this links new node into list as last element
        currentNode->next = newGame;
    }

    //output added record's details to console
    cout << "RECORD ADDED\n" << "Name: " << name << '\n'
        << "High Score: " << highScore << '\n'
        << "Initials: " << initials << '\n'
        << "Plays: " << plays << '\n'
        << "Revenue: " << '$' << revenue << '\n' << '\n';
}

//function to search for record given a search term
void searchRecord(GameData*& head, const string& searchTerm)
{
    //create flag to track if search term is found in any of the records
    bool searchTermFound{ false };

    //start from head of linked list
    //'current' is used as pointer for traversing list
    GameData* currentNode = head;

    //loop through each node in list until end is reached (when current is nullptr)
    while (currentNode != nullptr)
    {
        //if search term is found in name(using toLowercase functions on both names for case-insensitivity)
        if (toLowercase(currentNode->name).find(toLowercase(searchTerm)) != string::npos)
        {
            searchTermFound = true; //set flag to true since we found search term

            //output found record's details to console 
            cout << currentNode->name << " FOUND\n"
                << "High Score: " << currentNode->highScore << '\n'
                << "Initials: " << currentNode->initials << '\n'
                << "Plays: " << currentNode->plays << '\n'
                << "Revenue: " << '$' << currentNode->revenue << '\n' << '\n';
        }
        //move to next node in list to continue search (by setting 'current' to next node)
        currentNode = currentNode->next;
    }

    //if search term not found after going through all database lines, print message accordingly
    if (!searchTermFound)
    {
        cout << searchTerm << " NOT FOUND\n";
    }
}

//function to edit specific record within linked list
void editRecord(GameData*& head, const string& commandLine)
{
    //extract all fields from batch file command line:
    //find positions of double quotes to extract game name
    size_t doubleQuote1{ commandLine.find('\"') };
    size_t doubleQuote2{ commandLine.find('\"', doubleQuote1 + 1) };

    //extract name to edit from batchfile command line (between the two double quotes)
    string batchfileName{ commandLine.substr(doubleQuote1 + 1, doubleQuote2 - (doubleQuote1 + 1)) };

    //create variables for each space after second double quote(to use their positions as dilimiters for our fields)
    size_t space1{ commandLine.find(' ', doubleQuote2) };
    size_t space2{ commandLine.find(' ', space1 + 1) };

    //extract field number and new value
    string fieldNumber{ commandLine.substr(space1 + 1, 1) };  //string variable indicating which field to edit
    string newValue{ commandLine.substr(space2 + 1) };        //string variable indicating new value for specified field

    //create flag to track if name to edit can be found in linked list
    bool foundNameToEdit{ false };

    //start iterating from head of list to find correct record to edit
    GameData* currentNode = head;

    //loop through each node in list until end is reached (when current is nullptr)
    while (currentNode != nullptr)
    {
        //if name from batchfile command matches current record's name
        if (batchfileName == currentNode->name)
        {
            foundNameToEdit = true; //set flag to true as we have found the record to edit

            //update correct field with new value depending on the field number:
            if (fieldNumber == "1") //if field number is 1, update high score
            {
                //update high score to new value provided in command line
                currentNode->highScore = newValue;

                //output edited record's details to console
                cout << currentNode->name << " UPDATED\n"
                    << "UPDATE TO high score - VALUE " << cutLeadingZeroes(newValue) << '\n'
                    << "Name: " << currentNode->name << '\n'
                    << "High Score: " << cutLeadingZeroes(newValue) << '\n'
                    << "Initials: " << currentNode->initials << '\n'
                    << "Plays: " << currentNode->plays << '\n'
                    << "Revenue: " << '$' << currentNode->revenue << '\n' << '\n';

                //exit loop since record has been found and updated
                break;
            }
            else if (fieldNumber == "2") //if field number is 2, update initials
            {
                //update player's initials to new value provided
                currentNode->initials = newValue;

                //output edited record's details to console
                cout << currentNode->name << " UPDATED\n"
                    << "UPDATE TO initials - VALUE " << newValue << '\n'
                    << "Name: " << currentNode->name << '\n'
                    << "High Score: " << currentNode->highScore << '\n'
                    << "Initials: " << newValue << '\n'
                    << "Plays: " << currentNode->plays << '\n'
                    << "Revenue: " << '$' << currentNode->revenue << '\n' << '\n';

                //exit loop since record has been found and updated
                break;
            }
            else if (fieldNumber == "3") //if field number is 3, update plays(and thus revenue)
            {
                //update number of plays to new value provided
                currentNode->plays = newValue;

                //since our plays changed, we need to recalculate revenue (multiply new value by 0.25)
                //recalculate revenue by multiply plays by 0.25                                    
                double newRevenue = stod(newValue) * 0.25;
                stringstream revenueStream;                               //create stringstream for formatting
                revenueStream << fixed << setprecision(2) << newRevenue;  //format to two decimal places
                currentNode->revenue = revenueStream.str();               //convert back to string and update revenue in linked list

                //output edited record's details to console
                cout << currentNode->name << " UPDATED\n"
                    << "UPDATE TO plays - VALUE " << cutLeadingZeroes(newValue) << '\n'
                    << "Name: " << currentNode->name << '\n'
                    << "High Score: " << currentNode->highScore << '\n'
                    << "Initials: " << currentNode->initials << '\n'
                    << "Plays: " << cutLeadingZeroes(newValue) << '\n'
                    << "Revenue: " << '$' << currentNode->revenue << '\n' << '\n';

                //exit loop since record has been found and updated
                break;
            }
        }
        //if current record is not one to be edited, move to next record in list
        currentNode = currentNode->next;
    }
    //after entire list has been searched, verify if edit was performed
    //if batchfile name to edit cannot be found in linked list, print message accordingly
    if (foundNameToEdit == false)
    {
        cout << "Record to edit was not found.\n";
    }
}

//function to delete a record from linked list, given a game name
void deleteRecord(GameData*& head, const string& recordToDelete)
{
    //initialize pointers to help traverse and modify list: 'prev' will point to node before current one
    GameData* previousNode = nullptr;
    //'current' starts from head of list and will be used to traverse list
    GameData* currentNode = head;

    //loop through each node in list until end is reached (when current is nullptr)
    while (currentNode != nullptr)
    {
        //if current node's name matches name of record to delete (toLowercase for case-insensitivity)
        if (toLowercase(currentNode->name) == toLowercase(recordToDelete))
        {
            //store details for output before deletion
            string name = currentNode->name;
            string highScore = currentNode->highScore;
            string initials = currentNode->initials;
            string plays = currentNode->plays;
            string revenue = currentNode->revenue;

            //if 'prev' is nullptr, record to delete is at head of list
            if (previousNode == nullptr)
            {
                //set 'head' to next node, which removes current head from list
                head = currentNode->next;
            }
            //otherwise, if 'prev' is not nullptr, record to delete is not head of list
            else
            {
                //move 'next' pointer of 'prev' node so it bypasses/skips over 'current' (by setting 'prev->next' to 'current->next')
                //this links previous node directly to node after 'current' (effectively removes 'current' from list)
                previousNode->next = currentNode->next;
            }

            //free memory allocated for current node (effectively deletes record)
            delete currentNode;

            //output deleted record's details to console
            cout << "RECORD DELETED\n"
                << "Name: " << name << '\n'
                << "High Score: " << highScore << '\n'
                << "Initials: " << initials << '\n'
                << "Plays: " << plays << '\n'
                << "Revenue: " << '$' << revenue << '\n' << '\n';

            return; //exit function after record is deleted
        }

        //if current node is not one to delete, continue to traverse list and get ready for next iteration
        //move 'prev' and 'current' forward in list ('prev' becomes 'current', and 'current' becomes next node)
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    //if we exit while loop without returning, no matching record was found
    cout << "Record to delete was not found in the database file.\n";
}

//function to sort linked list of game records in ascending order based on specified sort method
void sortRecords(GameData*& head, const string& sortMethod)
{
    int size = 0;                  //initialize counter variable to keep track of size of linked list (total number of nodes)
    GameData* tempNode = head;     //create temporary pointer variable and initialize to point at head of list

    //use temporary pointer to traverse linked list:
    while (tempNode != nullptr) //while we have not reached past last node of list
    {
        size++;             //increment size counter for each node
        tempNode = tempNode->next;  //move temp to next node in list
    }

    //if list is empty or contains only one node, no sorting is needed
    if (size < 2)
    {
        return;
    }

    //use bubble sort algorithm to sort linked list:
    bool flag = true; //initialize flag to true to enter sorting loop at least once

    //continue looping as long as swap has occured in previous pass
    while (flag)
    {
        flag = false; //reset flag to false at start of each pass to see if swap occurs

        //use pointer to a pointer to track current node and manage swaps involving head of list
        GameData** currentPointer = &head;

        //iterate over list, and reduce range by one after each full pass since last element is correctly sorted
        for (int i = 0; i < size - 1 && (*currentPointer)->next != nullptr; ++i)
        {
            //dereference currentPointer to access the current node being examined
            GameData* currentNode = *currentPointer;

            //access the next node in the list for comparison and potential swapping
            GameData* next = currentNode->next;

            //determine if the current node and the next node need to be swapped based on sorting method:
            //if sort method is by name, compare current name with name in next record; if sort method is plays, compare current plays with plays in next record
            bool swapNeeded = (sortMethod == "name" && currentNode->name > next->name) ||
                (sortMethod == "plays" && stoi(currentNode->plays) > stoi(next->plays));

            //if swap is needed based on above evaluation
            if (swapNeeded)
            {
                //perform the swap by adjusting pointers; first, link current to the node after next
                currentNode->next = next->next;

                //then, link next back to current, effectively swapping their positions in list
                next->next = currentNode;

                //update the pointer in the preceding node (or the head pointer) to point to next, completing the swap
                *currentPointer = next;

                //set the flag to true, indicating that a swap occurred, and another pass is necessary
                flag = true;

                //move the currentPointer forward to the next pair of nodes to continue sorting
                currentPointer = &next->next;
            }
            //if no swap is needed, advance currentPointer to next node in preparation for next comparison
            else
            {
                currentPointer = &currentNode->next;
            }
        }
        //decrement size after each pass through list because last element of unsorted section is now correctly placed
        --size;
    }

    //after sorting, print sorted list to console:

    //if sort method is name, print out 'name' to console, otherwise print out 'plays'
    cout << "RECORDS SORTED BY " << (sortMethod == "name" ? "name" : "plays") << '\n';

    //traverse through entire linked list of currents
    for (GameData* node = head; node != nullptr; node = node->next)
    {
        //print out game record data for each node
        cout << node->name << ", " << node->highScore << ", " << node->initials
            << ", " << node->plays << ", $" << node->revenue << '\n';
    }
    cout << '\n';
}

//recursive function to print linked list data to file
void printList(ofstream& outputFile, GameData* head) 
{
    //base case: check if current node (head) is null, indicating end of list  
    if (head == nullptr) 
    {
        return; //there is nothing more to print, so return from function
    }
    
    //if current node (head) is not nullptr, we print its data to file:
    outputFile << head->name << ", " 
               << head->highScore << ", "
               << head->initials << ", "
               << head->plays << ", $"  
               << head->revenue << '\n';
    //recursive call: move to next node in list and try to print its data
    //will continue until base case is met (when head becomes nullptr, meaning we're at end of list)
    printList(outputFile, head->next);
}

//function to write linked list to file
void writeRecordsToFile(GameData* head, const string& filename) 
{
    //create output filestream object, 'filename' parameter used to open or create file where list's data will be stored
    ofstream newDatfile(filename); 

    //check if data file can be opened; if it cannot, print an error to the console and exit
    if (!newDatfile)
    {
        cerr << "Error: datafile could not be opened for writing.\n";
        return;
    }

    //call recursive print function, which will write data of each node of linked list to 'newDatfile'
    printList(newDatfile, head); 

    //close filestream object 
    newDatfile.close();            
}
