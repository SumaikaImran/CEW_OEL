#include<float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * Function to convert temperature from Kelvin to Celsius.
 *
 * @param kelvin The temperature in Kelvin.
 * @return The temperature in Celsius.
 */
float kelvinToCelsius(float kelvin) {
    return kelvin - 273.15;
}

/**
 * Function to calculate the minimum temperature from a file and write it to a file.
 *
 * @param filename The name of the input file.
 */
void min_temp(const char *filename) {
    FILE *inputFile = fopen(filename, "r");
    FILE *outputFile = fopen("min_temperature.txt", "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        return; // Return void since the function is declared as void
    }

    double temperature;
    double minTemperature;
    bool firstTemperature = true;

    // Read temperature values from the file
    while (fscanf(inputFile, "%lf", &temperature) == 1) {
        if (firstTemperature) {
            minTemperature = temperature;
            firstTemperature = false;
        } else {
            if (temperature < minTemperature) {
                minTemperature = temperature;
            }
        }
    }

    fprintf(outputFile, "Minimum Temperature: %.2f\n", minTemperature); // Added newline character to the output
    fclose(inputFile);
    fclose(outputFile);
}

/**
 * Function to calculate the maximum temperature from a file and write it to a file.
 *
 * @param filename The name of the input file.
 */
void max_temp(const char* filename) {
    FILE* inputFile = fopen(filename, "r");
    FILE* outputFile = fopen("max_temperature.txt", "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        return;
    }

    double temperature;
    double maxTemperature = -273.15; // Initialize with absolute zero

    // Read temperature values from the file
    while (fscanf(inputFile, "%lf", &temperature) == 1) {
        if (temperature > maxTemperature) {
            maxTemperature = temperature;
        }
    }

    fprintf(outputFile, "Maximum Temperature: %.2f\n", maxTemperature); // Added newline character to the output

    fclose(inputFile);
    fclose(outputFile);
}
