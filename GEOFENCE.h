


#define APRS_Not_Ok 1

#include "stdint.h"


// GEOFENCE ARRAYS (longitude, latitude)

static float ArgParUruF[] = {
	-57.79910,		-18.67750,
	-53.48140,		-26.66710,
	-57.13990,		-29.77390,
	-49.62520,		-34.51560,
	-60.24900,		-58.56250,
	-73.49850,		-50.35950,
	-67.54390,		-21.43260,
	-57.79910,		-18.67750
};

static float AustraliaF[] = {
	147.56840,		-46.92030,
	166.02540,		-29.15220,
	144.14060,		-9.18890,
	98.78910,		-11.69530,
	112.41210,		-39.77480,
	147.56840,		-46.92030
};

static float BrazilF[] = {
	-57.04100,		-29.76440,
	-49.65820,		-34.45220,
	-28.30080,		-5.87830,
	-51.50390,		4.30260,
	-60.55660,		5.00340,
	-74.17970,		-6.49000,
	-57.74410,		-18.56290,
	-53.34960,		-26.66710,
	-57.04100,		-29.76440
};

static float ChinaF[] = {
	87.18750,		49.38240,
	77.51104,		44.59703,
	71.63090,		36.94990,
	93.03226,		25.16517,
	110.39060,		15.96130,
	124.76074,		18.47961,
	124.54103,		36.73889,
	132.17135,		41.06670,
	136.35137,		47.03273,
	123.44237,		54.41894,
	104.58986,		45.05798,
	87.18750,		49.38240
};

static float CostNicPanF[] = {
	-88.76950,		11.99630,
	-80.20020,		4.80640,
	-76.61870,		9.42740,
	-82.70510,		15.39010,
	-88.76950,		11.99630
};

static float FranceF[] = {
	7.95410,		48.76340,
	1.91160,		50.94460,
	-3.64750,		48.29780,
	-1.49410,		43.34120,
	2.98830,		42.56930,
	7.40480,		43.78700,
	5.88870,		46.45300,
	7.95410,		48.76340
};

static float JapanF[] = {
	138.71337,		49.79545,
	130.96153,		38.83591,
	124.66191,		34.18805,
	124.76075,		24.68693,
	144.22850,		23.40280,
	156.09372,		49.72447,
	138.71337,		49.79545
};

static float LatviaF[] = {
	26.64180,		55.68380,
	28.17990,		56.20670,
	27.78440,		57.33250,
	25.00490,		58.00230,
	24.14790,		57.17200,
	21.78590,		57.68650,
	20.81910,		56.07200,
	22.19240,		56.44430,
	25.68600,		56.18230,
	26.64180,		55.68380
};

static float New_ZealandF[] = {
	179.99990,		-37.78810,
	179.99990,		-55.22580,
	154.33590,		-45.82880,
	172.26560,		-28.76770,
	179.99990,		-37.78810
};

static float RomaniaF[] = {
	27.12520,		47.95310,
	22.95040,		47.97520,
	20.63230,		45.85940,
	23.24710,		43.86620,
	29.13570,		43.85040,
	27.12520,		47.95310
};

static float South_KoreaF[] = {
	132.17653,		41.01310,
	124.61794,		36.70367,
	124.65089,		34.26179,
	130.87903,		38.87847,
	132.17653,		41.01310
};

static float ThailandF[] = {
	110.23679,		15.96138,
	93.60347,		24.72692,
	94.89994,		6.03131,
	104.89745,		6.07500,
	110.23679,		15.96138
};

static float  UKF[] = {
	-0.65920,		60.97310,
	-7.58060,		58.07790,
	-8.21780,		54.23960,
	-4.76810,		53.80070,
	-5.86670,		49.76710,
	1.30740,		50.85450,
	1.86770,		52.78950,
	-2.04350,		55.97380,
	-0.65920,		60.97310
};

static float VenezuelaF[] = {
	-66.65410,		0.18680,
	-60.99610,		5.41910,
	-59.52390,		9.38400,
	-72.15820,		12.49020,
	-72.48780,		7.10090,
	-67.96140,		5.72530,
	-66.65410,		0.18680
};


// VARIABLES
long GEOFENCE_APRS_frequency; 
long GEOFENCE_no_tx;



// FUNCTIONS
long pointInPolygon(long polyCorners, long * polygon, long latitude, long longitude);
long pointInPolygonF(long polyCorners, float * polygon, float latitude, float longitude);
void GEOFENCE_position(float latitude, float longitude);



/*
 * GEOFENCE.c
 *
 * Created: 5.11.2016 22:04:58
 *  Author: Tomy2
 *  Modified Ken K9YO 3.8.2023
 */ 

