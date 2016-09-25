// NOTE: maybe this file should be named differently !

#include <math.h>
#include <string.h>

#include "GPSInterface.h"
#include "libs/RPiGPSDemo/src/rpi_gps_demo.h"
#include "libs/GPSDemo/src/gps_demo.h"


int GPS_init(GPS_Actions* gpsHandler){
	#ifdef GPS_RPI
		gpsHandler->getGPS = getGPSSample_RPI;
		printf("uses RPi implementation.\n");
	#else
		gpsHandler->getGPS = getGPSSample_DEMO;
		printf("uses DEMO implementation\n");
	#endif

	return 0;
}

int create_edges(Zone_general* zone, Edge** edges){

	Edge tmp[zone->numVertices]; //dummy

	for(unsigned int i = 0; i < zone->numVertices; i++){
		tmp[i].p1 = zone->vertices[i % zone->numVertices];
		tmp[i].p2 = zone->vertices[(i+1) % zone->numVertices];
	}												

	// move the dummy edges to where the pointer points.
	*edges = malloc(sizeof(tmp));
	memcpy(*edges, tmp, sizeof(tmp));

	for(unsigned int i = 0; i < zone->numVertices; i++){
		printf("E%d: (%lf, %lf), (%lf,%lf)\n", (int)(i % zone->numVertices), (*edges)[i].p1.longitude,
						(*edges)[i].p1.latitude, (*edges)[i].p2.longitude, (*edges)[i].p2.latitude);
	}	

	return zone->numVertices;
}

/*
1. an upward edge includes its starting endpoint, and excludes its final endpoint;
2. a downward edge excludes its starting endpoint, and includes its final endpoint;
3. horizontal edges are excluded
4. the edge-ray intersection point must be strictly right of the point P.
*/

inline int isLeft(GEO_Point p, Edge e){
	/* Cross Product. */
	return ((e.p2.longitude - e.p1.longitude)*(p.latitude - e.p1.latitude) - 
		(e.p2.latitude - e.p1.latitude)*(p.longitude - e.p1.longitude));
}

inline bool upwards_cross(Edge e, GEO_Point p){
	return (e.p1.latitude <= p.latitude) && (e.p2.latitude > p.latitude);
}

inline bool downwards_cross(Edge e, GEO_Point p){
	return (e.p2.latitude <= p.latitude);
}

int wn_PnPoly(FullGPSData* location, Zone_general* zone, Edge* edges){
	//FIXME: check if both cc & ccw points' order, works.
	GEO_Point p = { .longitude = location->longitude, .latitude = location->latitude };
	int w = 0; // the winding number

	// NOTE: amount of edges = amount of vertices! hence zone->numvertices
	for(size_t i = 0; i < zone->numVertices; i++){
		if(upwards_cross(edges[i], p)){
				if(isLeft(p, edges[i]) > 0){
					w++;
				}
		} else if(downwards_cross(edges[i], p)){ 
			if(isLeft(p, edges[i]) < 0){
				w--;
			}
		}
	}
	
	return w;
}

/* TODO */

void find_mbr(Zone_general* polygon){
	polygon->mbr.p1.longitude = polygon->mbr.p2.longitude = polygon->vertices[0].longitude;
	polygon->mbr.p1.latitude = (polygon->mbr.p1.latitude = polygon->vertices[0].latitude);

	for(size_t i = 0; i < polygon->numVertices; i++){
		if (polygon->vertices[i].longitude < polygon->mbr.p1.longitude)
			polygon->mbr.p1.longitude = (polygon->vertices[i]).longitude;
		else if(polygon->vertices[i].longitude > polygon->mbr.p2.longitude)
			polygon->mbr.p2.longitude = (polygon->vertices[i]).longitude;

		if (polygon->vertices[i].latitude < polygon->mbr.p1.latitude)
			polygon->mbr.p1.latitude = (polygon->vertices[i]).latitude;
		else if(polygon->vertices[i].latitude > polygon->mbr.p2.latitude)
			polygon->mbr.p2.latitude = (polygon->vertices[i]).latitude;
	}

	//TODO: log.
	printf("xmin:%f xmax%f\nYmin:%f Ymax:%f\n\n", polygon->mbr.p1.longitude, polygon->mbr.p2.longitude,
												 polygon->mbr.p1.latitude, polygon->mbr.p2.latitude);
}
