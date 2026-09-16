#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <ctime>
#include <map>
#include <queue>
#include <limits>
#include <stack>
#include <cmath>
#include <functional>
#include <set>

#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
#else
    #include <sys/stat.h>
#endif

using namespace std;

// ============ COLOR CODES ============
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    void setColor(int color) { SetConsoleTextAttribute(hConsole, color); }
    void resetColor() { SetConsoleTextAttribute(hConsole, 7); }
    
    #define COLOR_RED FOREGROUND_RED | FOREGROUND_INTENSITY
    #define COLOR_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
    #define COLOR_YELLOW FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
    #define COLOR_BLUE FOREGROUND_BLUE | FOREGROUND_INTENSITY
    #define COLOR_MAGENTA FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    #define COLOR_CYAN FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    #define COLOR_WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#else
    #define COLOR_RED "\033[1;31m"
    #define COLOR_GREEN "\033[1;32m"
    #define COLOR_YELLOW "\033[1;33m"
    #define COLOR_BLUE "\033[1;34m"
    #define COLOR_MAGENTA "\033[1;35m"
    #define COLOR_CYAN "\033[1;36m"
    #define COLOR_WHITE "\033[1;37m"
    #define COLOR_RESET "\033[0m"
    
    void setColor(const string& color) { cout << color; }
    void resetColor() { cout << COLOR_RESET; }
#endif

// ============ DATA STRUCTURES ============
struct Hotel {
    string name;
    string category;
    double costPerNight;
    string city;
    int minStay;
    int maxStay;
};

struct FlightTicket {
    string passengerName;
    string passportNumber;
    int age;
    string gender;
    string fromCity;
    string toCity;
    string flightClass;
    double distance;
    double cost;
    string departureTime;
    string arrivalTime;
    string flightNumber;
    string bookingTime;
};

struct HotelStay {
    Hotel hotel;
    int stayDays;
    double totalCost;
    string checkInDate;
    string checkOutDate;
};

struct TripSegment {
    string fromCity;
    string toCity;
    vector<string> route;
    int distance;
    double fuelCost;
    double travelTime;
    vector<HotelStay> hotelStays;
    double segmentCost;
    string departureTime;
    string arrivalTime;
    string transportMode;
    FlightTicket flightTicket;
    double carToAirportCost;
    double carFromAirportCost;
    double carToAirportTime;
    double carFromAirportTime;
};

struct Trip {
    string username;
    vector<TripSegment> segments;
    double totalCost;
    double totalTravelTime;
    int totalStayDays;
    string status;
    string startDate;
    string endDate;
};

struct Tourist {
    string username;
    string password;
    string cnicPassport;
    int age;
    string email;
    string gender;
    string medicalIssues;
    string currentLocation;
};

struct Route {
    string destination;
    int distance;
    double cost;
    string difficulty;
    double travelTime;
};

struct City {
    string name;
    vector<Route> routes;
    vector<Hotel> hotels;
};

// ============ GLOBAL CONSTANTS AND VARIABLES ============
const string USER_FILE = "tourists.txt";
const string TRAVEL_RECORDS_DIR = "travel_records/";
const string TRIPS_DIR = "trips/";
const string FLIGHT_TICKETS_DIR = "flight_tickets/";
const double FUEL_COST_PER_KM = 5.0;
const double TRAVEL_TIME_PER_KM = 0.75 / 60.0;
const double FLIGHT_COST_ECONOMY = 50.0;
const double FLIGHT_COST_BUSINESS = 80.0;
const double FLIGHT_SPEED = 700.0;

map<string, City> carTravelGraph;
map<string, City> planeTravelGraph;
map<string, vector<Hotel>> cityHotels;
vector<string> airportCities = {"Karachi", "Lahore", "Islamabad", "Skardu", "Peshawar", "Quetta", "Faisalabad", "Multan"};

// ============ FUNCTION DECLARATIONS ============
void initializeCarTravelGraph();
void initializePlaneTravelGraph();
void initializeHotels();
void displayMainMenu();
void signUp();
bool login();
void userMenu(const string& username);
bool isUsernameExists(const string& username);
bool isValidEmail(const string& email);
bool isValidCNIC(const string& cnic);
bool isValidPassport(const string& passport);
void saveTouristToFile(const Tourist& tourist);
void viewTravelRecords(const string& username);
void updateCurrentLocation(const string& username);
void viewCurrentLocation(const string& username);
void clearInputBuffer();
bool createDirectory(const string& path);
string getCurrentDateTime();
string addDaysToDate(const string& startDate, int days);
string addHoursToDateTime(const string& startDateTime, double hours);
string addMinutesToDateTime(const string& startDateTime, int minutes);
Tourist getTouristInput();
void updateTouristInFile(const Tourist& tourist);
Tourist getTouristByUsername(const string& username);
void planNewTrip(const string& username);
void continueExistingTrip(const string& username);
void bookFlightTicket(const string& username);
void displayCities();
void displayAirportCitiesForFlight();
vector<vector<string>> findAllPaths(const string& start, const string& end, const map<string, City>& graph);
void displayRouteOptions(const vector<vector<string>>& allPaths, const string& transportMode);
void displayTripSummary(const Trip& trip);
void saveTripToFile(const Trip& trip);
void completeTrip(const string& username, Trip& trip);
void displayHeader(const string& title);
void displaySectionHeader(const string& section);
void displayHotelOptions(const string& city);
vector<HotelStay> selectHotels(const string& city, int totalStayDays);
Trip loadTrip(const string& username);
vector<Trip> loadAllTrips(const string& username);
double calculateRouteDistance(const vector<string>& route, const map<string, City>& graph);
double calculateTravelTime(int distance, const string& transportMode);
bool hasAirport(const string& city);
string findNearestAirport(const string& city);
FlightTicket createFlightTicket(const Tourist& tourist, const string& fromCity, const string& toCity, 
                               const string& flightClass, double distance, double cost, 
                               const string& departureTime, const string& arrivalTime);
void displayFlightTicket(const FlightTicket& ticket);
void saveFlightTicketToFile(const FlightTicket& ticket, const string& username);
void displayFlightBookingDashboard();
void displaySuccessMessage(const string& message);
void displayErrorMessage(const string& message);
void displayInfoMessage(const string& message);
void displayWarningMessage(const string& message);
void displayFlightRouteDetails(const string& fromCity, const string& toCity, 
                              const string& departureAirport, const string& arrivalAirport,
                              int flightDistance, double flightTime, double flightCost,
                              const string& departureTime, const string& arrivalTime);
void displayTransportOptions();
string selectTransportMode();
void displayAirportCities();
void displayFlightClassOptions(double distance);
void displayPriceBreakdown(double distance, const string& flightClass, double totalCost);

// ============ MAIN FUNCTION ============
int main() {
    displayHeader("E TOUR GUIDE - PAKISTAN TRAVEL COMPANION");
    
    initializeCarTravelGraph();
    initializePlaneTravelGraph();
    initializeHotels();
    
    if (!createDirectory(TRAVEL_RECORDS_DIR) || !createDirectory(TRIPS_DIR) || !createDirectory(FLIGHT_TICKETS_DIR)) {
        displayWarningMessage("Could not create required directories!");
    }
    
    while (true) {
        displayMainMenu();
        int choice;
        setColor(COLOR_CYAN);
        cout << "Enter your choice: ";
        resetColor();
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1: signUp(); break;
            case 2: 
                if (login()) {
                    displaySuccessMessage("Login successful!");
                } else {
                    displayErrorMessage("Login failed! Invalid credentials.");
                }
                break;
            case 3: 
                setColor(COLOR_CYAN);
                cout << "Thank you for using E Tour Guide!" << endl;
                resetColor();
                return 0;
            default:
                displayErrorMessage("Invalid choice! Please try again.");
        }
        cout << endl;
    }
    return 0;
}

// ============ COLORED UI FUNCTIONS ============
void displayHeader(const string& title) {
    setColor(COLOR_BLUE);
    cout << "\n";
    cout << "============================================================" << endl;
    cout << "                   " << title << endl;
    cout << "============================================================" << endl;
    cout << "\n";
    resetColor();
}

void displaySectionHeader(const string& section) {
    setColor(COLOR_CYAN);
    cout << "\n";
    cout << ">>> " << section << " <<<" << endl;
    cout << "------------------------------------------------------------" << endl;
    resetColor();
}

void displaySuccessMessage(const string& message) {
    setColor(COLOR_GREEN);
    cout << "✓ SUCCESS: " << message << endl;
    resetColor();
}

void displayErrorMessage(const string& message) {
    setColor(COLOR_RED);
    cout << "✗ ERROR: " << message << endl;
    resetColor();
}

void displayInfoMessage(const string& message) {
    setColor(COLOR_BLUE);
    cout << "ℹ INFO: " << message << endl;
    resetColor();
}

void displayWarningMessage(const string& message) {
    setColor(COLOR_YELLOW);
    cout << "⚠ WARNING: " << message << endl;
    resetColor();
}

void displayTransportOptions() {
    displaySectionHeader("TRANSPORTATION OPTIONS");
    setColor(COLOR_WHITE);
    cout << "1. Car Travel (Road Network)" << endl;
    cout << "2. Plane Travel (Flight Network)" << endl;
    cout << "------------------------------------------------------------" << endl;
    resetColor();
}

void displayAirportCities() {
    displaySectionHeader("AIRPORT CITIES AVAILABLE FOR FLIGHTS");
    setColor(COLOR_WHITE);
    cout << "Cities with airports: ";
    for (size_t i = 0; i < airportCities.size(); i++) {
        setColor(COLOR_YELLOW);
        cout << airportCities[i];
        resetColor();
        if (i < airportCities.size() - 1) cout << ", ";
    }
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    resetColor();
}

void displayAirportCitiesForFlight() {
    setColor(COLOR_CYAN);
    cout << "\nAVAILABLE AIRPORT CITIES:" << endl;
    cout << "------------------------------------------------------------" << endl;
    resetColor();
    int count = 0;
    for (const auto& city : airportCities) {
        setColor(COLOR_WHITE);
        cout << setw(15) << left << city;
        resetColor();
        count++;
        if (count % 4 == 0) cout << endl;
    }
    if (count % 4 != 0) cout << endl;
    setColor(COLOR_CYAN);
    cout << "------------------------------------------------------------" << endl;
    resetColor();
}

