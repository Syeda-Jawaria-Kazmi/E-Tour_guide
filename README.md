# E-Tour Guide Project

**E Tour Guide** is a sophisticated C++ console-based application designed to serve as a comprehensive travel companion for tourists exploring Pakistan. The system integrates multiple travel-related functionalities including route planning, transportation booking, hotel reservations, and travel record management. Built with object-oriented principles and advanced data structures, it provides users with an intuitive platform to plan and manage their entire travel itinerary across major Pakistani cities.

The application features a dual-mode transportation system (road and air travel), intelligent route finding, cost calculation, and personalized user profiles with medical considerations. It represents a complete travel management ecosystem that handles complex multi-segment journeys with varying transportation modes and accommodation options.

---

## Summary

This project implements a robust travel management system that enables users to:
* Create personalized travel profiles with medical and location information
* Plan multi-city trips with mixed transportation modes (car and plane)
* Book flight tickets with different class options
* Reserve hotels for specified durations
* Track travel history and current location
* Calculate comprehensive trip costs and time estimates

The system maintains persistent data storage for user profiles, travel records, trip details, and flight tickets. It features a color-coded console interface for enhanced user experience and implements sophisticated graph algorithms for optimal route finding between cities.

---

## Key Technical Achievements

* Implementation of graph-based route finding algorithms
* Multi-modal transportation planning
* Complex data persistence management
* Comprehensive error handling and input validation
* Real-time cost and time calculations

---

## Features

### User Registration & Authentication
* **Secure Signup:** Username availability checking
* **Comprehensive Profiles:** Creation with personal details and medical information
* **Identity Validation:** CNIC/Passport validation with format checking
* ✉️ **Verification:** Email validation and age verification
* 📍 **Location:** Current location tracking and updates

### 🧳 Transportation Planning
* 🛣️ **Dual-Mode Travel Options:** Road and air travel networks
* 🚘 **Car Travel:** Road network covering 18 major Pakistani cities
* ✈️ **Plane Travel:** Flight network between 8 airport cities
* 🔀 **Mixed Mode:** Car transfers to/from airports when needed

### 🗺️ Intelligent Route Finding
* 🛤️ **Multiple Options:** Multiple route options between any two cities
* 📍 **Shortest Path:** Shortest path identification
* ⏱️ **Metrics:** Real-time distance and travel time calculations
* 🚦 **Difficulty Classification:** Route difficulty classification (easy, medium, hard)

### ✈️ Flight Booking System
* 🎟️ **Comprehensive Management:** Flight scheduling and ticketing
* 💺 **Class Selection:** Economy and Business class options
* 💵 **Costing:** Real-time flight cost calculation based on distance
* 🔢 **Automation:** Automated flight number generation
* 📄 **Ticketing:** Detailed flight ticket generation and storage
* 🚌 **Transfers:** Airport transfer coordination for non-airport cities

### 🏨 Hotel Management
* 🛏️ **Accommodation Booking:** City-specific hotel databases
* ⭐ **Hotel Categories:** Multiple hotel categories (3-star to 5-star)
* 📅 **Flexible Stays:** Duration management for custom stays
* 💳 **Pricing:** Cost calculation per night
* 🏢 **Multi-Hotel Bookings:** Bookings across multiple hotels within the same city

### 🗺️ Trip Planning & Management
* 📌 **Multi-Segment Journeys:** Sequential destination addition
* 🔀 **Mixed Transit:** Mixed transportation modes within a single trip
* ⏰ **Scheduling:** Automatic time scheduling and coordination
* 📊 **Cost Aggregation:** Comprehensive expense calculation
* 🔄 **Updates:** Real-time itinerary updates

### 📂 Travel Records & History
* 💾 **Persistent Data:** Complete travel history tracking
* 📍 **Location Archive:** User location history and trip completion records
* 🎟️ **Ticket Archive:** Flight ticket archiving
* 📊 **Expense Tracking:** Budget reporting and tracking

### 🎨 Advanced UI/UX Features
* 🖥️ **Color-Coded Interface:** Professional visual layout for terminal screens
* 📂 **Section-Based Menus:** Organized console menus
* 🚦 **Message Differentiating:** Distinct success, error, and warning messages
* 💻 **Cross-Platform:** Formatting and color support across operating systems

---

# 🛠️ DSA Concepts Used

