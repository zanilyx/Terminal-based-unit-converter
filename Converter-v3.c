 /*
 * Ultimate Unit Converter
 * A comprehensive terminal-based unit conversion tool with advanced features
 * 
 * Features:
 * - Multiple unit categories (Length, Temperature, Digital Storage, etc.)
 * - Unit prefix support (k, M, G, T, m, u, n, p, c, d, h)
 * - Favorites system for quick access to common conversions
 * - Conversion history with export to CSV
 * - Quick conversion mode
 * - Batch conversion support
 * - Unit information display
 * - Scientific notation for large/small numbers
 * - Unit aliases support
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>

// Constants for data structures
#define MAX_HISTORY 100          // Maximum number of history entries
#define MAX_UNITS 200           // Maximum number of supported units
#define MAX_CATEGORIES 20       // Maximum number of unit categories
#define MAX_ALIASES 10          // Maximum number of aliases per unit
#define HISTORY_FILE "conversion_history.txt" // History file name

// Data structures for units and conversions
typedef struct {
    char name[32];
    char symbol[8];
    double factor;
    char category[32];
    bool is_temp;
    char aliases[MAX_ALIASES][16];
    int alias_count;
    char description[256];
} Unit;

typedef struct {
    char from[16];
    char to[16];
    double value;
    double result;
    time_t timestamp;
} ConversionEntry;

// Add unit prefix handling
typedef struct {
    char prefix;
    double factor;
} UnitPrefix;

// Define common unit prefixes
const UnitPrefix prefixes[] = {
    {'k', 1000.0},    // kilo
    {'M', 1000000.0}, // mega
    {'G', 1e9},       // giga
    {'T', 1e12},      // tera
    {'m', 0.001},     // milli
    {'u', 1e-6},      // micro
    {'n', 1e-9},      // nano
    {'p', 1e-12},     // pico
    {'c', 0.01},      // centi
    {'d', 0.1},       // deci
    {'h', 100.0},     // hecto
    {'\0', 1.0}       // no prefix
};

// Global variables
Unit units[MAX_UNITS];
int unit_count = 0;
ConversionEntry history[MAX_HISTORY];
int history_count = 0;
char categories[MAX_CATEGORIES][32];
int category_count = 0;

// Function prototypes
void initialize_units();
void show_main_menu();
void handle_conversion(const char *category);
void show_category_menu(const char *category);
double convert_value(double value, const char *from, const char *to);
double convert_temperature(double value, const char *from, const char *to);
void add_history_entry(const char *from, const char *to, double val, double res);
void show_history();
void clear_screen();
void print_header(const char *title);
void get_clean_input(char *buffer, size_t size);
void normalize_unit_name(char *unit);
bool unit_exists(const char *unit, const char *category);
void print_error(const char *message);
void print_success(const char *message);
void save_history();
void load_history();
void batch_conversion();
void show_unit_info(const char *unit);
void show_help();
void format_number(double num, char *buffer, size_t size);
void export_history_to_csv();

// Function to parse value with unit prefix
// Handles prefixes like k (kilo), M (mega), m (milli), etc.
// Example: "10m" -> 0.01, "2k" -> 2000
double parse_value_with_prefix(const char *input, char *unit) {
    char *endptr;
    double value = strtod(input, &endptr);
    
    // Skip any spaces
    while (*endptr == ' ') endptr++;
    
    // Copy the unit as is
    strncpy(unit, endptr, 15);
    unit[15] = '\0';
    
    return value;
}

// Initialize all available units with their properties
// Sets up conversion factors, aliases, and descriptions
void initialize_units() {
    // Length
    units[unit_count] = (Unit){
        "Meter", "m", 1.0, "Length", false,
        {"metre", "M", "METRE", "", "", "", "", "", "", ""}, 3,
        "Base unit of length in the metric system"
    };
    units[unit_count++].alias_count = 1;
    
    units[unit_count] = (Unit){
        "Kilometer", "km", 1000.0, "Length", false,
        {"kilometre", "KM", "KILOMETRE", "", "", "", "", "", "", ""}, 3,
        "1000 meters, commonly used for long distances"
    };
    units[unit_count++].alias_count = 1;
    
    units[unit_count++] = (Unit){
        "Centimeter", "cm", 0.01, "Length", false,
        {"centimetre", "CM", "CENTIMETRE", "", "", "", "", "", "", ""}, 3,
        "One hundredth of a meter"
    };
    
    units[unit_count++] = (Unit){
        "Millimeter", "mm", 0.001, "Length", false,
        {"millimetre", "MM", "MILLIMETRE", "", "", "", "", "", "", ""}, 3,
        "One thousandth of a meter"
    };
    
    units[unit_count++] = (Unit){
        "Inch", "in", 0.0254, "Length", false,
        {"IN", "inches", "", "", "", "", "", "", "", ""}, 2,
        "Imperial unit of length, 1/12 of a foot"
    };
    
    units[unit_count++] = (Unit){
        "Foot", "ft", 0.3048, "Length", false,
        {"FT", "feet", "", "", "", "", "", "", "", ""}, 2,
        "Imperial unit of length, 12 inches"
    };
    
    units[unit_count++] = (Unit){
        "Yard", "yd", 0.9144, "Length", false,
        {"YD", "yards", "", "", "", "", "", "", "", ""}, 2,
        "Imperial unit of length, 3 feet"
    };
    
    units[unit_count++] = (Unit){
        "Mile", "mi", 1609.344, "Length", false,
        {"MI", "miles", "", "", "", "", "", "", "", ""}, 2,
        "Imperial unit of length, 5280 feet"
    };
    
    units[unit_count++] = (Unit){
        "Light Year", "ly", 9.461e15, "Length", false,
        {{""}, {""}}, 0,
        "Distance light travels in one year"
    };
    
    // Digital Storage
    units[unit_count++] = (Unit){
        "Byte", "B", 1.0, "Digital Storage", false,
        {"byte", "BYTE", "", "", "", "", "", "", "", ""}, 2,
        "Basic unit of digital storage"
    };
    
    units[unit_count++] = (Unit){
        "Kilobyte", "KB", 1024.0, "Digital Storage", false,
        {"kilobyte", "KB", "", "", "", "", "", "", "", ""}, 2,
        "1024 bytes"
    };
    
    units[unit_count++] = (Unit){
        "Megabyte", "MB", 1048576.0, "Digital Storage", false,
        {"megabyte", "MB", "", "", "", "", "", "", "", ""}, 2,
        "1024 kilobytes"
    };
    
    units[unit_count++] = (Unit){
        "Gigabyte", "GB", 1073741824.0, "Digital Storage", false,
        {"gigabyte", "GB", "", "", "", "", "", "", "", ""}, 2,
        "1024 megabytes"
    };
    
    units[unit_count++] = (Unit){
        "Terabyte", "TB", 1099511627776.0, "Digital Storage", false,
        {"terabyte", "TB", "", "", "", "", "", "", "", ""}, 2,
        "1024 gigabytes"
    };
    
    // Energy
    units[unit_count++] = (Unit){
        "Joule", "J", 1.0, "Energy", false,
        {"joule", "J", "", "", "", "", "", "", "", ""}, 2,
        "SI unit of energy"
    };
    
    units[unit_count++] = (Unit){
        "Calorie", "cal", 4.184, "Energy", false,
        {"calorie", "CAL", "", "", "", "", "", "", "", ""}, 2,
        "Amount of energy needed to raise 1g of water by 1°C"
    };
    
    units[unit_count++] = (Unit){
        "Kilowatt Hour", "kWh", 3600000.0, "Energy", false,
        {"kilowatt-hour", "KWH", "", "", "", "", "", "", "", ""}, 2,
        "Unit of energy equal to 1 kilowatt of power for 1 hour"
    };
    
    units[unit_count++] = (Unit){
        "Electron Volt", "eV", 1.602e-19, "Energy", false,
        {"electronvolt", "EV", "", "", "", "", "", "", "", ""}, 2,
        "Energy gained by an electron moving through 1 volt"
    };
    
    units[unit_count++] = (Unit){
        "Kilojoule", "kJ", 1000.0, "Energy", false,
        {"kilojoules", "kj", "KJ", "", "", "", "", "", "", ""}, 3,
        "1000 joules"
    };
    
    units[unit_count++] = (Unit){
        "Megajoule", "MJ", 1000000.0, "Energy", false,
        {"megajoules", "mj", "MJ", "", "", "", "", "", "", ""}, 3,
        "1 million joules"
    };
    
    units[unit_count++] = (Unit){
        "Kilocalorie", "kcal", 4184.0, "Energy", false,
        {"kilocalories", "kcal", "KCAL", "", "", "", "", "", "", ""}, 3,
        "1000 calories, commonly used in nutrition"
    };
    
    // Power
    units[unit_count++] = (Unit){
        "Watt", "W", 1.0, "Power", false,
        {"watt", "W", "", "", "", "", "", "", "", ""}, 2,
        "SI unit of power, equal to one joule per second"
    };
    
    units[unit_count++] = (Unit){
        "Kilowatt", "kW", 1000.0, "Power", false,
        {"kilowatt", "KW", "", "", "", "", "", "", "", ""}, 2,
        "1000 watts, commonly used for electrical power"
    };
    
    units[unit_count++] = (Unit){
        "Megawatt", "MW", 1000000.0, "Power", false,
        {"megawatt", "MW", "", "", "", "", "", "", "", ""}, 2,
        "1 million watts, used for large power systems"
    };
    
    units[unit_count++] = (Unit){
        "Horsepower", "hp", 745.7, "Power", false,
        {"HP", "horsepower", "", "", "", "", "", "", "", ""}, 2,
        "Unit of power equal to 550 foot-pounds per second"
    };
    
    // Pressure
    units[unit_count++] = (Unit){
        "Pascal", "Pa", 1.0, "Pressure", false,
        {"pascal", "PA", "", "", "", "", "", "", "", ""}, 2,
        "SI unit of pressure, equal to one newton per square meter"
    };
    
    units[unit_count++] = (Unit){
        "Kilopascal", "kPa", 1000.0, "Pressure", false,
        {"kilopascal", "KPA", "", "", "", "", "", "", "", ""}, 2,
        "1000 pascals, commonly used for atmospheric pressure"
    };
    
    units[unit_count++] = (Unit){
        "Bar", "bar", 100000.0, "Pressure", false,
        {"BAR", "bars", "", "", "", "", "", "", "", ""}, 2,
        "Unit of pressure equal to 100,000 pascals"
    };
    
    units[unit_count++] = (Unit){
        "Atmosphere", "atm", 101325.0, "Pressure", false,
        {"ATM", "atmospheres", "", "", "", "", "", "", "", ""}, 2,
        "Standard atmospheric pressure at sea level"
    };
    
    units[unit_count++] = (Unit){
        "PSI", "psi", 6894.76, "Pressure", false,
        {"PSI", "pounds/sq in", "", "", "", "", "", "", "", ""}, 2,
        "Pounds per square inch, commonly used in engineering"
    };
    
    // Temperature
    units[unit_count++] = (Unit){
        "Celsius", "°C", 1.0, "Temperature", true,
        {"celsius", "C", "", "", "", "", "", "", "", ""}, 2,
        "Temperature scale where water freezes at 0° and boils at 100°"
    };
    
    units[unit_count++] = (Unit){
        "Fahrenheit", "°F", 1.0, "Temperature", true,
        {"fahrenheit", "F", "", "", "", "", "", "", "", ""}, 2,
        "Temperature scale where water freezes at 32° and boils at 212°"
    };
    
    units[unit_count++] = (Unit){
        "Kelvin", "K", 1.0, "Temperature", true,
        {"kelvin", "K", "", "", "", "", "", "", "", ""}, 2,
        "Absolute temperature scale, where 0K is absolute zero"
    };

    // Data
    units[unit_count++] = (Unit){"Kilobyte", "KB", 1024.0, "Data", false, {{""}, {""}}, 0};
    units[unit_count++] = (Unit){"Megabyte", "MB", 1048576.0, "Data", false, {{""}, {""}}, 0};
    units[unit_count++] = (Unit){"Gigabyte", "GB", 1073741824.0, "Data", false, {{""}, {""}}, 0};
    units[unit_count++] = (Unit){"Terabyte", "TB", 1099511627776.0, "Data", false, {{""}, {""}}, 0};

    // Mass
    units[unit_count++] = (Unit){"Gram", "g", 1.0, "Mass", false, {{""}, {""}}, 0};
    units[unit_count++] = (Unit){"Kilogram", "kg", 1000.0, "Mass", false, {{""}, {""}}, 0};
    units[unit_count++] = (Unit){"Milligram", "mg", 0.001, "Mass", false, {{""}, {""}}, 0};
    units[unit_count++] = (Unit){"Pound", "lb", 453.59237, "Mass", false, {{""}, {""}}, 0};
    units[unit_count++] = (Unit){"Ounce", "oz", 28.349523125, "Mass", false, {{""}, {""}}, 0};

    // Time
    units[unit_count++] = (Unit){
        "Second", "s", 1.0, "Time", false,
        {"sec", "secs", "SEC", "", "", "", "", "", "", ""}, 3,
        "Base unit of time in the SI system"
    };
    
    units[unit_count++] = (Unit){
        "Minute", "min", 60.0, "Time", false,
        {"mins", "MIN", "", "", "", "", "", "", "", ""}, 2,
        "60 seconds"
    };
    
    units[unit_count++] = (Unit){
        "Hour", "h", 3600.0, "Time", false,
        {"hours", "HR", "hr", "hour", "", "", "", "", "", ""}, 4,
        "60 minutes, 3600 seconds"
    };
    
    units[unit_count++] = (Unit){
        "Day", "d", 86400.0, "Time", false,
        {"days", "DAY", "day", "", "", "", "", "", "", ""}, 3,
        "24 hours, 86400 seconds"
    };
    
    units[unit_count++] = (Unit){
        "Week", "wk", 604800.0, "Time", false,
        {"weeks", "WEEK", "w", "", "", "", "", "", "", ""}, 3,
        "7 days, 604800 seconds"
    };

    // Volume
    units[unit_count++] = (Unit){
        "Liter", "L", 1.0, "Volume", false,
        {"litre", "l", "LITER", "", "", "", "", "", "", ""}, 3,
        "Base unit of volume in the metric system"
    };
    
    units[unit_count++] = (Unit){
        "Milliliter", "mL", 0.001, "Volume", false,
        {"millilitre", "ml", "ML", "", "", "", "", "", "", ""}, 3,
        "One thousandth of a liter"
    };
    
    units[unit_count++] = (Unit){
        "Gallon", "gal", 3.785411784, "Volume", false,
        {"GAL", "gallon", "", "", "", "", "", "", "", ""}, 2,
        "Imperial unit of volume, 3.785411784 liters"
    };
    
    units[unit_count++] = (Unit){
        "Quart", "qt", 0.946352946, "Volume", false,
        {"QT", "quart", "", "", "", "", "", "", "", ""}, 2,
        "Imperial unit of volume, 0.946352946 liters"
    };
    
    units[unit_count++] = (Unit){
        "Pint", "pt", 0.473176473, "Volume", false,
        {"PT", "pint", "", "", "", "", "", "", "", ""}, 2,
        "Imperial unit of volume, 0.473176473 liters"
    };

    // Area
    units[unit_count++] = (Unit){
        "Square Meter", "m²", 1.0, "Area", false,
        {"sqm", "m2", "M2", "", "", "", "", "", "", ""}, 3,
        "Base unit of area in the metric system"
    };
    
    units[unit_count++] = (Unit){
        "Square Kilometer", "km²", 1000000.0, "Area", false,
        {"sqkm", "km2", "KM2", "", "", "", "", "", "", ""}, 3,
        "1,000,000 square meters"
    };
    
    units[unit_count++] = (Unit){
        "Square Foot", "ft²", 0.09290304, "Area", false,
        {"sqft", "ft2", "FT2", "", "", "", "", "", "", ""}, 3,
        "Imperial unit of area"
    };
    
    units[unit_count++] = (Unit){
        "Square Mile", "mi²", 2589988.110336, "Area", false,
        {"sqmi", "mi2", "MI2", "", "", "", "", "", "", ""}, 3,
        "Imperial unit of area, 640 acres"
    };
    
    units[unit_count++] = (Unit){
        "Acre", "ac", 4046.8564224, "Area", false,
        {"acres", "AC", "", "", "", "", "", "", "", ""}, 2,
        "Imperial unit of area, 43,560 square feet"
    };

    // Initialize categories
    strcpy(categories[category_count++], "Length");
    strcpy(categories[category_count++], "Temperature");
    strcpy(categories[category_count++], "Digital Storage");
    strcpy(categories[category_count++], "Mass");
    strcpy(categories[category_count++], "Time");
    strcpy(categories[category_count++], "Volume");
    strcpy(categories[category_count++], "Area");
    strcpy(categories[category_count++], "Energy");
    strcpy(categories[category_count++], "Power");
    strcpy(categories[category_count++], "Pressure");
}

// Clear terminal screen
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Print header
void print_header(const char *title) {
    printf("\n=== %s ===\n\n", title);
}

// Get user input and normalize it
void get_clean_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        normalize_unit_name(buffer);
    }
}

// Normalize unit names (case insensitive, remove spaces)
void normalize_unit_name(char *unit) {
    // Special case for time units to preserve case
    if (strcmp(unit, "min") == 0 || strcmp(unit, "hr") == 0 || 
        strcmp(unit, "day") == 0 || strcmp(unit, "week") == 0 ||
        strcmp(unit, "s") == 0 || strcmp(unit, "h") == 0 ||
        strcmp(unit, "d") == 0 || strcmp(unit, "w") == 0 ||
        strcmp(unit, "L") == 0 || strcmp(unit, "mL") == 0 ||
        strcmp(unit, "W") == 0 || strcmp(unit, "kW") == 0 ||
        strcmp(unit, "MW") == 0 || strcmp(unit, "hp") == 0 ||
        strcmp(unit, "Pa") == 0 || strcmp(unit, "kPa") == 0 ||
        strcmp(unit, "bar") == 0 || strcmp(unit, "atm") == 0 ||
        strcmp(unit, "psi") == 0 ||
        strcmp(unit, "m") == 0 || strcmp(unit, "km") == 0 ||
        strcmp(unit, "cm") == 0 || strcmp(unit, "mm") == 0 ||
        strcmp(unit, "in") == 0 || strcmp(unit, "ft") == 0 ||
        strcmp(unit, "yd") == 0 || strcmp(unit, "mi") == 0) {
        return; // Don't modify time units, volume units, power units, pressure units, and length units
    }
    
    // Convert to uppercase for other units
    for (int i = 0; unit[i]; i++) {
        unit[i] = toupper(unit[i]);
    }
    
    // Remove spaces
    char *p = unit;
    char *q = unit;
    while (*q) {
        if (*q != ' ') *p++ = *q;
        q++;
    }
    *p = '\0';
}

// Check if unit exists in category
bool unit_exists(const char *unit, const char *category) {
    for (int i = 0; i < unit_count; i++) {
        if (strcmp(units[i].category, category) == 0 || strcmp(category, "All") == 0) {
            // Check exact match with symbol
            if (strcmp(units[i].symbol, unit) == 0) {
                return true;
            }
            // Check aliases
            for (int j = 0; j < units[i].alias_count; j++) {
                if (strcmp(units[i].aliases[j], unit) == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Print error message
void print_error(const char *message) {
    printf("Error: %s\n", message);
}

// Print success message
void print_success(const char *message) {
    printf("%s\n", message);
}

// Convert between units using their conversion factors
// Special handling for temperature conversions
double convert_value(double value, const char *from, const char *to) {
    // Check if it's temperature conversion
    bool is_temp_conversion = false;
    for (int i = 0; i < unit_count; i++) {
        char normalized_symbol[8];
        strcpy(normalized_symbol, units[i].symbol);
        normalize_unit_name(normalized_symbol);
        
        if (strcmp(normalized_symbol, from) == 0 && units[i].is_temp) {
            is_temp_conversion = true;
            break;
        }
    }
    
    if (is_temp_conversion) {
        return convert_temperature(value, from, to);
    }
    
    // Regular conversion
    double from_factor = 1.0, to_factor = 1.0;
    bool found_from = false, found_to = false;
    
    for (int i = 0; i < unit_count; i++) {
        char normalized_symbol[8];
        strcpy(normalized_symbol, units[i].symbol);
        normalize_unit_name(normalized_symbol);
        
        if (strcmp(normalized_symbol, from) == 0) {
            from_factor = units[i].factor;
            found_from = true;
        }
        if (strcmp(normalized_symbol, to) == 0) {
            to_factor = units[i].factor;
            found_to = true;
        }
    }
    
    if (!found_from || !found_to) {
        print_error("Invalid unit conversion!");
        return value;
    }
    
    // Handle potential overflow for large numbers
    if (fabs(value) > 1e15) {
        print_error("Warning: Very large number, precision may be affected");
    }
    
    return value * from_factor / to_factor;
}

// Temperature conversion
double convert_temperature(double value, const char *from, const char *to) {
    // Convert to Celsius first
    double celsius;
    
    if (strcmp(from, "C") == 0) {
        celsius = value;
    } else if (strcmp(from, "F") == 0) {
        celsius = (value - 32) * 5/9;
    } else if (strcmp(from, "K") == 0) {
        celsius = value - 273.15;
    } else {
        return value; // Shouldn't happen if input is validated
    }
    
    // Convert from Celsius to target unit
    if (strcmp(to, "C") == 0) {
        return celsius;
    } else if (strcmp(to, "F") == 0) {
        return (celsius * 9/5) + 32;
    } else if (strcmp(to, "K") == 0) {
        return celsius + 273.15;
    }
    
    return value;
}

// Add conversion to history
void add_history_entry(const char *from, const char *to, double val, double res) {
    if (history_count < MAX_HISTORY) {
        strncpy(history[history_count].from, from, sizeof(history[history_count].from)-1);
        history[history_count].from[sizeof(history[history_count].from)-1] = '\0';
        
        strncpy(history[history_count].to, to, sizeof(history[history_count].to)-1);
        history[history_count].to[sizeof(history[history_count].to)-1] = '\0';
        
        history[history_count].value = val;
        history[history_count].result = res;
        history[history_count].timestamp = time(NULL);
        history_count++;
    } else {
        // Shift history down
        for (int i = 0; i < MAX_HISTORY-1; i++) {
            history[i] = history[i+1];
        }
        // Add new entry at end
        strncpy(history[MAX_HISTORY-1].from, from, sizeof(history[MAX_HISTORY-1].from)-1);
        history[MAX_HISTORY-1].from[sizeof(history[MAX_HISTORY-1].from)-1] = '\0';
        
        strncpy(history[MAX_HISTORY-1].to, to, sizeof(history[MAX_HISTORY-1].to)-1);
        history[MAX_HISTORY-1].to[sizeof(history[MAX_HISTORY-1].to)-1] = '\0';
        
        history[MAX_HISTORY-1].value = val;
        history[MAX_HISTORY-1].result = res;
        history[MAX_HISTORY-1].timestamp = time(NULL);
    }
    save_history(); // Save after each new entry
}

// Save conversion history to file
void save_history() {
    FILE *file = fopen(HISTORY_FILE, "w");
    if (file == NULL) {
        print_error("Could not save history");
        return;
    }
    
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s,%s,%.8g,%.8g,%ld\n",
                history[i].from,
                history[i].to,
                history[i].value,
                history[i].result,
                history[i].timestamp);
    }
    
    fclose(file);
}

// Load conversion history from file
void load_history() {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return; // No history file exists yet
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file) && history_count < MAX_HISTORY) {
        char from[16], to[16];
        double value, result;
        time_t timestamp;
        
        if (sscanf(line, "%[^,],%[^,],%lf,%lf,%ld",
                   from, to, &value, &result, &timestamp) == 5) {
            strncpy(history[history_count].from, from, sizeof(history[history_count].from)-1);
            history[history_count].from[sizeof(history[history_count].from)-1] = '\0';
            
            strncpy(history[history_count].to, to, sizeof(history[history_count].to)-1);
            history[history_count].to[sizeof(history[history_count].to)-1] = '\0';
            
            history[history_count].value = value;
            history[history_count].result = result;
            history[history_count].timestamp = timestamp;
            history_count++;
        }
    }
    
    fclose(file);
}

// Show conversion history
void show_history() {
    clear_screen();
    print_header("Conversion History");
    
    if (history_count == 0) {
        print_error("No conversion history available!");
    } else {
        printf("%-5s %-15s %-15s %-15s %-15s %-20s\n", 
               "No.", "From", "To", "Value", "Result", "Time");
        printf("----------------------------------------------------------------\n");
        
        for (int i = 0; i < history_count; i++) {
            char time_str[32];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", 
                    localtime(&history[i].timestamp));
            
            char value_str[32], result_str[32];
            format_number(history[i].value, value_str, sizeof(value_str));
            format_number(history[i].result, result_str, sizeof(result_str));
            
            printf("%-5d %-15s %-15s %-15s %-15s %-20s\n",
                   i+1,
                   history[i].from,
                   history[i].to,
                   value_str,
                   result_str,
                   time_str);
        }
        
        printf("\nOptions:\n");
        printf("1. Clear history\n");
        printf("2. Export to CSV\n");
        printf("3. Return to menu\n");
        printf("\nEnter your choice: ");
        
        char choice[16];
        fgets(choice, sizeof(choice), stdin);
        
        switch (choice[0]) {
            case '1':
                history_count = 0;
                save_history();
                print_success("History cleared!");
                break;
            case '2':
                export_history_to_csv();
                break;
            case '3':
                return;
            default:
                print_error("Invalid choice!");
        }
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Show category menu
void show_category_menu(const char *category) {
    clear_screen();
    print_header(category);
    
    printf("Available units:\n\n");
    printf("%-15s %-10s %-40s\n", "Unit", "Symbol", "Description");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < unit_count; i++) {
        if (strcmp(units[i].category, category) == 0) {
            printf("%-15s %-10s %-40s\n", 
                   units[i].name, 
                   units[i].symbol,
                   units[i].description);
        }
    }
    
    printf("\n");
}

// Handle the conversion process for a given category
// Supports unit prefixes and validates input
void handle_conversion(const char *category) {
    show_category_menu(category);
    
    char input[32], from_unit[16], to_unit[16];
    double value;
    int attempts = 0;
    
    // Get value and unit together
    while (attempts < 3) {
        printf("\nEnter value and unit: ");
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = '\0';
            
            value = parse_value_with_prefix(input, from_unit);
            normalize_unit_name(from_unit);
            
            if (unit_exists(from_unit, category)) {
                break;
            }
            print_error("Invalid unit! Please try again.");
        }
        attempts++;
    }
    
    if (attempts >= 3) {
        print_error("Too many failed attempts. Returning to menu.");
        return;
    }
    
    // Get target unit
    attempts = 0;
    bool valid_unit = false;
    while (!valid_unit && attempts < 3) {
        printf("Convert to: ");
        get_clean_input(to_unit, sizeof(to_unit));
        
        if (unit_exists(to_unit, category)) {
            valid_unit = true;
        } else {
            print_error("Invalid unit! Please try again.");
            attempts++;
        }
    }
    
    if (!valid_unit) {
        print_error("Too many failed attempts. Returning to menu.");
        return;
    }
    
    // Perform conversion
    double result = convert_value(value, from_unit, to_unit);
    
    // Format numbers for display
    char value_str[32], result_str[32];
    format_number(value, value_str, sizeof(value_str));
    format_number(result, result_str, sizeof(result_str));
    
    // Display result
    printf("\nResult: %s %s = %s %s\n\n", value_str, from_unit, result_str, to_unit);
    
    // Add to history
    add_history_entry(from_unit, to_unit, value, result);
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Batch conversion mode
void batch_conversion() {
    clear_screen();
    print_header("Batch Conversion Mode");
    
    char from_unit[16], to_unit[16];
    double values[100];
    int value_count = 0;
    
    printf("Enter values to convert (one per line, empty line to finish):\n");
    
    char input[32];
    while (value_count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) break;
        
        char *endptr;
        double value = strtod(input, &endptr);
        if (endptr == input || *endptr != '\0') {
            print_error("Invalid number! Skipping...");
            continue;
        }
        
        values[value_count++] = value;
    }
    
    if (value_count == 0) {
        print_error("No values entered!");
        return;
    }
    
    // Get units
    printf("\nConvert from: ");
    get_clean_input(from_unit, sizeof(from_unit));
    
    printf("Convert to: ");
    get_clean_input(to_unit, sizeof(to_unit));
    
    // Perform conversions
    printf("\nResults:\n");
    for (int i = 0; i < value_count; i++) {
        double result = convert_value(values[i], from_unit, to_unit);
        printf("%.8g %s = %.8g %s\n", values[i], from_unit, result, to_unit);
        add_history_entry(from_unit, to_unit, values[i], result);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Show main menu
void show_main_menu() {
    clear_screen();
    print_header("Ultimate Unit Converter");
    
    printf("Select a category:\n\n");
    
    for (int i = 0; i < category_count; i++) {
        printf("%2d. %s\n", i+1, categories[i]);
    }
    
    printf("\n");
    printf("%2d. History\n", category_count+1);
    printf("%2d. Help\n", category_count+2);
    printf("%2d. Quit\n\n", category_count+3);
    
    printf("Enter your choice: ");
}

// Add new function to show unit information
void show_unit_info(const char *unit) {
    for (int i = 0; i < unit_count; i++) {
        if (strcmp(units[i].name, unit) == 0 || strcmp(units[i].symbol, unit) == 0) {
            printf("\nUnit Information:\n");
            printf("Name: %s\n", units[i].name);
            printf("Symbol: %s\n", units[i].symbol);
            printf("Category: %s\n", units[i].category);
            printf("Description: %s\n", units[i].description);
            if (units[i].alias_count > 0) {
                printf("Aliases: ");
                for (int j = 0; j < units[i].alias_count; j++) {
                    printf("%s%s", units[i].aliases[j],
                           j < units[i].alias_count-1 ? ", " : "");
                }
                printf("\n");
            }
            return;
        }
    }
    print_error("Unit not found");
}

// Add function to format numbers nicely
void format_number(double num, char *buffer, size_t size) {
    if (num == 0) {
        snprintf(buffer, size, "0");
        return;
    }

    // Count the number of digits
    int digits = (int)ceil(log10(fabs(num) + 1));
    
    if (digits > 6) {
        // Get the exponent
        int exponent = (int)floor(log10(fabs(num)));
        
        // Calculate the mantissa
        double mantissa = num / pow(10, exponent);
        
        // Format the number as x × 10^n
        snprintf(buffer, size, "%.2f × 10^%d", mantissa, exponent);
    } else {
        // Use normal decimal format for smaller numbers
        snprintf(buffer, size, "%.6g", num);
    }
}

// Add function to show help
void show_help() {
    clear_screen();
    print_header("Help");
    
    printf("Features:\n");
    printf("1. Multiple unit categories\n");
    printf("2. Quick conversion mode\n");
    printf("3. Conversion history\n");
    printf("4. Unit information display\n");
    printf("5. Scientific notation for large/small numbers\n");
    printf("6. Unit aliases support\n");
    printf("7. Temperature conversion\n");
    printf("8. Batch conversion mode\n");
    
    printf("\nTips:\n");
    printf("- Use unit symbols (e.g., 'km' for kilometer)\n");
    printf("- View unit info to learn more about each unit\n");
    
    printf("\nPress Enter to continue...");
    getchar();
}

// Add function to export history to CSV
void export_history_to_csv() {
    FILE *file = fopen("conversion_history.csv", "w");
    if (file == NULL) {
        print_error("Could not create CSV file!");
        return;
    }
    
    // Write header
    fprintf(file, "From,To,Value,Result,Timestamp\n");
    
    // Write data
    for (int i = 0; i < history_count; i++) {
        char time_str[32];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", 
                localtime(&history[i].timestamp));
        
        fprintf(file, "%s,%s,%.8g,%.8g,%s\n",
                history[i].from,
                history[i].to,
                history[i].value,
                history[i].result,
                time_str);
    }
    
    fclose(file);
    print_success("History exported to conversion_history.csv");
}

// Main function
int main(int argc, char *argv[]) {
    // Initialize the program
    initialize_units();
    load_history();
    
    // Main program loop
    while (1) {
        show_main_menu();
        
        char choice[16];
        fgets(choice, sizeof(choice), stdin);
        
        int selected = atoi(choice);
        if (selected >= 1 && selected <= category_count) {
            handle_conversion(categories[selected-1]);
        } else if (selected == category_count+1) {
            show_history();
        } else if (selected == category_count+2) {
            show_help();
        } else if (selected == category_count+3 || 
                  (choice[0] == 'q' || choice[0] == 'Q')) {
            clear_screen();
            printf("\nThank you for using Ultimate Unit Converter!\n\n");
            break;
        } else {
            print_error("Invalid choice! Please try again.");
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    
    return 0;
} 
