#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>  // for system()
#include <ctime>

using namespace std;

// Fetch exchange rates JSON and save locally
bool fetch_exchange_rates() {
    int result = system("curl -s https://api.exchangerate-api.com/v4/latest/USD > currency.json");
    return (result == 0);
}

// Parse JSON file and extract the rate for the given currency code
double get_exchange_rate(const string& currency) {
    ifstream file("currency.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open currency.json\n";
        return -1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    string key = "\"" + currency + "\":";
    size_t pos = content.find(key);
    if (pos == string::npos) {
        cerr << "Error: Currency code " << currency << " not found in data\n";
        return -1;
    }

    size_t start = pos + key.length();
    size_t end = content.find_first_of(",}", start);
    if (end == string::npos) end = content.length();

    string rate_str = content.substr(start, end - start);

    try {
        return stod(rate_str);
    } catch (...) {
        cerr << "Error: Failed to parse rate for " << currency << endl;
        return -1;
    }
}

// Utility function for waiting (milliseconds)
void waiting(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

// Convert from one currency to another using USD as base
// rates is a map from currency code to USD rate (1 USD = rate currency units)
double convert_currency(double amount, const string& from, const string& to, double rate_from, double rate_to) {
    // Convert from "from" currency to USD, then USD to "to" currency
    double amount_in_usd = amount / rate_from;
    double converted = amount_in_usd * rate_to;
    return converted;
}

int main() {
    system("color 6E");

    if (!fetch_exchange_rates()) {
        cerr << "Failed to fetch exchange rates. Exiting.\n";
        return 1;
    }

    // Get live exchange rates relative to USD
    double rate_INR = get_exchange_rate("INR");
    double rate_GBP = get_exchange_rate("GBP");
    double rate_EUR = get_exchange_rate("EUR");
    double rate_AED = get_exchange_rate("AED");
    double rate_USD = 1.0;  // base currency

    if (rate_INR < 0 || rate_GBP < 0 || rate_EUR < 0 ) {
        cerr << "Error: Could not retrieve all required rates. Exiting.\n";
        return 1;
    }

    int choice;

    while (true) {
        system("cls");

        cout << "\t\t\t**********************************************************************************" << endl;
        cout << "\t\t\t*                        WELCOME TO THE LIVE CURRENCY CONVERTER                  *" << endl;
        cout << "\t\t\t**********************************************************************************" << endl;

        cout << "\n\nChoose conversion option:" << endl;
        cout << "\t\t\t [1] Dollar (USD) to Rupee (INR)" << endl;
        cout << "\t\t\t [2] Rupee (INR) to Dollar (USD)" << endl;
        cout << "\t\t\t [3] Pound (GBP) to Rupee (INR)" << endl;
        cout << "\t\t\t [4] Rupee (INR) to Pound (GBP)" << endl;
        cout << "\t\t\t [5] Euro (EUR) to Rupee (INR)" << endl;
        cout << "\t\t\t [6] Rupee (INR) to Euro (EUR)" << endl;
        cout << "\t\t\t [7] Dollar (USD) to Pound (GBP)" << endl;
        cout << "\t\t\t [8] Pound (GBP) to Dollar (USD)" << endl;
        cout << "\t\t\t [9] Dollar (USD) to Euro (EUR)" << endl;
        cout << "\t\t\t [10] Euro (EUR) to Dollar (USD)" << endl;
        cout << "\t\t\t [11] Euro (EUR) to Pound (GBP)" << endl;
        cout << "\t\t\t [12] Pound (GBP) to Euro (EUR)" << endl;
        cout << "\t\t\t [13] Dirham (AED) to Rupees (INR)" << endl;
        cout << "\t\t\t [0] Exit" << endl;

        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 0) break;

        double input_amount, output_amount;

        switch (choice) {
            case 1:
                cout << "Enter amount in USD: ";
                cin >> input_amount;
                output_amount = input_amount * rate_INR;
                cout << input_amount << " USD = " << output_amount << " INR\n";
                break;

            case 2:
                cout << "Enter amount in INR: ";
                cin >> input_amount;
                output_amount = input_amount / rate_INR;
                cout << input_amount << " INR = " << output_amount << " USD\n";
                break;

            case 3:
                cout << "Enter amount in GBP: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "GBP", "INR", rate_GBP, rate_INR);
                cout << input_amount << " GBP = " << output_amount << " INR\n";
                break;

            case 4:
                cout << "Enter amount in INR: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "INR", "GBP", rate_INR, rate_GBP);
                cout << input_amount << " INR = " << output_amount << " GBP\n";
                break;

            case 5:
                cout << "Enter amount in EUR: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "EUR", "INR", rate_EUR, rate_INR);
                cout << input_amount << " EUR = " << output_amount << " INR\n";
                break;

            case 6:
                cout << "Enter amount in INR: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "INR", "EUR", rate_INR, rate_EUR);
                cout << input_amount << " INR = " << output_amount << " EUR\n";
                break;

            case 7:
                cout << "Enter amount in USD: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "USD", "GBP", rate_USD, rate_GBP);
                cout << input_amount << " USD = " << output_amount << " GBP\n";
                break;

            case 8:
                cout << "Enter amount in GBP: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "GBP", "USD", rate_GBP, rate_USD);
                cout << input_amount << " GBP = " << output_amount << " USD\n";
                break;

            case 9:
                cout << "Enter amount in USD: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "USD", "EUR", rate_USD, rate_EUR);
                cout << input_amount << " USD = " << output_amount << " EUR\n";
                break;

            case 10:
                cout << "Enter amount in EUR: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "EUR", "USD", rate_EUR, rate_USD);
                cout << input_amount << " EUR = " << output_amount << " USD\n";
                break;

            case 11:
                cout << "Enter amount in EUR: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "EUR", "GBP", rate_EUR, rate_GBP);
                cout << input_amount << " EUR = " << output_amount << " GBP\n";
                break;

            case 12:
                cout << "Enter amount in GBP: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "GBP", "EUR", rate_GBP, rate_EUR);
                cout << input_amount << " GBP = " << output_amount << " EUR\n";
                break;

             case 13:
                cout << "Enter amount in AED: ";
                cin >> input_amount;
                output_amount = convert_currency(input_amount, "AED", "INR", rate_AED , rate_INR);
                cout << input_amount << " AED = " << output_amount << " INR\n";
                break;

            default:
                cout << "Invalid choice. Please select between 0-13.\n";
                break;
        }
        cout << "Press any key to continue...\n";
        cin.ignore();
        cin.get();
    }

    cout << "Thank you for using the currency converter.\n";
    return 0;
}