#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

// const to maintain code and system crash
const int maxBuilding = 6;
const int maxRoom = 12;
const int maxUser = 10000;
const int maxReservation = 100000;

//for user data arrays
string userID[maxUser];
string userName[maxUser];
string userType[maxUser];
string userProg[maxUser];
int userCount = 0;

//for reservation data arrays
string resBldg[maxReservation];
string resRoom[maxReservation];
string resDate[maxReservation];
string resTime[maxReservation];
string resUID[maxReservation];
string resUName[maxReservation];
string resUProg[maxReservation];
string resStatus[maxReservation];
int reservationCount = 0;

//for current user
string currentUserID = "";
string currentUName = "";
string currentUType = "";
string currentUProgram = "";

//6 building data
const string buildings[maxBuilding] = {
    "ICT Laboratory",
    "Health and Sciences Building",
    "Engineering and Architecture Building",
    "Nantes Building",
    "Public Administration and Education Building",
    "Hospitality Management Laboratory"
};
// room numbers 2d array
const string roomNumbers[maxBuilding][maxRoom] = {
    {"LQ-103 |Lab 1", "LQ-104 |Lab 2", "LQ-207 |Lab 3",
     "","","","","","","","",""},

    {"LQ-105 |FOOD LAB","LQ-106","LQ-107","LQ-108","LQ-203",
     "LQ-204","LQ-205 |PHYS LAB","LQ-206 |CHEM LAB","","","",""},

    {"LQ-109","LQ-110","","",
     "LQ-113","LQ-114","LQ-115","LQ-116","LQ-208 |DRAFT LAB",
     "LQ-209 |CEA FUNC. RM.","LQ-210","LQ-211"},

    {"LQ-120","LQ-121","LQ-122","LQ-216 |SPEECH LAB",
     "LQ-217 |KEY. LAB","LQ-218 |SIMULATION RM./AVR","","","","","",""},

    {"LQ-117","LQ-118","LQ-119 |EDTECH","LQ-212",
     "LQ-213","LQ-214","LQ-215","","","","","",},

    {"LQ-100 |KITCHEN LAB","LQ-101 |BEVERAGE LAB",
     "LQ-102 |TISSUE LAB","","","","","","","","",""}

};

//function declaration returning = 7, nonr = 13
void printHeader();
void printWelcome();
void showSystemInfo();
void loadData();
void saveData();
bool validateIDformat(string id, string type);
void registerUser();
bool loginUser();
void mainMenu();
void iSKEDYUL();
bool displayBuildingRooms(int &buildingChoice, int &roomChoice,
                         string &selectedBldg, string &selectedRoom);
int convertToMinutes(string time); //helper function to convert time to minute
bool checkConflict(string buildings, string roomNumbers, string date, string time);
bool isValidDate(string date);
bool ValidTime(string time);
void viewRoomStatus();
void viewAllBuildings();
void viewOneBuilding();
void cancelReservation();
void exitProgram();

int main(){
    bool running = true;

    while (running){
        system("cls");
        printWelcome();
        int choice;

        cout << endl
             << "   > ACCESS PORTAL:\n\n"
             << "       [1] Register\n"
             << "       [2] Login\n"
             << "       [3] Exit\n\n"
             << "-----------------------------\n"
             << "   Select option: ";
        cin >> choice;
        cin.ignore();

        switch (choice){
            case 1:
                loadData();
                registerUser();
                saveData();
            break;

            case 2:
                loadData();
                if (loginUser()){
                    mainMenu();
                }
            break;

            case 3:
                system("cls");
                saveData();
                running = false;
                currentUserID = "";
                currentUType = "";
                currentUName = "";
            cout << "\n[!] Successfully logged out.\n\n"
                 << "Thank you for using iSKEDYUL! <3\n\n";
            break;

            default:
                cout << "\n[X] Invalid choice!\n\n"
                     << "[!] Please choose a valid number [1-3].\n\n"
                     << "------------------------------------------\n"
                     << "> Press Enter to continue...";
                cin.ignore();
                cin.get();
        }
    }

    return 0;
}

void printHeader(){
    cout << "================================================================\n"
         << "                         iSKEDYUL v1.0\n"
         << "             PUP Lopez Classroom Reservation System\n"
         << "================================================================\n";
}

void printWelcome(){
    cout << "                     WELCOME TO iSKEDYUL v1.0\n"
         << "              PUP Lopez Classroom Reservation System\n"
         << "================================================================\n";
}

