#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>
#include <cctype>

using namespace std;

// Struct to store user's first and last name
struct users {
	string first_name;
	string last_name;
};

// Struct to hold the currently logged-in user's info
struct Current_User {
	string full_name;
	string mobile_number;
	string email;
	string username = "lily";  // Default username
	string password;
} currentUser;

// Struct to store flight booking details
struct flight {
	users fullName;
	int departFrom = 0;
	string departDate;
	int departSlot = 0;
	int returnFrom = 0;
	string returnDate;
	int returnSlot = 0;
};

// Struct to store check-in information
struct checkininfo {
	users name;
	string passportnumber;
	users contactperson;
	string contactperson_number;
};

// Struct to store payment and check-in status
struct paymentdetails {
	double flightticket = 0.0;
	string paymentstatus;
	checkininfo passenger[10];  // List of passengers
	string checkinstatus;
} paymentDetail;

const int SIZE = 100;

// Arrays to store data for check-ins, payments, users, and bookings
checkininfo Checkin[SIZE];
paymentdetails paymentd[SIZE];
Current_User User[SIZE];        // Array for registered users
flight passengers[SIZE];        // Array for flight bookings

// Predefined flight data for demonstration
flight Flight = {
	{"Lily Lee Li", "Ling"},   // Full name
	7,                         // departFrom
	"12/08/2025",              // departDate
	1,                         // departSlot
	8,                         // returnFrom
	"14/08/2025",              // returnDate
	3                          // returnSlot
};
int noTickets = 3;

// Function prototypes
void menu();
void FlightSchedule();
void registration(Current_User&, users&);
void readUser(Current_User[], int&);
int login(Current_User[], int);
void performBooking();
void readBooking(flight[], int&);
void editBooking(flight[], int);
void payment();
void readPaymentCheckIn(paymentdetails&);
void checkIn(paymentdetails&);
void printInvoice(flight[], paymentdetails);
bool returnToMainMenu();

int main() {
	int choice;
	users name;
	int userCount = 0;       // Keeps track of the total number of users
	int contRL = 0;          // Used to control registration/login loop
	string username;
	int select;              // Menu selection
	int passengerCount = noTickets; // Total number of passengers to be handled

	// Load user data from file into the User array
	readUser(User, userCount);

	do {
		cout << "   **********       *********          **********        ******    *******      " << endl;
		cout << "      ****        ******                 *****            ***       ***         " << endl;
		cout << "      ***          ****                   ***             ***     ***           " << endl;
		cout << "      ***            *****                ***             ***   ****            " << endl;
		cout << "      ***              *****              ***             **** ***              " << endl;
		cout << "   *  ***                *****        *   ***             ***    ****           " << endl;
		cout << "  *   ***               ******       *    ***             ***       ****        " << endl;
		cout << "   ******          *********          *******            ******      *******    " << endl;
		cout << endl;
		// Display registration/login options
		cout << "==============================" << endl;
		cout << "   WELCOME TO JSJK AIRLINES   " << endl;
		cout << "==============================" << endl;
		cout << " 1. Register New User" << endl;
		cout << " 2. Login" << endl;
		cout << " 3. Quit" << endl;
		cout << "------------------------------" << endl;
		cout << "Enter your choice: ";
		cin >> choice;


		if (choice == 1) {
			// Register a new user
			registration(User[userCount], name);
			userCount++;

			// Ask if user wants to proceed to login
			cout << "Do you want to continue login? (1-yes, 2-no): ";
			cin >> contRL;

			if (contRL == 2)
				return 0; // Exit program if user chooses not to continue
		}
		else if (choice == 2) {
			// Attempt login
			if (login(User, userCount)) {
				contRL = 0;  // Exit login/register loop if login is successful
				break;
			}
			else {
				cout << "Login failed. Try again.\n";
				cout << "Do you want to continue register/login? (1-yes, 2-no): ";
				cin >> contRL;

				if (contRL == 2)
					return 0; // Exit if user chooses not to continue
			}
		}
		else {
			// Exit for any other option
			return 0;
		}

		system("cls"); // Clear console screen
	} while (contRL == 1);

	// Show the flight schedule after login
	FlightSchedule();

	do {
		system("cls"); // Clear screen before showing the menu
		cout << "Username: " << currentUser.username << endl;
		menu(); // Display main menu options
		cout << "Enter your choice: ";
		cin >> select;

		system("cls"); // Clear screen after input
		cin.ignore();  // Clear newline character from buffer

		// Handle menu selections
		if (select == 1) {
			performBooking();  // Option 1: Book a flight
			if (!returnToMainMenu())
				return 0;
		}
		else if (select == 2) {
			readBooking(passengers, passengerCount); // Read booking data
			if (!returnToMainMenu())
				return 0;
			if (passengerCount > 0) {
				editBooking(passengers, passengerCount); // Option 2: Edit booking if exists
			}
			else {
				cout << "No bookings found.\n";
			}
		}
		else if (select == 3) {
			payment(); // Option 3: Make a payment
			if (!returnToMainMenu())
				return 0;
		}
		else if (select == 4) {
			checkIn(paymentDetail); // Option 4: Check-in
			if (!returnToMainMenu())
				return 0;
		}
		else if (select == 5) {
			printInvoice(passengers, paymentDetail); // Option 5: Print invoice
			if (!returnToMainMenu())
				return 0;
		}
		else if (select == 6) {
			return 0; // Exit the program
		}
		else {
			cout << "Not available" << endl; // Handle invalid input
		}

		// Prepare empty files when booking or editing is selected
		if (select == 1 || select == 2) {
			ofstream outFile(currentUser.username + "_paymentCheckIn.txt");
			ofstream outFile1(currentUser.username + "_invoice.txt");
		}

	} while (returnToMainMenu()); // Loop continues only if returnToMainMenu() = True

	return 0;
}