/*
	Adapted version of pointInPolygon() function from:	http://alienryderflex.com/polygon/
	
	Returns '0' if the point is outside of the polygon and '1' if it's inside.
	
	Expects input DEGREES * 100000 for latitude and longitude. Eg 4961070 for 49.61070 N.
	The reason is to make sure all calculations fit inside long.
	
	However, this function is not very accurate due to rounding within the computation.
*/
long pointInPolygon(long polyCorners, long * polygon, long latitude, long longitude)
{
	long i;
	long j = polyCorners * 2 - 2;
	long oddNodes = 0;

	for(i = 0; i < polyCorners * 2; i += 2)
	{
		if((polygon[i + 1] < latitude && polygon[j + 1] >= latitude
			|| polygon[j + 1] < latitude && polygon[i + 1] >= latitude)
			&& (polygon[i] <= longitude || polygon[j] <= longitude))
		{
			oddNodes ^= (polygon[i] + (latitude - polygon[i + 1])
			/ (polygon[j + 1] - polygon[i + 1]) * (polygon[j] - polygon[i]) < longitude);
		}

		j = i;
	}

	return oddNodes;
}


/*
	Adapted version of pointInPolygon() function from:	http://alienryderflex.com/polygon/
	
	Returns '0' if the point is outside of the polygon and '1' if it's inside.
	
	Uses FLOAT input for better accuracy.
*/
long pointInPolygonF(long polyCorners, float * polygon, float latitude, float longitude)
{
	long i;
	long j = polyCorners * 2 - 2;
	long oddNodes = 0;

	for(i = 0; i < polyCorners * 2; i += 2)
	{
		if((polygon[i + 1] < latitude && polygon[j + 1] >= latitude
		|| polygon[j + 1] < latitude && polygon[i + 1] >= latitude)
		&& (polygon[i] <= longitude || polygon[j] <= longitude))
		{
			oddNodes ^= (polygon[i] + (latitude - polygon[i + 1])
			/ (polygon[j + 1] - polygon[i + 1]) * (polygon[j] - polygon[i]) < longitude);
		}

		j = i;
	}

	return oddNodes;
}


/*
	Changes GEOFENCE_APRS_frequency and GEOFENCE_no_tx global variables based on the input coordinates.
	
	FREQUENCIES:
						Africa				144.800
						Europe				144.800
						Russia				144.800
						Canada				144.390
						Mexico				144.390
						USA					144.390
						Costa Rica			145.010
						Nicaragua			145.010
						Panama				145.010
						Venezuela			145.010
						Brazil				145.570
						Colombia			144.390
						Chile				144.390
						Argentina			144.930
						Paraguay			144.930
						Uruguay				144.930
						China				144.640
						Japan				144.660
						South Korea			144.620
						Thailand			145.525
						Australia			145.175
						New Zealand			144.575
						Indonesia			144.390
						Malaysia			144.390
		
	NO AIRBORNE APRS:
						France
						Latvia
						United Kingdom
	
	Expected input FLOAT for latitude and longitude as in GPS_UBX_latitude_Float and GPS_UBX_longitude_Float.
*/
void GEOFENCE_position(float latitude, float longitude)
{

	// SECTOR 1
	if(longitude > -38.0 && longitude < 73.0)
	{
		
		// S 1/2
		if(latitude > 0.0)
		{
			if(pointInPolygonF(9, UKF, latitude, longitude) == 1)				{GEOFENCE_no_tx = 1;GEOFENCE_APRS_frequency = 144800000;}
			else if(pointInPolygonF(10, LatviaF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 1;GEOFENCE_APRS_frequency = 144800000;}
			else if(pointInPolygonF(8, FranceF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 1;GEOFENCE_APRS_frequency = 144800000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144800000;}
		}
		
		// S 2/2
		else
		{
			if(pointInPolygonF(9, BrazilF, latitude, longitude) == 1)			{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145570000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144800000;}
		}
	}
	
	// SECTOR 2
	else if(longitude <= -38.0)
	{
		
		// S 1/2
		if(latitude > 12.5) // North America
		{
																				{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144390000;}
		}
		
		// S 2/2
		else
		{
			if(pointInPolygonF(8, ArgParUruF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144930000;}
			else if(pointInPolygonF(9, BrazilF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145570000;}
			else if(pointInPolygonF(7, VenezuelaF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145010000;}
			else if(pointInPolygonF(5, CostNicPanF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145010000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144390000;}
		}
	}
	
	// SECTOR 3
	else if(longitude >= 73.0)
	{
		
		// S 1/2
		if(latitude > 19.2)
		{
			if(pointInPolygonF(12, ChinaF, latitude, longitude) == 1)			{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144640000;}
			else if(pointInPolygonF(7, JapanF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144660000;}
			else if(pointInPolygonF(5, South_KoreaF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144620000;}
			else if(pointInPolygonF(5, ThailandF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145525000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144800000;}
		}
		
		// S 2/2
		else
		{
			if(pointInPolygonF(6, AustraliaF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145175000;}
			else if(pointInPolygonF(5, New_ZealandF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144575000;}
			else if(pointInPolygonF(5, ThailandF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145525000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144390000;}
		}
	}
	
	// shouldn't get here
	else
	{
																				{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144800000;}
	}
}