// load function - every open ng system; data is not empty
void loadData(){
    ifstream uFile("iSKEV1users.txt"); //for reading file
    ifstream resFile("iSKEV1reservations.txt");

    //load users
    if (uFile.is_open()){
        userCount = 0;
        while (getline(uFile, userID[userCount], '|')){
               getline(uFile, userName[userCount], '|');
               getline(uFile, userType[userCount], '|');
               getline(uFile, userProg[userCount]);
        userCount++;
        if (userCount >= maxUser)
            break;
        }
    uFile.close(); //when done
    }

    //load reservations
    if (resFile.is_open()){
        reservationCount = 0;
            while (getline(resFile, resBldg[reservationCount], '|')){
                   getline(resFile, resRoom[reservationCount], '|');
                   getline(resFile, resDate[reservationCount], '|');
                   getline(resFile, resTime[reservationCount], '|');
                   getline(resFile, resUID[reservationCount], '|');
                   getline(resFile, resUName[reservationCount], '|');
                   getline(resFile, resUProg[reservationCount], '|');
                   getline(resFile, resStatus[reservationCount]);
        reservationCount++;
            if (reservationCount >= maxReservation)
                break;
            }
        resFile.close();
    }
}

//for data to save, temporary to permanent data
void saveData(){
    ofstream uFile("iSKEV1users.txt"); //for writing file
    ofstream resfile("iSKEV1reservations.txt");

    //save users
    for (int u = 0; u < userCount; u++){
        uFile << userID[u] << "|"
              << userName[u] << "|"
              << userType[u] << "|"
              << userProg[u] << "\n";
            }
    uFile.close();

    //save reservations
    for (int r = 0; r < reservationCount; r++){
        resfile << resBldg[r] << "|"
                << resRoom[r] << "|"
                << resDate[r] << "|"
                << resTime[r] << "|"
                << resUID[r] << "|"
                << resUName[r] << "|"
                << resUProg[r] << "|"
                << resStatus[r] << "\n";
    }
    resfile.close();
}

bool validateIDformat(string id, string type){ // ID validation function
    if (type == "Faculty"){ //valid faculty ID: 5 digits
        if (id.length() != 5){
            cout << "[X] Invalid Format! Enter 5 Digits only.\n"
                 << "[!] Expected Format: XXXXX\n";
            return false;
        }

        for (int i = 0; i < 5; i++){ //char must use digit only
            if (!isdigit(id[i])){
                cout << "[X] Invalid Format!\n"
                     << "    Faculty ID must contain only digit.";
            return false;
            }
        }
    return true;

    } else if (type == "Student") { //Valid student ID Format: 20XX-XXXXX-LQ-0
        if (id.length() != 15) {
            cout << "[X] Invalid Format!\n\n"
                 << "[!] Expected Format: 20XX-XXXXX-LQ-0";
            return false;
        }

        // checking of student ID (DASHES)
        if (id[4] != '-' || id[10] != '-' || id[13] != '-'){
            cout << "[X] Invalid Format! Incorrect Dash Position.\n\n"
                 << "[!] Expected Format: 20XX-XXXXX-LQ-0";
            return false;
        }

    //ID validation per part
    bool valid = true;
        //Part 1: must start with "20"
        if (valid && (id[0] != '2' || id[1] != '0')){
            cout << "[X] Invalid Format!\n"
                 << "    ID must start with '20'.\n\n"
                 << "[!] Expected Format: 20XX-XXXXX-LQ-0";
            return false;
        }

        //Part 1.1: check positions 2-3 are digits/year ('25' for 2025)
        for (int i = 2; i <= 3; i++){
            if (!isdigit(id[i])){
                cout << "[X] Invalid Format!\n"
                     << "    Year must be in digit.\n\n"
                     << "[!] Expected Format: 20XX-XXXXX-LQ-0";
            return false;
                break;
            }
        }

        //Part 2: must be 5 digit Student ID no. (pos 5-9)
        if (valid){ //to ensure only check if previous parts were valid
            for (int i = 5; i <= 9; i++){
                if (!isdigit(id[i])){
                    cout << "[X] Invalid Format!\n"
                         << "    ID must be 5 Digit Student Number.\n\n"
                         << "[!] Expected Format: 20XX-XXXXX-LQ-0";
                return false;
                    break;
                }
            }
        }

        //Part 3: Must be LQ (pos 11-12)
        if (valid && (id[11] != 'L' || id[12] != 'Q')) {
            cout << "[X] Invalid Format!\n"
                 << "    The third part must use 'LQ'.\n\n"
                 << "[!] Expected Format: 20XX-XXXXX-LQ-0";
            return false;
            }

        //Part 4: Must end with '0', pos 14
        if (valid && id[14] != '0'){
            cout << "[X] Invalid Format!\n"
                 << "    Last part must end with '0'.\n\n"
                 << "[!] Expected Format: 20XX-XXXXX-LQ-0";
            return false;
        }
        return true;
     }
     return false;
}