// Function to display main menu options for logged-in user
void menu() {
	cout << "       ****                                                                                                                              " << endl;
	cout << "       *    *                                                                                                                            " << endl;
	cout << "       *      *                                                                                                                          " << endl;
	cout << "       *        *                                                                                                                        " << endl;
	cout << "       *          *                                                                                                                      " << endl;
	cout << "   *****            *************************************************************************                                            " << endl;
	cout << " *                                                                         *********          *                                          " << endl;
	cout << " *                     ***     ***    ***    ***   ***   ***   ***        *          *           *                                       " << endl;
	cout << " *                     * *     * *    * *    * *   * *   * *   * *       **************           *                                      " << endl;
	cout << " *    ******           ***     ***    ***    ***   ***   ***   ***                               *                                       " << endl;
	cout << " *    *    *                                                                                   *                                         " << endl;
	cout << " *    *    *               ********************                                              *                                           " << endl;
	cout << "  *  *    * ***************                     *******************************************                                              " << endl;
	cout << "    *    *               *                    *                                                                                          " << endl;
	cout << "   *****                *                    *                                                                                           " << endl;
	cout << "                      *                     *                                                                                            " << endl;
	cout << "                    *                     *                                                                                              " << endl;
	cout << "                    *********************                                                                                                " << endl;
	cout << endl;
	cout << "============================================" << endl;
	cout << "          JSJK Airline Main Menu            " << endl;
	cout << "============================================" << endl;
	cout << " 1. Book Flight Ticket(s)" << endl;
	cout << " 2. Edit Booking" << endl;
	cout << " 3. Perform Payment" << endl;
	cout << " 4. Check-In Flight" << endl;
	cout << " 5. Print Invoice" << endl;
	cout << " 6. Quit" << endl;
	cout << "------------------------------------------" << endl;

}

//Function for returning to main menu
bool returnToMainMenu() {
	char decision;
	cout << "\nDo you want to return to the main menu? (y-yes, n-no): ";
	cin >> decision;
	cin.ignore(); // Clear the input buffer

	if (decision == 'y' || decision == 'Y') {
		system("cls"); // Clear the screen
		return true;   // Return true to indicate going back to the main menu
	}
	else {
		return false;  // Return false to exit or continue with the current flow
	}
}

// Display available flights and time slots
void FlightSchedule() {
	cout << "       ****                                                                                                                              " << endl;
	cout << "       *    *                                                                                                                            " << endl;
	cout << "       *      *                                                                                                                          " << endl;
	cout << "       *        *                                                                                                                        " << endl;
	cout << "       *          *                                                                                                                      " << endl;
	cout << "   *****            *************************************************************************                                            " << endl;
	cout << " *                                                                         *********          *                                          " << endl;
	cout << " *                     ***     ***    ***    ***   ***   ***   ***        *          *           *                                       " << endl;
	cout << " *                     * *     * *    * *    * *   * *   * *   * *       **************           *                                      " << endl;
	cout << " *    ******           ***     ***    ***    ***   ***   ***   ***                               *                                       " << endl;
	cout << " *    *    *                                                                                   *                                         " << endl;
	cout << " *    *    *               ********************                                              *                                           " << endl;
	cout << "  *  *    * ***************                     *******************************************                                              " << endl;
	cout << "    *    *               *                    *                                                                                          " << endl;
	cout << "   *****                *                    *                                                                                           " << endl;
	cout << "                      *                     *                                                                                            " << endl;
	cout << "                    *                     *                                                                                              " << endl;
	cout << "                    *********************                                                                                                " << endl;
	cout << endl;
	cout << "==========================================" << endl;
	cout << "             Flights Available            " << endl;
	cout << "==========================================" << endl;
	cout << " 1. KL - Penang       --> RM200" << endl;
	cout << " 2. Penang - KL       --> RM200" << endl;
	cout << " 3. KL - Johor        --> RM200" << endl;
	cout << " 4. Johor - KL        --> RM200" << endl;
	cout << " 5. KL - Singapore    --> RM250" << endl;
	cout << " 6. Singapore - KL    --> RM250" << endl;
	cout << " 7. KL - Bangkok      --> RM300" << endl;
	cout << " 8. Bangkok - KL      --> RM300" << endl;
	cout << endl;
	cout << "   **********       *********          **********        ******    *******      " << endl;
	cout << "      ****        ******                 *****            ***       ***         " << endl;
	cout << "      ***          ****                   ***             ***     ***           " << endl;
	cout << "      ***            *****                ***             ***   ****            " << endl;
	cout << "      ***              *****              ***             **** ***              " << endl;
	cout << "   *  ***                *****        *   ***             ***    ****           " << endl;
	cout << "  *   ***               ******       *    ***             ***       ****        " << endl;
	cout << "   ******          *********          *******            ******      *******    " << endl;
	cout << endl;
	cout << "\n==========================================" << endl;
	cout << "     Available Departure/Return Times     " << endl;
	cout << "==========================================" << endl;
	cout << " 1. 08:00 A.M." << endl;
	cout << " 2. 13:00 P.M." << endl;
	cout << " 3. 18:00 P.M." << endl;
	cout << " 4. 23:00 P.M." << endl;
	cout << "------------------------------------------" << endl;

}