void displayFlightClassOptions(double distance) {
    displaySectionHeader("FLIGHT CLASS OPTIONS");
    
    double economyCost = distance * FLIGHT_COST_ECONOMY;
    double businessCost = distance * FLIGHT_COST_BUSINESS;
    
    setColor(COLOR_WHITE);
    cout << "Flight Distance: " << distance << " km" << endl;
    cout << endl;
    
    // Economy Class
    setColor(COLOR_GREEN);
    cout << "-----------------------------------------------------------" << endl;
    cout << "|                   ECONOMY CLASS                          |" << endl;
    cout << "-----------------------------------------------------------" << endl;
    setColor(COLOR_WHITE);
    cout << "|   Total Flight Cost: Rs. " << setw(30) << left << economyCost << endl;
    cout << "|   Features:                                              | " << endl;
    cout << "|      Standard seating                                    |" << endl;
    cout << "|      Basic amenities                                     |" << endl;
    cout << "|      Complimentary snacks                                |" << endl;
    setColor(COLOR_GREEN);
    cout << "-----------------------------------------------------------" << endl;
    cout << endl;
    
    // Business Class
    setColor(COLOR_MAGENTA);
    cout << "-----------------------------------------------------------" << endl;
    cout << "|                   BUSINESS CLASS                        |" << endl;
    cout << "-----------------------------------------------------------" << endl;
    setColor(COLOR_WHITE);
    cout << "|   Total Flight Cost: Rs. " << setw(30) << left << businessCost << endl;
    cout << "|   Features:                                             |" << endl;
    cout << "|      Premium seating with extra legroom                 |" << endl;
    cout << "|      Priority check-in and boarding                     |" << endl;
    cout << "|      Gourmet meals                                      |" << endl;
    cout << "|      Premium amenities                                  |" << endl;
    setColor(COLOR_MAGENTA);
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
    
    setColor(COLOR_CYAN);
    cout << "X-----------------------------------------------------------X" << endl;
    resetColor();
}

void displayPriceBreakdown(double distance, const string& flightClass, double totalCost) {
    displaySectionHeader("FLIGHT COST BREAKDOWN");
    
    setColor(COLOR_WHITE);
    cout << "Flight Class: " << flightClass << endl;
    cout << "Distance: " << distance << " km" << endl;
    
    setColor(COLOR_YELLOW);
    cout << "-----------------------------------------------------------" << endl;
    cout << "|                    TOTAL AMOUNT                          |" << endl;
    cout << "-----------------------------------------------------------" << endl;
    setColor(COLOR_GREEN);
    cout << "|              Rs. " << setw(38) << left << totalCost << "|" << endl;
    setColor(COLOR_YELLOW);
    cout << "X-----------------------------------------------------------X" << endl;
    resetColor();
}

string selectTransportMode() {
    int choice;
    setColor(COLOR_CYAN);
    cout << "Select transport mode (1-2): ";
    resetColor();
    cin >> choice;
    clearInputBuffer();
    
    if (choice == 1) return "Car";
    else if (choice == 2) return "Plane";
    else {
        displayErrorMessage("Invalid transport mode selection!");
        return "";
    }
}

// ============ GRAPH INITIALIZATION FUNCTIONS ============
void initializeCarTravelGraph() {
    vector<string> cities = {"Abbottabad", "Bahawalpur", "Faisalabad", "Gilgit", "Gujranwala", 
                            "Hyderabad", "Islamabad", "Jhelum", "Karachi", "Lahore", 
                            "Mansehra", "Multan", "Peshawar", "Quetta", "Rahim Yar Khan", 
                            "Skardu", "Sukkur", "Hunza"};
    
    for (const auto& city : cities) carTravelGraph[city] = City{city, {}, {}};
    
    carTravelGraph["Karachi"].routes.push_back({"Hyderabad", 165, 825, "easy", calculateTravelTime(165, "Car")});
    carTravelGraph["Hyderabad"].routes.push_back({"Karachi", 165, 825, "easy", calculateTravelTime(165, "Car")});
    carTravelGraph["Hyderabad"].routes.push_back({"Sukkur", 290, 1450, "easy", calculateTravelTime(290, "Car")});
    carTravelGraph["Sukkur"].routes.push_back({"Hyderabad", 290, 1450, "easy", calculateTravelTime(290, "Car")});
    carTravelGraph["Sukkur"].routes.push_back({"Multan", 350, 1750, "easy", calculateTravelTime(350, "Car")});
    carTravelGraph["Multan"].routes.push_back({"Sukkur", 350, 1750, "easy", calculateTravelTime(350, "Car")});
    carTravelGraph["Multan"].routes.push_back({"Lahore", 335, 1675, "easy", calculateTravelTime(335, "Car")});
    carTravelGraph["Lahore"].routes.push_back({"Multan", 335, 1675, "easy", calculateTravelTime(335, "Car")});
    carTravelGraph["Lahore"].routes.push_back({"Islamabad", 380, 1900, "easy", calculateTravelTime(380, "Car")});
    carTravelGraph["Islamabad"].routes.push_back({"Lahore", 380, 1900, "easy", calculateTravelTime(380, "Car")});
    carTravelGraph["Islamabad"].routes.push_back({"Peshawar", 185, 925, "easy", calculateTravelTime(185, "Car")});
    carTravelGraph["Peshawar"].routes.push_back({"Islamabad", 185, 925, "easy", calculateTravelTime(185, "Car")});
    carTravelGraph["Islamabad"].routes.push_back({"Abbottabad", 150, 750, "medium", calculateTravelTime(150, "Car")});
    carTravelGraph["Abbottabad"].routes.push_back({"Islamabad", 150, 750, "medium", calculateTravelTime(150, "Car")});
    carTravelGraph["Abbottabad"].routes.push_back({"Mansehra", 40, 200, "medium", calculateTravelTime(40, "Car")});
    carTravelGraph["Mansehra"].routes.push_back({"Abbottabad", 40, 200, "medium", calculateTravelTime(40, "Car")});
    carTravelGraph["Mansehra"].routes.push_back({"Gilgit", 450, 2250, "hard", calculateTravelTime(450, "Car")});
    carTravelGraph["Gilgit"].routes.push_back({"Mansehra", 450, 2250, "hard", calculateTravelTime(450, "Car")});
    carTravelGraph["Gilgit"].routes.push_back({"Skardu", 170, 850, "hard", calculateTravelTime(170, "Car")});
    carTravelGraph["Skardu"].routes.push_back({"Gilgit", 170, 850, "hard", calculateTravelTime(170, "Car")});
    carTravelGraph["Gilgit"].routes.push_back({"Hunza", 100, 500, "medium", calculateTravelTime(100, "Car")});
    carTravelGraph["Hunza"].routes.push_back({"Gilgit", 100, 500, "medium", calculateTravelTime(100, "Car")});
    carTravelGraph["Faisalabad"].routes.push_back({"Lahore", 130, 650, "easy", calculateTravelTime(130, "Car")});
    carTravelGraph["Lahore"].routes.push_back({"Faisalabad", 130, 650, "easy", calculateTravelTime(130, "Car")});
    carTravelGraph["Faisalabad"].routes.push_back({"Multan", 200, 1000, "easy", calculateTravelTime(200, "Car")});
    carTravelGraph["Multan"].routes.push_back({"Faisalabad", 200, 1000, "easy", calculateTravelTime(200, "Car")});
    carTravelGraph["Gujranwala"].routes.push_back({"Lahore", 70, 350, "easy", calculateTravelTime(70, "Car")});
    carTravelGraph["Lahore"].routes.push_back({"Gujranwala", 70, 350, "easy", calculateTravelTime(70, "Car")});
    carTravelGraph["Gujranwala"].routes.push_back({"Islamabad", 220, 1100, "easy", calculateTravelTime(220, "Car")});
    carTravelGraph["Islamabad"].routes.push_back({"Gujranwala", 220, 1100, "easy", calculateTravelTime(220, "Car")});
    carTravelGraph["Quetta"].routes.push_back({"Multan", 650, 3250, "hard", calculateTravelTime(650, "Car")});
    carTravelGraph["Multan"].routes.push_back({"Quetta", 650, 3250, "hard", calculateTravelTime(650, "Car")});
}

