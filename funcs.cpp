#include <iostream>
#include <string>
#include <limits>
#include <map>
#include <vector>
#include <cmath>
#include "funcs.h"
#include <algorithm> // For std::transform
#include "funcs.h"

float inverting_input_voltage;
float non_inverting_input_voltage;
float feedback_resistor;
float input_resistor;
float ground_resistor;
float gain;


// Helper to calculate cutoff frequency
static double calculate_cutoff_frequency(double r, double c, double factor = 1.0) {
    return 1 / (r * c * 2 * 3.14 * factor);
}

// Function to display cutoff frequency with units
void display_cutoff_frequency(float cutoff_freq) {
    if (cutoff_freq > 1e6) {
        std::cout << "The cutoff frequency is " << cutoff_freq / 1e6 << " MHz\n";
    }
    else if (cutoff_freq > 1e3) {
        std::cout << "The cutoff frequency is " << cutoff_freq / 1e3 << " kHz\n";
    }
    else {
        std::cout << "The cutoff frequency is " << cutoff_freq << " Hz\n";
    }
}

//Perform calculations for Butterworth filter
void butterworth_filter(int num_poles, float r, float c) {
    std::cout << "\nPerforming calculations for Butterworth with " << num_poles << " poles...\n";

    std::vector<float> gains;
    if (num_poles == 2) {
        gains = { 1.586 };
    }
    else if (num_poles == 4) {
        gains = { 1.152, 2.325 };
    }
    else if (num_poles == 6) {
        gains = { 1.068, 1.586, 2.483 };
    }

    for (size_t stage = 0; stage < gains.size(); ++stage) {
        std::cout << "Stage " << stage + 1 << ":\n";
        std::cout << "The filter gain is " << gains[stage] << '\n';
        float cutoff_freq = calculate_cutoff_frequency(r, c);
        display_cutoff_frequency(cutoff_freq);
    }
}


