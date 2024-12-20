#include <iostream>
#include <string>
#include <limits>
#include <map>
#include <vector>
#include <cmath>
#include "funcs.h"
#include <algorithm> // For std::transform


void press_to_continue() {
    std::cout << "\nPress Enter to continue...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Function that clears the screen
void clearscreen() {
#ifdef _WIN32
    system("cls");  // For Windows
#elif __APPLE__
    system("clear");  // For MacOS
#else
    system("clear");  // For Linux 
#endif
}


// Function that only allows a positive input
bool validate_positive_input(double& value, const std::string& prompt) {
    std::cout << prompt;
    std::cin >> value;
    if (std::cin.fail() || value <= 0) { // checks if input is less than negative
        std::cin.clear(); // Clear error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cerr << "Error: Value must be a positive number.\n";
        return false;
    }
    return true;
}

// Function to calculate cutoff frequency
double calculate_cutoff_frequency(double r, double c, double factor = 1.0) {
    return 1 / (r * c * 2 * 3.14 * factor);
}

// Function to display cutoff frequency with units
void display_cutoff_frequency(float cutoff_freq) {
    if (cutoff_freq > 1e6) {
        std::cout << "\nThe cutoff frequency is " << cutoff_freq / 1e6 << " MHz\n";
    }
    else if (cutoff_freq > 1e3) {
        std::cout << "\nThe cutoff frequency is " << cutoff_freq / 1e3 << " kHz\n";
    }
    else {
        std::cout << "\nThe cutoff frequency is " << cutoff_freq << " Hz\n";
    }
}

// Function to convert resistance based on the unit
void resistor_input(double raw_resist, double& resistance, std::string& unit) {
    bool valid = false;
    while (!valid) {
        if (unit == "k" || unit == "K") {
            resistance = raw_resist * 1e3; // Kilo-ohms to ohms
            valid = true;
        }
        else if (unit == "M" || unit == "m") {
            resistance = raw_resist * 1e6; // Mega-ohms to ohms
            valid = true;
        }
        else if (unit == "O" || unit == "o") {
            resistance = raw_resist; // Ohms
            valid = true;
        }
        else {
            std::cerr << "Invalid unit. Please use 'k' for kilo-ohms, 'M' for mega-ohms, or 'O' for ohms.\n";
            std::cout << "Enter a valid unit: ";
            std::cin >> unit; // Re-prompt for the unit
        }
    }
}

// Function to convert frequency based on the unit
void Fc_input(double raw_freq, double& frequency, std::string& unit) {
    bool valid = false; // Validation flag
    while (!valid) {
        if (unit == "k" || unit == "K") {
            frequency = raw_freq * 1e3; // Kilo Hz to Hz
            valid = true;
        }
        else if (unit == "M" || unit == "m") {
            frequency = raw_freq * 1e6; // Mega Hz to Hz
            valid = true;
        }
        else if (unit == "H" || unit == "h") {
            frequency = raw_freq; // Hertz
            valid = true;
        }
        else {
            std::cerr << "Invalid unit. Please use 'k' for Kilo Hz, 'M' for Mega Hz, or 'H' for Hertz.\n";
            std::cout << "Enter a valid unit: ";
            std::cin >> unit; // Re-prompt for unit
        }
    }
}
// Function to convert capacitance based on the unit
void capacitor_input(double raw_cap, double& capacitance, std::string& unit) {
    bool valid = false; // Validation flag
    while (!valid) {
        if (unit == "u") { // Microfarads to farads
            capacitance = raw_cap * 1e-6;
            valid = true;
        }
        else if (unit == "n") { // Nanofarads to farads
            capacitance = raw_cap * 1e-9;
            valid = true;
        }
        else if (unit == "p") { // Picofarads to farads
            capacitance = raw_cap * 1e-12;
            valid = true;
        }
        else {
            std::cerr << "Invalid unit. Please use u (microfarads), n (nanofarads), or p (picofarads).\n";
            std::cout << "Enter a valid unit: ";
            std::cin >> unit; // Re-prompt for unit
        }
    }
}
// Function to return to main menu
void go_back_to_main() {
    std::string input;
    do {
        std::cout << "\nEnter 'b' or 'B' to go back to main menu: ";
        std::cin >> input;
        clearscreen();

    } while (input != "b" && input != "B");
}



void menu_item_1() {
    int choice;
    do {
        clearscreen();  // Clear the screen at the start of the menu
        std::cout << "\n--- Resistor Calculator ---\n";
        std::cout << "1. Calculate resistance from color codes\n";
        std::cout << "2. Solve Resistor Network\n";
        std::cout << "3. Find nearest NPV resistor\n";
        std::cout << "4. Get NPV value and color code for a resistor\n";
        std::cout << "5. Back to main menu\n";
        std::cout << "Select an option: ";
        std::cin >> choice;

        // Block invalid input until correct
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 1 and 5: ";
            std::cin >> choice;
        }

        switch (choice) {
            case 1:
                clearscreen();
                calculate_resistor_from_color_code();
                break;
            case 2:
                clearscreen();
                combine_resistors();
                break;
            case 3:
                clearscreen();
                find_nearest_npv_resistor();
                break;
            case 4: {
                clearscreen();
                double resistor_value;
                std::cout << "Enter resistor value (in ohms): ";
                std::cin >> resistor_value;
                get_npv_and_color_code_for_resistor(resistor_value);
                break;
            }
            case 5:
                clearscreen();
                std::cout << "Returning to main menu...\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
        }

        if (choice != 5) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    } while (choice != 5);
}