void initializePlaneTravelGraph() {
    for (const auto& city : airportCities) planeTravelGraph[city] = City{city, {}, {}};
    
    planeTravelGraph["Karachi"].routes.push_back({"Lahore", 1030, 0, "easy", calculateTravelTime(1030, "Plane")});
    planeTravelGraph["Karachi"].routes.push_back({"Islamabad", 1150, 0, "easy", calculateTravelTime(1150, "Plane")});
    planeTravelGraph["Karachi"].routes.push_back({"Quetta", 630, 0, "easy", calculateTravelTime(630, "Plane")});
    planeTravelGraph["Karachi"].routes.push_back({"Multan", 710, 0, "easy", calculateTravelTime(710, "Plane")});
    planeTravelGraph["Karachi"].routes.push_back({"Faisalabad", 980, 0, "easy", calculateTravelTime(980, "Plane")});
    planeTravelGraph["Karachi"].routes.push_back({"Peshawar", 1280, 0, "easy", calculateTravelTime(1280, "Plane")});
    planeTravelGraph["Karachi"].routes.push_back({"Skardu", 1400, 0, "medium", calculateTravelTime(1400, "Plane")});
    
    planeTravelGraph["Lahore"].routes.push_back({"Karachi", 1030, 0, "easy", calculateTravelTime(1030, "Plane")});
    planeTravelGraph["Lahore"].routes.push_back({"Islamabad", 280, 0, "easy", calculateTravelTime(280, "Plane")});
    planeTravelGraph["Lahore"].routes.push_back({"Peshawar", 380, 0, "easy", calculateTravelTime(380, "Plane")});
    planeTravelGraph["Lahore"].routes.push_back({"Multan", 310, 0, "easy", calculateTravelTime(310, "Plane")});
    planeTravelGraph["Lahore"].routes.push_back({"Faisalabad", 130, 0, "easy", calculateTravelTime(130, "Plane")});
    planeTravelGraph["Lahore"].routes.push_back({"Quetta", 820, 0, "easy", calculateTravelTime(820, "Plane")});
    planeTravelGraph["Lahore"].routes.push_back({"Skardu", 450, 0, "medium", calculateTravelTime(450, "Plane")});
    
    planeTravelGraph["Islamabad"].routes.push_back({"Karachi", 1150, 0, "easy", calculateTravelTime(1150, "Plane")});
    planeTravelGraph["Islamabad"].routes.push_back({"Lahore", 280, 0, "easy", calculateTravelTime(280, "Plane")});
    planeTravelGraph["Islamabad"].routes.push_back({"Peshawar", 160, 0, "easy", calculateTravelTime(160, "Plane")});
    planeTravelGraph["Islamabad"].routes.push_back({"Skardu", 320, 0, "medium", calculateTravelTime(320, "Plane")});
    planeTravelGraph["Islamabad"].routes.push_back({"Quetta", 920, 0, "easy", calculateTravelTime(920, "Plane")});
    planeTravelGraph["Islamabad"].routes.push_back({"Faisalabad", 260, 0, "easy", calculateTravelTime(260, "Plane")});
    planeTravelGraph["Islamabad"].routes.push_back({"Multan", 450, 0, "easy", calculateTravelTime(450, "Plane")});
    
    planeTravelGraph["Peshawar"].routes.push_back({"Karachi", 1280, 0, "easy", calculateTravelTime(1280, "Plane")});
    planeTravelGraph["Peshawar"].routes.push_back({"Lahore", 380, 0, "easy", calculateTravelTime(380, "Plane")});
    planeTravelGraph["Peshawar"].routes.push_back({"Islamabad", 160, 0, "easy", calculateTravelTime(160, "Plane")});
    planeTravelGraph["Peshawar"].routes.push_back({"Quetta", 1050, 0, "easy", calculateTravelTime(1050, "Plane")});
    
    planeTravelGraph["Quetta"].routes.push_back({"Karachi", 630, 0, "easy", calculateTravelTime(630, "Plane")});
    planeTravelGraph["Quetta"].routes.push_back({"Lahore", 820, 0, "easy", calculateTravelTime(820, "Plane")});
    planeTravelGraph["Quetta"].routes.push_back({"Islamabad", 920, 0, "easy", calculateTravelTime(920, "Plane")});
    planeTravelGraph["Quetta"].routes.push_back({"Peshawar", 1050, 0, "easy", calculateTravelTime(1050, "Plane")});
    
    planeTravelGraph["Multan"].routes.push_back({"Karachi", 710, 0, "easy", calculateTravelTime(710, "Plane")});
    planeTravelGraph["Multan"].routes.push_back({"Lahore", 310, 0, "easy", calculateTravelTime(310, "Plane")});
    planeTravelGraph["Multan"].routes.push_back({"Islamabad", 450, 0, "easy", calculateTravelTime(450, "Plane")});
    planeTravelGraph["Multan"].routes.push_back({"Faisalabad", 180, 0, "easy", calculateTravelTime(180, "Plane")});
    
    planeTravelGraph["Faisalabad"].routes.push_back({"Karachi", 980, 0, "easy", calculateTravelTime(980, "Plane")});
    planeTravelGraph["Faisalabad"].routes.push_back({"Lahore", 130, 0, "easy", calculateTravelTime(130, "Plane")});
    planeTravelGraph["Faisalabad"].routes.push_back({"Islamabad", 260, 0, "easy", calculateTravelTime(260, "Plane")});
    planeTravelGraph["Faisalabad"].routes.push_back({"Multan", 180, 0, "easy", calculateTravelTime(180, "Plane")});
    
    planeTravelGraph["Skardu"].routes.push_back({"Karachi", 1400, 0, "medium", calculateTravelTime(1400, "Plane")});
    planeTravelGraph["Skardu"].routes.push_back({"Lahore", 450, 0, "medium", calculateTravelTime(450, "Plane")});
    planeTravelGraph["Skardu"].routes.push_back({"Islamabad", 320, 0, "medium", calculateTravelTime(320, "Plane")});
}

void initializeHotels() {
    cityHotels["Karachi"] = {
        {"Pearl Continental", "5-star", 25000, "Karachi", 1, 30},
        {"Avari Towers", "5-star", 22000, "Karachi", 1, 30},
        {"Beach Luxury", "4-star", 15000, "Karachi", 1, 30}
    };
    
    cityHotels["Lahore"] = {
        {"Pearl Continental", "5-star", 23000, "Lahore", 1, 30},
        {"Nishat Hotel", "5-star", 26000, "Lahore", 2, 30},
        {"Avari Lahore", "4-star", 16000, "Lahore", 1, 30}
    };
    
    cityHotels["Islamabad"] = {
        {"Serena Hotel", "5-star", 30000, "Islamabad", 2, 30},
        {"Marriott Islamabad", "5-star", 28000, "Islamabad", 1, 30},
        {"Ramada Plaza", "4-star", 18000, "Islamabad", 1, 30}
    };
    
    vector<string> otherCities = {"Faisalabad", "Multan", "Peshawar", "Quetta", "Hyderabad", "Gujranwala", "Skardu", "Hunza"};
    for (const auto& city : otherCities) {
        cityHotels[city] = {
            {"Grand Hotel", "4-star", 12000, city, 1, 30},
            {"City Inn", "3-star", 8000, city, 1, 30}
        };
    }
}

// ============ MAIN MENU FUNCTIONS ============
void displayMainMenu() {
    displaySectionHeader("MAIN MENU");
    setColor(COLOR_WHITE);
    cout << "1. Sign Up" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    setColor(COLOR_CYAN);
    cout << "------------------------------------------------------------" << endl;
    resetColor();
}

void displayUserMenu() {
    displaySectionHeader("USER DASHBOARD");
    setColor(COLOR_WHITE);
    cout << "1. Plan New Trip" << endl;
    cout << "2. Continue Existing Trip" << endl;
    cout << "3. Book Flight Ticket" << endl;
    cout << "4. View Travel Records" << endl;
    cout << "5. View Current Location" << endl;
    cout << "6. Update Current Location" << endl;
    cout << "7. Logout" << endl;
    setColor(COLOR_CYAN);
    cout << "------------------------------------------------------------" << endl;
    resetColor();
}

// ============ USER MANAGEMENT FUNCTIONS ============
void signUp() {
    displayHeader("USER REGISTRATION");
    Tourist newTourist = getTouristInput();
    saveTouristToFile(newTourist);
    
    string travelFilename = TRAVEL_RECORDS_DIR + newTourist.username + "_travels.txt";
    ofstream travelFile(travelFilename);
    if (travelFile.is_open()) {
        travelFile << "TRAVEL RECORDS FOR: " << newTourist.username << endl;
        travelFile << "Created on: " << getCurrentDateTime() << endl;
        travelFile << "Current Location: " << newTourist.currentLocation << endl;
        travelFile << "============================================================" << endl;
        travelFile.close();
    }
    
    displaySuccessMessage("Registration successful! Please login with your credentials.");
}

Tourist getTouristInput() {
    Tourist tourist;
    
    displaySectionHeader("PERSONAL INFORMATION");
    while (true) {
        setColor(COLOR_CYAN);
        cout << "Enter username: ";
        resetColor();
        getline(cin, tourist.username);
        if (tourist.username.empty()) {
            displayErrorMessage("Username cannot be empty!");
            continue;
        }
        if (isUsernameExists(tourist.username)) {
            displayErrorMessage("Username already exists!");
        } else break;
    }
    
    while (true) {
        setColor(COLOR_CYAN);
        cout << "Enter password: ";
        resetColor();
        getline(cin, tourist.password);
        if (tourist.password.empty()) {
            displayErrorMessage("Password cannot be empty!");
        } else break;
    }
    
    while (true) {
        setColor(COLOR_CYAN);
        cout << "Enter CNIC (13 digits) or Passport: ";
        resetColor();
        getline(cin, tourist.cnicPassport);
        if (tourist.cnicPassport.empty()) {
            displayErrorMessage("CNIC/Passport cannot be empty!");
            continue;
        }
        if (tourist.cnicPassport.length() == 13 ? !isValidCNIC(tourist.cnicPassport) : !isValidPassport(tourist.cnicPassport)) {
            displayErrorMessage("Invalid format!");
        } else break;
    }
    
    while (true) {
        setColor(COLOR_CYAN);
        cout << "Enter age: ";
        resetColor();
        cin >> tourist.age;
        clearInputBuffer();
        if (tourist.age < 1 || tourist.age > 120) {
            displayErrorMessage("Please enter valid age (1-120)!");
        } else break;
    }
    
    while (true) {
        setColor(COLOR_CYAN);
        cout << "Enter email: ";
        resetColor();
        getline(cin, tourist.email);
        if (!isValidEmail(tourist.email)) {
            displayErrorMessage("Invalid email format!");
        } else break;
    }
    
    while (true) {
        setColor(COLOR_CYAN);
        cout << "Enter gender (Male/Female/Other): ";
        resetColor();
        getline(cin, tourist.gender);
        string genderLower = tourist.gender;
        transform(genderLower.begin(), genderLower.end(), genderLower.begin(), ::tolower);
        if (genderLower == "male" || genderLower == "female" || genderLower == "other") break;
        else displayErrorMessage("Invalid gender!");
    }
    
    setColor(COLOR_CYAN);
    cout << "Medical issues (or 'None'): ";
    resetColor();
    getline(cin, tourist.medicalIssues);
    if (tourist.medicalIssues.empty()) tourist.medicalIssues = "None";
    
    displayCities();
    setColor(COLOR_CYAN);
    cout << "Enter current location: ";
    resetColor();
    getline(cin, tourist.currentLocation);
    if (carTravelGraph.find(tourist.currentLocation) == carTravelGraph.end()) {
        tourist.currentLocation = "Not specified";
    }
    
    return tourist;
}

bool login() {
    string username, password;
    displayHeader("USER LOGIN");
    setColor(COLOR_CYAN);
    cout << "Username: ";
    resetColor();
    getline(cin, username);
    setColor(COLOR_CYAN);
    cout << "Password: ";
    resetColor();
    getline(cin, password);
    
    if (username.empty() || password.empty()) return false;
    
    ifstream file(USER_FILE);
    if (!file.is_open()) return false;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUsername, storedPassword;
        getline(ss, storedUsername, ',');
        getline(ss, storedPassword, ',');
        
        if (storedUsername == username && storedPassword == password) {
            file.close();
            userMenu(username);
            return true;
        }
    }
    file.close();
    return false;
}

void userMenu(const string& username) {
    while (true) {
        displayUserMenu();
        setColor(COLOR_CYAN);
        cout << "Enter your choice: ";
        resetColor();
        int choice;
        cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1: planNewTrip(username); break;
            case 2: continueExistingTrip(username); break;
            case 3: bookFlightTicket(username); break;
            case 4: viewTravelRecords(username); break;
            case 5: viewCurrentLocation(username); break;
            case 6: updateCurrentLocation(username); break;
            case 7: 
                setColor(COLOR_CYAN);
                cout << "Logging out..." << endl;
                resetColor();
                return;
            default: displayErrorMessage("Invalid choice!");
        }
    }
}

// ============ FLIGHT BOOKING FUNCTIONS ============
void displayFlightBookingDashboard() {
    displayHeader("FLIGHT BOOKING DASHBOARD");
    displayAirportCities();
}