// Function to fetch gains and factors based on Chebyshev type and poles
void chebyshev_filter_data(int num_poles, int type, std::vector<float>& gains, std::vector<float>& factors_low, std::vector<float>& factors_high) {
    if (type == 2) { // 0.5 dB Chebyshev
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

// Perform calculations for Chebyshev filters
void chebyshev_filter(int num_poles, int type, const std::string& filter_type, float r, float c) {
    std::vector<float> gains, factors_low, factors_high;

    // Fetch appropriate gains and cutoff factors
    chebyshev_filter_data(num_poles, type, gains, factors_low, factors_high);

    // Determine the factors based on filter type (low-pass or high-pass)
    std::vector<float> factors = (filter_type == "low") ? factors_low : factors_high;

    // Iterate through each pole stage
    for (size_t stage = 0; stage < gains.size(); ++stage) {
        std::cout << "Stage " << stage + 1 << ":\n";
        std::cout << "The filter gain is " << gains[stage] << '\n';

        float cutoff_freq = calculate_cutoff_frequency(r, c, factors[stage]);
        display_cutoff_frequency(cutoff_freq);
    }
}


void clearscreen() {
#ifdef _WIN32
    system("cls");
#endif // !Win32

}
bool validate_positive_input(float& value, const std::string& prompt) {
    std::cout << prompt;
    std::cin >> value;
    if (std::cin.fail() || value <= 0) {
        std::cin.clear(); // Clear error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cerr << "Error: Value must be a positive number.\n";
        return false;
    }
    return true;
}

void go_back_to_main() {
    std::string input;
    do {
        std::cout << "\nEnter 'b' or 'B' to go back to main menu: ";
        std::cin >> input;
        clearscreen();

    } while (input != "b" && input != "B");
}

void menu_item_1() {
    clearscreen();
    int choice;
    do {
        std::cout << "\n--- Resistor Calculator ---\n";
        std::cout << "1. Calculate resistance from color codes\n";
        std::cout << "2. Solve Resistor Network\n";
        std::cout << "3. Find nearest NPV resistor\n";
        std::cout << "4. Get NPV value and color code for a resistor\n";  // New option
        std::cout << "5. Back to main menu\n";
        std::cout << "Select an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            calculate_resistor_from_color_code();
            break;
        case 2:
            combine_resistors();
            break;
        case 3:
            find_nearest_npv_resistor();
            break;
        case 4: {
            double resistor_value;
            std::cout << "Enter resistor value (in ohms): ";
            std::cin >> resistor_value;
            get_npv_and_color_code_for_resistor(resistor_value);  // Call the new function
            break;
        }
        case 5:
            std::cout << "Returning to main menu...\n";
            break;
        default:
            std::cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);
}
void calculate_resistor_from_color_code() {
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
    std::cout << "Resistance: " << resistance << " ohms\n";
}

void combine_resistors() {
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
    std::vector<double> npv_resistors = {
    1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2,
    10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82,
    100, 120, 150, 180, 220, 270, 330, 390, 470, 560, 680, 820,
    1000, 1200, 1500, 1800, 2200, 2700, 3300, 3900, 4700, 5600, 6800, 8200,
    10000, 12000, 15000, 18000, 22000, 27000, 33000, 39000, 47000, 56000, 68000, 82000,
    100000, 120000, 150000, 180000, 220000, 270000, 330000, 390000, 470000, 560000, 680000, 820000,
    1000000, 1200000, 1500000, 1800000, 2200000, 2700000, 3300000, 3900000, 4700000, 5600000, 6800000, 8200000,
    10000000 // Extendable for even higher ranges
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
std::vector<double> npv_resistors = {
    1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2,
    10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82,
    100, 120, 150, 180, 220, 270, 330, 390, 470, 560, 680, 820,
    1000, 1200, 1500, 1800, 2200, 2700, 3300, 3900, 4700, 5600, 6800, 8200,
    10000, 12000, 15000, 18000, 22000, 27000, 33000, 39000, 47000, 56000, 68000, 82000,
    100000, 120000, 150000, 180000, 220000, 270000, 330000, 390000, 470000, 560000, 680000, 820000,
    1000000, 1200000, 1500000, 1800000, 2200000, 2700000, 3300000, 3900000, 4700000, 5600000, 6800000, 8200000,
    10000000 // Extendable for even higher ranges
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

void get_npv_and_color_code_for_resistor(double resistance) {
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
    int significant_value = static_cast<int>(normalized_value * 10);

    int first_digit = significant_value / 10;
    int second_digit = significant_value % 10;

    // Adjust multiplier for exact matching
    if (normalized_value < 1.0) {
        --magnitude;
        normalized_value *= 10;
    }

    if (digit_to_color.find(first_digit) != digit_to_color.end() &&
        digit_to_color.find(second_digit) != digit_to_color.end() &&
        multiplier_to_color.find(magnitude) != multiplier_to_color.end()) {
        std::string first_band = digit_to_color[first_digit];
        std::string second_band = digit_to_color[second_digit];
        std::string multiplier_band = multiplier_to_color[magnitude];

        std::cout << "Color Code: [" << first_band << ", " << second_band << ", " << multiplier_band << "]\n";
    } else {
        std::cout << "Error: Unable to calculate color code for this resistor.\n";
    }
}


void menu_item_2() {
    int choice;
    std::string repeat_choice;
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

        // Perform calculations based on the user's choice
        if (choice == 1) {
            // Inverting Op-Amp
            std::cout << "\n>> Inverting Op-Amp Configuration\n";

            // Validate each input
            if (!validate_positive_input(inverting_input_voltage, "Enter the inverting input voltage (volts): "))
                return;
            if (!validate_positive_input(feedback_resistor, "Enter the feedback resistor value (ohms): "))
                return;
            if (!validate_positive_input(input_resistor, "Enter the input resistor value (ohms): "))
                return;

            gain = -feedback_resistor / input_resistor;
            float output_voltage = gain * inverting_input_voltage;

            std::cout << "\nThe gain of the inverting op-amp is: " << gain << "\n";
            std::cout << "The output voltage is: " << output_voltage << " V\n";

        }
        else if (choice == 2) {
            // Non-Inverting Op-Amp
            std::cout << "\n>> Non-Inverting Op-Amp Configuration\n";

            // Validate each input
            if (!validate_positive_input(non_inverting_input_voltage, "Enter the non-inverting input voltage (volts): "))
                return;
            if (!validate_positive_input(feedback_resistor, "Enter the feedback resistor value (ohms): "))
                return;
            if (!validate_positive_input(ground_resistor, "Enter the ground resistor value (ohms): "))
                return;

            gain = 1 + (feedback_resistor / ground_resistor);
            float output_voltage = gain * non_inverting_input_voltage;

            std::cout << "\nThe gain of the non-inverting op-amp is: " << gain << "\n";
            std::cout << "The output voltage is: " << output_voltage << " V\n";

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
    float capacitance, frequency, resistance_needed;
    std::cout << "Enter capacitor value in Farads: ";
    std::cin >> capacitance;
    std::cout << "Enter desired cutoff frequency in Hertz: ";
    std::cin >> frequency;
    resistance_needed = 1 / (2 * 3.14 * capacitance * frequency);
    std::cout << "Required resistance = " << resistance_needed << " ohms\n";
}
void calculate_cap_filter() {
    float resistance, frequency, capacitance_needed;
    std::cout << "Enter resistor value in Ohms: ";
    std::cin >> resistance;
    std::cout << "Enter desired cutoff frequency in Hertz: ";
    std::cin >> frequency;
    capacitance_needed = 1 / (2 * 3.14 * resistance * frequency);
    std::cout << "Required capacitance = " << capacitance_needed << " Farads\n";
}
void calculate_coff_freq_filter() {
    float capacitance, resistance, cutoff_frequency;
    std::cout << "Enter capacitor value in Farads: ";
    std::cin >> capacitance;
    std::cout << "Enter resistor value in Ohms: ";
    std::cin >> resistance;
    cutoff_frequency = 1 / (2 * 3.14 * capacitance * resistance);
    std::cout << "Cutoff frequency = " << cutoff_frequency << " Hz\n";
}
void lowpassfilter() {
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
        }
    } while (choice != 4);
}
void highpassfilter() {
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
    std::cout << "\n>> Menu 3\n";
    
    // you can call a function from here that handles menu 3
    int choice;
    do {
        clearscreen();
        std::cout << "\n--- Filter Calculator ---\n";
        std::cout << "Are you building a High pass filter or a Low pass filter?\n";
        std::cout << "1. Low Pass Filter\n";
        std::cout << "2. High Pass Filter\n";
        std::cout << "3. Exit to Main Menu\n";
        std::cout << "Select an option: ";
        std::cin >> choice;
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


void print_sallen_key_diagram(double r, double c, double ra, double rb) {
    std::cout << "\nGeneral Sallen-Key Filter Circuit Diagram:\n";
    std::cout << R"(
|--------------------------------------------------------------------------------------------------------------|
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
void get_component_values(double& r, double& c, double& ra, double& rb) {
    std::cout << "\nEnter values for Resistors and Capacitors:\n";

    std::cout << "  R = R1 = R2 (shared value for Z1 and Z3) in ohms: ";
    std::cin >> r;

    std::cout << "  C = C1 = C2 (shared value for Z2 and Z4): ";
    std::cin >> c;

    std::cout << "  RA: ";
    std::cin >> ra;

    std::cout << "  RB: ";
    std::cin >> rb;
}

void menu_item_4() {

    int choice;
    std::string filter_type;
    std::string repeat_choice;
    // Initialize component values
    double r, c, ra, rb;

    // Get user inputs for the components
    get_component_values(r, c, ra, rb);

    do {
    clearscreen();
    std::cout << "\n>> Menu 4: Sallen-Key Filter Diagram\n";

    // Taking user input for the number of poles
    int num_poles;
    std::cout << "Enter the number of poles (2, 4, or 6): ";
    std::cin >> num_poles;

    if (num_poles != 2 && num_poles != 4 && num_poles != 6) {
        std::cout << "Invalid input. Only 2, 4, or 6 poles are allowed.\n";
        return;
    }

    std::cout << "\nEnter whether the filter is 'high' or 'low' pass: ";
    std::cin >> filter_type;

    // Convert filter type input to lowercase for consistent comparison
    for (auto& c : filter_type) {
        c = tolower(c);
    }

    if (filter_type != "high" && filter_type != "low") {
        std::cout << "\nInvalid filter type. Please specify 'high' or 'low'.\n";
        return;
    }

    std::cout << "\nGenerating a Sallen-Key " << filter_type << " filter with " << num_poles << " poles:\n";
    int diagram_count = num_poles / 2; // Each diagram represents 2 poles
    // Print the diagrams
    for (int i = 0; i < diagram_count; ++i) {
        std::cout << "\nPole Pair " << (i + 1) << ":\n";
        print_sallen_key_diagram(r, c, ra, rb);

        //Outputs of a high pass circuit
        if (filter_type == "high") {
            std::cout << "\nComponent Values:\n";
            std::cout << "  Z1 = C1 = " << c << " micro farads\n";
            std::cout << "  Z2 = C2 = " << r << " micro farads\n";
            std::cout << "  Z3 = R1 = " << c << " ohms\n";
            std::cout << "  Z4 = R2 = " << r << " ohms\n";
            std::cout << "\n Resistor R: \n";
            get_npv_and_color_code_for_resistor(r);
        }
        // Outputs of low pass circuit
        if (filter_type == "low") {
            std::cout << "\nComponent Values:\n";
            std::cout << "  Z1 = R1 = " << c << " ohms\n";
            std::cout << "  Z2 = R2 = " << r << " ohms\n";
            std::cout << "  Z3 = C1 = " << c << " micro farads\n";
            std::cout << "  Z4 = C2 = " << r << " micro farads\n";
            std::cout << "\n Resistor R: \n";
            get_npv_and_color_code_for_resistor(r);
        }
        // Outputs of feedback resistor
        std::cout << "  RA = " << ra << " ohms\n";
        std::cout << "  RB = " << rb << " ohms\n";
        std::cout << "\n Resistor RA: \n";
        get_npv_and_color_code_for_resistor(ra);
        std::cout << "\n Resistor RB: ";
        get_npv_and_color_code_for_resistor(rb);
    }
        // Display the filter type selection submenu
        std::cout << "\nSelect Filter Type:\n";
        std::cout << "1. Butterworth\n";
        std::cout << "2. 0.5 dB Chebyshev\n";
        std::cout << "3. 2 dB Chebyshev\n";
        std::cout << "4. Back to main menu\n";
        std::cout << "Select choice: ";
        std::cin >> choice;

        // Validate the choice input
        if (choice < 1 || choice > 4) {
            std::cout << "\nInvalid choice. Please restart and select a valid filter type.\n";
            return;
        }
        if (choice == 4) {
            return;
        }

        // Perform calculations based on user input
        std::cout << "\n--- Filter Configuration ---\n";

        // Execute calculations based on the chosen filter type
        if (choice == 1) { // Butterworth
            std::cout << "\nButterworth filter:\n";
            butterworth_filter(num_poles, r, c);
        }
        else if (choice == 2) { // 0.5 dB Chebyshev
            std::cout << "\n0.5 dB Chebyshev filter:\n";
            chebyshev_filter(num_poles, 2, filter_type, r, c);
        }
        else if (choice == 3) { // 2 dB Chebyshev
            std::cout << "\n2 dB Chebyshev filter:\n";
            chebyshev_filter(num_poles, 3, filter_type, r, c);
        }


        // Ask if the user wants to repeat or exit this menu item
        std::cout << "\nWould you like to perform another calculation in this menu? (y/n): ";
        std::cin >> repeat_choice;

    } while (repeat_choice == "y" || repeat_choice == "Y");

    // Optional: Go back to the main menu after exiting this submenu
    std::cout << "\nReturning to the main menu...\n";
}