// Validate that a string contains only alphabetic characters and spaces
bool validateString(const string& s) {
	for (const char c : s) {
		if (!isalpha(c) && !isspace(c))
			return false;
	}
	return true;
}

// User registration function
void registration(Current_User& user, users& name) {
	cout << "\nPlease Key in details for registration:\n";
	cin.ignore();

	// Get and validate first name
	while (true) {
		cout << "First name: ";
		getline(cin, name.first_name);
		if (validateString(name.first_name))
			break;
		else
			cout << "Invalid input. Use alphabetic characters and spaces only.\n";
	}

	// Get and validate last name
	while (true) {
		cout << "Last name: ";
		getline(cin, name.last_name);
		if (validateString(name.last_name))
			break;
		else
			cout << "Error: Last name must contain only letters and spaces.\n";
	}

	// Get and validate mobile number (only digits, length 10 or 11)
	while (true) {
		cout << "Mobile No: ";
		getline(cin, user.mobile_number);
		bool is_valid = true;
		for (int i = 0; i < user.mobile_number.length(); ++i) {
			if (!isdigit(user.mobile_number[i])) {
				is_valid = false;
				break;
			}
		}
		if (is_valid && (user.mobile_number.length() == 10 || user.mobile_number.length() == 11))
			break;
		else
			cout << "Error: Mobile number must contain only digits and be 10 or 11 digits long. Please try again.\n";
	}

	// Get and validate email (must contain '@')
	while (true) {
		cout << "Email: ";
		getline(cin, user.email);

		bool contain_email_symbol = false;
		for (int i = 0; i < user.email.length(); ++i) {
			if (user.email[i] == '@') {
				contain_email_symbol = true;
				break;
			}
		}

		if (contain_email_symbol)
			break;
		else {
			cout << "Invalid email (missing @).\n";
		}
	}

	// Get username
	cout << "Username: ";
	getline(cin, user.username);

	// Get and validate password (min 8 chars, 1 symbol, 1 uppercase, 1 number)
	while (true) {
		cout << "Password (At least 8 characters (1 symbol, 1 uppercase letter, 1 number)): ";
		getline(cin, user.password);
		bool password_contain_upper = false, password_contain_symbol = false, password_contain_digit = false;
		for (int i = 0; i < user.password.length(); i++) {
			char pwd = user.password[i];
			if (isupper(pwd)) password_contain_upper = true;
			if (ispunct(pwd)) password_contain_symbol = true;
			if (isdigit(pwd)) password_contain_digit = true;
		}
		if (user.password.length() >= 8 && password_contain_upper && password_contain_symbol && password_contain_digit)
			break;
		else
			cout << "Password must be at least 8 characters long, include 1 uppercase letter, 1 symbol, and 1 number.\n";
	}

	// Confirm registration
	char confirm;
	while (true) {
		cout << "Confirm to register? (y-yes, n-no): ";
		cin >> confirm;
		cin.ignore();
		confirm = tolower(confirm);
		if (confirm == 'y' || confirm == 'n')
			break;
		cout << "Invalid input. Please enter 'y' or 'n'.\n";
	}

	// Cancel registration if user chooses 'n'
	if (confirm == 'n') {
		cout << "Registration cancelled.\n";
		return;
	}

	// Combine full name and show registration info
	user.full_name = name.first_name + " " + name.last_name;
	cout << "User registered successfully!\n"
		<< "Full Name: " << user.full_name << endl
		<< "Mobile: " << user.mobile_number << endl
		<< "Email: " << user.email << endl;

	// Save registration data to file
	ofstream datafile("user.txt", ios::app);
	if (datafile.is_open()) {
		datafile << name.first_name << endl
			<< name.last_name << endl
			<< user.mobile_number << endl
			<< user.email << endl
			<< user.username << endl
			<< user.password << endl;
		datafile.close();
	}
}

// Load registered users from file
void readUser(Current_User Users[], int& count) {
	ifstream datafile;
	datafile.open("user.txt");
	string first_name, last_name;

	// Read user data line-by-line from the file
	while (getline(datafile, first_name) &&
		getline(datafile, last_name) &&
		getline(datafile, Users[count].mobile_number) &&
		getline(datafile, Users[count].email) &&
		getline(datafile, Users[count].username) &&
		getline(datafile, Users[count].password)) {
		Users[count].full_name = first_name + " " + last_name;
		count++;
	}

	datafile.close();
}

// Handle user login
int login(Current_User users[], int count) {
	string username_insert, password_insert;
	cin.ignore();

	// Ask for username
	cout << "Username: ";
	getline(cin, username_insert);

	// Search for username
	for (int i = 0; i < count; ++i) {
		if (username_insert == users[i].username) {
			// Ask for password if username matches
			cout << "Password: ";
			getline(cin, password_insert);

			// Check password
			if (password_insert == users[i].password) {
				currentUser = users[i];
				cout << "Login Successfully!\n";
				return 1;
			}
			else {
				cout << "Username or Password not correct!\n";
				cout << "If you have not registered, please register first!\n";
				return 0;
			}
		}
	}

	// If username not found
	cout << "Username or Password not correct!\n";
	cout << "If you have not registered, please register first!\n";
	return 0;
}