void displayFlightRouteDetails(const string& fromCity, const string& toCity, 
                              const string& departureAirport, const string& arrivalAirport,
                              int flightDistance, double flightTime, double flightCost,
                              const string& departureTime, const string& arrivalTime) {
    displaySectionHeader("FLIGHT ROUTE DETAILS");
    
    setColor(COLOR_WHITE);
    cout << "From: " << fromCity;
    if (fromCity != departureAirport) {
        setColor(COLOR_YELLOW);
        cout << " (Departure Airport: " << departureAirport << ")";
    }
    setColor(COLOR_WHITE);
    cout << endl;
    
    cout << "To: " << toCity;
    if (toCity != arrivalAirport) {
        setColor(COLOR_YELLOW);
        cout << " (Arrival Airport: " << arrivalAirport << ")";
    }
    setColor(COLOR_WHITE);
    cout << endl;
    
    setColor(COLOR_CYAN);
    cout << "Flight Distance: " << flightDistance << " km" << endl;
    cout << "Flight Time: " << fixed << setprecision(1) << flightTime << " hours" << endl;
    
    setColor(COLOR_GREEN);
    cout << "Flight Cost: Rs. " << flightCost << endl;
    resetColor();
    
    setColor(COLOR_WHITE);
    cout << "Take-off: " << departureTime << endl;
    cout << "Landing: " << arrivalTime << endl;
    resetColor();
}

void bookFlightTicket(const string& username) {
    displayFlightBookingDashboard();
    
    Tourist tourist = getTouristByUsername(username);
    if (tourist.username.empty()) {
        displayErrorMessage("User not found!");
        return;
    }
    
    displayAirportCitiesForFlight();
    string fromCity, toCity;
    
    setColor(COLOR_CYAN);
    cout << "Enter departure city: ";
    resetColor();
    getline(cin, fromCity);
    if (find(airportCities.begin(), airportCities.end(), fromCity) == airportCities.end()) {
        displayErrorMessage("Invalid departure city! Please choose from airport cities.");
        return;
    }
    
    setColor(COLOR_CYAN);
    cout << "Enter destination city: ";
    resetColor();
    getline(cin, toCity);
    if (find(airportCities.begin(), airportCities.end(), toCity) == airportCities.end()) {
        displayErrorMessage("Invalid destination city! Please choose from airport cities.");
        return;
    }
    
    if (fromCity == toCity) {
        displayErrorMessage("Departure and destination cities cannot be the same!");
        return;
    }
    
    string departureAirport = fromCity;
    string arrivalAirport = toCity;
    
    vector<vector<string>> flightPaths = findAllPaths(departureAirport, arrivalAirport, planeTravelGraph);
    if (flightPaths.empty()) {
        displayErrorMessage("No flight route available between " + departureAirport + " and " + arrivalAirport);
        return;
    }
    
    int flightDistance = calculateRouteDistance(flightPaths[0], planeTravelGraph);
    double flightTime = flightDistance / FLIGHT_SPEED;
    
    displayFlightClassOptions(flightDistance);
    
    int classChoice;
    setColor(COLOR_CYAN);
    cout << "Select flight class (1-2): ";
    resetColor();
    cin >> classChoice;
    clearInputBuffer();
    
    if (classChoice < 1 || classChoice > 2) {
        displayErrorMessage("Invalid class selection!");
        return;
    }
    
    string flightClass = (classChoice == 1) ? "Economy" : "Business";
    double flightCostPerKm = (classChoice == 1) ? FLIGHT_COST_ECONOMY : FLIGHT_COST_BUSINESS;
    double totalFlightCost = flightDistance * flightCostPerKm;
    
    displayPriceBreakdown(flightDistance, flightClass, totalFlightCost);
    
    string bookingTime = getCurrentDateTime();
    string departureTime = addMinutesToDateTime(bookingTime, 30);
    string arrivalTime = addHoursToDateTime(departureTime, flightTime);
    
    displayFlightRouteDetails(fromCity, toCity, departureAirport, arrivalAirport, 
                             flightDistance, flightTime, totalFlightCost, departureTime, arrivalTime);
    
    FlightTicket ticket = createFlightTicket(tourist, departureAirport, arrivalAirport, 
                                           flightClass, flightDistance, totalFlightCost, 
                                           departureTime, arrivalTime);
    ticket.bookingTime = bookingTime;
    
    displaySectionHeader("FLIGHT TICKET CONFIRMATION");
    displayFlightTicket(ticket);
    
    setColor(COLOR_CYAN);
    cout << "\nConfirm booking? (y/n): ";
    resetColor();
    char confirm;
    cin >> confirm;
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        saveFlightTicketToFile(ticket, username);
        
        string filename = TRAVEL_RECORDS_DIR + username + "_travels.txt";
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << "\n[FLIGHT BOOKING - " << getCurrentDateTime() << "]" << endl;
            file << "From: " << fromCity << " To: " << toCity << endl;
            file << "Flight: " << ticket.flightNumber << " | Class: " << ticket.flightClass << endl;
            file << "Flight Distance: " << ticket.distance << " km | Flight Cost: Rs. " << ticket.cost << endl;
            file << "Departure: " << ticket.departureTime << " | Arrival: " << ticket.arrivalTime << endl;
            file << "------------------------------------------------------------" << endl;
            file.close();
        }
        
        displaySuccessMessage("Flight ticket booked successfully!");
    } else {
        displayInfoMessage("Flight booking cancelled.");
    }
}

// ============ LOCATION MANAGEMENT ============
void viewCurrentLocation(const string& username) {
    Tourist tourist = getTouristByUsername(username);
    if (tourist.username.empty()) {
        displayErrorMessage("User not found!");
        return;
    }
    
    displayHeader("CURRENT LOCATION");
    setColor(COLOR_WHITE);
    cout << "User: " << username << endl;
    setColor(COLOR_CYAN);
    cout << "Your current location is: " << tourist.currentLocation << endl;
    resetColor();
}

void updateCurrentLocation(const string& username) {
    displayHeader("UPDATE LOCATION");
    displayCities();
    
    string newLocation;
    setColor(COLOR_CYAN);
    cout << "Enter new location: ";
    resetColor();
    getline(cin, newLocation);
    
    if (newLocation.empty() || carTravelGraph.find(newLocation) == carTravelGraph.end()) {
        displayErrorMessage("Invalid city!");
        return;
    }
    
    Tourist tourist = getTouristByUsername(username);
    if (tourist.username.empty()) {
        displayErrorMessage("User not found!");
        return;
    }
    
    string oldLocation = tourist.currentLocation;
    tourist.currentLocation = newLocation;
    updateTouristInFile(tourist);
    
    string filename = TRAVEL_RECORDS_DIR + username + "_travels.txt";
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << "\n[LOCATION UPDATE]" << endl;
        file << "Date: " << getCurrentDateTime() << endl;
        file << "From: " << oldLocation << " -> To: " << newLocation << endl;
        file << "------------------------------------------------------------" << endl;
        file.close();
    }
    
    displaySuccessMessage("Location updated from " + oldLocation + " to " + newLocation);
}

// ============ TRANSPORTATION FUNCTIONS ============
bool hasAirport(const string& city) {
    return find(airportCities.begin(), airportCities.end(), city) != airportCities.end();
}

string findNearestAirport(const string& city) {
    string nearestAirport;
    int minDistance = INT_MAX;
    
    for (const auto& airportCity : airportCities) {
        vector<vector<string>> paths = findAllPaths(city, airportCity, carTravelGraph);
        if (!paths.empty()) {
            int distance = calculateRouteDistance(paths[0], carTravelGraph);
            if (distance < minDistance) {
                minDistance = distance;
                nearestAirport = airportCity;
            }
        }
    }
    
    return nearestAirport;
}

FlightTicket createFlightTicket(const Tourist& tourist, const string& fromCity, const string& toCity, 
                               const string& flightClass, double distance, double cost, 
                               const string& departureTime, const string& arrivalTime) {
    FlightTicket ticket;
    ticket.passengerName = tourist.username;
    ticket.passportNumber = tourist.cnicPassport;
    ticket.age = tourist.age;
    ticket.gender = tourist.gender;
    ticket.fromCity = fromCity;
    ticket.toCity = toCity;
    ticket.flightClass = flightClass;
    ticket.distance = distance;
    ticket.cost = cost;
    ticket.departureTime = departureTime;
    ticket.arrivalTime = arrivalTime;
    
    stringstream ss;
    ss << "PK" << setw(4) << setfill('0') << (rand() % 10000);
    ticket.flightNumber = ss.str();
    
    return ticket;
}

void displayFlightTicket(const FlightTicket& ticket) {
    setColor(COLOR_BLUE);
    cout << "============================================================" << endl;
    cout << "                      FLIGHT TICKET" << endl;
    cout << "============================================================" << endl;
    resetColor();
    
    setColor(COLOR_WHITE);
    cout << "Flight Number: " << ticket.flightNumber << endl;
    cout << "Passenger: " << ticket.passengerName << endl;
    cout << "Passport: " << ticket.passportNumber << endl;
    cout << "Age: " << ticket.age << " | Gender: " << ticket.gender << endl;
    cout << "Route: " << ticket.fromCity << " -> " << ticket.toCity << endl;
    
    setColor(COLOR_CYAN);
    cout << "Class: " << ticket.flightClass << endl;
    cout << "Distance: " << ticket.distance << " km" << endl;
    
    setColor(COLOR_GREEN);
    cout << "Cost: Rs. " << ticket.cost << endl;
    resetColor();
    
    setColor(COLOR_WHITE);
    cout << "Departure: " << ticket.departureTime << endl;
    cout << "Arrival: " << ticket.arrivalTime << endl;
    if (!ticket.bookingTime.empty()) {
        cout << "Booking Time: " << ticket.bookingTime << endl;
    }
    
    setColor(COLOR_BLUE);
    cout << "============================================================" << endl;
    resetColor();
}

void saveFlightTicketToFile(const FlightTicket& ticket, const string& username) {
    string filename = FLIGHT_TICKETS_DIR + username + "_" + ticket.flightNumber + ".txt";
    ofstream file(filename);
    if (file.is_open()) {
        file << "============================================================" << endl;
        file << "                      FLIGHT TICKET" << endl;
        file << "============================================================" << endl;
        file << "Flight Number: " << ticket.flightNumber << endl;
        file << "Passenger: " << ticket.passengerName << endl;
        file << "Passport: " << ticket.passportNumber << endl;
        file << "Age: " << ticket.age << " | Gender: " << ticket.gender << endl;
        file << "Route: " << ticket.fromCity << " -> " << ticket.toCity << endl;
        file << "Class: " << ticket.flightClass << endl;
        file << "Distance: " << ticket.distance << " km" << endl;
        file << "Cost: Rs. " << ticket.cost << endl;
        file << "Departure: " << ticket.departureTime << endl;
        file << "Arrival: " << ticket.arrivalTime << endl;
        file << "Booking Time: " << ticket.bookingTime << endl;
        file << "============================================================" << endl;
        file.close();
    }
}

