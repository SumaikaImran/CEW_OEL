#include<float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <stdbool.h>
#define USERNAME "malaikamustafa662@gmail.com"
#define APP_PASSWORD "riii nill inxa gvmf"

/**
 * Function to calculate the average temperature from a file.
 *
 * @param filename The name of the file.
 * @return The average temperature.
 */
double Average_temp(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 0.0;
    }

    double sum = 0.0;
    int count = 0;
    double value;

    while (fscanf(file, "%lf", &value) == 1) {
        sum += value;
        count++;
    }

    fclose(file);

    if (count > 0) {
        double avg = (sum / count);
        FILE* ptr = fopen("avg_temperature.txt", "w");
        if (ptr == NULL) {
            printf("Failed to create the output file.\n");
            return 0.0;
        }
        fprintf(ptr, "Average Temperature: %.2f\n", avg);
        fclose(ptr);
        return avg;
    } else {
        printf("No data found in the file.\n");
        return 0.0;
    }
}



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
int write_report(){
// Open the input files
    FILE* file1 = fopen("avg_temperature.txt", "r");
    FILE* file2 = fopen("max_temperature.txt", "r");
    FILE* file3 = fopen("min_temperature.txt", "r");

    // Open the output file
    FILE* outputFile = fopen("report.txt", "w");

    // Check if all files were opened successfully
    if (file1 == NULL || file2 == NULL || file3 == NULL || outputFile == NULL) {
        printf("Failed to open the files.\n");
        return 1;
    }

    char ch;
   char *statement = "Weather Data Analysis of today's weather of Karachi!\n\nThis report presents an analysis of weather data for today. The data includes:\n1.Average Temperature\n2.Maximum Temperature\n2.Minimum Temperature\n\n";
int statementLength = strlen(statement);
fwrite(statement, sizeof(char), statementLength, outputFile);

    // Read and write data from file1
    while ((ch = fgetc(file1)) != EOF) {
        fputc(ch, outputFile);
    }

    // Read and write data from file2
    while ((ch = fgetc(file2)) != EOF) {
        fputc(ch, outputFile);
    }

    // Read and write data from file3
    while ((ch = fgetc(file3)) != EOF) {
        fputc(ch, outputFile);
    }
    const char *statement1 = "\nStay updated, stay safe!";
int statementLength1 = strlen(statement1);
fwrite(statement1, sizeof(char), statementLength1, outputFile);
    // Close all the files
    fclose(file1);
    fclose(file2);
    fclose(file3);
    fclose(outputFile);
}