void registerUser(){ //registering user function
    if (userCount >= maxUser){ // check array if full to prevent system crash
        cout << "\n[!] Maximum number of user reached!\n";
        return;
    }

    system("cls");
    printHeader();
        cout << "\n [!] iSKEDYUL >> ACCESS PORTAL >> USER REGISTRATION\n"
         << "----------------------------------------------------------------\n\n";

    string id, name, type, program;
    int typeChoice;

    cout << "Full Name: ";
        getline(cin, name);
    cout << "Year and Program (Ex: BSIT-1): ";
        getline(cin, program);

    cout << "\n> Select User Type: \n"
         << "[1] Faculty Member\n"
         << "[2] Student\n\n"
         << "----------------------\n"
         << "Enter Choice: ";
    cin >> typeChoice;
    cin.ignore();

    bool isValid = false; //to control loop

    switch (typeChoice){
        case 1:
            type = "Faculty";
            cout << "\nValid Faculty ID Format: XXXXX\n";

            do { //to validate ID
                cout << "> Enter Faculty ID: ";
                cin >> id;

                if (validateIDformat(id, "Faculty")){
                    isValid = true;
                } else {
                cout << "\n[!] Please try again.\n\n";
                  }
            } while (!isValid);

        break;

        case 2:
            type = "Student";
            cout << "\nValid Student ID Format: 20XX-XXXXX-LQ-0\n";

            do {
                cout << "> Enter Student ID: ";
                cin >> id;

                if (validateIDformat(id, "Student")){
                    isValid = true;
                } else {
                cout << "\n[!] Please try again.\n\n";
                  }
            } while (!isValid);

        break;

        default:
            cout << "\n[X] Invalid choice!\n"
                 << "    Registration cancelled.\n";
            return;
        }

    for (int i = 0; i < userCount; i++){
        if (userID[i] == id){
            cout << "\n[!] Error: ID " << id << " is already registered.\n"
                 << "   Registration cancelled.\n\n"
                 << "> Press Enter to return to menu...";
            cin.get();
            return;
        }
    }

    //save user data
    userID[userCount] = id;
    userName[userCount] = name;
    userType[userCount] = type;
    userProg[userCount] = program;
    userCount++;
    saveData();

    cout << "\n[!] Registration Successful!\n"
         << "      You can now login.\n\n"
         << "---------------------------------\n"
         << "> Press Enter to continue...";
    cin.ignore();
    cin.get();
}

bool loginUser(){
    if (userCount == 0){ //system will not accept login if no registered user.
        cout << "\n[!] No users registered yet.\n"
             << "    Please register first.\n";
        system("pause");
        return false;
    }

    system("cls");
    printHeader();
        cout << " [!] iSKEDYUL >> ACCESS PORTAL >> LOGIN\n"
             << "----------------------------------------------------------------\n";

    int userIndex = -1;
    int attempt = 0;
    string inName, inID;

    while (true){ //login
        cout << "\nEnter Name: ";
            getline(cin, inName);
        cout << "Enter ID: ";
            getline(cin, inID);

    //user search
    userIndex = -1; //reset every attempt
    for (int u = 0; u < userCount; u++){
        if (userID[u] == inID && userName[u] == inName){
            userIndex = u; //if user found
            break; //stop for search
        }
    }

    if (userIndex != -1){ //if user is found
        currentUName = userName[userIndex];
        currentUserID = userID[userIndex];
        currentUType = userType[userIndex];
        currentUProgram = userProg[userIndex];

        cout << "                      LOGIN SUCCESSFUL!\n\n";

        system("cls");
        printHeader();

        cout << "\n\nWelcome, " << currentUName << "! (" << currentUserID << ")\n"
             << "       " << currentUType << " of " << currentUProgram << endl
             << "\n--------------------------------\n"
             << "   Preparing your dashboard...\n\n"
             << "> Press Enter to access Main Menu...";
        cin.get();
        mainMenu();
        return true;

    } else { //if user is not found
        cout << "\n[X] Invalid Credentials!\n"
             << "    Press 1 to Exit or any other key to try again.\n";

        string input;
        getline(cin, input);

        if (input == "1") {
                cout << "\n[!] Login Cancelled.\n";
                system("pause");
                return false;
        }

        system("cls");
        printHeader();
            cout << " [!] iSKEDYUL >> LOGIN\n"
                 << "----------------------------------------------------------------\n";
      }
    }
}