// Function to perform flight booking for passengers
void performBooking() {
	int passengerNum;
	flight booking[10]; // Array to store booking details for up to 10 passengers
	FlightSchedule(); // Display available flights and time slots

	string filename;
	filename = currentUser.username; // Use current user's username as the filename
	ofstream datafile(filename + "_Booking.txt"); // Open booking file for writing

	do {
		cout << "Number of passenger(s): ";
		cin >> passengerNum;
		cout << "Please provide the details as below: \n\n";

		// If number of passengers is 10 or fewer
		if (passengerNum <= 10) {
			for (int i = 0; i < passengerNum; i++) {
				booking[i] = Flight; // Copy base flight info
				cout << "Passenger " << i + 1 << endl;

				// For the first passenger, ask all details
				if (i == 0) {
					cin.ignore();
					cout << "First Name: ";
					getline(cin, booking[i].fullName.first_name);
					cout << "Last Name: ";
					getline(cin, booking[i].fullName.last_name);
					cout << "Departure Flight: ";
					cin >> booking[i].departFrom;
					cout << "Date of Departure (DD/MM/YYYY): ";
					cin.ignore();
					getline(cin, booking[i].departDate);
					cout << "Slot of Departure: ";
					cin >> booking[i].departSlot;
					cout << "Return Flight: ";
					cin >> booking[i].returnFrom;
					cin.ignore();
					cout << "Date of Return (DD/MM/YYYY): ";
					getline(cin, booking[i].returnDate);
					cout << "Slot of return: ";
					cin >> booking[i].returnSlot;
				}
				// For second passenger, display already input details
				else if (i == 1) {
					cin.ignore();
					cout << "First Name: ";
					getline(cin, booking[i].fullName.first_name);
					cout << "Last Name: ";
					getline(cin, booking[i].fullName.last_name);
					cout << "Departure Flight: " << booking[0].departFrom << endl;
					cout << "Date of Departure (DD/MM/YYYY): " << booking[1].departDate << endl;
					cout << "Slot of Departure: " << booking[0].departSlot << endl;
					cout << "Return Flight: " << booking[0].returnFrom << endl;
					cout << "Date of Return (DD/MM/YYYY): " << booking[0].returnDate << endl;
					cout << "Slot of return: " << booking[0].returnSlot << endl;
				}
				// For third passenger onwards, same as second (display flight info)
				else {
					cout << "First Name: ";
					getline(cin, booking[i].fullName.first_name);
					cout << "Last Name: ";
					getline(cin, booking[i].fullName.last_name);
					cout << "Departure Flight: " << booking[0].departFrom << endl;
					cout << "Date of Departure (DD/MM/YYYY): " << booking[0].departDate << endl;
					cout << "Slot of Departure: " << booking[0].departSlot << endl;
					cout << "Return Flight: " << booking[0].returnFrom << endl;
					cout << "Date of Return (DD/MM/YYYY): " << booking[0].returnDate << endl;
					cout << "Slot of return: " << booking[0].returnSlot << endl;
				}

				// Save booking details to file
				if (datafile.is_open()) {
					datafile << booking[i].fullName.first_name << endl
						<< booking[i].fullName.last_name << endl
						<< booking[0].departFrom << endl
						<< booking[0].departDate << endl
						<< booking[0].departSlot << endl
						<< booking[0].returnFrom << endl
						<< booking[0].returnDate << endl
						<< booking[0].returnSlot << endl;
				}
			}
			datafile.close(); // Close booking file
			noTickets = passengerNum; // Set number of passengers from global noTickets
		}
		else
			cout << "The maximum number of the passenger is 10. Please try again.";
	} while (passengerNum > 10); // Repeat if over limit
	cin.get();
}

// Function to read booking data from file into array
void readBooking(flight bookings[], int& bookingCount) {
	ifstream inFile(currentUser.username + "_Booking.txt"); // Open booking file

	for (int i = 0; i < bookingCount; i++) {
		if (inFile.is_open()) {
			Flight = bookings[i]; // Copy base flight info
			getline(inFile, bookings[i].fullName.first_name);
			getline(inFile, bookings[i].fullName.last_name);
			inFile >> bookings[i].departFrom;
			inFile.ignore();
			getline(inFile, bookings[i].departDate);
			inFile >> bookings[i].departSlot;
			inFile >> bookings[i].returnFrom;
			inFile.ignore();
			getline(inFile, bookings[i].returnDate);
			inFile >> bookings[i].returnSlot;
			inFile.ignore();
		}
	}
	inFile.close(); // Close file after reading
}

