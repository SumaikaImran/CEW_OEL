#include<stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "oel.h"

/**
 * Fetches weather data from OpenWeatherMap API and performs temperature-related actions.
 * 
 * Returns:
 *     0 if the program executed successfully.
 */
int main() {
    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a CURL handle
    curl = curl_easy_init();
    if (curl) {
        // Set the API URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/weather?q=karachi&appid=8fdf43faa7e4e36bc3ca800b3d4ea362");

        // Set the callback function to write fetched data to a file
        FILE *file = fopen("data.json", "w");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // Perform the request
        res = curl_easy_perform(curl);

        // Cleanup
        fclose(file);
        curl_easy_cleanup(curl);
    }

    // Cleanup libcurl
    curl_global_cleanup();

    // Read the JSON file
    FILE *fptr = fopen("data.json", "r");
    if (fptr == NULL) {
        printf("Failed to open the JSON file.\n");
        return 1;
    }

    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), fptr);
    fclose(fptr);

    // Parse the JSON 
    struct json_object *json = json_tokener_parse(buffer);

    struct json_object *main_obj, *temperature;
    if (json_object_object_get_ex(json, "main", &main_obj) &&
        json_object_object_get_ex(main_obj, "temp", &temperature)
    ) {
        float temp = json_object_get_double(temperature);
        float celsius_temp = kelvinToCelsius(temp);
        char h_message[1000] = "It's getting cold out there!\nRemember to stay indoors or seek shade during the hottest hours of the day, wear lightweight and breathable clothing, drink plenty of water, and apply sunscreen to protect your skin from the sun's rays. \nTake care and stay cool!";
        char c_message[1000] = "It's getting cold out there!\nRemember to bundle up in warm layers, wear a cozy hat and gloves, and stay hydrated even in the cold weather.\nStay warm and stay safe!";

        if (celsius_temp > 50) {
            send_mail(h_message);
        }
        if (celsius_temp < 5) {
            send_mail(c_message);
        }

        // Append temperature to file
        FILE* textfile = fopen("weather_data.txt", "a");
        if (textfile == NULL) {
            printf("Failed to open weather data file!\n");
            return 1;
        }
        fprintf(textfile, "%.2f\n", celsius_temp);
        fclose(textfile);

        const char* filename = "weather_data.txt";
        Average_temp(filename);
        min_temp(filename);
        max_temp(filename);
        write_report();
        json_object_put(json);
    }
    else {
        printf("Failed to extract temperature from file.\n");
    }

    return 0;
}