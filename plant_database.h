// (source: https://sustainablecampus.unimelb.edu.au/__data/assets/pdf_file/0005/2839190/Indoor-plant-workshop-Light-and-Moisture-Requirements.pdf)

// WATER
// 1: Dry: Does not need very much water and can stand low humidity. 
// 2: Dry-moist
// 3: Moist – Requires a moderate amount of water and loves some humidity in the atmosphere.

// LIGHT LEVEL
// 1: Low – range 270 to 807 Lux.
// 2: Medium – range of 807 to 1614 Lux.
// 3: High – range 1614 to 5000 Lux.
// 4: Very high - range 5000 to 10764 Lux.

// Temperature for plants (in Celsius)
// 1. Aechmea fasciata: 15 - 25
// 2. Anthurium: 18 - 26
// 3. Aspidistra elatior: 7 - 29
// 4. Begonia: 18 - 27
// 5. Calathea: 18 - 30
// 6. Chlorophytum elatum: 15 - 32
// 7. Cordyline fruticosa: 18 - 30 
// 8. Nephrolepsis cordifolia: 15 - 25
// 9. Sansevieria: 13 - 29

// Humidity for plants (in %)
// 1. Aechmea fasciata: 40 - 60
// 2. Anthurium: 50 - 80
// 3. Aspidistra elatior: 30 - 50
// 4. Begonia: 60 - 90
// 5. Calathea: 50 - 90
// 6. Chlorophytum elatum: 50 - 60
// 7. Cordyline fruticosa: 40 - 60
// 8. Nephrolepsis cordifolia: 60 - 90
// 9. Sansevieria: 30 - 50

const int plant_len = 9;


char* short_name[plant_len] = {"Aechmea fasciata", "Anthurium", "Aspidistra ...", "Begonia", "Calathea", "Chlorophytum ...", 
                    "Cordyline ...", "Nephrolepsis ...", "Sansevieria"};
char* plant_name[plant_len] = {"Aechmea \nfasciata", "Anthurium", "Aspidistra \nelatior", "Begonia", "Calathea", "Chlorophytum \nelatum", 
                    "Cordyline \nfruticosa", "Nephrolepsis \ncordifolia", "Sansevieria"};
int plant_water[plant_len] = {3, 3, 2, 3, 3, 3, 3, 3, 1};
int plant_water_val[4] = {2700, 2500, 2300, 2000};  // guesswork based on sensor readings
int plant_light_level[plant_len] = {2, 2, 1, 4, 2, 3, 2, 2, 1};
int plant_light_val[5] = {270, 807, 1614, 5000, 10764};

int plant_temp_min[plant_len] = {15, 18, 7, 18, 18, 15, 18, 15, 13};
int plant_temp_max[plant_len] = {25, 26, 29, 27, 30, 32, 30, 25, 29};

int plant_humidity_min[plant_len] = {40, 50, 30, 60, 50, 50, 40, 60, 30};
int plant_humidity_max[plant_len] = {60, 80, 50, 90, 90, 60, 60, 90, 50};