void mainMenu(){
    int choice;
    bool logout = false;
    int userIndex = -1;

    for (int i = 0; i < userCount; i++) {
        if (userID[i] == currentUserID) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) return;

    do {
        system("cls");
        printHeader();

        cout << "[!] iSKEDYUL >> MAIN MENU\n"
             << "----------------------------------------------------------------\n\n"
             << "MAIN MENU - Logged in as: " << currentUName << " ("
             << currentUserID << ")\n"
             << "================================================================\n\n"
             << "[1] iSKEDYUL a Room\n"
             << "[2] View Room Status\n"
             << "[3] Cancel Reservation\n"
             << "[4] Logout\n\n"
             << "---------------------------------------\n"
             << "Enter Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice){
        case 1:
            loadData();
            iSKEDYUL();
            saveData();
        break;

        case 2:
            loadData();
            viewRoomStatus();
            saveData();
        break;

        case 3:
            loadData();
            cancelReservation();
            saveData();
        break;

        case 4:
            exitProgram();
            logout = true;
            currentUserID = "";
            currentUType = "";
            currentUName = "";
        break;

        default:
            cout << "\n[X] Invalid Choice!\n"
                 << "    Please try again.\n\n"
                 << "--------------------------------\n"
                 << "> Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }
    } while (!logout);
}

void iSKEDYUL(){
    system("cls");
    printHeader();

    int buildingChoice, roomChoice;
    string selectedBldg, selectedRoom;

    if (!displayBuildingRooms(buildingChoice, roomChoice, selectedBldg, selectedRoom)){
        cout << "\n[!] Reservation cancelled.\n";
        system("pause");
        return;
    }

    string date; //enter pref date
    bool validDate = false;

        do {
            cout << "Enter Date (MM/DD/YYYY): ";
            cin >> date;
            cin.ignore();

        //enter pref date
        validDate = isValidDate(date);

            if (!validDate){
                cout << "\n[X] Invalid Format!\n"
                     << "    Enter Valid Date Format.\n\n"
                     << "[!] Date Format: MM/DD/YYYY\n\n";
                }
        } while (!validDate);

     //enter pref time slot
     string time;
     bool validTime = false;

        do {
            cout << "Enter Time Slot (HH:MM AM/PM-HH:MM AM/PM): ";
            getline(cin, time);

        validTime = ValidTime(time);

            if (!validTime){
                cout << "\n[X] Invalid Format!\n"
                     << "    Enter Valid Time Slot Format.\n\n"
                     << "[!] Time Format: HH:MM AM/PM-HH:MM AM/PM\n"
                     << "    ex. 05:00 PM-07:30 PM\n\n";
                }
        } while (!validTime);

    //check room conflict or room availability
    if (checkConflict(selectedBldg, selectedRoom, date, time)){
        cout << "\n\n[!] This room is already reserved.\n\n"
             << "---------------------------------------\n"
             << "\n> Press Enter to continue...";
        cin.get();
        return;
            }

    //iSKEDYUL Summary
    cout << "\n\n"
         << "      *** iSKEDYUL CONFIRMATION ***\n"
         << "---------------------------------------\n";
    cout << "Building: " << selectedBldg << endl
         << "Room Number: " << selectedRoom << endl
         << "Date: " << date << endl
         << "Time: " << time << endl;

    //find user name
    int userIndex = -1;
    for (int u = 0; u < userCount; u++){
        if (userID[u] == currentUserID){
            userIndex = u;
            break;
        }
    }
    if (userIndex != -1){
        cout << "Reserved by: " << userName[userIndex] << endl;
    }
    cout << "---------------------------------------\n";

    char confirm;
        cout << "\n[!] Confirm Reservation? (Y/N): ";
        cin >> confirm;
        cin.ignore();

    if (confirm == 'Y' || confirm == 'y'){
        if (reservationCount < maxReservation){ //saving all reservation
            resBldg[reservationCount] = selectedBldg;
            resRoom[reservationCount] = selectedRoom;
            resDate[reservationCount] = date;
            resTime[reservationCount] = time;
            resUID[reservationCount] = currentUserID;
            resUName[reservationCount] = currentUName;
            resUProg[reservationCount] = currentUProgram;
            resStatus[reservationCount] = "Reserved";
            reservationCount++;
    saveData();

            cout << "\n> Reservation Confirmed Successfully!\n";
        } else {
            cout << "\n[!] Maximum reservations reached!\n";
            }
        } else {
            cout << "\n[X] Reservation cancelled.\n"; }
        system("pause");
}