// ============ TRIP PLANNING FUNCTIONS ============
void planNewTrip(const string& username) {
    displayHeader("PLAN NEW TRIP");
    
    Tourist tourist = getTouristByUsername(username);
    if (tourist.username.empty()) {
        displayErrorMessage("User not found!");
        return;
    }
    
    Trip trip;
    trip.username = username;
    trip.status = "planning";
    trip.startDate = getCurrentDateTime();
    
    string currentCity = tourist.currentLocation;
    bool continuePlanning = true;
    string currentDateTime = trip.startDate;
    
    while (continuePlanning) {
        displaySectionHeader("ADD DESTINATION");
        displayCities();
        setColor(COLOR_CYAN);
        cout << "Current location: " << currentCity << endl;
        resetColor();
        setColor(COLOR_CYAN);
        cout << "Enter destination city: ";
        resetColor();
        string destination;
        getline(cin, destination);
        
        if (destination.empty() || carTravelGraph.find(destination) == carTravelGraph.end()) {
            displayErrorMessage("Invalid destination!");
            continue;
        }
        
        if (currentCity == destination) {
            displayErrorMessage("You are already in " + destination + "!");
            continue;
        }
        
        displayTransportOptions();
        string transportMode = selectTransportMode();
        if (transportMode.empty()) continue;
        
        TripSegment segment;
        segment.fromCity = currentCity;
        segment.toCity = destination;
        segment.transportMode = transportMode;
        
        if (transportMode == "Plane") {
            if (hasAirport(currentCity) && hasAirport(destination)) {
                vector<vector<string>> flightPaths = findAllPaths(currentCity, destination, planeTravelGraph);
                if (flightPaths.empty()) {
                    displayErrorMessage("No direct flight available between " + currentCity + " and " + destination);
                    continue;
                }
                
                vector<string> selectedRoute = flightPaths[0];
                int flightDistance = calculateRouteDistance(selectedRoute, planeTravelGraph);
                double flightTime = flightDistance / FLIGHT_SPEED;
                
                displayFlightClassOptions(flightDistance);
                
                setColor(COLOR_CYAN);
                cout << "Enter choice (1-2): ";
                resetColor();
                int classChoice;
                cin >> classChoice;
                clearInputBuffer();
                
                if (classChoice < 1 || classChoice > 2) {
                    displayErrorMessage("Invalid class selection!");
                    continue;
                }
                
                string flightClass = (classChoice == 2) ? "Business" : "Economy";
                double flightCost = flightDistance * ((classChoice == 2) ? FLIGHT_COST_BUSINESS : FLIGHT_COST_ECONOMY);
                
                displayPriceBreakdown(flightDistance, flightClass, flightCost);
                
                segment.route = selectedRoute;
                segment.distance = flightDistance;
                segment.travelTime = flightTime;
                segment.segmentCost = flightCost;
                segment.departureTime = currentDateTime;
                segment.arrivalTime = addHoursToDateTime(currentDateTime, flightTime);
                segment.flightTicket = createFlightTicket(tourist, currentCity, destination, flightClass, 
                                                         flightDistance, flightCost, segment.departureTime, segment.arrivalTime);
                
                trip.segments.push_back(segment);
                trip.totalCost += flightCost;
                trip.totalTravelTime += flightTime;
                
                displaySectionHeader("FLIGHT BOOKED");
                displayFlightTicket(segment.flightTicket);
                saveFlightTicketToFile(segment.flightTicket, username);
                
            } else {
                string departureAirport = hasAirport(currentCity) ? currentCity : findNearestAirport(currentCity);
                string arrivalAirport = hasAirport(destination) ? destination : findNearestAirport(destination);
                
                setColor(COLOR_YELLOW);
                cout << "Flight routing:" << endl;
                if (!hasAirport(currentCity)) {
                    cout << "No airport in " << currentCity << ". Nearest airport: " << departureAirport << endl;
                }
                if (!hasAirport(destination)) {
                    cout << "No airport in " << destination << ". Nearest airport: " << arrivalAirport << endl;
                }
                resetColor();
                
                vector<vector<string>> flightPaths = findAllPaths(departureAirport, arrivalAirport, planeTravelGraph);
                if (flightPaths.empty()) {
                    displayErrorMessage("No flight route available between " + departureAirport + " and " + arrivalAirport);
                    continue;
                }
                
                vector<string> flightRoute = flightPaths[0];
                int flightDistance = calculateRouteDistance(flightRoute, planeTravelGraph);
                double flightTime = flightDistance / FLIGHT_SPEED;
                
                displayFlightClassOptions(flightDistance);
                
                setColor(COLOR_CYAN);
                cout << "Enter choice (1-2): ";
                resetColor();
                int classChoice;
                cin >> classChoice;
                clearInputBuffer();
                
                if (classChoice < 1 || classChoice > 2) {
                    displayErrorMessage("Invalid class selection!");
                    continue;
                }
                
                string flightClass = (classChoice == 2) ? "Business" : "Economy";
                double flightCost = flightDistance * ((classChoice == 2) ? FLIGHT_COST_BUSINESS : FLIGHT_COST_ECONOMY);
                
                displayPriceBreakdown(flightDistance, flightClass, flightCost);
                
                double carToAirportCost = 0;
                double carFromAirportCost = 0;
                double carToAirportTime = 0;
                double carFromAirportTime = 0;
                string segmentDateTime = currentDateTime;
                
                if (currentCity != departureAirport) {
                    vector<vector<string>> carPaths = findAllPaths(currentCity, departureAirport, carTravelGraph);
                    if (!carPaths.empty()) {
                        vector<string> carRoute = carPaths[0];
                        int carDistance = calculateRouteDistance(carRoute, carTravelGraph);
                        carToAirportTime = calculateTravelTime(carDistance, "Car");
                        carToAirportCost = carDistance * FUEL_COST_PER_KM;
                        segmentDateTime = addHoursToDateTime(segmentDateTime, carToAirportTime);
                    }
                }
                
                segmentDateTime = addMinutesToDateTime(segmentDateTime, 30);
                string flightDepartureTime = segmentDateTime;
                string flightArrivalTime = addHoursToDateTime(segmentDateTime, flightTime);
                segmentDateTime = flightArrivalTime;
                
                if (arrivalAirport != destination) {
                    vector<vector<string>> carPaths = findAllPaths(arrivalAirport, destination, carTravelGraph);
                    if (!carPaths.empty()) {
                        vector<string> carRoute = carPaths[0];
                        int carDistance = calculateRouteDistance(carRoute, carTravelGraph);
                        carFromAirportTime = calculateTravelTime(carDistance, "Car");
                        carFromAirportCost = carDistance * FUEL_COST_PER_KM;
                        segmentDateTime = addHoursToDateTime(segmentDateTime, carFromAirportTime);
                    }
                }
                
                vector<string> completeRoute;
                if (currentCity != departureAirport) completeRoute.push_back(currentCity);
                completeRoute.push_back(departureAirport);
                completeRoute.push_back(arrivalAirport);
                if (arrivalAirport != destination) completeRoute.push_back(destination);
                
                segment.route = completeRoute;
                segment.distance = flightDistance;
                segment.travelTime = flightTime + carToAirportTime + carFromAirportTime;
                segment.segmentCost = flightCost + carToAirportCost + carFromAirportCost;
                segment.departureTime = currentDateTime;
                segment.arrivalTime = segmentDateTime;
                segment.transportMode = "Plane + Car";
                segment.flightTicket = createFlightTicket(tourist, departureAirport, arrivalAirport, 
                                                         flightClass, flightDistance, flightCost, 
                                                         flightDepartureTime, flightArrivalTime);
                segment.carToAirportCost = carToAirportCost;
                segment.carFromAirportCost = carFromAirportCost;
                segment.carToAirportTime = carToAirportTime;
                segment.carFromAirportTime = carFromAirportTime;
                
                trip.segments.push_back(segment);
                trip.totalCost += segment.segmentCost;
                trip.totalTravelTime += segment.travelTime;
                
                displaySectionHeader("FLIGHT WITH CAR TRANSFER BOOKED");
                displayFlightTicket(segment.flightTicket);
                saveFlightTicketToFile(segment.flightTicket, username);
                
                if (currentCity != departureAirport || arrivalAirport != destination) {
                    setColor(COLOR_YELLOW);
                    cout << "CAR TRAVEL DETAILS:" << endl;
                    if (currentCity != departureAirport) {
                        cout << "Car from " << currentCity << " to " << departureAirport << ": Rs. " << carToAirportCost 
                             << " (" << carToAirportTime << " hours)" << endl;
                    }
                    if (arrivalAirport != destination) {
                        cout << "Car from " << arrivalAirport << " to " << destination << ": Rs. " << carFromAirportCost 
                             << " (" << carFromAirportTime << " hours)" << endl;
                    }
                    resetColor();
                }
            }
        } else {
            vector<vector<string>> allPaths = findAllPaths(currentCity, destination, carTravelGraph);
            if (allPaths.empty()) {
                displayErrorMessage("No route found between " + currentCity + " and " + destination);
                continue;
            }
            
            displayRouteOptions(allPaths, "Car");
            setColor(COLOR_CYAN);
            cout << "Select route (1-" << allPaths.size() << "): ";
            resetColor();
            int routeChoice;
            cin >> routeChoice;
            clearInputBuffer();
            
            if (routeChoice < 1 || routeChoice > allPaths.size()) {
                displayErrorMessage("Invalid route selection!");
                continue;
            }
            
            vector<string> selectedRoute = allPaths[routeChoice - 1];
            int distance = calculateRouteDistance(selectedRoute, carTravelGraph);
            double fuelCost = distance * FUEL_COST_PER_KM;
            double travelTime = calculateTravelTime(distance, "Car");
            
            segment.route = selectedRoute;
            segment.distance = distance;
            segment.fuelCost = fuelCost;
            segment.travelTime = travelTime;
            segment.segmentCost = fuelCost;
            segment.departureTime = currentDateTime;
            segment.arrivalTime = addHoursToDateTime(currentDateTime, travelTime);
            
            trip.segments.push_back(segment);
            trip.totalCost += fuelCost;
            trip.totalTravelTime += travelTime;
        }
        
        setColor(COLOR_CYAN);
        cout << "Enter total days to stay in " << destination << ": ";
        resetColor();
        int totalStayDays;
        cin >> totalStayDays;
        clearInputBuffer();
        
        if (totalStayDays > 0) {
            setColor(COLOR_CYAN);
            cout << "Do you want to book hotels in " << destination << "? (y/n): ";
            resetColor();
            char bookHotel;
            cin >> bookHotel;
            clearInputBuffer();
            
            if (bookHotel == 'y' || bookHotel == 'Y') {
                vector<HotelStay> hotelStays = selectHotels(destination, totalStayDays);
                if (!hotelStays.empty()) {
                    double totalHotelCost = 0;
                    for (const auto& stay : hotelStays) {
                        totalHotelCost += stay.totalCost;
                        trip.segments.back().hotelStays.push_back(stay);
                    }
                    trip.totalCost += totalHotelCost;
                    trip.totalStayDays += totalStayDays;
                    displaySuccessMessage("Hotels booked successfully for " + to_string(totalStayDays) + " days");
                } else {
                    displayInfoMessage("No hotels booked for this segment.");
                }
            } else {
                displayInfoMessage("No hotels booked for this segment.");
            }
        } else {
            displayInfoMessage("No stay days specified. Moving to next destination.");
        }
        
        currentDateTime = addHoursToDateTime(trip.segments.back().arrivalTime, totalStayDays * 24);
        currentCity = destination;
        
        displaySectionHeader("SEGMENT ADDED SUCCESSFULLY");
        setColor(COLOR_WHITE);
        cout << "Transport: " << trip.segments.back().transportMode << endl;
        cout << "Route: ";
        for (size_t i = 0; i < trip.segments.back().route.size(); i++) {
            cout << trip.segments.back().route[i];
            if (i < trip.segments.back().route.size() - 1) cout << " -> ";
        }
        cout << endl;
        cout << "Distance: " << trip.segments.back().distance << " km" << endl;
        cout << "Travel Time: " << fixed << setprecision(1) << trip.segments.back().travelTime << " hours" << endl;
        
        if (trip.segments.back().transportMode == "Car") {
            setColor(COLOR_YELLOW);
            cout << "Fuel Cost: Rs. " << trip.segments.back().segmentCost << endl;
        } else if (trip.segments.back().transportMode == "Plane") {
            setColor(COLOR_GREEN);
            cout << "Flight Cost: Rs. " << trip.segments.back().flightTicket.cost << endl;
        } else if (trip.segments.back().transportMode == "Plane + Car") {
            setColor(COLOR_GREEN);
            cout << "Flight Cost: Rs. " << trip.segments.back().flightTicket.cost << endl;
            if (trip.segments.back().carToAirportCost > 0) {
                setColor(COLOR_YELLOW);
                cout << "Car to Airport Cost: Rs. " << trip.segments.back().carToAirportCost << endl;
            }
            if (trip.segments.back().carFromAirportCost > 0) {
                setColor(COLOR_YELLOW);
                cout << "Car from Airport Cost: Rs. " << trip.segments.back().carFromAirportCost << endl;
            }
        }
        
        double hotelCost = 0;
        for (const auto& stay : trip.segments.back().hotelStays) {
            hotelCost += stay.totalCost;
        }
        if (hotelCost > 0) {
            setColor(COLOR_MAGENTA);
            cout << "Hotel Cost: Rs. " << hotelCost << endl;
        }
        
        setColor(COLOR_CYAN);
        double totalSegmentCost = trip.segments.back().segmentCost + hotelCost;
        cout << "Total Segment Cost: Rs. " << totalSegmentCost << endl;
        resetColor();
        
        setColor(COLOR_CYAN);
        cout << "\nAdd another destination? (y/n): ";
        resetColor();
        char cont;
        cin >> cont;
        clearInputBuffer();
        continuePlanning = (cont == 'y' || cont == 'Y');
    }
    
    displayTripSummary(trip);
    
    setColor(COLOR_CYAN);
    cout << "\nConfirm and save this trip? (y/n): ";
    resetColor();
    char confirm;
    cin >> confirm;
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        trip.status = "ongoing";
        trip.endDate = currentDateTime;
        saveTripToFile(trip);
        
        string filename = TRAVEL_RECORDS_DIR + username + "_travels.txt";
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << "\n[NEW TRIP STARTED - " << getCurrentDateTime() << "]" << endl;
            file << "Tourist: " << username << endl;
            file << "Start Date: " << trip.startDate << endl;
            file << "Current Total Cost: Rs. " << trip.totalCost << endl;
            file << "Current Total Travel Time: " << fixed << setprecision(1) << trip.totalTravelTime << " hours" << endl;
            file << "Current Total Stay Days: " << trip.totalStayDays << endl;
            file << "Trip Segments:" << endl;
            for (const auto& segment : trip.segments) {
                file << "  " << segment.fromCity << " -> " << segment.toCity << " (" << segment.transportMode << ")" << endl;
            }
            file << "============================================================" << endl;
            file.close();
        }
        
        displaySuccessMessage("Trip saved successfully!");
        
        tourist.currentLocation = currentCity;
        updateTouristInFile(tourist);
    } else {
        displayInfoMessage("Trip planning cancelled.");
    }
}