void calculate_resistor_from_color_code() {
    clearscreen();  // Clear screen at the beginning of the function

    // Define color-to-value and multiplier maps
    std::map<std::string, int> color_code = {
        {"black", 0}, {"brown", 1}, {"red", 2}, {"orange", 3}, {"yellow", 4},
        {"green", 5}, {"blue", 6}, {"violet", 7}, {"gray", 8}, {"white", 9}
    };
    std::map<std::string, double> multipliers = {
        {"black", 1}, {"brown", 10}, {"red", 100}, {"orange", 1000}, {"yellow", 10000},
        {"green", 100000}, {"blue", 1000000}, {"gold", 0.1}, {"silver", 0.01}
    };

    // Helper lambda to convert a string to lowercase
    auto to_lower = [](std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    };

    std::string band1, band2, multiplier;
    
    std::cout << "Enter first color band: ";
    std::cin >> band1;
    to_lower(band1); // Convert to lowercase
    std::cout << "Enter second color band: ";
    std::cin >> band2;
    to_lower(band2); // Convert to lowercase
    std::cout << "Enter multiplier band: ";
    std::cin >> multiplier;
    to_lower(multiplier); // Convert to lowercase

    // Validate the color bands
    if (color_code.find(band1) == color_code.end() ||
        color_code.find(band2) == color_code.end() ||
        multipliers.find(multiplier) == multipliers.end()) {
        std::cout << "Invalid color code entered. Please try again.\n";
        return;
    }

    // Calculate the resistance
    int significant_digits = color_code[band1] * 10 + color_code[band2];
    double resistance = significant_digits * multipliers[multiplier];
    
    clearscreen();  // Clear screen before displaying the result

    std::cout << "Resistance: " << resistance << " ohms\n";
}


void combine_resistors() {
    clearscreen();  // Clear the screen at the beginning of the function

    int num_series, num_parallel;

    // Input for series resistors
    std::cout << "Enter the number of resistors in series: ";
    std::cin >> num_series;

    std::vector<double> series_resistances(num_series);
    double total_series_resistance = 0;

    for (int i = 0; i < num_series; i++) {
        std::cout << "Enter value of series resistor " << i + 1 << " (in ohms): ";
        std::cin >> series_resistances[i];
        total_series_resistance += series_resistances[i]; // Sum up for series
    }
    std::cout << "Total resistance of resistors in series: " << total_series_resistance << " ohms\n";

    // Input for parallel resistors
    std::cout << "Enter the number of resistors in parallel: ";
    std::cin >> num_parallel;

    std::vector<double> parallel_resistances(num_parallel);
    double total_inverse_parallel = 0;

    for (int i = 0; i < num_parallel; i++) {
        std::cout << "Enter value of parallel resistor " << i + 1 << " (in ohms): ";
        std::cin >> parallel_resistances[i];
        if (parallel_resistances[i] == 0) {
            std::cout << "Error: Resistor value cannot be zero in parallel combination.\n";
            return;
        }
        total_inverse_parallel += 1 / parallel_resistances[i]; // Sum of inverses for parallel
    }

    double total_parallel_resistance = 1 / total_inverse_parallel;
    std::cout << "Total resistance of resistors in parallel: " << total_parallel_resistance << " ohms\n";

    clearscreen();  // Clear screen before showing the final combination

    // Combine the two results
    int combination_type;
    std::cout << "Enter 1 to combine the series and parallel resistances in series, or 2 to combine them in parallel: ";
    std::cin >> combination_type;

    if (combination_type == 1) {
        // Combine in series
        double combined_resistance = total_series_resistance + total_parallel_resistance;
        std::cout << "Total combined resistance (series): " << combined_resistance << " ohms\n";
    }
    else if (combination_type == 2) {
        // Combine in parallel
        double combined_inverse = (1 / total_series_resistance) + (1 / total_parallel_resistance);
        double combined_resistance = 1 / combined_inverse;
        std::cout << "Total combined resistance (parallel): " << combined_resistance << " ohms\n";
    }
    else {
        std::cout << "Invalid combination type. Try again.\n";
    }
}