// Function to edit booking details
void editBooking(flight passengers[], int bookingCount) {
	char yesNo;
	char confirm = 'n';
	int choice;

	cout << "Change passenger name? (y-yes, n-no): ";
	cin >> yesNo;
	cin.ignore();

	// If user wants to change a passenger's name
	if (yesNo == 'y' || yesNo == 'Y') {
		for (int i = 0; i < bookingCount; i++) {
			cout << i + 1 << ". First Name: " << passengers[i].fullName.first_name
				<< " Last Name: " << passengers[i].fullName.last_name << endl;
		}

		int passengerNum;
		cout << "Select passenger to edit: ";
		cin >> passengerNum;
		cin.ignore();

		// Change the selected passenger's name
		if (passengerNum > 0 && passengerNum <= bookingCount) {
			cout << "Enter new First Name: ";
			getline(cin, passengers[passengerNum - 1].fullName.first_name);
			cout << "Enter new Last Name: ";
			getline(cin, passengers[passengerNum - 1].fullName.last_name);
		}
	}

	// Menu to select which booking detail to edit
	cout << endl;
	cout << "   **********       *********          **********        ******    *******      " << endl;
	cout << "      ****        ******                 *****            ***       ***         " << endl;
	cout << "      ***          ****                   ***             ***     ***           " << endl;
	cout << "      ***            *****                ***             ***   ****            " << endl;
	cout << "      ***              *****              ***             **** ***              " << endl;
	cout << "   *  ***                *****        *   ***             ***    ****           " << endl;
	cout << "  *   ***               ******       *    ***             ***       ****        " << endl;
	cout << "   ******          *********          *******            ******      *******    " << endl;
	cout << endl;
	cout << "\nPlease select the item to amend:\n";
	cout << "1. Departure Flight\n";
	cout << "2. Departure Date\n";
	cout << "3. Departure Slot\n";
	cout << "4. Return Flight\n";
	cout << "5. Return Date\n";
	cout << "6. Return Slot\n";
	cout << "Select: ";
	cin >> choice;

	// Based on user choice, update booking info
	if (choice == 1) {
		cout << "Old departure flight: " << passengers[0].departFrom << endl;
		cout << "New departure flight (1-8): ";
		int newDepartFrom;
		cin >> newDepartFrom;
		for (int i = 0; i < bookingCount; i++) {
			passengers[i].departFrom = newDepartFrom;
		}
		system("Pause");
	}

	else if (choice == 2) {
		cout << "Old departure date: " << passengers[0].departDate << endl;
		cout << "New departure date (DD/MM/YYYY): ";
		string newDepartDate;
		getline(cin, newDepartDate);
		for (int i = 0; i < bookingCount; i++) {
			passengers[i].departDate = newDepartDate;
		}
		system("Pause");
	}

	else if (choice == 3) {
		cout << "Old departure flight: " << passengers[0].departSlot << endl;
		cout << "New departure flight (1-8): ";
		int newDepartSlot;
		cin >> newDepartSlot;
		for (int i = 0; i < bookingCount; i++) {
			passengers[i].departSlot = newDepartSlot;
		}
		system("Pause");
	}

	else if (choice == 4) {
		cout << "Old departure flight: " << passengers[0].returnFrom << endl;
		cout << "New departure flight (1-8): ";
		int newReturnFrom;
		cin >> newReturnFrom;
		for (int i = 0; i < bookingCount; i++) {
			passengers[i].returnFrom = newReturnFrom;
		}
		system("Pause");
	}

	else if (choice == 5) {
		cout << "Old departure date: " << passengers[0].departDate << endl;
		cout << "New departure date (DD/MM/YYYY): ";
		string newReturnDate;
		getline(cin, newReturnDate);
		for (int i = 0; i < bookingCount; i++) {
			passengers[i].returnDate = newReturnDate;
		}
		system("Pause");
	}

	else if (choice == 6) {
		cout << "Old departure flight: " << passengers[0].returnSlot << endl;
		cout << "New departure flight (1-8): ";
		int newReturnSlot;
		cin >> newReturnSlot;
		for (int i = 0; i < bookingCount; i++) {
			passengers[i].returnSlot = newReturnSlot;
		}
		system("Pause");
	}
	else {
		cout << "Invalid choice.\n";
		return;
	}

	// Save updated booking data to file
	ofstream outFile(currentUser.username + "_Booking.txt");
	if (outFile.is_open()) {
		for (int i = 0; i < bookingCount; i++) {
			outFile << passengers[i].fullName.first_name << endl;
			outFile << passengers[i].fullName.last_name << endl;
			outFile << passengers[i].departFrom << endl;
			outFile << passengers[i].departDate << endl;
			outFile << passengers[i].departSlot << endl;
			outFile << passengers[i].returnFrom << endl;
			outFile << passengers[i].returnDate << endl;
			outFile << passengers[i].returnSlot << endl;
		}
		outFile.close();
		cout << "Booking updated successfully!" << endl;
	}
	else {
		cout << "Error updating booking data." << endl;
	}

	system("pause"); // Pause to allow user to view result
	cin.get();
}