bool displayBuildingRooms(int &buildingChoice, int &roomChoice,
                          string &selectedBldg, string &selectedRoom){
    buildingChoice = 0;
    roomChoice = 0;
    selectedBldg = "";
    selectedRoom = "";

    int userIndex = -1; //finding current user
        for (int u = 0; u < userCount; u++) {
            if (userID[u] == currentUserID) {
                userIndex = u;
                break;
            }
        }

        cout << " [!] iSKEDYUL >> MAIN MENU >> iSKEDYUL A ROOM\n"
             << "----------------------------------------------------------------\n\n"
             << "> AVAILABLE BUILDINGS:\n\n";
            for (int i = 0; i < 6; i++){
                cout << "[" << i + 1 << "] " << buildings[i] << "\n";
            }

        cout << "-------------------------------------------------------\n"
             << "   Select Building: ";
        cin >> buildingChoice;

        if (buildingChoice < 1 || buildingChoice > 6){ //validate
            cout << "\n[X] Invalid Building!\n"
                 << "    Please choose between (1-6).\n";
            return false;
                }

    selectedBldg = buildings[buildingChoice - 1]; //count array, index

    system("cls");

        cout << "----------------------------------------------------------------\n"
             << " [!] iSKEDYUL A ROOM >> " << selectedBldg << endl
             << "----------------------------------------------------------------\n\n"
             << "Choose a room:\n";

    //add [number] choice to room
    int roomCount = 0;
        for (int r = 0; r < maxRoom; r++){
            if (roomNumbers[buildingChoice - 1][r] != ""){
                roomCount++;
            }
        }
        //prints room in chosen bldg
        for (int r = 0;  r < roomCount; r++){
            cout << "[" << r + 1 << "] " << roomNumbers[buildingChoice - 1][r] << "\n";
        }

        cout << "----------------------------\n"
             << "Room Choice: ";
        cin >> roomChoice;

    //if user choice < 1, system will not continue
    if (roomChoice < 1 ||  roomChoice > roomCount) {
        cout << "\n[X] ERROR: Invalid selection!\n"
             << "    Room number " << roomChoice << "does not exist.\n\n"
             << "[!] Please choose a valid room number.\n\n"
             << "----------------------------------------\n"
             << "> Press Enter to try again...";
        cin.ignore();
        cin.get();
        return false;
    }

    //finding selected room, skipping empty room strings ""
    int counter = 0;
    for (int r = 0; r < maxRoom; r++){
        if (roomNumbers[buildingChoice - 1][r]!= ""){
            counter++;
            if (counter == roomChoice){
                selectedRoom = roomNumbers[buildingChoice - 1][r];
                break;
            }
        }
    }
    return true;
}

int convertToMinutes(string time){ //additional helper function to convert time to minute
    //find specific positions
    size_t colonPosition = time.find(':');
    size_t spacePosition = time.find(' ', colonPosition);

    //extracts parts
    int hour = stoi(time.substr(0, colonPosition));
    int minute = stoi(time.substr(colonPosition + 1, spacePosition - (colonPosition +1)));
    string ampm = time.substr(spacePosition + 1); //get if am / pm

    //convert to 24-hour format time
    if (ampm == "PM" && hour != 12) hour += 12;
    if (ampm == "AM" && hour == 12) hour = 0;

    return (hour * 60) + minute;
}

bool checkConflict(string buildings, string roomNumbers, string date, string time){
    size_t dashPosition = time.find('-'); //finds dash na nag-sseparate start and end time
    if (dashPosition == string::npos)
        return false; //check if invalid format

    //extracting characters
    string inStart = time.substr(0, dashPosition);
    string inEnd = time.substr(dashPosition + 1);

    //convert to minutes into int
    int intStart = convertToMinutes(inStart);
    int intEnd = convertToMinutes(inEnd);

    for (int i = 0; i < reservationCount; i++){ //check if same bldg,room,date,time
        if (resBldg[i] == buildings &&
            resRoom[i] == roomNumbers &&
            resDate[i] == date &&
            resStatus[i] == "Reserved"){

            //extract existing reservation time
            string existingTime = resTime[i];
            size_t existingDash = existingTime.find('-');

            string existingStartTime = existingTime.substr(0, existingDash);
            string existingEndTime = existingTime.substr(existingDash + 1);

            //conver to integer
            int existStart = convertToMinutes(existingStartTime);
            int existEnd = convertToMinutes(existingEndTime);

            //check for overlap time sched
            //logic: startA < EndB and StartB < EndA
            if (intStart < existEnd && existStart < intEnd){
                return true; //conflict found!
            }
        }
    }
        return false; //if no conflict
}