void find_nearest_npv_resistor() {
    clearscreen();  // Clear the screen at the beginning of the function

    std::vector<double> npv_resistors = {
        1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2,
        10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82,
        100, 120, 150, 180, 220, 270, 330, 390, 470, 560, 680, 820,
        1000, 1200, 1500, 1800, 2200, 2700, 3300, 3900, 4700, 5600, 6800, 8200,
        10000, 12000, 15000, 18000, 22000, 27000, 33000, 39000, 47000, 56000, 68000, 82000,
        100000, 120000, 150000, 180000, 220000, 270000, 330000, 390000, 470000, 560000, 680000, 820000,
        1000000, 1200000, 1500000, 1800000, 2200000, 2700000, 3300000, 3900000, 4700000, 5600000, 6800000, 8200000,
        10000000
    };

    double target_resistance;
    std::cout << "Enter target resistance (in ohms): ";
    std::cin >> target_resistance;

    if (target_resistance <= 0) {
        std::cout << "Invalid resistance value. Must be greater than zero.\n";
        return;
    }

    double closest_resistor = npv_resistors[0];
    double min_difference = std::abs(target_resistance - closest_resistor);

    for (double resistor : npv_resistors) {
        double difference = std::abs(target_resistance - resistor);
        if (difference < min_difference) {
            closest_resistor = resistor;
            min_difference = difference;
        }
    }

    std::cout << "Nearest NPV resistor: " << closest_resistor << " ohms\n";

    // Suggest combination if exact match is not found
    if (closest_resistor != target_resistance) {
        clearscreen();  // Clear screen before suggesting combinations
        std::cout << "Suggested combinations: \n";
        for (double r1 : npv_resistors) {
            for (double r2 : npv_resistors) {
                if (std::abs((r1 + r2) - target_resistance) < min_difference) {
                    std::cout << "Series: " << r1 << " ohms + " << r2 << " ohms\n";
                }
                if (std::abs((1 / ((1 / r1) + (1 / r2))) - target_resistance) < min_difference) {
                    std::cout << "Parallel: " << r1 << " ohms || " << r2 << " ohms\n";
                }
            }
        }
    }
}

