#ifndef FUNCS_H
#define FUNCS_H

void menu_item_1();
void menu_item_2();
void menu_item_3();
void menu_item_4();

// General Menu functions
void Fc_input(double raw_freq, double& frequency, const std::string& unit);
void capacitor_input(double raw_cap, double& capacitance, const std::string& unit);
void resistor_input(double raw_resist, double& resistance, const std::string& unit);
void display_cutoff_frequency(float cutoff_freq);
double calculate_cutoff_frequency(double r, double c, double factor);

void go_back_to_main();
bool validate_positive_input(double& value, const std::string& prompt);
void clearscreen();
void press_to_continue();

// Menu item 1 functions
void calculate_resistor_from_color_code();
void combine_resistors();
void get_npv_and_color_code_for_resistor(double resistance);
void find_nearest_npv_resistor();

// Menu item 3 functions
void calculate_res_filter();
void calculate_cap_filter();
void calculate_coff_freq_filter();
void lowpassfilter();
void highpassfilter();

//Menu item 4 functions
void print_sallen_key_diagram();
void get_component_values(double& r, double& c, double& ra, double& rb);
void butterworth_filter(int num_poles, float r, float c, float ra, float rb);
void chebyshev_filter_data(int num_poles, int type, std::vector<float>& gains, std::vector<float>& factors_low, std::vector<float>& factors_high);
void chebyshev_filter(int num_poles, int type, const std::string& filter_type, float r, float c, float ra, float rb);

#endif

