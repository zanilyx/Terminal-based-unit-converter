# Terminal Unit Converter

A powerful command-line unit converter that supports multiple categories of unit conversions with a clean, user-friendly interface.

## Features

- **Multiple Categories**: Length, Temperature, Data, Mass, Time, Volume, and more
- **Scientific Notation Support**: Handles both small and large numbers
- **Conversion History**: Keeps track of your recent conversions
- **Direct Command Line Usage**: Quick conversions without entering interactive mode
- **Case-Insensitive Input**: Accepts units in any case (KM, km, Km)
- **Error Handling**: Robust input validation and error messages
- **Temperature Conversion**: Special handling for temperature units

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/terminal-unit-converter.git
cd terminal-unit-converter
```

2. Compile the program:
```bash
gcc -std=c17 -o converter Converter-v3.c -lm
```

## Usage

### Interactive Mode
```bash
./converter
```

### Direct Conversion
```bash
./converter [value] [from_unit] [to_unit]
```

Example:
```bash
./converter 100 km mi
```

## Supported Categories

### Length
- Meter (m)
- Kilometer (km)
- Centimeter (cm)
- Millimeter (mm)
- Inch (in)
- Foot (ft)
- Yard (yd)
- Mile (mi)

### Temperature
- Celsius (C)
- Fahrenheit (F)
- Kelvin (K)

### Data
- Byte (B)
- Kilobyte (KB)
- Megabyte (MB)
- Gigabyte (GB)
- Terabyte (TB)

### Mass
- Gram (g)
- Kilogram (kg)
- Milligram (mg)
- Pound (lb)
- Ounce (oz)

### Time
- Second (s)
- Minute (min)
- Hour (hr)
- Day (day)
- Week (week)

### Volume
- Liter (L)
- Milliliter (mL)
- Gallon (gal)
- Quart (qt)
- Pint (pt)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details. 