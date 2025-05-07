# Ultimate Unit Converter

A powerful command-line unit converter with advanced features and a clean, user-friendly interface.

## Features

- **Multiple Categories**: Length, Temperature, Digital Storage, Mass, Time, Volume, Area, Speed, Energy, Power, Pressure
- **Unit Prefix Support**: Handles prefixes like kilo (k), mega (M), milli (m), etc.
- **Favorites System**: Save and manage your most-used conversions
- **Conversion History**: Track your conversions with timestamps
- **Export to CSV**: Export conversion history for analysis
- **Quick Conversion Mode**: Fast access to common conversions
- **Batch Conversion**: Convert multiple values at once
- **Unit Information**: Detailed information about each unit
- **Scientific Notation**: Handles both small and large numbers
- **Unit Aliases**: Support for alternative unit names
- **Temperature Conversion**: Special handling for temperature units

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/zanilyx/Terminal-based-unit-converter.git
   cd Terminal-based-unit-converter
   ```

2. Compile the program:
   ```bash
   gcc Converter-v3.c -o converter -lm
   ```

## Usage

Run the program:
```bash
./converter
```

### Unit Prefixes
- k (kilo) = 1000
- M (mega) = 1,000,000
- G (giga) = 1e9
- T (tera) = 1e12
- m (milli) = 0.001
- u (micro) = 1e-6
- n (nano) = 1e-9
- p (pico) = 1e-12
- c (centi) = 0.01
- d (deci) = 0.1
- h (hecto) = 100

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
- Light Year (ly)

### Temperature
- Celsius (C)
- Fahrenheit (F)
- Kelvin (K)

### Digital Storage
- Bit (b)
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

### Area
- Square Meter (m²)
- Square Kilometer (km²)
- Square Foot (ft²)
- Square Mile (mi²)
- Acre (ac)

### Speed
- Meter per Second (m/s)
- Kilometer per Hour (km/h)
- Mile per Hour (mph)
- Knot (kt)

### Energy
- Joule (J)
- Calorie (cal)
- Kilowatt Hour (kWh)
- Electron Volt (eV)

### Power
- Watt (W)
- Horsepower (hp)
- Kilowatt (kW)

### Pressure
- Pascal (Pa)
- Bar (bar)
- Atmosphere (atm)
- PSI (psi)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thanks to all contributors who have helped improve this project
- Inspired by the need for a simple yet powerful unit conversion tool 