void continueExistingTrip(const string& username) {
    displayHeader("CONTINUE EXISTING TRIP");
    
    vector<Trip> trips = loadAllTrips(username);
    if (trips.empty()) {
        displayErrorMessage("No ongoing trips found!");
        return;
    }
    
    cout << "Ongoing Trips:" << endl;
    for (size_t i = 0; i < trips.size(); i++) {
        if (trips[i].status == "ongoing") {
            cout << i+1 << ". To: " << trips[i].segments.back().toCity << " (Current)" << endl;
        }
    }
    
    setColor(COLOR_CYAN);
    cout << "Select trip to continue: ";
    resetColor();
    int choice;
    cin >> choice;
    clearInputBuffer();
    
    if (choice < 1 || choice > trips.size()) {
        displayErrorMessage("Invalid selection!");
        return;
    }
    
    Trip trip = trips[choice-1];
    displayTripSummary(trip);
    
    cout << "\n1. Add another destination" << endl;
    cout << "2. Complete this trip" << endl;
    setColor(COLOR_CYAN);
    cout << "Enter choice: ";
    resetColor();
    cin >> choice;
    clearInputBuffer();
    
    if (choice == 1) {
        displayInfoMessage("Please use Plan New Trip feature for additional destinations");
    } else if (choice == 2) {
        completeTrip(username, trip);
    }
}

// ============ ROUTE PLANNING FUNCTIONS ============
vector<vector<string>> findAllPaths(const string& start, const string& end, const map<string, City>& graph) {
    vector<vector<string>> allPaths;
    
    if (start == end) {
        allPaths.push_back({start});
        return allPaths;
    }
    
    if (graph.find(start) == graph.end() || graph.find(end) == graph.end()) {
        return allPaths;
    }
    
    queue<vector<string>> q;
    q.push({start});
    
    while (!q.empty() && allPaths.size() < 5) {
        vector<string> currentPath = q.front();
        q.pop();
        
        string currentCity = currentPath.back();
        
        if (currentCity == end) {
            allPaths.push_back(currentPath);
            continue;
        }
        
        if (graph.find(currentCity) == graph.end()) {
            continue;
        }
        
        for (const Route& route : graph.at(currentCity).routes) {
            if (find(currentPath.begin(), currentPath.end(), route.destination) == currentPath.end()) {
                vector<string> newPath = currentPath;
                newPath.push_back(route.destination);
                q.push(newPath);
            }
        }
    }
    
    sort(allPaths.begin(), allPaths.end(), [&](const vector<string>& a, const vector<string>& b) {
        return calculateRouteDistance(a, graph) < calculateRouteDistance(b, graph);
    });
    
    return allPaths;
}

void displayRouteOptions(const vector<vector<string>>& allPaths, const string& transportMode) {
    displaySectionHeader("AVAILABLE " + transportMode + " ROUTES");
    
    for (size_t i = 0; i < allPaths.size(); i++) {
        const vector<string>& route = allPaths[i];
        int distance = calculateRouteDistance(route, (transportMode == "Car") ? carTravelGraph : planeTravelGraph);
        
        if (transportMode == "Car") {
            double fuelCost = distance * FUEL_COST_PER_KM;
            double travelTime = calculateTravelTime(distance, "Car");
            
            setColor(COLOR_WHITE);
            cout << i+1 << ". ";
            for (size_t j = 0; j < route.size(); j++) {
                cout << route[j];
                if (j < route.size() - 1) cout << " -> ";
            }
            cout << endl;
            setColor(COLOR_CYAN);
            cout << "   Distance: " << distance << " km" << endl;
            cout << "   Travel Time: " << fixed << setprecision(1) << travelTime << " hours" << endl;
            setColor(COLOR_YELLOW);
            cout << "   Fuel Cost: Rs. " << fuelCost << endl;
            
            if (i == 0) {
                setColor(COLOR_GREEN);
                cout << "   SHORTEST ROUTE" << endl;
            }
        } else {
            double travelTime = calculateTravelTime(distance, "Plane");
            
            setColor(COLOR_WHITE);
            cout << i+1 << ". ";
            for (size_t j = 0; j < route.size(); j++) {
                cout << route[j];
                if (j < route.size() - 1) cout << " -> ";
            }
            cout << endl;
            setColor(COLOR_CYAN);
            cout << "   Flight Distance: " << distance << " km" << endl;
            cout << "   Flight Time: " << fixed << setprecision(1) << travelTime << " hours" << endl;
            setColor(COLOR_YELLOW);
            cout << "   Cost: Will be calculated based on class selection" << endl;
            
            if (i == 0) {
                setColor(COLOR_GREEN);
                cout << "   SHORTEST FLIGHT ROUTE" << endl;
            }
        }
        cout << endl;
        resetColor();
    }
}

double calculateRouteDistance(const vector<string>& route, const map<string, City>& graph) {
    if (route.size() < 2) return 0;
    
    int distance = 0;
    for (size_t i = 0; i < route.size() - 1; i++) {
        string from = route[i];
        string to = route[i+1];
        
        if (graph.find(from) != graph.end()) {
            for (const Route& r : graph.at(from).routes) {
                if (r.destination == to) {
                    distance += r.distance;
                    break;
                }
            }
        }
    }
    return distance;
}

double calculateTravelTime(int distance, const string& transportMode) {
    if (transportMode == "Car") return distance * TRAVEL_TIME_PER_KM;
    else return distance / FLIGHT_SPEED;
}

