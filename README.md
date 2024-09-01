# Arcade Database Linked List Version

## Project Description
This C++ program manages and manipulates arcade game records using a linked list data structure. It provides functionality to add, search, edit, delete, and sort game records based on various criteria.

## Features
- Add new game records
- Search for existing records
- Edit record details (high score, initials, number of plays)
- Delete records
- Sort records by name or number of plays
- Read from and write to data files

## Files
- `main.cpp`: The main program file containing all the functions and logic
- `samplebatch.txt`: A sample batch file with commands to manipulate the records
- `db.txt`: Initial database file with game records
- `freeplay.dat`: Output file with updated game records after processing

## How to Use
1. Compile the C++ program
2. Run the executable
3. When prompted, enter the name of the database file (e.g., "db.txt")
4. Enter the name of the batch file (e.g., "samplebatch.txt")
5. The program will process the commands in the batch file and update the records accordingly
6. The updated records will be written to "freeplay.dat"

## Data Structure
The program uses a singly linked list to store game records. Each node in the list is represented by a `GameData` struct containing:
- `name`: Game name (string)
- `highScore`: Highest score for the game (string)
- `initials`: Initials of the player with the highest score (string)
- `plays`: Number of times the game has been played (string)
- `revenue`: Total revenue made from the game (string)
- `next`: Pointer to the next GameData node in the linked list

The linked list allows for efficient insertion, deletion, and traversal of records. It provides flexibility in managing a dynamic set of game records, allowing for easy addition and removal of games without the need for contiguous memory allocation.

## Database Structure
The database file (e.g., db.txt) should be structured as follows:
- Each line represents one game record
- Fields are comma-separated in the following order: Name, High Score, Initials, Plays, Revenue
- Revenue should include a dollar sign ($) prefix

Example:
```
Pac-Man, 1000000, PAC, 300, $2499.7500
Spy Hunter, 700000, SPH, 50, $12.50
```

## Batch File Commands
The batch file can contain the following commands:

1. Add Record: `1 "Game Name" HighScore Initials Plays Revenue`
   - Example: `1 "Donkey Kong" 500000 JWS 25 $006.2500`

2. Search Record: `2 SearchTerm`
   - Example: `2 Kong`

3. Edit Record: `3 "Game Name" FieldNumber NewValue`
   - FieldNumber: 1 for High Score, 2 for Initials, 3 for Plays
   - Example: `3 "Donkey Kong" 3 9999`

4. Delete Record: `4 "Game Name"`
   - Example: `4 "Pac-Man"`

5. Sort Records: `5 SortMethod`
   - SortMethod: "name" or "plays"
   - Example: `5 plays`

## Functions
- `createLinkedList`: Reads data from the file and creates the initial linked list
- `addRecord`: Adds a new record to the list
- `searchRecord`: Searches for a record by game name
- `editRecord`: Modifies an existing record
- `deleteRecord`: Removes a record from the list
- `sortRecords`: Sorts the list by name or number of plays
- `writeRecordsToFile`: Writes the updated list back to a file

## Note
This program was created as a project to demonstrate proficiency in C++ programming and data structure manipulation. It simulates a simple record management system for arcade games.