void get_npv_and_color_code_for_resistor(double resistance) {

   
    std::vector<double> npv_resistors = {
        1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2,
        10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82,
        100, 120, 150, 180, 220, 270, 330, 390, 470, 560, 680, 820,
        1000, 1200, 1500, 1800, 2200, 2700, 3300, 3900, 4700, 5600, 6800, 8200,
        10000, 12000, 15000, 18000, 22000, 27000, 33000, 39000, 47000, 56000, 68000, 82000,
        100000, 120000, 150000, 180000, 220000, 270000, 330000, 390000, 470000, 560000, 680000, 820000,
        1000000, 1200000, 1500000, 1800000, 2200000, 2700000, 3300000, 3900000, 4700000, 5600000, 6800000, 8200000,
        10000000 // E12 series
    };

    // Maps for color code
    std::map<int, std::string> digit_to_color = {
        {0, "black"}, {1, "brown"}, {2, "red"}, {3, "orange"}, {4, "yellow"},
        {5, "green"}, {6, "blue"}, {7, "violet"}, {8, "gray"}, {9, "white"}
    };
    std::map<int, std::string> multiplier_to_color = {
        {0, "black"}, {1, "brown"}, {2, "red"}, {3, "orange"}, {4, "yellow"},
        {5, "green"}, {6, "blue"}, {7, "violet"}, {8, "gray"}, {9, "white"},
        {-1, "gold"}, {-2, "silver"}
    };

    // Find the closest NPV resistor
    double closest_resistor = npv_resistors[0];
    double min_difference = std::abs(resistance - closest_resistor);

    for (double resistor : npv_resistors) {
        double difference = std::abs(resistance - resistor);
        if (difference < min_difference) {
            closest_resistor = resistor;
            min_difference = difference;
        }
    }

    std::cout << "Nearest NPV resistor: " << closest_resistor << " ohms\n";

    // Calculate color code
    int magnitude = static_cast<int>(std::log10(closest_resistor));
    double normalized_value = closest_resistor / std::pow(10, magnitude);
    // Adjust for edge cases where normalized value is exactly 10
    if (normalized_value >= 10.0) {
        normalized_value /= 10;
        ++magnitude;
    }

    int first_digit = static_cast<int>(normalized_value);
    int second_digit = static_cast<int>((normalized_value - first_digit) * 10);

  

    if (digit_to_color.find(first_digit) != digit_to_color.end() &&
        digit_to_color.find(second_digit) != digit_to_color.end() &&
        multiplier_to_color.find(magnitude) != multiplier_to_color.end()) {
        std::string first_band = digit_to_color[first_digit];
        std::string second_band = digit_to_color[second_digit];
        std::string multiplier_band = multiplier_to_color[magnitude];

        // Outputs the color code of the input resistor
        std::cout << "Color Code: [" << first_band << ", " << second_band << ", " << multiplier_band << "]\n";
    }
    else {
        std::cout << "Error: Unable to calculate color code for this resistor.\n";
    }
}
void menu_item_2() {
    int choice;
    double inverting_input_voltage;
    double non_inverting_input_voltage;
    double feedback_resistor;
    double input_resistor;
    double ground_resistor;
    double gain;
    std::string repeat_choice, unit;
    do {
        clearscreen();
        std::cout << "\n>> The Op-Amp \n";

        // Display a sub-menu for selecting the op-amp configuration
        std::cout << "\nOp-Amp Configuration:\n";
        std::cout << "1. Inverting Op-Amp\n";
        std::cout << "2. Non-Inverting Op-Amp\n";
        std::cout << "3. Back to main menu\n";
        std::cout << "Select choice: ";
        std::cin >> choice;

        // Blocks invalid input until correct
        while (std::cin.fail()) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 1 and 3.\n";
                std::cout << "Enter an integer!\n";
                std::cin >> choice;
            }
        }

        double raw_resist;
        std::string unit;

        // Perform calculations based on the user's choice
        if (choice == 1) {
            // Inverting Op-Amp
            std::cout << "\n>> Inverting Op-Amp Configuration\n";
            std::cout << R"(
                         *----| FR |----*
                         |              |
                         |     |\       |
            Vin--| IR |--*-----|-\      |
                               |  \_____*____Vout
                               |  /     
                         *-----|+/                         
                         |     |/       
                       -----
                        --- 
            )" << std::endl;
            // Validate each input
            std::cout << "Enter the inverting input voltage (volts): ";
            while (!(std::cin >> inverting_input_voltage)) {
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                std::cout << "Invalid input. Please enter a numeric value for the inverting input voltage: ";
            }

            // Get resistance input
            std::cout << "Enter unit for R (k for kilo-ohms, M for mega-ohms, O for ohms): ";
            std::cin >> unit;
            if (!validate_positive_input(raw_resist, "Enter the feedback resistor value (ohms): ")) {
                std::cout << "Please enter a positive integer: ";
                std::cin >> raw_resist;
            }
            resistor_input(raw_resist, feedback_resistor, unit);
            if (feedback_resistor <= 0) {
                std::cerr << "Invalid resistance input.\n";
                return;
            }

            // Get resistance input
            std::cout << "Enter unit for R (k for kilo-ohms, M for mega-ohms, O for ohms): ";
            std::cin >> unit;
            if (!validate_positive_input(input_resistor, "Enter the input resistor value (ohms): ")) {
                std::cout << "Please enter a positive integer: ";
                std::cin >> raw_resist;
            }
            resistor_input(raw_resist, input_resistor, unit);
            if (input_resistor <= 0) {
                std::cerr << "Invalid resistance input.\n";
                return;
            }

            gain = -feedback_resistor / input_resistor;
            float output_voltage = gain * inverting_input_voltage;

            // Use the absolute value of the output voltage for unit handling
            double abs_output_voltage = fabs(output_voltage);

            // Perform unit conversion based on magnitude
            if (abs_output_voltage >= 1e-3 && abs_output_voltage < 1) {
                abs_output_voltage *= 1e3;  // Convert to millivolts
                unit = "mV";
            }
            else if (abs_output_voltage >= 1e-6 && abs_output_voltage < 1e-3) {
                abs_output_voltage *= 1e6;  // Convert to microvolts
                unit = "uV";
            }
            else {
                unit = "V";  // Keep as volts
            }

            // Restore sign for display
            double final_output_voltage = (output_voltage < 0) ? -abs_output_voltage : abs_output_voltage;

            // Display results
            std::cout << "\nThe gain of the inverting op-amp is: " << gain << "\n";
            std::cout << "The output voltage is: " << final_output_voltage << " " << unit << "\n";
        }
        else if (choice == 2) {
            // Non-Inverting Op-Amp
            std::cout << "\n>> Non-Inverting Op-Amp Configuration\n";
            std::cout << R"(
      
                               |\       
            Vin--| IR |--*-----|+\      
                               |  \_____*____Vout
                               |  /     |
                         *-----|-/    |----|                   
                         |     |/     | FR |
                         |            |----|
                         |              |
                         *--------------*
                                        |  
                                      |----|                   
                                      | GR |
                                      |----|
                                        |
                                        |
                                      -----
                                       --- 
            )" << std::endl;
            // Validate each input
            if (!validate_positive_input(non_inverting_input_voltage, "Enter the non-inverting input voltage (volts): "))
                return;
            // Get resistance input
            std::cout << "Enter unit for R (k for kilo-ohms, M for mega-ohms, O for ohms): ";
            std::cin >> unit;
            if (!validate_positive_input(raw_resist, "Enter the feedback resistor value (ohms): ")) {
                std::cout << "Please enter a positive integer: ";
                std::cin >> raw_resist;
            }
            resistor_input(raw_resist, feedback_resistor, unit);
            if (feedback_resistor <= 0) {
                std::cerr << "Invalid resistance input.\n";
                return;
            }

            // Get resistance input
            std::cout << "Enter unit for R (k for kilo-ohms, M for mega-ohms, O for ohms): ";
            std::cin >> unit;
            if (!validate_positive_input(ground_resistor, "Enter the ground resistor value (ohms): ")) {
                std::cout << "Please enter a positive integer: ";
                std::cin >> raw_resist;
            }
            resistor_input(raw_resist, ground_resistor, unit);
            if (ground_resistor <= 0) {
                std::cerr << "Invalid resistance input.\n";
                return;
            }


            gain = 1 + (feedback_resistor / ground_resistor);
            float output_voltage = gain * non_inverting_input_voltage;
            if (output_voltage <= 1e6 && output_voltage > 1e3) {
                output_voltage *= 1e-3;
                unit = "kV";
            }
            else if (output_voltage >= 1e6) {
                output_voltage *= 1e-6;
                unit = "MV";
            }
            else if (output_voltage <= 1e3 && output_voltage > 1) {
                unit = "V";
            }
            std::cout << "\nThe gain of the non-inverting op-amp is: " << gain << "\n";

            std::cout << "The output voltage is: " << output_voltage << " " << unit << "\n";

        }
        else if (choice == 3) {
            return; // Exit this function
        }
        // Ask if the user wants to repeat or exit this menu item
        std::cout << "\nWould you like to perform another calculation in this menu? (y/n): ";
        std::cin >> repeat_choice;
    } while (repeat_choice == "y" || repeat_choice == "Y");
}