bool isValidDate(string date){ //date validation
    if (date.length()!= 10){
        cout << "\n[!] Date Format must be (MM/DD/YYYY).\n\n";
        return false;
    }

    if (date[2] != '/' || date[5] != '/'){
        cout << "\n[!] Date must use '/' as separator\n"
             << "    Date Format: (MM/DD/YYYY)\n\n";
        return false;
    }

    for (int d = 0; d < 10; d++){ //check if all ch is digits, except '/'
        if (d == 2 || d == 5) continue; //skip '/'
        if (!isdigit(date[d])){
            cout << "\n[!] Date must contain only numbers except slashes '/'.\n\n";
            return false; //if date is not validated
        }
    }

    //convert substrings to integeers
    int month = stoi(date.substr(0, 2));
    int day = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    // check year
    if (year < 2025 || year > 2029) {
        cout << "\n[!] Year must be between 2025 and 2029.\n\n";
        return false;
    }

    // check month (1-12)
    if (month < 1 || month > 12){
        cout << "\n[!] Invalid Month. Must be between 01 and 12.\n\n";
        return false;
    }

    //check days based on month
    int daysInMonth;

    if (month == 2){ //leap year checker
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap){
            daysInMonth = 29;
        } else {
            daysInMonth = 28;
          }
    }

    //if april, june, september, november (30 days)
    else if (month == 4 || month == 6 || month == 9 || month == 11){
        daysInMonth = 30;
    }

    //january, march, may, july, august, october, december (31 days)
    else {
        daysInMonth = 31;
    }

    if (day <  1 || day > daysInMonth){
        cout << "\n[!] Invalid Day for this month. Max Days: " << daysInMonth << ".\n\n";
        return false;
    }
    return true; //if date is validated
}

bool ValidTime(string time){ // check if it's not empty
    //Part 1: length checker (HH:MM XX-HH:MM XX = 17 char)
    if (time.length() != 17){
        cout << "\n[!] Invalid Length.\n"
             << "   Format must be exactly: HH:MM AM/PM-HH:MM AM/PM\n"
             << "   (Use leading zeros, e.g., 05:00 PM-07:30 PM)\n\n";
        return false;
    }

    //Part 2: colons, space, dash position checker
    if (time[2] != ':' || time[5] != ' ' || time[8] != '-'
        || time [11] != ':' || time[14] != ' '){
            cout << "\n[!] Invalid Format Structure.\n"
                 << "    Missing colons, spaces, or dash in correct places.\n"
                 << "    Format: HH:MM AM/PM-HH:MM AM/PM\n\n";
            return false;
        }

    //Part 3: digit checker
    int digitIndex[] = {0, 1, 3, 4, 9, 10, 12, 13};
    for (int digit : digitIndex){
        if (!isdigit(time[digit])){
            cout << "\n[!] Time must contain numbers for Hours and Minutes.\n\n";
            return false;
        }
    }

    //Part 4: range checker convert to integers
    int startHour = stoi(time.substr(0, 2));
    int startMin = stoi(time.substr(3, 2));
    string startAMPM = time.substr(6, 2); // AM/PM

    int endHour = stoi(time.substr(9, 2));
    int endMin = stoi(time.substr(12, 2));
    string endAMPM = time.substr(15, 2); // AM/PM

    //Part 5: check start time range
    if (startHour < 1 || startHour > 12 || startMin < 0 || startMin > 59){
        cout << "\n[!] Invalid Start Time. Hours (01-12), Minutes (00-59).\n\n";
        return false;
    }

    //check end time range
    if (endHour < 1 || endHour > 12 || endMin < 0 || endMin > 59){
        cout << "\n[!] Invalid End Time. Hours (01-12), Minutes (00-59).\n\n";
        return false;
    }

    //Part 6: check if AM/PM
    if ((startAMPM != "AM" && startAMPM != "PM") ||
        (endAMPM != "AM" && endAMPM != "PM")){
            cout << "\n[!] Invalid Suffix. Must be 'AM' or 'PM'.\n\n";
            return false;
        }
    return true;
}

void viewRoomStatus(){
    system("cls");
    printHeader();

    cout << " [!] iSKEDYUL >> MAIN MENU >> ROOM STATUS AND AVAILABILITY\n"
         << "----------------------------------------------------------------\n\n";

    int choice;
    cout << "VIEW OPTIONS: \n"
         << "[1] View All Buildings & Rooms\n"
         << "[2] View Specific Building\n"
         << "[3] Back to Main Menu\n\n"
         << "-----------------------------------\n"
         << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice){
        case 1:
        viewAllBuildings();
        break;

        case 2:
        viewOneBuilding();
        break;

        case 3:
        return;

        default:
            cout << "\n[X] Invalid Choice!\n";
            viewRoomStatus();
    }
}