void payment() {
	int NoTickets;
	NoTickets = noTickets;// Number of tickets to process payment for with global value
	char proceed, confirm;
	ofstream outfile;
	string cardholderName;
	string cardNumber;
	int method;
	string date;
	string securityCode;
	string bankaccholderName;
	string bankaccNumber;
	string departTime, returnTime;

	// Price list for each route
	double priceList[8] = { 200.00,200.00,200.00,200.00,250.00,250.00,300.00,300.00 };
	// Flight route names
	string locations[8] = { "KL - Penang","Penang - KL","KL - Johor","Johor - KL","KL - Singapore","Singapore - KL","KL - Bangkok","Bangkok - KL" };
	// Departure/Return time slots
	string timeSlot[4] = { "8.00A.M.","13.00P.PM","18.00P.M.","23.00P.M." };
	double totalPrice = 0.00;

	// Load booking information from file
	ifstream checkfile(currentUser.username + "_Booking.txt");
	if (checkfile.is_open()) {
		for (int i = 0; i < noTickets; i++) {
			getline(checkfile, Flight.fullName.first_name);
			getline(checkfile, Flight.fullName.last_name);
			checkfile >> Flight.departFrom;
			checkfile.ignore();
			getline(checkfile, Flight.departDate);
			checkfile >> Flight.departSlot;
			checkfile >> Flight.returnFrom;
			checkfile.ignore();
			getline(checkfile, Flight.returnDate);
			checkfile >> Flight.returnSlot;
			checkfile.ignore();
		}
		checkfile.close();
	}
	else {
		cout << "No booking available, please book your flight ticket(s)! " << endl;
	}

	// Calculate total price
	double departPrice = priceList[Flight.departFrom - 1];
	double returnPrice = priceList[Flight.returnFrom - 1];
	totalPrice = (departPrice + returnPrice) * noTickets;

	// Open file to store payment and check-in details
	outfile.open((currentUser.username + "_paymentCheckIn.txt"));

	do {
		// Display flight and pricing details
		cout << "Total Flight Ticket(s): " << NoTickets << endl;
		cout << "Departure Flight: " << locations[Flight.departFrom - 1] << ", " << Flight.departDate << ", " << timeSlot[Flight.departSlot - 1] << endl;
		cout << "Return Flight: " << locations[Flight.returnFrom - 1] << ", " << Flight.returnDate << ", " << timeSlot[Flight.returnSlot - 1] << endl;
		cout << "Total Payment: " << "(RM " << departPrice << " + RM " << returnPrice << ")" << " * " << NoTickets << " = RM " << static_cast<int>(totalPrice) << endl;

		// Ask user whether to proceed with payment
		cout << "Continue for payment? (y = yes, n = no) :";
		cin >> proceed;

		if (proceed == 'Y' || proceed == 'y') {
			cout << "Please choose your payment method: " << endl;
			cout << "1. Credit Card/Debit Card" << endl;
			cout << "2. Bank Transfer" << endl;
			cout << "Choice: ";
			cin >> method;

			// Handle credit/debit card payment
			if (method == 1) {
				cout << "Transfer Amount: RM " << static_cast<int>(totalPrice) << endl;
				cin.ignore();

				// Get and validate cardholder name
				do {
					cout << "Card Holder Name: ";
					getline(cin, cardholderName);

					if (validateString(cardholderName)) {
						break;
					}
					else {
						cout << "Invalid name. Please enter alphabetic characters and spaces only.\n";
					}
				} while (!(validateString(cardholderName)));

				// Get and validate card number
				do {
					cout << "Card Number: ";
					getline(cin, cardNumber);
					bool is_valid = true;
					if (cardNumber.length() == 10) {
						for (int i = 0; i < cardNumber.length(); ++i) {
							if (!isdigit(cardNumber[i])) {
								is_valid = false;
								break;
							}
						}
					}
					else {
						cout << "Invalid card number. Card number must contain only 10 digits." << endl;
					}
				} while (cardNumber.length() != 10);

				// Get card expiry date
				cout << "Card Valid Date (MM/YYYY): ";
				getline(cin, date);

				// Get and validate security code
				do {
					cout << "Security Code: ";
					cin >> securityCode;
					bool is_valid = true;

					for (int i = 0; i < securityCode.length(); ++i) {
						if (!isdigit(securityCode[i])) {
							is_valid = false;
							cout << "Security code must only contain 3 digits." << endl;
							cout << "Invalid security code. Please try again." << endl;
							break;
						}
					}
				} while (false);
			}
			// Handle bank transfer method
			else if (method == 2) {
				cout << "Transfer Amount: RM " << static_cast<int>(totalPrice) << endl;
				cin.ignore();

				// Get and validate bank account holder name
				do {
					cout << "Bank Account Holder Name: ";
					getline(cin, bankaccholderName);
					if (validateString(bankaccholderName)) {
						break;
					}
					else {
						cout << "Invalid name. Please enter alphabetic characters and spaces only." << endl;
					}
				} while (!validateString(bankaccholderName));

				// Get and validate bank account number
				do {
					cout << "Bank Account Number: ";
					getline(cin, bankaccNumber);
					bool is_valid = true;

					for (int i = 0; i < bankaccNumber.length(); ++i) {
						if (!isdigit(bankaccNumber[i])) {
							is_valid = false;
							break;
						}
					}

					if ((true) && (bankaccNumber.length() == 12)) {
						break;
					}
					else {
						cout << "Invalid bank account number. Bank account number must contain only 12 digits." << endl;
					}
				} while ((false) || !(bankaccNumber.length() == 12));
			}
			else {
				"Invalid option. Please enter 1 or 2 only.";
			}
		}

		// Confirm transaction
		cout << "Confirm Transaction? (y - yes, n - no): ";
		cin >> confirm;
		if (confirm == 'n' || confirm == 'N') {
			break;
			cin.get();
		}
		// Update and write payment detail
		else if (confirm == 'y' || confirm == 'Y') {
			cout << "Payment successsful!" << endl;
			paymentDetail.flightticket = totalPrice;
			paymentDetail.paymentstatus = "Paid";
			outfile << totalPrice << endl;
			outfile << "Paid" << endl;
		}
	} while (!(confirm == 'n' || confirm == 'N' || confirm == 'y' || confirm == 'Y'));


	// Placeholder passenger data
	for (int i = 0; i < NoTickets; i++) {
		outfile << "NULL" << endl; // First Name
		outfile << "NULL" << endl; // Last Name
		outfile << "NULL" << endl; // Passport Number
		outfile << "NULL" << endl; // Contact First Name
		outfile << "NULL" << endl; // Contact Last Name
		outfile << "NULL" << endl; // Contact Number
	}
	outfile << "Not Check" << endl; // Check-in status
	outfile.close();
	cin.get();
}