void calculate_res_filter() {
    clearscreen();
    float  resistance_needed;
    double raw_cap, raw_freq, frequency = 0, capacitance = 0;
    std::string unit;
    std::cout << "--- Resistor Calculator ---\n";
    std::cout << "Enter unit for the capacitance (u for microfarads, n for nanofarads, p for picofarads): ";
    std::cin >> unit;

    // Get resistance input
    if (!validate_positive_input(raw_cap, "Enter the capacitor value: "))
        return;

    capacitor_input(raw_cap, capacitance, unit);
    if (capacitance == 0) {
        std::cerr << "Invalid input.\n";
        return;
    }

    std::cout << "Enter unit for the cutoff frequency(k for Kilo Hz, M for Mega Hz, H for Hz): ";
    std::cin >> unit;

    // Get resistance input
    if (!validate_positive_input(raw_freq, "Enter the frequency value: "))
        return;

    Fc_input(raw_freq, frequency, unit);
    if (frequency == 0) {
        std::cerr << "Invalid input.\n";
        return;
    }

    resistance_needed = 1 / (2 * 3.14 * capacitance * frequency);
    if (resistance_needed > 1e3) {
        resistance_needed = resistance_needed / 1e3;
        unit = "kOhms";
    }
    else if (resistance_needed > 1e6) {
        resistance_needed = resistance_needed / 1e6;
        unit = "MOhms";
    }
    else {
        resistance_needed;
        unit = "Ohms";
    }
    std::cout << "Required resistance = " << resistance_needed << " " << unit << "\n";

}

void calculate_cap_filter() {
    clearscreen();
    float  capacitance_needed;
    double raw_freq, raw_resist, resistance = 0, frequency = 0;
    std::string unit;
    std::cout << "--- Capacitor Calculator ---\n";
    std::cout << "Enter unit for the resistance (k for kilo-ohms, M for mega-ohms, O for ohms): ";
    std::cin >> unit;

    // Get resistance input
    if (!validate_positive_input(raw_resist, "Enter the resistor value: "))
        return;

    resistor_input(raw_resist, resistance, unit);
    if (resistance == 0) {
        std::cerr << "Invalid resistance input.\n";
        return;
    }

    std::cout << "Enter unit for the cutoff frequency (k for Kilo Hz, M for Mega Hz, H for Hz): ";
    std::cin >> unit;

    // Get resistance input
    if (!validate_positive_input(raw_freq, "Enter the frequency value: "))
        return;

    Fc_input(raw_freq, frequency, unit);
    if (frequency == 0) {
        std::cerr << "Invalid input.\n";
        return;
    }

    capacitance_needed = 1 / (2 * 3.14 * resistance * frequency);
    // Determine the unit of capacitance
    if (1e-9 < capacitance_needed >= 1e-6) {
        capacitance_needed *= 1e6;
        unit = "uF"; // Microfarads
    }
    else if (1e-12 < capacitance_needed >= 1e-9) {
        capacitance_needed *= 1e9;
        unit = "nF"; // Nanofarads
    }
    else {
        capacitance_needed *= 1e12;
        unit = "pF"; // Picofarads
    }
    std::cout << "Required capacitance = " << capacitance_needed << " " << unit << "\n";
}

void calculate_coff_freq_filter() {
    clearscreen();
    double raw_cap, raw_resist, resistance = 0, capacitance = 0;
    std::string unit;
    float cutoff_frequency;
    std::cout << "--- Cutoff Frequency Calculator ---\n";
    std::cout << "Enter unit for the capacitance (u for microfarads, n for nanofarads, p for picofarads): ";
    std::cin >> unit;

    // Get capacitance input
    if (!validate_positive_input(raw_cap, "Enter the capacitor value: "))
        return;

    capacitor_input(raw_cap, capacitance, unit);
    if (capacitance == 0) {
        std::cerr << "Invalid input.\n";
        return;
    }

    std::cout << "Enter unit for the resistance(k for kilo-ohms, M for mega-ohms, O for ohms): ";
    std::cin >> unit;

    // Get resistance input
    if (!validate_positive_input(raw_resist, "Enter the resistor value: "))
        return;

    resistor_input(raw_resist, resistance, unit);
    if (resistance == 0) {
        std::cerr << "Invalid resistance input.\n";
        return;
    }

    cutoff_frequency = calculate_cutoff_frequency(resistance, capacitance, 1.0);
    std::cout << "Cutoff frequency = " << cutoff_frequency << " Hz\n";
}