void viewAllBuildings(){
    system ("cls");
    printHeader();

    cout << " [!] iSKEDYUL >> MAIN MENU >> ROOM STATUS AND AVAILABILITY\n"
         << "----------------------------------------------------------------\n\n"
         << "> View All Buildings and Rooms\n";

    //count active reservations
    int activeReservations = 0;
    for (int res = 0; res < reservationCount; res++){
        if (resStatus[res] == "Reserved"){
            activeReservations++;
        }
    }

    if (reservationCount == 0){
        cout << "\n[!] No active reservation found.\n\n";
        cout << "> All Rooms are available.\n";

    } else {
        cout << "\n> CURRENT RESERVATIONS:\n";
        cout << "============================================================================================================\n"
             << "  #  | Building            | Room          | Date          | Time Slot          | Status      | Reserved By\n"
             << "============================================================================================================\n";

        int rowNumber = 0;
        int activeCount = 0; //track active reservations in display

        //loop all bldg&room
        for (int b = 0; b < maxBuilding; b++){
            for (int r = 0; r < maxRoom; r++){
                if (roomNumbers[b][r] != ""){
                    rowNumber++;

                    string roomName = roomNumbers[b][r];
                    string buildingName = buildings[b];

            //check if room has active reservation
            bool hasReservation = false;

            string currentRoomDate = "";
            string currentRoomTime = "";
            string reservedBy = "";

            for (int res = 0; res < reservationCount; res++){
                if (resBldg[res] == buildingName &&
                    resRoom[res] == roomName &&
                    resStatus[res] == "Reserved"){ //only count reserved status
                        hasReservation = true;
                        currentRoomDate = resDate[res];
                        currentRoomTime = resTime[res];
                        reservedBy = resUName[res];
                        activeCount++;
                        break;
                }
            }
            //print table
            cout << "| " << setw(2) << rowNumber << " | "
                 << setw(20) << left << buildingName.substr(0, 20) << "| "
                 << setw(14) << left << roomName.substr(0, 14) << "| ";

            if (hasReservation){
                cout << setw(14) << left << currentRoomDate.substr(0, 14) << "| "
                     << setw(19) << left << currentRoomTime.substr(0, 19) << "| "
                     << setw(12) << left << "[RESERVED]" << "| "
                     << setw(13) << left << reservedBy.substr(0, 30) << "\n";
            } else {
                cout << setw(14) << left << "---" << "| "
                     << setw(19) << left << "---" << "| "
                     << setw(12) << left << "[AVAILABLE]" << "| "
                     << setw(13) << left << "---" << "\n";
              }

            }
          }
        }

    cout << "============================================================================================================\n\n";

    //summary
    cout << "                                     *** ROOM SUMMARY ***\n";
    cout << "           > TOTAL ROOMS: " << rowNumber << " | ACTIVE RESERVATIONS: " << activeCount << "\n\n";
     }
    cout << "> Press Enter to continue...";
    cin.get();
}

void viewOneBuilding(){
    while (true){
    system("cls");
    printHeader();

    int buildingChoice;
    //print building list
    cout << "> Select Building:\n";
    for (int b = 0; b < maxBuilding; b++){
        cout << "[" << (b+1) << "] " << buildings[b] << "\n";
    }
    cout << "[0] Back to Selecting Building\n\n";
    cout << "Enter Building number: ";
    cin >> buildingChoice;
    cin.ignore();

    if (buildingChoice == 0){
        return;
    }

    if (buildingChoice < 1 || buildingChoice > maxBuilding){
        cout << "[X] Invalid building number!\n";
        system("pause");
        continue;
    }

    int bldgIndex = buildingChoice - 1;
    string selectedBuilding = buildings[bldgIndex];

    system("cls");
    printHeader();
    cout << " [!] iSKEDYUL >> MAIN MENU >> ROOM STATUS AND AVAILABILITY\n"
         << "----------------------------------------------------------------\n\n"
         << "< " << selectedBuilding << endl;
    cout << "============================================================================================\n"
         << " #  | Room           | Date           | Time Slot           | Status     | Reserved By\n"
         << "============================================================================================\n";

    int totalRooms = 0;
    int reservedCount = 0;
    int availableCount = 0;

    // show room in specific bldg
    for (int r = 0; r < maxRoom; r++){
        if (roomNumbers[bldgIndex][r] != ""){
            totalRooms++;
        //check if reserved

        string roomFullName = roomNumbers[bldgIndex][r];

        //remove '|'
        string roomNumber = roomFullName;
        string roomDescription = "";

        // check for active reservations
        bool isReserved = false;
        string date = "---";
        string time = "---";
        string status = "[AVAILABLE]";
        string reservedBy = "";

    //loop through reservation
    for (int res = 0; res < reservationCount; res++){
        if (resBldg[res] == selectedBuilding &&
            resRoom[res] == roomFullName &&
            resStatus[res] == "Reserved"){
                isReserved = true;
                date = resDate[res];
                time = resTime[res];
                status = "[RESERVED]";
                reservedBy = resUName[res];
                reservedCount++;
                break;
        }
    }
        if (!isReserved){
            availableCount++;
        }
        //display row
        cout << "| " << setw(2) << totalRooms << "| "
             << setw(14) << left << roomFullName.substr(0, 14) << " | "
             << setw(14) << left << date.substr(0, 14) << " | "
             << setw(19) << left << time.substr(0, 19) << " | "
             << setw(10) << left << status.substr(0, 10) << " | "
             << setw(18) << left << reservedBy.substr(0, 30) << "\n";
        }
    }

    cout << "============================================================================================\n\n";

    //room summary
    cout << "                                     *** ROOM SUMMARY ***\n"
         << "       > Total Rooms: " << totalRooms << endl
         << "       > Available: " << availableCount << endl
         << "       > Reserved: " << reservedCount << "\n\n";

    int choice;
    cout << "-----------------------------------------------\n\n"
         << "[1] View another building\n"
         << "[2] Back to Main Menu\n\n"
         << "-----------------------------------------------\n\n"
         << "Enter choice: ";
    cin >> choice;
    cin.ignore();

        if (choice == 2){
        return;
        }
  }
}