void readPaymentCheckIn(paymentdetails& paydetails)
{
	ifstream datafile;
	int passengerCount = 0; // Number of passengers

	// Open file containing payment and check-in details
	datafile.open(currentUser.username + "_paymentCheckIn.txt");

	// Read flight ticket price and payment status
	datafile >> paydetails.flightticket;
	paymentDetail.flightticket = paydetails.flightticket;
	datafile.ignore();
	getline(datafile, paydetails.paymentstatus);
	paymentDetail.paymentstatus = paydetails.paymentstatus;

	// If payment is marked as "Paid", load passenger check-in info
	if (paydetails.paymentstatus == "Paid" && datafile.is_open()) {
		for (int i = 0; i < passengerCount; i++) {
			(getline(datafile, paydetails.passenger[i].name.first_name) &&
				getline(datafile, paydetails.passenger[i].name.last_name) &&
				getline(datafile, paydetails.passenger[i].passportnumber) &&
				getline(datafile, paydetails.passenger[i].contactperson.first_name) &&
				getline(datafile, paydetails.passenger[i].contactperson.last_name) &&
				getline(datafile, paydetails.passenger[i].contactperson_number));
			// Copy data to global structure
			paymentDetail.passenger[i].name.first_name = paydetails.passenger[i].name.first_name;
			paymentDetail.passenger[i].name.last_name = paydetails.passenger[i].name.last_name;
			paymentDetail.passenger[i].passportnumber = paydetails.passenger[i].passportnumber;
			paymentDetail.passenger[i].contactperson.first_name = paydetails.passenger[i].contactperson.first_name;
			paymentDetail.passenger[i].contactperson.last_name = paydetails.passenger[i].contactperson.last_name;
			paymentDetail.passenger[i].contactperson_number = paydetails.passenger[i].contactperson_number;
		}

		// Read check-in status
		getline(datafile, paydetails.checkinstatus);
		paymentDetail.checkinstatus = paydetails.checkinstatus;
		paydetails = paymentDetail;
		datafile.close();
	}
	else {
		cout << "Unable to open the file." << endl;
	}
}


void checkIn(paymentdetails& details) {
	string line;
	int lineNo = 0;
	ofstream outfile;
	Current_User currentuser;
	string fname, lname;
	string passportNum;
	string contactpersonfName, contactpersonlName, contactpersonphoneNum, sameFirstName, sameLastName, sameContactNumber;
	char checkin;
	char contactPerson;
	int samePerson;

	// Open the payment and check-in status file for the current user
	ifstream checkfile(currentUser.username + "_paymentCheckIn.txt");
	if (checkfile.is_open()) {
		// Read through the file to get payment and check-in status
		while (getline(checkfile, line)) {
			lineNo++;
			if (lineNo == 2) {
				details.paymentstatus = line;
			}
			if (lineNo == 21) {
				details.checkinstatus = line;
			}
		}
		checkfile.close();

		// Display the payment status
		cout << "Paymant Status: " << details.paymentstatus << endl;

		// If already checked in, inform the user
		if (details.checkinstatus == "Checked") {
			cout << "Already checked-in!" << endl;
		}
		// If payment is completed and not checked-in, proceed with check-in
		else if (details.paymentstatus == "Paid" && details.checkinstatus != "Checked") {
			cout << "~ Proceed to check in ~" << endl;

			if (noTickets <= 10) {
				for (int i = 0; i < noTickets; i++) {
					cout << "Passenger " << (i + 1) << endl;

					// For first passenger, input all details
					if (i == 0) {
						cout << "First Name: ";
						getline(cin, details.passenger[i].name.first_name);
						cout << "Last Name: ";
						getline(cin, details.passenger[i].name.last_name);
						cout << "Passport Number: ";
						getline(cin, details.passenger[i].passportnumber);
						cout << "Contact Person First Name: ";
						getline(cin, details.passenger[i].contactperson.first_name);
						cout << "Contact Person Last Name: ";
						getline(cin, details.passenger[i].contactperson.last_name);
						cout << "Contact Person Phone Number: ";
						getline(cin, details.passenger[i].contactperson_number);
					}
					// For other passengers, ask if they have the same contact person
					else {
						cout << "First Name: ";
						getline(cin, details.passenger[i].name.first_name);
						cout << "Last Name: ";
						getline(cin, details.passenger[i].name.last_name);;
						cout << "Passport Number: ";
						getline(cin, details.passenger[i].passportnumber);

						do {
							cout << "Same contact person as previous? (y-yes, n-no): ";
							cin >> contactPerson;
							cin.ignore();

							if (contactPerson == 'y' || contactPerson == 'Y') {
								// Choose which previous passenger to copy contact details from
								cout << "Which same contact person? ";
								cin >> samePerson;
								cin.ignore();
								for (int j = 0; j < noTickets; j++) {
									if (samePerson == (j + 1)) {
										cout << "Contact Person First Name: " << details.passenger[j].contactperson.first_name << endl;
										sameFirstName = details.passenger[j].contactperson.first_name;
										cout << "Contact Person Last Name: " << details.passenger[j].contactperson.last_name << endl;
										sameLastName = details.passenger[j].contactperson.last_name;
										cout << "Contact Person Phone Number: " << details.passenger[j].contactperson_number << endl;
										sameContactNumber = details.passenger[j].contactperson_number;
									}
								}
								// Assign copied contact details
								details.passenger[i].contactperson.first_name = sameFirstName;
								details.passenger[i].contactperson.last_name = sameLastName;
								details.passenger[i].contactperson_number = sameContactNumber;
							}
							else if (contactPerson == 'n' || contactPerson == 'N') {
								// Get new contact person details
								cout << "Contact Person First Name: ";
								getline(cin, details.passenger[i].contactperson.first_name);
								cout << "Contact Person Last Name: ";
								getline(cin, details.passenger[i].contactperson.last_name);
								cout << "Contact Person Phone Number: ";
								getline(cin, details.passenger[i].contactperson_number);
							}
							else {
								cout << "Invalid contact person. Please try again!" << endl;
							}
						} while (!(contactPerson == 'n' || contactPerson == 'N' || contactPerson == 'y' || contactPerson == 'Y'));
					}
				}
			}

			// Confirm final check-in
			do {
				cout << "Confirm check in? (y-yes, n-no): ";
				cin >> checkin;

				if (checkin == 'n' || checkin == 'N') {
					system("pause");
				}
				else if (checkin == 'y' || checkin == 'Y') {
					// Write all check-in details to file
					outfile.open(currentuser.username + "_paymentCheckIn.txt", ios::out);
					if (outfile.is_open()) {
						outfile << paymentDetail.flightticket << endl;
						outfile << paymentDetail.paymentstatus << endl;

						for (int i = 0; i < noTickets; i++) {
							paymentDetail = details;
							outfile << details.passenger[i].name.first_name << endl
								<< details.passenger[i].name.last_name << endl
								<< details.passenger[i].passportnumber << endl
								<< details.passenger[i].contactperson.first_name << endl
								<< details.passenger[i].contactperson.last_name << endl
								<< details.passenger[i].contactperson_number << endl;
						}
						outfile << "Checked" << endl;
						outfile.close();

						cout << "Check in completed!" << endl;
					}
					else {
						cout << "Error: Unable to open file for writing.\n";
					}
				}
				else {
					cout << "Invalid input. Please enter only y or Y or n or N to continue.\n";
				}
			} while (!(checkin == 'n' || checkin == 'N' || checkin == 'y' || checkin == 'Y'));
		}
	}
	else {
		// If file couldn't be opened, assume no payment was made
		cout << "Payment Status: Not Paid" << endl;
		cout << " No payment for flight ticket(s) yet, please proceed to make payment before check in.Thank you!" << endl;
	}
	cin.get();
}

