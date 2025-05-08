#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

// External declarations
extern void initialize_units();
extern double convert_value(double value, const char *from, const char *to);

// Function to check if two doubles are approximately equal
bool approx_equal(double a, double b, double tolerance) {
    return fabs(a - b) < tolerance;
}

// Function to test a conversion and print result
void test_conversion(const char* category, const char* from_unit, const char* to_unit, 
                    double test_value, double expected_result) {
    char input[32];
    sprintf(input, "%.2f%s", test_value, from_unit);
    
    char from[16], to[16];
    strcpy(from, from_unit);
    strcpy(to, to_unit);
    
    double value = test_value;
    double result = convert_value(value, from, to);
    
    bool passed = approx_equal(result, expected_result, 0.0001);
    printf("%-15s: %10.4f %-6s -> %-6s = %10.4f : %s\n",
           category, test_value, from_unit, to_unit, result,
           passed ? "PASSED" : "FAILED");
    
    if (!passed) {
        printf("    Expected: %10.4f, Got: %10.4f\n", expected_result, result);
    }
}

// Test function that will be called from main program
void run_tests() {
    printf("\n=== Running Unit Tests ===\n");
    
    printf("\n=== Testing Length Conversions ===\n");
    test_conversion("Length", "m", "km", 1000.0, 1.0);      // 1000m = 1km
    test_conversion("Length", "km", "m", 1.0, 1000.0);      // 1km = 1000m
    test_conversion("Length", "m", "cm", 1.0, 100.0);       // 1m = 100cm
    test_conversion("Length", "cm", "mm", 1.0, 10.0);       // 1cm = 10mm
    test_conversion("Length", "m", "ft", 1.0, 3.28084);     // 1m = 3.28084ft
    test_conversion("Length", "ft", "in", 1.0, 12.0);       // 1ft = 12in
    test_conversion("Length", "mi", "km", 1.0, 1.60934);    // 1mi = 1.60934km
    
    printf("\n=== Testing Temperature Conversions ===\n");
    test_conversion("Temperature", "C", "F", 0.0, 32.0);     // 0°C = 32°F
    test_conversion("Temperature", "C", "K", 0.0, 273.15);   // 0°C = 273.15K
    test_conversion("Temperature", "F", "C", 32.0, 0.0);     // 32°F = 0°C
    test_conversion("Temperature", "K", "C", 273.15, 0.0);   // 273.15K = 0°C
    
    printf("\n=== Testing Mass Conversions ===\n");
    test_conversion("Mass", "kg", "g", 1.0, 1000.0);        // 1kg = 1000g
    test_conversion("Mass", "g", "mg", 1.0, 1000.0);        // 1g = 1000mg
    test_conversion("Mass", "kg", "lb", 1.0, 2.20462);      // 1kg = 2.20462lb
    test_conversion("Mass", "lb", "oz", 1.0, 16.0);         // 1lb = 16oz
    
    printf("\n=== Testing Time Conversions ===\n");
    test_conversion("Time", "hr", "min", 1.0, 60.0);        // 1hr = 60min
    test_conversion("Time", "min", "s", 1.0, 60.0);         // 1min = 60s
    test_conversion("Time", "day", "hr", 1.0, 24.0);        // 1day = 24hr
    test_conversion("Time", "week", "day", 1.0, 7.0);       // 1week = 7days
    
    printf("\n=== Testing Volume Conversions ===\n");
    test_conversion("Volume", "L", "mL", 1.0, 1000.0);      // 1L = 1000mL
    test_conversion("Volume", "gal", "L", 1.0, 3.78541);    // 1gal = 3.78541L
    test_conversion("Volume", "gal", "qt", 1.0, 4.0);       // 1gal = 4qt
    test_conversion("Volume", "qt", "pt", 1.0, 2.0);        // 1qt = 2pt
    
    printf("\n=== Testing Area Conversions ===\n");
    test_conversion("Area", "m2", "km2", 1000000.0, 1.0);   // 1000000m² = 1km²
    test_conversion("Area", "m2", "ft2", 1.0, 10.7639);     // 1m² = 10.7639ft²
    test_conversion("Area", "acre", "m2", 1.0, 4046.86);    // 1acre = 4046.86m²
    
    printf("\n=== Testing Digital Storage Conversions ===\n");
    test_conversion("Digital Storage", "B", "KB", 1024.0, 1.0);    // 1024B = 1KB
    test_conversion("Digital Storage", "KB", "MB", 1024.0, 1.0);   // 1024KB = 1MB
    test_conversion("Digital Storage", "MB", "GB", 1024.0, 1.0);   // 1024MB = 1GB
    test_conversion("Digital Storage", "GB", "TB", 1024.0, 1.0);   // 1024GB = 1TB
    
    printf("\n=== Testing Speed Conversions ===\n");
    test_conversion("Speed", "m/s", "km/h", 1.0, 3.6);      // 1m/s = 3.6km/h
    test_conversion("Speed", "km/h", "mph", 1.0, 0.621371); // 1km/h = 0.621371mph
    test_conversion("Speed", "kt", "km/h", 1.0, 1.852);     // 1kt = 1.852km/h
    
    printf("\n=== Testing Energy Conversions ===\n");
    test_conversion("Energy", "J", "cal", 4.184, 1.0);      // 4.184J = 1cal
    test_conversion("Energy", "kWh", "J", 1.0, 3600000.0);  // 1kWh = 3600000J
    
    printf("\n=== Testing Power Conversions ===\n");
    test_conversion("Power", "W", "kW", 1000.0, 1.0);       // 1000W = 1kW
    test_conversion("Power", "hp", "W", 1.0, 745.7);        // 1hp = 745.7W
    
    printf("\n=== Testing Pressure Conversions ===\n");
    test_conversion("Pressure", "Pa", "bar", 100000.0, 1.0); // 100000Pa = 1bar
    test_conversion("Pressure", "atm", "Pa", 1.0, 101325.0); // 1atm = 101325Pa
    test_conversion("Pressure", "psi", "Pa", 1.0, 6894.76);  // 1psi = 6894.76Pa
    
    printf("\n=== Test Suite Complete ===\n");
} 