void cancelReservation(){
    system("cls");
    printHeader();

    cout << " [!] iSKEDYUL >> MAIN MENU >> CANCEL ROOM RESERVATION\n"
         << "----------------------------------------------------------------\n\n";

    //check if user has any bookings
    int userBookingCount = 0;
    int userBookingIndices[maxReservation]; //store indices of users' booking

    for (int booking = 0; booking < reservationCount; booking++){
        if (resUID[booking] == currentUserID && resStatus[booking] == "Reserved"){
            userBookingIndices[userBookingCount] = booking;
            userBookingCount++;
        }
    }

    if (userBookingCount == 0){ //if no bookings found
        cout << "[!] No Room Reservations found on your account.\n\n"
             << "> Press Enter to return to Main Menu...";
        cin.get();
        return;
    }

    cout << "Your Active iSKEDYUL:\n" //print active scheds the user reserved
         << "============================================================================================\n"
         << "# | Building             | Room           | Date              | Time Slot\n"
         << "============================================================================================\n";

    for (int res = 0; res < userBookingCount; res++){
        int index = userBookingIndices[res];
        cout << (res + 1) << " | "
             << setw(20) << left << resBldg[index].substr(0, 20) << " | "
             << setw(14) << left << resRoom[index].substr(0, 14) << " | "
             << setw(17) << left << resDate[index].substr(0, 17) << " | "
             << resTime[index] << endl;
    }

    cout << "============================================================================================\n";

    //choose reservation to cancel
    int choice;
    cout << "\n> Select a reservation to cancel: ";
        cin >> choice;
        cin.ignore();

    if(choice < 1 || choice > userBookingCount){
        cout << "\n[X] Invalid selection!\n"
             << "\nPress Enter to return to Main Menu...";
        cin.get();
        return;
    }

    int selectedIndex = userBookingIndices[choice - 1];

    //print cancellation summary
    cout << "\n\n       *** CANCELLATION SUMMARY ***\n"
         << "---------------------------------------\n"
         << "Building: " << resBldg[selectedIndex] << endl
         << "Room: " << resRoom[selectedIndex] << endl
         << "Date: " << resDate[selectedIndex] << endl
         << "Time: " << resTime[selectedIndex] << endl
         << "Reserved By: " << resUName[selectedIndex] << endl
         << "Program: " << resUProg[selectedIndex] << endl
         << "---------------------------------------\n\n";

    //ask for confirmation
    char confirm;
    cout << "[!] Confirm Cancellation? (Y/N): ";
    cin >> confirm;
    cin.ignore();

    if (confirm == 'Y' || confirm == 'y'){ //mark room as cancelled
        resStatus[selectedIndex] = "Cancelled";
    saveData();

        cout << "\n\n       CANCELLATION SUCCESSFUL!\n"
             << "---------------------------------------\n"
             << "Your reservation has been cancelled.\n\n";

    } else {
        cout << "\n[!] Cancellation cancelled.\n";
    }

    cout << "---------------------------------------\n"
         << "> Press Enter to return to Main Menu...";
    cin.get();
}

void exitProgram(){
    system("cls");
    printHeader();

    cout << " [!] iSKEDYUL >> MAIN MENU >> LOGOUT\n"
         << "----------------------------------------------------------------\n\n"
         << "Thank you for using iSKEDYUL!\n";
    saveData();

    cout << "All data has been saved.\n\n"
         << "Logging out...\n"
         << "Goodbye, " << currentUName << "!\n\n"
         << "----------------------------------------------------------------\n\n"
         << "> Press Enter to exit the program...";
    cin.ignore();
}