void printInvoice(flight passengers[], paymentdetails Paymentdetails)
{
	// Price and location/time slot info
	double priceList[8] = { 200.00,200.00,200.00,200.00,250.00,250.00,300.00,300.00 };
	string locations[8] = { "KL - Penang","Penang - KL","KL - Johor","Johor - KL","KL - Singapore","Singapore - KL","KL - Bangkok","Bangkok - KL" };
	string timeSlot[4] = { "8.00A.M.","13.00P.PM","18.00P.M.","23.00P.M." };
	int passengerCount = 0;
	ofstream outfile;

	// Read flight booking details from file
	ifstream checkfile(currentUser.username + "_Booking.txt");
	if (checkfile.is_open()) {
		for (int i = 0; i < noTickets; i++) {
			getline(checkfile, Flight.fullName.first_name);
			getline(checkfile, Flight.fullName.last_name);
			checkfile >> Flight.departFrom;
			checkfile.ignore();
			getline(checkfile, Flight.departDate);
			checkfile >> Flight.departSlot;
			checkfile >> Flight.returnFrom;
			checkfile.ignore();
			getline(checkfile, Flight.returnDate);
			checkfile >> Flight.returnSlot;
			checkfile.ignore();
		}
		checkfile.close();
	}

	// Open invoice file for writing
	outfile.open(currentUser.username + "_invoice.txt");

	if (outfile.is_open()) {
		// Write basic flight and booking info to invoice
		outfile << "Welcome to JSJK Airline Company" << endl;
		outfile << "Departure Flight: " << locations[Flight.departFrom - 1] << endl;
		outfile << "Departure Date: " << Flight.departDate << endl;
		outfile << "Departure Slot: " << timeSlot[Flight.departSlot - 1] << endl;
		outfile << "Return Flight: " << locations[Flight.returnFrom - 1] << endl;
		outfile << "Return Date: " << Flight.returnDate << endl;
		outfile << "Return Slot: " << timeSlot[Flight.returnSlot - 1] << endl << endl;

		// Load payment and passenger details
		readPaymentCheckIn(paymentDetail);

		// Write passenger and contact info to invoice
		for (int i = 0; i < passengerCount; i++) {
			Paymentdetails.passenger[i] = paymentDetail.passenger[i];
			outfile << "Name: " << Paymentdetails.passenger[i].name.first_name << " " << Paymentdetails.passenger[i].name.last_name << endl;
			outfile << "Passport Number: " << Paymentdetails.passenger[i].passportnumber << endl;
			outfile << "Contact Person: " << Paymentdetails.passenger[i].contactperson.first_name << " " << Paymentdetails.passenger[i].contactperson.last_name << endl;
			outfile << "Contact Person Mobile No: " << Paymentdetails.passenger[i].contactperson_number << endl;
		}
		outfile.close();
		cout << "Invoice printed, please check your folder." << endl;
	}
	else {
		cout << "Unable to print invoice." << endl;
	}
	cin.get();
}