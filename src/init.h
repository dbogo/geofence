#ifndef INIT_H
#define INIT_H

#include "types.h"
#include "logInterface.h"
#include "GPSInterface.h"


#define COMMON_ERR_STR "Try geofence -h for help.\n"

#define ARGV_ERROR 1
#define FOPEN_FAIL 2
#define ARGV_OK 0

extern FILE* argvInputFile;

/*!
 * @brief      Initialize some important stuff
 * @param 	   GPSHandler a pointer to a struct with function pointers to gps related functions.
 * @param      zone       pointer to Zone_general strcut
 * @param      logMaster  pointer to Log_Master struct
 * @param      edges      double pointer to the Edge struct. pointer to the array of edges 
 * 						  of the polygon represented by zone.
 */
void init(GPS_Actions* GPSHandler, FullGPSData* gpsData, Zone_general* zone, Log_Master* logMaster, Edge** edges);

/**
 * @brief      calls function to initialize some stuff that works only on the Raspberry-Pi
 *             e.g the WiringPi library or PiFace-cad
 *             exists only if #ifdef WIRINGPI
 */
void init_platform_specific_modules(void);

/**
 * @brief      initializes the data pointed to by the @p struct pointer
 * @param      gpsData  double pointer to the data
 */
void init_gps_data(FullGPSData** gpsData);

/**
 * @brief      Receives argc, argv from main and parses that input
 * @param      zone  A struct with the geofence information (polygon, count, etc.)
 * @param[in]  argc  argc
 * @param      args  argv. should contain the name of the file with the inputs.
 * @return     returns different exit codes (defined in init.h) depending on the situation. 
 */
int parse_input_args(Zone_general* zone, int argc, char** args);

/**
 * @brief      initializes the geofence vertices etc from the argv.
 *             the format is xx.xxx,xx.xxx xx.xxx,xx.xxx ... etc
 *
 * @param      zone  pointer to a Zone_general struct
 * @param  	   argc  argc
 * @param 	   args  argv
 *
 * @return     one of the error/success codes defined in init.h
 */
int init_geofence_from_argv(Zone_general* zone, int argc, char** args);

/**
 * @brief      initializes the geofence vertices from the data that's user-made file which is specified in argv
 *
 * @param      zone           pointer to a Zone_general struct
 * @param      args           argv
 *
 * @return     one of the error/success codes defined in init.h
 */
int init_geofence_from_file(Zone_general* zone, char** args);

/**
 * @brief      Display the command-line usage statement
 */
void display_help_message(void);


/**
 * @brief      Parses a single line that contains vertex coordinates.
 * @param      str   the line of text that is read from the input file
 * @return     returns a struct of the point, initialized with the coordinates from the line.
 */
GEO_Point parse_line(char* str);

#endif /* INIT_H */