void lowpassfilter() {
    clearscreen();
    std::cout << "Selected low Pass Filter:\n";
    std::cout << "          +----- R ---------------o V_out\n";
    std::cout << "          ^                |      ^ \n";
    std::cout << "    V_in  |                C      | \n";
    std::cout << "          |                |      |\n";
    std::cout << "          +-----------------------o\n";
    int choice;
    do {
        std::cout << "1. Calculate resistance\n";
        std::cout << "2. Calculate capacitance\n";
        std::cout << "3. Calculate cutoff frequency\n";
        std::cout << "4. Back to Filter Menu\n";
        std::cout << "Select an option: ";
        std::cin >> choice;

        // Blocks invalid input until correct
        while (std::cin.fail()) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 1 and 3.\n";
                std::cout << "Enter an integer!\n";
                std::cin >> choice;
            }
        }

        switch (choice) {
        case 1:
            calculate_res_filter();
            break;
        case 2:
            calculate_cap_filter();
            break;
        case 3:
            calculate_coff_freq_filter();
            break;
        case 4:
            std::cout << "Returning to Filter Menu...\n";
            break;
        default:
            std::cout << "Invalid option. Try again.\n";
            return;
        }
    } while (choice != 4);
}

void highpassfilter() {
    clearscreen();
    std::cout << "Selected high pass filter:\n";
    std::cout << "          +----- C ---------------o V_out\n";
    std::cout << "          ^                |      ^ \n";
    std::cout << "    V_in  |                R      | \n";
    std::cout << "          |                |      |\n";
    std::cout << "          +-----------------------o\n";
    int choice;
    do {
        std::cout << "1. Calculate resistance\n";
        std::cout << "2. Calculate capacitance\n";
        std::cout << "3. Calculate cutoff frequency\n";
        std::cout << "4. Back to Filter Menu\n";
        std::cout << "Select an option: ";
        std::cin >> choice;

        // Blocks invalid input until correct
        while (std::cin.fail()) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 1 and 3.\n";
                std::cout << "Enter an integer!\n";
                std::cin >> choice;
            }
        }

        switch (choice) {
        case 1:
            calculate_res_filter();
            break;
        case 2:
            calculate_cap_filter();
            break;
        case 3:
            calculate_coff_freq_filter();
            break;
        case 4:
            std::cout << "Returning to Main Menu...\n";
            break;
        default:
            std::cout << "Invalid option. Try again.\n";
        }
    } while (choice != 4);
}

void menu_item_3() {
    int choice;
    do {
        clearscreen();
        // Sub menu item 3
        std::cout << "\n--- Filter Calculator ---\n";
        std::cout << "Are you building a High pass filter or a Low pass filter?\n";
        std::cout << "1. Low Pass Filter\n";
        std::cout << "2. High Pass Filter\n";
        std::cout << "3. Exit to Main Menu\n";
        std::cout << "Select an option: ";
        std::cin >> choice;

        // Blocks invalid input until correct
        while (std::cin.fail()) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 1 and 3.\n";
                std::cout << "Enter an integer!\n";
                std::cin >> choice;
            }
        }

        switch (choice) {
        case 1:
            lowpassfilter();
            break;
        case 2:
            highpassfilter();
            break;
        case 3:
            std::cout << "Returning to main menu...\n";
            break;
        default:
            std::cout << "Invalid option. Try again.\n";
        }
    } while (choice != 3); // Repeat the menu until the user chooses to exit
    //  std::cout << "\n>> High or Low pass RC Filters\n";
}

// Sallen Key circuit diagram
void print_sallen_key_diagram() {
    std::cout << R"(
|-------------------------------------Sallen-Key Filter Circuit Diagram:---------------------------------------|
|                                                                                                              |    
|                                              |-----|                                                         |
|                                *-------------| Z3  |------------------------------------*                    |
|                                |             |-----|                                    |                    |
|                                |                                                        |                    |
|                                |                                       |\               |                    |
|                                |                                       | \              |                    |
|                                |                                       |  \             |                    |
|                                |                                       |   \            |                    |
|                                |                                       |+   \           |                    |
|                                |                                       |     \          |                    |
|                     |----|     |             |----|                    |     /----------------------Vout     |
|     Vin-------------| Z1 | ----*-------------| Z2 |---------*----------|    /            |                   |
|                     |----|                   |----|         |          |   /             |                   |
|                                                             |       *--|_ /            |----|                |
|                                                             |       |  | /             | RA |                |
|                                                             |       |  |/              |----|                |
|                                                           |-----|   |                    |                   |
|                                                           | Z4  |   *____________________*                   |
|                                                           |_____|                        |                   |
|                                                             |                          |----|                |
|                                                             |                          | RB |                |
|                                                             |                          |____|                |
|                                                             |                            |                   |
|                                                             |                            |                   | 
|                                                             |                            |                   |
|                                                           -----                        -----                 |
|                                                            ---                          ---                  |
|                                                                                                              |
|--------------------------------------------------------------------------------------------------------------|
)" << std::endl;

}