// ============ HOTEL MANAGEMENT ============
void displayHotelOptions(const string& city) {
    displaySectionHeader("AVAILABLE HOTELS IN " + city);
    
    if (cityHotels.find(city) == cityHotels.end()) {
        setColor(COLOR_WHITE);
        cout << "Standard Hotel (3-star) - Rs. 6000/night (1-30 days)" << endl;
        cout << "Budget Inn - Rs. 3000/night (1-30 days)" << endl;
        resetColor();
        return;
    }
    
    const vector<Hotel>& hotels = cityHotels[city];
    for (size_t i = 0; i < hotels.size(); i++) {
        setColor(COLOR_WHITE);
        cout << i+1 << ". " << hotels[i].name << " (" << hotels[i].category << ")" << endl;
        setColor(COLOR_YELLOW);
        cout << "   Rs. " << hotels[i].costPerNight << "/night" << endl;
        setColor(COLOR_CYAN);
        cout << "   Stay: " << hotels[i].minStay << "-" << hotels[i].maxStay << " days" << endl;
        cout << endl;
        resetColor();
    }
}

vector<HotelStay> selectHotels(const string& city, int totalStayDays) {
    vector<HotelStay> stays;
    int remainingDays = totalStayDays;
    
    setColor(COLOR_WHITE);
    cout << "You have " << totalStayDays << " days to stay in " << city << endl;
    cout << "You can book multiple hotels for your stay." << endl;
    resetColor();
    
    while (remainingDays > 0) {
        displayHotelOptions(city);
        setColor(COLOR_CYAN);
        cout << "Select hotel for next stay (0 to finish): ";
        resetColor();
        int choice;
        cin >> choice;
        clearInputBuffer();
        
        if (choice == 0) {
            if (remainingDays > 0) {
                setColor(COLOR_YELLOW);
                cout << "You still have " << remainingDays << " days unbooked!" << endl;
                resetColor();
                setColor(COLOR_CYAN);
                cout << "Do you want to continue booking? (y/n): ";
                resetColor();
                char cont;
                cin >> cont;
                clearInputBuffer();
                if (cont == 'n' || cont == 'N') break;
            } else break;
        }
        
        if (cityHotels.find(city) == cityHotels.end() || choice < 1 || choice > cityHotels[city].size()) {
            displayErrorMessage("Invalid hotel selection!");
            continue;
        }
        
        Hotel selectedHotel = cityHotels[city][choice-1];
        
        setColor(COLOR_CYAN);
        cout << "Enter days to stay at " << selectedHotel.name << " (available " 
             << remainingDays << " days, hotel allows " << selectedHotel.minStay << "-" 
             << selectedHotel.maxStay << " days): ";
        resetColor();
        int stayDays;
        cin >> stayDays;
        clearInputBuffer();
        
        if (stayDays < selectedHotel.minStay || stayDays > selectedHotel.maxStay) {
            displayErrorMessage("Invalid stay duration for this hotel!");
            continue;
        }
        
        if (stayDays > remainingDays) {
            displayErrorMessage("You only have " + to_string(remainingDays) + " days remaining!");
            continue;
        }
        
        HotelStay stay;
        stay.hotel = selectedHotel;
        stay.stayDays = stayDays;
        stay.totalCost = selectedHotel.costPerNight * stayDays;
        
        stays.push_back(stay);
        remainingDays -= stayDays;
        
        setColor(COLOR_GREEN);
        cout << "Booked " << selectedHotel.name << " for " << stayDays << " days - Rs. " << stay.totalCost << endl;
        resetColor();
        setColor(COLOR_WHITE);
        cout << "Remaining days to book: " << remainingDays << endl;
        resetColor();
        
        if (remainingDays > 0) {
            setColor(COLOR_CYAN);
            cout << "Book another hotel? (y/n): ";
            resetColor();
            char another;
            cin >> another;
            clearInputBuffer();
            if (another == 'n' || another == 'N') break;
        }
    }
    
    return stays;
}

// ============ TRIP DISPLAY AND MANAGEMENT ============
void displayTripSummary(const Trip& trip) {
    displayHeader("TRIP SUMMARY");
    
    setColor(COLOR_WHITE);
    cout << "Tourist: " << trip.username << endl;
    setColor(COLOR_GREEN);
    cout << "Total Cost: Rs. " << trip.totalCost << endl;
    resetColor();
    setColor(COLOR_CYAN);
    cout << "Total Travel Time: " << fixed << setprecision(1) << trip.totalTravelTime << " hours" << endl;
    cout << "Total Stay Days: " << trip.totalStayDays << endl;
    
    double totalTimeUsed = (trip.totalTravelTime / 24.0) + trip.totalStayDays;
    cout << "Total Time Used: " << fixed << setprecision(1) << totalTimeUsed << " days" << endl;
    resetColor();
    
    cout << "\nTRIP ITINERARY:" << endl;
    setColor(COLOR_BLUE);
    cout << "============================================================" << endl;
    resetColor();
    
    for (const auto& segment : trip.segments) {
        cout << "\nSegment: " << segment.fromCity << " -> " << segment.toCity << endl;
        setColor(COLOR_CYAN);
        cout << "Transport: " << segment.transportMode << endl;
        resetColor();
        cout << "Route: ";
        for (size_t i = 0; i < segment.route.size(); i++) {
            cout << segment.route[i];
            if (i < segment.route.size() - 1) cout << " -> ";
        }
        cout << endl;
        setColor(COLOR_WHITE);
        cout << "Distance: " << segment.distance << " km" << endl;
        cout << "Travel Time: " << fixed << setprecision(1) << segment.travelTime << " hours" << endl;
        
        if (segment.transportMode == "Car") {
            setColor(COLOR_YELLOW);
            cout << "Fuel Cost: Rs. " << segment.fuelCost << endl;
        } else if (segment.transportMode == "Plane") {
            setColor(COLOR_GREEN);
            cout << "Flight Cost: Rs. " << segment.flightTicket.cost << endl;
            cout << "Flight Class: " << segment.flightTicket.flightClass << endl;
        } else if (segment.transportMode == "Plane + Car") {
            setColor(COLOR_GREEN);
            cout << "Flight Cost: Rs. " << segment.flightTicket.cost << endl;
            cout << "Flight Class: " << segment.flightTicket.flightClass << endl;
            if (segment.carToAirportCost > 0) {
                setColor(COLOR_YELLOW);
                cout << "Car to Airport Cost: Rs. " << segment.carToAirportCost << endl;
            }
            if (segment.carFromAirportCost > 0) {
                setColor(COLOR_YELLOW);
                cout << "Car from Airport Cost: Rs. " << segment.carFromAirportCost << endl;
            }
        }
        
        setColor(COLOR_WHITE);
        cout << "Departure: " << segment.departureTime << endl;
        cout << "Arrival: " << segment.arrivalTime << endl;
        
        if (!segment.hotelStays.empty()) {
            setColor(COLOR_MAGENTA);
            cout << "Hotel Stays in " << segment.toCity << ":" << endl;
            for (const auto& stay : segment.hotelStays) {
                setColor(COLOR_WHITE);
                cout << "   " << stay.hotel.name << " (" << stay.hotel.category << ")" << endl;
                setColor(COLOR_YELLOW);
                cout << "   " << stay.stayDays << " days - Rs. " << stay.totalCost << endl;
            }
        }
        
        double segmentHotelCost = 0;
        for (const auto& stay : segment.hotelStays) {
            segmentHotelCost += stay.totalCost;
        }
        setColor(COLOR_CYAN);
        cout << "Segment Total: Rs. " << (segment.segmentCost + segmentHotelCost) << endl;
        resetColor();
        setColor(COLOR_BLUE);
        cout << "------------------------------------------------------------" << endl;
        resetColor();
    }
}

void completeTrip(const string& username, Trip& trip) {
    trip.status = "completed";
    trip.endDate = getCurrentDateTime();
    saveTripToFile(trip);
    
    string filename = TRAVEL_RECORDS_DIR + username + "_travels.txt";
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << "\n[COMPLETED TRIP - " << trip.endDate << "]" << endl;
        file << "User: " << username << endl;
        file << "Start Time: " << trip.startDate << endl;
        file << "End Time: " << trip.endDate << endl;
        file << "Total Trip Cost: Rs. " << trip.totalCost << endl;
        file << "Total Travel Time: " << fixed << setprecision(1) << trip.totalTravelTime << " hours" << endl;
        file << "Total Stay Days: " << trip.totalStayDays << endl;
        
        file << "Complete Route: ";
        for (size_t i = 0; i < trip.segments.size(); i++) {
            file << trip.segments[i].fromCity;
            if (i < trip.segments.size() - 1) file << " -> ";
        }
        file << " -> " << trip.segments.back().toCity << endl;
        
        file << "Transport Details:" << endl;
        for (const auto& segment : trip.segments) {
            file << "   " << segment.fromCity << " -> " << segment.toCity;
            file << " (" << segment.transportMode << ")";
            if (segment.transportMode == "Plane") {
                file << " [" << segment.flightTicket.flightClass << " class]";
            }
            file << endl;
        }
        
        file << "============================================================" << endl;
        file.close();
    }
    
    displaySuccessMessage("Trip completed successfully!");
    displayTripSummary(trip);
}

// ============ FILE OPERATIONS ============
void saveTripToFile(const Trip& trip) {
    string filename = TRIPS_DIR + trip.username + "_trip.txt";
    ofstream file(filename);
    if (file.is_open()) {
        file << "USERNAME:" << trip.username << endl;
        file << "STATUS:" << trip.status << endl;
        file << "TOTAL_COST:" << trip.totalCost << endl;
        file << "TOTAL_TRAVEL_TIME:" << trip.totalTravelTime << endl;
        file << "TOTAL_STAY_DAYS:" << trip.totalStayDays << endl;
        file << "START_DATE:" << trip.startDate << endl;
        file << "END_DATE:" << trip.endDate << endl;
        
        for (const auto& segment : trip.segments) {
            file << "SEGMENT_START" << endl;
            file << "FROM:" << segment.fromCity << endl;
            file << "TO:" << segment.toCity << endl;
            file << "DISTANCE:" << segment.distance << endl;
            file << "FUEL_COST:" << segment.fuelCost << endl;
            file << "TRAVEL_TIME:" << segment.travelTime << endl;
            file << "SEGMENT_COST:" << segment.segmentCost << endl;
            file << "DEPARTURE:" << segment.departureTime << endl;
            file << "ARRIVAL:" << segment.arrivalTime << endl;
            file << "TRANSPORT_MODE:" << segment.transportMode << endl;
            file << "CAR_TO_AIRPORT_COST:" << segment.carToAirportCost << endl;
            file << "CAR_FROM_AIRPORT_COST:" << segment.carFromAirportCost << endl;
            file << "CAR_TO_AIRPORT_TIME:" << segment.carToAirportTime << endl;
            file << "CAR_FROM_AIRPORT_TIME:" << segment.carFromAirportTime << endl;
            
            file << "ROUTE:";
            for (size_t i = 0; i < segment.route.size(); i++) {
                file << segment.route[i];
                if (i < segment.route.size() - 1) file << "->";
            }
            file << endl;
            
            if (segment.transportMode == "Plane" || segment.transportMode == "Plane + Car") {
                file << "FLIGHT:" << segment.flightTicket.passengerName << "|" 
                     << segment.flightTicket.passportNumber << "|" << segment.flightTicket.age << "|"
                     << segment.flightTicket.gender << "|" << segment.flightTicket.fromCity << "|"
                     << segment.flightTicket.toCity << "|" << segment.flightTicket.flightClass << "|"
                     << segment.flightTicket.distance << "|" << segment.flightTicket.cost << "|"
                     << segment.flightTicket.departureTime << "|" << segment.flightTicket.arrivalTime << "|"
                     << segment.flightTicket.flightNumber << endl;
            }
            
            for (const auto& stay : segment.hotelStays) {
                file << "HOTEL:" << stay.hotel.name << "|" << stay.hotel.category << "|" 
                     << stay.hotel.costPerNight << "|" << stay.stayDays << "|" << stay.totalCost << endl;
            }
            file << "SEGMENT_END" << endl;
        }
        file.close();
    }
}