### 🕸️ Graph Theory
* **Implementation:** Adjacency list representation using `std::map<string, City>`
* **Application:**
  * Road network graph for car travel
  * Flight network graph for air travel
  * Route finding between cities
  * Shortest path calculations

### 🔍 Breadth-First Search (BFS)
* **Implementation:** `findAllPaths()` function using `std::queue`
* **Application:** Finding all possible routes between two cities
* **Complexity:** *O(V + E)* for each route finding operation

### 📊 Sorting Algorithms
* **Implementation:** `std::sort` with custom comparators
* **Application:** Sorting routes by distance for optimal selection

### 🔎 Search Algorithms
* **Linear Search:** User authentication, city validation
* **Binary Search:** Underlying operations within `std::map`

### 📦 Dynamic Data Structures
* Extensive use of STL containers for efficient memory management
* Memory-efficient storage of complex travel data

---

# 📦 Data Structures Used

### 🧩 STL Containers Utilization
* `std::vector` : Dynamic arrays for routes, segments, and hotels
* `std::map` : Efficient city lookups and graph representation
* `std::string` : Extensive string manipulation and storage
* `std::queue` : BFS implementation for route finding
* `std::stack` : Potential use in path reconstruction

### 📁 File System Data Structures
* Text-based persistent storage
* Directory management for organized data storage
* CSV-like formatting for user data

---

# 🧮 Algorithm and Complexity Analysis

### 🛣️ Route Finding Algorithm
`vector<vector<string>> findAllPaths(const string& start, const string& end, const map<string, City>& graph)`
* **BFS-based path finding:** Maximum 5 paths limit
* **Time Complexity:** *O(V + E)* for limited path finding
* **Space Complexity:** *O(V)* for queue and visited tracking

### 📏 Complexity Analysis
* **Time Complexity:** *O(V + E)* where V is vertices (cities) and E is edges (routes)
* **Space Complexity:** *O(V)* for storing paths and queue
* **Optimization:** Limits to 5 shortest paths for practical usage

### 🚗 Distance Calculation
`double calculateRouteDistance(const vector<string>& route, const map<string, City>& graph)`
* **Time Complexity:** *O(n)* where n is route length

### 🔑 User Authentication
* **Time Complexity:** *O(n)* for linear file search
* **Optimization:** Could be improved with hashing or database indexing

### 💰 Trip Cost Calculation
* **Time Complexity:** *O(m × n)* where m is segments and n is hotels per segment
* **Space Complexity:** *O(1)* for calculations

---

### 📊 Overall System Complexity

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **User Login** | *O(n)* | *O(1)* |
| **Route Finding** | *O(V + E)* | *O(V)* |
| **Trip Planning** | *O(m × n)* | *O(m × n)* |
| **File Operations** | *O(n)* | *O(1)* |

---

# 🛡️ Error Boundaries and Handling

### 📝 Input Validation
* `bool isValidEmail(const string& email);`
* `bool isValidCNIC(const string& cnic);`
* `bool isValidPassport(const string& passport);`
* `bool isUsernameExists(const string& username);`

### ⚙️ System Robustness
* **File System Errors:** Directory creation checks and fallbacks
* **Invalid User Input:** Comprehensive input validation with clear error messages
* **Graph Connectivity:** Route availability checks before planning
* **Memory Management:** Proper STL usage preventing memory leaks
* **Data Consistency:** Regular data validation during file operations

### 🔄 Exception Handling Strategies
* **Graceful Degradation:** System continues operation after non-critical errors
* **User Feedback:** Color-coded error messages for different severity levels
* **Data Recovery:** Robust file reading with partial data loading capability
* **Boundary Conditions:** Handling of edge cases in route finding and cost calculations

### 🎯 Specific Error Scenarios Handled
* **Non-existent routes:** Informative error messages with suggestions
* **Invalid dates:** Automatic date/time calculations with validation
* **File access issues:** Directory creation and fallback mechanisms
* **Memory constraints:** Efficient data structure usage
* **User input errors:** Comprehensive validation and retry mechanisms

---

# 📝 Conclusion

This project represents a significant achievement in combining theoretical computer science concepts with practical application development. It stands as a testament to effective problem-solving, systematic design, and comprehensive implementation in the domain of travel technology solutions. The E TOUR GUIDE system successfully delivers on its core objective: providing travelers with an intelligent, reliable, and comprehensive platform for planning and managing their journeys across Pakistan.