// Asks the user to input circuit components
void get_component_values(double& r, double& c, double& ra, double& rb) {
    std::cout << "\nEnter values for Resistors and Capacitors:\n";
    double raw_resist, raw_cap;
    std::string unit;


    // Get resistance input
    std::cout << "Enter unit for R (k for kilo-ohms, M for mega-ohms, O for ohms): ";
    std::cin >> unit;
    if (!validate_positive_input(raw_resist, "Enter the resistance of R = R1 = R2 : ")) {
        std::cout << "Please enter a positive integer: ";
        std::cin >> raw_resist;
    }
    resistor_input(raw_resist, r, unit);

    std::cout << "\nEnter unit for C (u for microfarads, n for nanofarads, p for picofarads): ";
    std::cin >> unit;

    // Get resistance input
    if (!validate_positive_input(raw_cap, "Enter the capacitance C = C1 = C2  "))
        return;

    capacitor_input(raw_cap, c, unit);
    if (c <= 0) {
        std::cerr << " \nInvalid capacitance input.";
        return;
    }
    // Get resistance input
    std::cout << "\nEnter unit for RB (k for kilo-ohms, M for mega-ohms, O for ohms): ";
    std::cin >> unit;
    if (!validate_positive_input(raw_resist, "Enter the resistance of RB : ")) {
        std::cout << "Please enter a positive integer: ";
        std::cin >> raw_resist;
    }
    resistor_input(raw_resist, rb, unit);


}

// Butterworth filter calculator
void butterworth_filter(int num_poles, double r, double c, double ra, double rb, int pole_pair_index) {
    std::cout << "\nPerforming calculations for Butterworth with " << num_poles << " poles, Pole Pair " << pole_pair_index << "...\n";
    press_to_continue();

    std::vector<double> gains;
    if (num_poles == 2) {
        gains = { 1.586 };
    }
    else if (num_poles == 4) {
        gains = { 1.152, 2.325 };
    }
    else if (num_poles == 6) {
        gains = { 1.068, 1.586, 2.483 };
    }

    double gain = gains[pole_pair_index - 1]; // Adjust for zero-based index
    std::cout << "The filter gain for Pole Pair " << pole_pair_index << " is " << gain << '\n';

    // Ensure gain is valid for calculation
    if (gain <= 1.0) {
        std::cerr << "Invalid gain value (" << gain << "). Must be greater than 1.\n";
        return;
    }

    // Calculate `ra` using `rb` and gain
    ra = rb * (gain - 1);

    // Display component values
    std::cout << "\nResistor RA: " << ra << "\n";
    get_npv_and_color_code_for_resistor(ra);

    std::cout << "\nResistor RB: " << rb << " \n";
    get_npv_and_color_code_for_resistor(rb);

    // Calculate and display the cutoff frequency for this pole pair
    double cutoff_freq = calculate_cutoff_frequency(r, c);
    display_cutoff_frequency(cutoff_freq);
}

// Function to fetch gains and factors based on Chebyshev type and poles
void chebyshev_filter_data(int num_poles, int type, std::vector<float>& gains, std::vector<float>& factors_low, std::vector<float>& factors_high) {

    if (type == 2) { // 0.5 dB Chebyshev
        // Characteristics for each pole
        if (num_poles == 2) {
            gains = { 1.842 };
            factors_low = { 1.231 };
            factors_high = { 0.812 };
        }
        else if (num_poles == 4) {
            gains = { 1.582, 2.660 };
            factors_low = { 0.597, 1.031 };
            factors_high = { 1.675, 0.970 };
        }
        else if (num_poles == 6) {
            gains = { 1.537, 2.448, 2.846 };
            factors_low = { 0.396, 0.768, 1.011 };
            factors_high = { 2.525, 1.302, 0.989 };
        }
    }
    else if (type == 3) { // 2 dB Chebyshev
        if (num_poles == 2) {
            gains = { 2.114 };
            factors_low = { 0.907 };
            factors_high = { 1.103 };
        }
        else if (num_poles == 4) {
            gains = { 1.924, 2.782 };
            factors_low = { 0.471, 0.964 };
            factors_high = { 2.123, 1.037 };
        }
        else if (num_poles == 6) {
            gains = { 1.891, 2.648, 2.904 };
            factors_low = { 0.316, 0.730, 0.983 };
            factors_high = { 3.165, 1.370, 1.017 };
        }
    }
}