Trip loadTrip(const string& username) {
    Trip trip;
    string filename = TRIPS_DIR + username + "_trip.txt";
    ifstream file(filename);
    
    if (!file.is_open()) return trip;
    
    string line;
    TripSegment currentSegment;
    bool inSegment = false;
    
    while (getline(file, line)) {
        if (line.find("USERNAME:") == 0) trip.username = line.substr(9);
        else if (line.find("STATUS:") == 0) trip.status = line.substr(7);
        else if (line.find("TOTAL_COST:") == 0) trip.totalCost = stod(line.substr(11));
        else if (line.find("TOTAL_TRAVEL_TIME:") == 0) trip.totalTravelTime = stod(line.substr(18));
        else if (line.find("TOTAL_STAY_DAYS:") == 0) trip.totalStayDays = stoi(line.substr(16));
        else if (line.find("START_DATE:") == 0) trip.startDate = line.substr(11);
        else if (line.find("END_DATE:") == 0) trip.endDate = line.substr(9);
        else if (line == "SEGMENT_START") { inSegment = true; currentSegment = TripSegment(); }
        else if (line == "SEGMENT_END") { inSegment = false; trip.segments.push_back(currentSegment); }
        else if (inSegment) {
            if (line.find("FROM:") == 0) currentSegment.fromCity = line.substr(5);
            else if (line.find("TO:") == 0) currentSegment.toCity = line.substr(3);
            else if (line.find("DISTANCE:") == 0) currentSegment.distance = stoi(line.substr(9));
            else if (line.find("FUEL_COST:") == 0) currentSegment.fuelCost = stod(line.substr(10));
            else if (line.find("TRAVEL_TIME:") == 0) currentSegment.travelTime = stod(line.substr(12));
            else if (line.find("SEGMENT_COST:") == 0) currentSegment.segmentCost = stod(line.substr(13));
            else if (line.find("DEPARTURE:") == 0) currentSegment.departureTime = line.substr(10);
            else if (line.find("ARRIVAL:") == 0) currentSegment.arrivalTime = line.substr(8);
            else if (line.find("TRANSPORT_MODE:") == 0) currentSegment.transportMode = line.substr(15);
            else if (line.find("CAR_TO_AIRPORT_COST:") == 0) currentSegment.carToAirportCost = stod(line.substr(20));
            else if (line.find("CAR_FROM_AIRPORT_COST:") == 0) currentSegment.carFromAirportCost = stod(line.substr(22));
            else if (line.find("CAR_TO_AIRPORT_TIME:") == 0) currentSegment.carToAirportTime = stod(line.substr(20));
            else if (line.find("CAR_FROM_AIRPORT_TIME:") == 0) currentSegment.carFromAirportTime = stod(line.substr(22));
            else if (line.find("ROUTE:") == 0) {
                string routeStr = line.substr(6);
                stringstream ss(routeStr);
                string city;
                while (getline(ss, city, '>')) {
                    if (!city.empty()) currentSegment.route.push_back(city);
                }
            }
            else if (line.find("FLIGHT:") == 0) {
                stringstream ss(line.substr(7));
                string part;
                vector<string> parts;
                
                while (getline(ss, part, '|')) parts.push_back(part);
                
                if (parts.size() >= 12) {
                    currentSegment.flightTicket.passengerName = parts[0];
                    currentSegment.flightTicket.passportNumber = parts[1];
                    currentSegment.flightTicket.age = stoi(parts[2]);
                    currentSegment.flightTicket.gender = parts[3];
                    currentSegment.flightTicket.fromCity = parts[4];
                    currentSegment.flightTicket.toCity = parts[5];
                    currentSegment.flightTicket.flightClass = parts[6];
                    currentSegment.flightTicket.distance = stod(parts[7]);
                    currentSegment.flightTicket.cost = stod(parts[8]);
                    currentSegment.flightTicket.departureTime = parts[9];
                    currentSegment.flightTicket.arrivalTime = parts[10];
                    currentSegment.flightTicket.flightNumber = parts[11];
                }
            }
            else if (line.find("HOTEL:") == 0) {
                stringstream ss(line.substr(6));
                string part;
                HotelStay stay;
                
                getline(ss, part, '|'); stay.hotel.name = part;
                getline(ss, part, '|'); stay.hotel.category = part;
                getline(ss, part, '|'); stay.hotel.costPerNight = stod(part);
                getline(ss, part, '|'); stay.stayDays = stoi(part);
                getline(ss, part, '|'); stay.totalCost = stod(part);
                
                currentSegment.hotelStays.push_back(stay);
            }
        }
    }
    file.close();
    return trip;
}

vector<Trip> loadAllTrips(const string& username) {
    vector<Trip> trips;
    Trip trip = loadTrip(username);
    if (!trip.username.empty()) trips.push_back(trip);
    return trips;
}

// ============ TRAVEL RECORDS ============
void viewTravelRecords(const string& username) {
    displayHeader("TRAVEL HISTORY - " + username);
    
    string filename = TRAVEL_RECORDS_DIR + username + "_travels.txt";
    ifstream file(filename);
    
    if (!file.is_open()) {
        displayErrorMessage("No travel records found!");
        return;
    }
    
    string line;
    bool hasContent = false;
    while (getline(file, line)) {
        cout << line << endl;
        hasContent = true;
    }
    file.close();
    
    if (!hasContent) displayInfoMessage("No travel history available yet.");
}

// ============ UTILITY FUNCTIONS ============
void displayCities() {
    setColor(COLOR_CYAN);
    cout << "\nAVAILABLE CITIES:" << endl;
    cout << "------------------------------------------------------------" << endl;
    resetColor();
    int count = 0;
    for (const auto& city : carTravelGraph) {
        setColor(COLOR_WHITE);
        cout << setw(15) << left << city.first;
        resetColor();
        count++;
        if (count % 4 == 0) cout << endl;
    }
    if (count % 4 != 0) cout << endl;
    setColor(COLOR_CYAN);
    cout << "------------------------------------------------------------" << endl;
    resetColor();
}

string addDaysToDate(const string& startDate, int days) {
    return startDate + " + " + to_string(days) + " days";
}

string addHoursToDateTime(const string& startDateTime, double hours) {
    return startDateTime + " + " + to_string((int)hours) + " hours";
}

string addMinutesToDateTime(const string& startDateTime, int minutes) {
    return startDateTime + " + " + to_string(minutes) + " minutes";
}

// ============ VALIDATION AND FILE OPERATIONS ============
bool isUsernameExists(const string& username) {
    ifstream file(USER_FILE);
    if (!file.is_open()) return false;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUsername;
        getline(ss, storedUsername, ',');
        if (storedUsername == username) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return (atPos != string::npos && dotPos != string::npos && 
            dotPos > atPos + 1 && email.length() > dotPos + 1);
}

bool isValidCNIC(const string& cnic) {
    if (cnic.length() != 13) return false;
    for (char c : cnic) if (!isdigit(c)) return false;
    return true;
}

bool isValidPassport(const string& passport) {
    if (passport.length() < 6 || passport.length() > 9) return false;
    for (char c : passport) if (!isalnum(c)) return false;
    return true;
}

void saveTouristToFile(const Tourist& tourist) {
    ofstream file(USER_FILE, ios::app);
    if (file.is_open()) {
        file << tourist.username << "," << tourist.password << "," 
             << tourist.cnicPassport << "," << tourist.age << "," 
             << tourist.email << "," << tourist.gender << "," 
             << tourist.medicalIssues << "," << tourist.currentLocation << endl;
        file.close();
    }
}

void updateTouristInFile(const Tourist& updatedTourist) {
    ifstream inFile(USER_FILE);
    vector<string> lines;
    string line;
    
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            stringstream ss(line);
            string username;
            getline(ss, username, ',');
            
            if (username == updatedTourist.username) {
                stringstream newLine;
                newLine << updatedTourist.username << "," << updatedTourist.password << "," 
                       << updatedTourist.cnicPassport << "," << updatedTourist.age << "," 
                       << updatedTourist.email << "," << updatedTourist.gender << "," 
                       << updatedTourist.medicalIssues << "," << updatedTourist.currentLocation;
                lines.push_back(newLine.str());
            } else lines.push_back(line);
        }
        inFile.close();
    }
    
    ofstream outFile(USER_FILE);
    for (const auto& l : lines) outFile << l << endl;
    outFile.close();
}

Tourist getTouristByUsername(const string& username) {
    Tourist tourist;
    ifstream file(USER_FILE);
    if (!file.is_open()) return tourist;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUsername;
        getline(ss, storedUsername, ',');
        
        if (storedUsername == username) {
            tourist.username = storedUsername;
            getline(ss, tourist.password, ',');
            getline(ss, tourist.cnicPassport, ',');
            
            string ageStr;
            getline(ss, ageStr, ',');
            if (!ageStr.empty()) tourist.age = stoi(ageStr);
            
            getline(ss, tourist.email, ',');
            getline(ss, tourist.gender, ',');
            getline(ss, tourist.medicalIssues, ',');
            getline(ss, tourist.currentLocation);
            
            file.close();
            return tourist;
        }
    }
    file.close();
    return tourist;
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool createDirectory(const string& path) {
    #ifdef _WIN32
        return _mkdir(path.c_str()) == 0 || errno == EEXIST;
    #else
        return mkdir(path.c_str(), 0777) == 0 || errno == EEXIST;
    #endif
}

string getCurrentDateTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string datetime(dt);
    datetime.erase(datetime.find('\n'), 1);
    return datetime;
}