// Chebyshev filter calculator
void chebyshev_filter(int num_poles, int type, const std::string& filter_type, double r, double c, double ra, double rb, int pole_pair_index) {
    std::vector<float> gains, factors_low, factors_high;

    // Fetch appropriate gains and cutoff factors based on the filter type and number of poles
    chebyshev_filter_data(num_poles, type, gains, factors_low, factors_high);

    // Determine the factors based on filter type (low-pass or high-pass)
    std::vector<float> factors = (filter_type == "low") ? factors_low : factors_high;
    std::cout << "\nPerforming calculations for Chebyshev with " << num_poles << " poles, Pole Pair " << pole_pair_index << "...\n";
    press_to_continue();

    double gain = gains[pole_pair_index - 1]; // Adjust for zero-based index
    std::cout << "The filter gain for Pole Pair " << pole_pair_index << " is " << gain << '\n';

    // Ensure gain is valid for calculation
    if (gain <= 1.0) {
        std::cerr << "Invalid gain value (" << gain << "). Must be greater than 1.\n";
        return;
    }

    // Calculate `ra` using `rb` and gain
    ra = rb * (gain - 1);

    // Display component values for this pole pair
    std::cout << "\nResistor RA: " << ra << "\n";
    get_npv_and_color_code_for_resistor(ra);

    std::cout << "\nResistor RB: " << rb << " \n";
    get_npv_and_color_code_for_resistor(rb);

    // Calculate and display the cutoff frequency for this pole pair
    double cutoff_freq = calculate_cutoff_frequency(r * factors_low[pole_pair_index - 1], c * factors_high[pole_pair_index - 1]);
    display_cutoff_frequency(cutoff_freq);
}

// Menu item 4
void menu_item_4() {
    int choice = 0;
    std::string filter_type, repeat_choice;
    double r = 0, c = 0, ra = 0, rb = 0;

    do {
        clearscreen();
        std::cout << "\n>> Menu 4: Sallen-Key Filter Configuration\n";

        // Display filter type submenu
        std::cout << "\n-- Select Filter Type: --\n";
        std::cout << "1. Butterworth\n";
        std::cout << "2. 0.5 dB Chebyshev\n";
        std::cout << "3. 2 dB Chebyshev\n";
        std::cout << "4. Back to main menu\n";
        std::cout << "Select choice: ";
        std::cin >> choice;
      // Validate choice
        while (std::cin.fail() || choice < 1 || choice > 4) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 1 and 4: ";
            std::cin >> choice;
        }


        // User input for the number of poles
        int num_poles;
        std::cout << "\n Enter the number of poles (2, 4, or 6): ";
        std::cin >> num_poles;

        if (num_poles != 2 && num_poles != 4 && num_poles != 6) {
            std::cout << "\nInvalid input. Only 2, 4, or 6 poles are allowed.\n";
            return;
        }

        // Filter type selection
        std::cout << "\nEnter whether the filter is 'high' or 'low' pass: ";
        std::cin >> filter_type;

        // Convert input to lowercase
        std::transform(filter_type.begin(), filter_type.end(), filter_type.begin(), ::tolower);

        if (filter_type != "high" && filter_type != "low") {
            std::cout << "\nInvalid filter type. Please specify 'high' or 'low'.\n";
            return;
        }


  
        // Number of pole pairs
        int diagram_count = num_poles / 2;

        // Loop through each pole pair
        for (int i = 0; i < diagram_count; ++i) {
            std::cout << "\n--- Configuration for Pole Pair " << (i + 1) << " ---\n";

            double r = 0, c = 0, ra = 0, rb = 0;
            std::cout << "\nEnter component values for Pole Pair " << (i + 1) << ":\n";
            get_component_values(r, c, ra, rb);

            // Display the Sallen-Key diagram
            std::cout << "\nGenerating a Sallen-Key " << filter_type << " filter diagram...\n";
            print_sallen_key_diagram();

            // Call the selected filter function with current pole pair data
            if (choice == 1) { // Butterworth
                butterworth_filter(num_poles, r, c, ra, rb, i + 1); // Pass pole pair index (i + 1)
            }
            else if (choice == 2) { // 0.5 dB Chebyshev
                chebyshev_filter(num_poles, 2, filter_type, r, c, ra, rb, i + 1); // Pass pole pair index (i + 1)
            }
            else if (choice == 3) { // 2 dB Chebyshev
                chebyshev_filter(num_poles, 3, filter_type, r, c, ra, rb, i + 1); // Pass pole pair index (i + 1)
            }

            if (filter_type == "high") {
                std::cout << "\n  Z1 = C1 = " << c << " farads\n";
                std::cout << "  Z2 = C2 = " << c << " farads\n";
                std::cout << "  Z3 = R1 = " << r << " ohms\n";
                std::cout << "  Z4 = R2 = " << r << " ohms\n";
            }
            else { // Low-pass filter
                std::cout << "\n  Z1 = R1 = " << r << " ohms\n";
                std::cout << "  Z2 = R2 = " << r << " ohms\n";
                std::cout << "  Z3 = C1 = " << c << " farads\n";
                std::cout << "  Z4 = C2 = " << c << " farads\n";
            }

            // Display the resistor color codes for R1 & R2
            std::cout << "\nResistor R1 & R2:\n";
            get_npv_and_color_code_for_resistor(r);

            // Separator for clarity
            std::cout << "\n----------------------------\n";
        }

        // Prompt to repeat or exit
        std::cout << "\nWould you like to perform another calculation in this menu? (y/n): ";
        std::cin >> repeat_choice;

    } while (repeat_choice == "y" || repeat_choice == "Y");

    std::cout << "\nReturning to the main menu...\n";
}
