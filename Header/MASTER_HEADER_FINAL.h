//constants header

const int NSIDE = 2;
const int NRPD = 16;
const int NTS = 10;

int NChannels = 50;

double RPD_Cuts[2][16] = {0};

//Frac values begin
double SUM_Zero_Zero_blocks = 12344385;//1189942;

//double Fractionals_Zero_Zero_blocks[16] = {97437, 286007, 350465, 81356, 497512, 2220690, 2017750, 319411, 493131, 2426990, 0/*2599370*/, 379290, 251604, 1349060, 1389510, 184172};//{3062.5522, 7192.7705, 7838.7592, 3243.2185, 23174.7141, 147410.2537, 145177.2869, 23494.1899, 35129.4175, 252145.8565, 253389.8197, 34650.027, 11489.5848, 95352.9511, 96308.8677, 11025.7629}; //{3722, 9023, 9985, 3319, 28957, 194653, 189180, 28047, 31514, 220689, 218147, 31090, 12178, 98593, 99981, 10864};
//used a series of -5 -2.5 0 2.5 5 y geant values and weighted and summed ^
double RPD_fC_Sum[2] = {0};

double RPD_Data_Channel_SUM[2][16] = {0};

double Fractional_Weights[2][16] = {0};
//frac values end

double RawDataRPD[NSIDE][NRPD][NTS] = { {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}},    //neg  // these are used to store the raw data 
										    {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}} }; //pos

double RPD_Block_In_Phi[16] = {2.35619, 1.89255, 1.24905, 0.785398, 2.81984, 2.35619, 0.785398, 0.321751, -2.81984, -2.35619, -0.785398, -0.321751, -2.35619, -1.89255, -1.24905, -0.785398};
/*  positions in cm from 0,0 being the center of the rpd -x is left -y is down*/

double Raw_RXN_Plane_NEG = 0;
double Raw_RXN_Plane_POS = 0;
double NEG_PreCor_RXN_PLANE = 0;
double POS_PreCor_RXN_PLANE = 0;

double POS_CORRECTED_RPD_Event_Plane_Psi = 0;
double NEG_CORRECTED_RPD_Event_Plane_Psi = 0;

double POS_Fourier_CORRECTED_RPD_Event_Plane_Psi = 0;
double NEG_Fourier_CORRECTED_RPD_Event_Plane_Psi = 0;

double RPD = 3;

int N_bad_RPDs = 0;

int N_entries = 0;

int RunNumber = 0;
int EventNumber = 0;
int NumberofAcceptedTracks = 0;
double centBin = 0;

double RPD_Frac[2][16] = {0};

int centIter_Array_Value = 0;

int P = 0;
int N = 0;

double centval = 0;
double CentralityValue = 0;

double X_q_Vector[2] = {0};
double Y_q_Vector[2] = {0};

double   Mean_X_Pos = 0; 
double STDDEV_X_Pos = 0;
double   Mean_Y_Pos = 0;
double STDDEV_Y_Pos = 0;
double   Mean_X_Neg = 0;
double STDDEV_X_Neg = 0;
double   Mean_Y_Neg = 0;
double STDDEV_Y_Neg = 0;

double   Mean_X_Pos_CENT[10] = {0}; //upgrade so 10 centrality bin seperation for rxn plane calibration can be done 
double STDDEV_X_Pos_CENT[10] = {0};
double   Mean_Y_Pos_CENT[10] = {0};
double STDDEV_Y_Pos_CENT[10] = {0};
double   Mean_X_Neg_CENT[10] = {0};
double STDDEV_X_Neg_CENT[10] = {0};
double   Mean_Y_Neg_CENT[10] = {0};
double STDDEV_Y_Neg_CENT[10] = {0};

double Pos_Fourier_Mean_Sin = 0;
double Pos_Fourier_Mean_Cos = 0;
double Neg_Fourier_Mean_Sin = 0;
double Neg_Fourier_Mean_Cos = 0;

double Pos_Fourier_Mean_Sin_CENT[10][100] = {0}; //BE CAREFUL WILL HAVE EMPTY VALUES I WANT TO USE AN ARRAY OF VECTORS HERE BUT COMPLICATED
double Pos_Fourier_Mean_Cos_CENT[10][100] = {0};
double Neg_Fourier_Mean_Sin_CENT[10][100] = {0};
double Neg_Fourier_Mean_Cos_CENT[10][100] = {0};

#include <vector>

std::vector<int> Event;
std::vector<int> cenIter_storage_vector;
std::vector<double> CentralityBin;
std::vector<int> QuantityofAccepetedTracks;
std::vector<double> C_Vector_X_q_POS;
std::vector<double> C_Vector_X_q_NEG;
std::vector<double> C_Vector_Y_q_POS;
std::vector<double> C_Vector_Y_q_NEG;
std::vector<double> Pos_Reaction_Plane_Storage_Vector[10];
std::vector<double> Neg_Reaction_Plane_Storage_Vector[10];
std::vector<double> Pre_Fourier_Pos_Reaction_Plane_Storage_Vector;
std::vector<double> Pre_Fourier_Neg_Reaction_Plane_Storage_Vector;

//




#include <cstdint>
#include <cstring>
#include <math.h>

#ifndef RPD_CUTS
#define RPD_CUTS


using namespace std;

//Cuts
double Cut_326776[2][16] = {{3.0,4.0,5.0,5.0,3.0,4.0,4.0,4.0,4.0,5.0,6.0,7.0,5.0,5.0,5.0,6.0},{4.0,15.0,4.0,4.0,4.0,4.5,4.0,5.0,4.0,5.0,12.0,4.0,5.0,4.0,5.0,5.0}};
double Cut_326822[2][16] = {{3.0,4.0,5.0,5.0,3.0,4.0,4.0,4.0,4.0,5.0,6.0,7.0,5.0,5.0,5.0,6.0},{4.0,15.0,4.0,4.0,4.0,4.5,4.0,5.0,4.0,5.0,12.0,4.0,5.0,4.0,5.0,5.0}};
double Cut_326883[2][16] = {{3.0,4.0,5.0,5.0,3.0,4.0,4.0,4.0,4.0,5.0,6.0,7.0,5.0,5.0,5.0,6.0},{4.0,15.0,4.0,4.0,4.0,4.5,4.0,5.0,4.0,5.0,12.0,4.0,5.0,4.0,5.0,5.0}};
double Cut_326943[2][16] = {{3.0,4.0,5.0,5.0,3.0,4.0,4.0,4.0,4.0,5.0,6.0,7.0,5.0,5.0,5.0,6.0},{4.0,15.0,4.0,4.0,4.0,4.5,4.0,5.0,4.0,5.0,12.0,4.0,5.0,4.0,5.0,5.0}};
double Cut_327078[2][16] = {{4.0,6.0,5.0,5.0,3.0,5.0,4.5,5.0,6.0,6.0,6.0,6.0,6.0,5.0,6.0,6.0},{5.0,16.0,5.0,4.0,4.0,5.0,5.0,6.0,4.0,6.0,13.0,4.0,5.0,5.0,6.0,6.0}};
double Cut_327211[2][16] = {{4.0,6.0,5.0,5.0,3.0,5.0,4.5,5.0,6.0,6.0,6.0,6.0,6.0,5.0,6.0,6.0},{5.0,16.0,5.0,4.0,4.0,5.0,5.0,6.0,4.0,6.0,13.0,4.0,5.0,5.0,6.0,6.0}};
double Cut_327327[2][16] = {{4.0,6.0,5.0,5.0,3.0,5.0,4.5,5.0,6.0,6.0,6.0,6.0,6.0,5.0,6.0,6.0},{5.0,16.0,5.0,4.0,4.0,5.0,5.0,6.0,4.0,6.0,13.0,4.0,5.0,5.0,6.0,6.0}};
double Cut_327464[2][16] = {{4.0,6.0,5.0,5.0,3.0,5.0,4.5,5.0,6.0,6.0,6.0,6.0,6.0,5.0,6.0,6.0},{5.0,16.0,5.0,4.0,4.0,5.0,5.0,6.0,4.0,6.0,13.0,4.0,5.0,5.0,6.0,6.0}};
double Cut_327524[2][16] = {{4.0,6.0,5.0,5.0,3.0,5.0,4.5,5.0,6.0,6.0,6.0,6.0,6.0,5.0,6.0,6.0},{5.0,16.0,5.0,4.0,4.0,5.0,5.0,6.0,4.0,6.0,13.0,4.0,5.0,5.0,6.0,6.0}};
double Cut_327560[2][16] = {{4.0,6.0,5.0,5.0,3.0,5.0,4.5,5.0,6.0,6.0,6.0,6.0,6.0,5.0,6.0,6.0},{5.0,16.0,5.0,4.0,4.0,5.0,5.0,6.0,4.0,6.0,13.0,4.0,5.0,5.0,6.0,6.0}};
double Cut_Default[2][16] = {{pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9)},{pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9),pow(9,9)}};

void RPD_Cuts_Generator(int RunNumber, double (&RPD_Cuts)[2][16]){

	switch (RunNumber) {
		case 326776 :
			memcpy(RPD_Cuts, Cut_326776, sizeof(RPD_Cuts));
			break;
		case 326822 :
			memcpy(RPD_Cuts, Cut_326822, sizeof(RPD_Cuts));
			break;
		case 326883 :
			memcpy(RPD_Cuts, Cut_326883, sizeof(RPD_Cuts));
			break;
		case 326943 :
			memcpy(RPD_Cuts, Cut_326943, sizeof(RPD_Cuts));
			break;
		case 327078 :
			memcpy(RPD_Cuts, Cut_327078, sizeof(RPD_Cuts));
			break;
		case 327211 :
			memcpy(RPD_Cuts, Cut_327211, sizeof(RPD_Cuts));
			break;
		case 327327 :
			memcpy(RPD_Cuts, Cut_327327, sizeof(RPD_Cuts));
			break;
		case 327464 :
			memcpy(RPD_Cuts, Cut_327464, sizeof(RPD_Cuts));
			break;
		case 327524 :
			memcpy(RPD_Cuts, Cut_327524, sizeof(RPD_Cuts));
			break;
		case 327560 :
			memcpy(RPD_Cuts, Cut_327560, sizeof(RPD_Cuts));
			break;
		default:
			memcpy(RPD_Cuts, Cut_Default, sizeof(RPD_Cuts));

	}

}

#endif

#ifndef SRPD_Calibrations
#define SRPD_Calibrations

	//SRPD_weights_runnumber[side][cent][block]
//side = 0 for Neg, side = 1 for Pos
//cent = 0 for 0-10% (0-20)......cent = 9 for 90-100% (180-200)
//block = 0 for upper left corner......block = 15 for bottom right corner


double SRPD_weights_326776[2][10][16] = 
{
{       {1.171, 1, 1, 1, 1, 1, 1, 1.069, 1, 1, 1, 1, 1, 1.384, 1, 1},
	{1.255, 1, 1, 1, 1, 1, 1, 1.055, 1, 1, 1, 1, 1.072, 1.465, 1, 1},
	{1.312, 1, 1, 1, 1, 1.061, 1, 1, 1, 1, 1, 1, 1.113, 1.516, 1, 1},
	{1.351, 1, 1, 1, 1, 1.089, 1, 1, 1, 1, 1, 1, 1.133, 1.55, 1, 1},
	{1.398, 1, 1, 1, 1, 1.121, 1, 1, 1, 1, 1, 1, 1.166, 1.588, 1, 1},				
	{1.5, 1, 1, 1, 1, 1.142, 1, 1, 1, 1, 1, 1, 1.185, 1.616, 1, 1},				
	{1.443, 1, 1, 1, 1, 1.163, 1, 1, 1, 1, 1, 1, 1.206, 1.638, 1, 1},				
	{1.467, 1, 1, 1, 1, 1.181, 1, 1, 1, 1, 1, 1, 1.221, 1.656, 1, 1},				
	{1.476, 1, 1, 1, 1, 1.188, 1, 1, 1, 1.04, 1, 1, 1.226, 1.659, 1, 1},
	{1.054, 1, 1, 1, 1, 1.25, 1, 1, 1, 1, 1, 1, 1.215, 1.734, 1, 1}		},

{       {1, 5.126, 1.194, 2.2, 2.244, 1.063, 1.941, 1, 1.267, 1.288, 22.31, 7.3, 1, 1, 1.593, 1.712},
	{1, 4.639, 1.114, 2.148, 2.097, 1, 1.852, 1, 1.167, 1.17, 21.36, 7.289, 1, 1, 1.536, 1.734},
	{1, 4.355, 1.065, 2.137, 2.017, 1, 1.795, 0.95, 1.109, 1.108, 23.75, 8.4, 1, 1, 1.491, 1.9},
	{1, 4.5, 1.046, 2.163, 1.98, 1, 1.767, 1.05, 1.083, 1.073, 22.3, 8.75, 1, 1, 1.45, 1.975},
	{1, 4.077, 1, 2.161, 1.921, 1, 1.729, 1.05, 1.05, 1, 21.75, 9.285, 1, 1, 1.452, 2.01},
	{1, 3.988, 1, 2.178, 1.887, 1, 1.65, 1.084, 1, 1, 20.85, 8.95, 1, 1, 1.441, 2.125},
	{1, 3.923, 1, 2.218, 1.847, 1, 1.65, 1.133, 1, 1, 20.75, 9.25, 1, 1, 1.431, 2.2},
	{1, 4.1, 1, 2.222, 1.813, 1, 1.65, 1.2, 1, 1, 20.95, 9.55, 1, 1, 1.423, 2.15},
	{1, 4, 1, 2.234, 1.787, 1, 1.701, 1.172, 1, 1, 19.95, 9.75, 1, 1, 1.408, 2.333},
	{1, 4.5, 1, 2.075, 1.75, 1, 1.625, 1.174, 1, 1, 18.5, 7.665, 1, 1, 1.4, 1.974}		}
};


double SRPD_weights_326822[2][10][16] = 
{
{       {1.185, 1, 1, 1, 1, 1, 1, 1.125, 1, 1, 1, 1, 1, 1.384, 1, 1},
	{1.269, 1, 1, 1, 1, 1, 1, 1.105, 1, 1, 1, 1, 1, 1.456, 1, 1},
	{1.324, 1, 1, 1, 1, 1.06, 1, 1.069, 1, 1, 1, 1, 1.098, 1.495, 1, 1},
	{1.368, 1, 1, 1, 1, 1.089, 1, 1.042, 1, 1, 1, 1, 1.127, 1.535, 1, 1},
	{1.399, 1, 1, 1, 1, 1.114, 1, 1, 1, 1, 1, 1, 1.149, 1.555, 1, 1},				
	{1.427, 1, 1, 1, 1, 1.141, 1, 1, 1, 1, 1, 1, 1.171, 1.589, 1, 1},				
	{1.454, 1, 1, 1, 1, 1.155, 1, 1, 1, 1, 1, 1, 1.185, 1.604, 1, 1},				
	{1.463, 1, 1, 1, 1, 1.171, 1, 1, 1, 1, 1, 1, 1.197, 1.62, 1, 1},				
	{1.475, 1, 1, 1, 1, 1.175, 1, 1, 1, 1, 1, 1, 1.206, 1.612, 1, 1},
	{1.52, 1, 1, 1, 1, 1.333, 1, 1, 1, 1, 1, 1, 1.209, 1.688, 1, 1}		},

{       {1, 5.011, 1.19, 2.149, 2.2, 1.049, 1.911, 1, 1.259, 1.271, 21.67, 6.692, 1, 1, 1.56, 1.568},
	{1, 4.62, 1.108, 2.102, 2.071, 1, 1.827, 1, 1.162, 1.17, 21.15, 6.75, 1, 1, 1.495, 1.587},
	{1, 4.361, 1.075, 2.12, 2.001, 1, 1.784, 1, 1.115, 1.115, 20.5, 6.95, 1, 1, 1.466, 1.634},
	{1, 4.197, 1.045, 2.108, 1.948, 1, 1.749, 1, 1.081, 1.077, 20.25, 7.15, 1, 1, 1.442, 1.69},
	{1, 4.114, 1, 2.137, 1.91, 1, 1.737, 1, 1, 1.051, 19.85, 7.5, 1, 1, 1.435, 1.85},
	{1, 3.999, 1, 2.139, 1.864, 1, 1.712, 1, 1, 1, 19.46, 7.627, 1, 1, 1.422, 1.797},
	{1, 3.947, 1, 2.18, 1.836, 1, 1.711, 1.055, 1, 1, 19.95, 8.25, 1, 1, 1.416, 1.925},
	{1, 3.807, 1, 2.197, 1.809, 1, 1.706, 1.082, 1, 1, 19.75, 8.333, 1, 1, 1.406, 1.95},
	{1, 3.665, 1, 2.19, 1.769, 1, 1.707, 1, 1, 1, 19.15, 8.294, 1, 1, 1.389, 1.995},
	{1, 3.45, 1, 2, 1.667, 1, 1.667, 1, 1, 1, 17, 6.85, 1, 1, 1.5, 1.772}		}
};


double SRPD_weights_326883[2][10][16] = 
{
{       {1.211, 1, 1, 1, 1, 1, 1, 1.184, 1, 1, 1, 1, 1, 1.391, 1, 1},
	{1.294, 1, 1, 1, 1, 1.041, 1, 1.154, 1, 1, 1, 1, 1.067, 1.467, 1, 1},
	{1.352, 1, 1, 1, 1, 1.083, 1, 1.128, 1, 1, 1, 1, 1.105, 1.515, 1, 1},
	{1.396, 1, 1, 1, 1, 1.116, 1, 1.103, 1, 1, 1, 1, 1.137, 1.55, 1, 1},
	{1.425, 1, 1, 1, 1, 1.137, 1, 1.068, 1, 1, 1, 1, 1.153, 1.577, 1, 1},				
	{1.45, 1, 1, 1, 1, 1.165, 1, 1.033, 1, 1, 1, 1, 1.173, 1.605, 1, 1},				
	{1.48, 1, 1, 1, 1, 1.179, 1, 1, 1, 1, 1, 1, 1.188, 1.618, 1, 1},				
	{1.491, 1, 1, 1, 1, 1.206, 1, 1, 1, 1, 1, 1, 1.207, 1.645, 1, 1},				
	{1.497, 1, 1, 1, 1, 1.206, 1, 1, 1, 1, 1, 1, 1.208, 1.638, 1, 1},
	{1.529, 1, 1.105, 1, 1, 1.402, 1, 1, 1.13, 1.104, 1, 1, 1.211, 1.687, 1, 1}	},

{       {1, 4.866, 1.156, 2.076, 2.174, 1, 1.854, 1, 1.249, 1.256, 20.85, 6.259, 1, 1, 1.517, 1.472},
	{1, 4.478, 1.08, 2.056, 2.051, 1, 1.773, 1, 1.156, 1.15, 20.5, 6.4, 1, 1, 1.458, 1.507},
	{1, 4.275, 1, 2.042, 1.964, 1, 1.724, 1, 1.1, 1.096, 19.84, 6.5, 1, 1, 1.43, 1.555},
	{1, 4.095, 1, 2.037, 1.913, 1, 1.69, 1, 1.062, 1.058, 19.48, 6.625, 1, 1, 1.405, 1.595},
	{1, 3.994, 1, 2.058, 1.873, 1, 1.675, 1, 1, 1, 19.25, 6.9, 1, 1, 1.393, 1.626},
	{1, 3.913, 1, 2.078, 1.841, 1, 1.663, 1, 1, 1, 19.15, 7.333, 1, 1, 1.386, 1.7},
	{1, 3.853, 1, 2.106, 1.806, 1, 1.657, 1, 1, 1, 19.4, 7.775, 1, 1, 1.381, 1.85},
	{1, 3.7, 1, 2.137, 1.779, 1, 1.6, 1, 1, 1, 18.95, 7.7, 1, 1, 1.359, 1.825},
	{1, 3.57, 1, 2.122, 1.74, 1, 1.636, 1, 1, 1, 18.45, 7.68, 1, 1, 1.348, 1.817},
	{1, 3.359, 1, 1.933, 1.534, 1, 1.467, 1, 1, 1, 16.25, 6.36, 1, 1, 1.227, 1.665}		}
};


double SRPD_weights_326943[2][10][16] = 
{
{       {1.206, 1, 1, 1, 1, 1, 1, 1.242, 1, 1, 1, 1, 1, 1.378, 1, 1},
	{1.286, 1, 1, 1, 1, 1, 1, 1.221, 1, 1, 1, 1, 1.053, 1.442, 1, 1},
	{1.345, 1, 1, 1, 1, 1.072, 1, 1.198, 1, 1, 1, 1, 1.094, 1.491, 1, 1},
	{1.382, 1, 1, 1, 1, 1.094, 1, 1.17, 1, 1, 1, 1, 1.115, 1.512, 1, 1},
	{1.413, 1, 1, 1, 1, 1.117, 1, 1.14, 1, 1, 1, 1, 1.131, 1.538, 1, 1},				
	{1.446, 1, 1, 1, 1, 1.138, 1, 1.109, 1, 1, 1, 1, 1.155, 1.558, 1, 1},				
	{1.465, 1, 1, 1, 1, 1.15, 1, 1.084, 1, 1, 1, 1, 1.168, 1.572, 1, 1},				
	{1.482, 1, 1, 1, 1, 1.168, 1, 1.072, 1, 1, 1, 1, 1.181, 1.582, 1, 1},				
	{1.473, 1, 1, 1, 1, 1.18, 1, 1.065, 1, 1, 1, 1, 1.179, 1.585, 1, 1},
	{1.488, 1, 1.122, 1, 1, 1.356, 1, 1.164, 1, 1.161, 1, 1, 1.165, 1.625, 1, 1}	},

{       {1, 4.894, 1.15, 2.028, 2.183, 1, 1.836, 1, 1.257, 1.266, 20.75, 5.912, 1, 1, 1.503, 1.398},
	{1, 4.504, 1.086, 1.996, 2.065, 1, 1.778, 1, 1.166, 1.173, 20.55, 5.924, 1, 1, 1.464, 1.426},
	{1, 4.259, 1.042, 1.974, 1.98, 1, 1.728, 1, 1.11, 1.111, 19.67, 6.15, 1, 1, 1.427, 1.446},
	{1, 4.134, 1, 1.975, 1.936, 1, 1.71, 1, 1.08, 1.083, 19.65, 6.333, 1, 1, 1.421, 1.488},
	{1, 4.049, 1, 1.986, 1.896, 1, 1.697, 1, 1.059, 1.057, 19.55, 6.375, 1, 1, 1.41, 1.54},
	{1, 3.947, 1, 2.007, 1.856, 1, 1.683, 1, 1, 1, 19.45, 6.6, 1, 1, 1.4, 1.582},
	{1, 3.893, 1, 2.036, 1.829, 1, 1.689, 1, 1, 1, 19.66, 6.789, 1, 1, 1.403, 1.631},
	{1, 3.751, 1, 2.024, 1.802, 1, 1.686, 1, 1, 1, 19.66, 7.05, 1, 1, 1.397, 1.7},
	{1, 3.612, 1, 2.025, 1.773, 1, 1.674, 1, 1, 1, 18.52, 6.959, 1, 1, 1.373, 1.685},
	{1, 3.511, 1, 1.861, 1.607, 1, 1.477, 1, 1, 1, 16.9, 5.991, 1, 1, 1.29, 1.593}		}
};


double SRPD_weights_327078[2][10][16] = 
{
{       {1.255, 1, 1, 1, 1, 1, 1, 1.257, 1, 1, 1, 1, 1, 1.435, 1, 1},
	{1.339, 1, 1, 1, 1, 1.074, 1, 1.248, 1, 1, 1, 1, 1.084, 1.505, 1, 1},
	{1.395, 1, 1, 1, 1, 1.113, 1, 1.213, 1, 1, 1, 1, 1.121, 1.545, 1, 1},
	{1.44, 1, 1, 1, 1, 1.152, 1, 1.188, 1, 1, 1, 1, 1.152, 1.591, 1, 1},
	{1.471, 1, 1, 1, 1, 1.171, 1, 1.155, 1, 1, 1, 1, 1.172, 1.607, 1, 1},				
	{1.503, 1, 1.054, 1, 1, 1.195, 1, 1.121, 1, 1, 1, 1, 1.192, 1.645, 1, 1},				
	{1.519, 1, 1.07, 1, 1, 1.207, 1, 1.098, 1, 1.044, 1, 1, 1.2, 1.664, 1, 1},				
	{1.525, 1, 1.073, 1, 1, 1.205, 1, 1.076, 1, 1.056, 1, 1, 1.202, 1.678, 1, 1},				
	{1.523, 1, 1.078, 1, 1, 1.203, 1, 1.054, 1, 1.06, 1, 1, 1.202, 1.685, 1, 1},
	{1.615, 1, 1.168, 1, 1, 1.435, 1, 1.132, 1.069, 1.22, 1, 1, 1.162, 1.833, 1, 1}	},

{       {1, 4.776, 1.101, 2.003, 2.098, 1, 1.744, 1, 1.23, 1.209, 19.38, 5.678, 1, 1, 1.427, 1.338},
	{1, 4.332, 1, 1.977, 1.988, 1, 1.687, 1, 1.133, 1.116, 19.22, 5.691, 1, 1, 1.387, 1.366},
	{1, 4.148, 1, 1.994, 1.914, 1, 1.652, 1, 1.082, 1.066, 18.75, 5.899, 1, 1, 1.358, 1.399},
	{1, 3.987, 1, 1.985, 1.86, 1, 1.611, 1, 1.051, 1, 18.33, 5.969, 1, 1, 1.333, 1.43},
	{1, 3.937, 1, 2.012, 1.818, 1, 1.601, 1, 1, 1, 18.21, 6.15, 1, 1, 1.331, 1.472},
	{1, 3.828, 1, 2.032, 1.777, 1, 1.582, 1, 1, 1, 18.1, 6.418, 1, 1, 1.312, 1.515},
	{1, 3.802, 1, 2.046, 1.751, 1, 1.574, 1, 1, 1, 18.08, 6.615, 1, 1, 1.306, 1.555},
	{1, 3.745, 1, 2.068, 1.735, 1, 1.574, 1, 1, 1, 18.11, 6.755, 1, 1, 1.3, 1.592},
	{1, 3.64, 1, 2.073, 1.692, 1, 1.567, 1, 1, 1, 17.42, 6.762, 1, 1, 1.287, 1.611},
	{1, 3.622, 1, 2.041, 1.487, 1, 1.403, 1, 1, 1, 17.83, 6.386, 1, 1, 1.206, 1.553}	}
};


double SRPD_weights_327211[2][10][16] = 
{
{       {1.272, 1, 1, 1, 1, 1, 1, 1.369, 1, 1, 1, 1, 1, 1.414, 1, 1},
	{1.353, 1, 1, 1, 1, 1.06, 1, 1.362, 1, 1, 1, 1, 1.068, 1.479, 1, 1},
	{1.403, 1, 1, 1, 1, 1.099, 1, 1.339, 1, 1, 1, 1, 1.104, 1.517, 1, 1},
	{1.44, 1, 1, 1, 1, 1.126, 1, 1.32, 1, 1, 1, 1, 1.128, 1.547, 1, 1},
	{1.471, 1, 1.039, 1, 1, 1.144, 1, 1.288, 1, 1, 1, 1, 1.15, 1.566, 1, 1},				
	{1.496, 1, 1.059, 1, 1, 1.166, 1, 1.265, 1, 1, 1, 1, 1.165, 1.594, 1, 1},				
	{1.512, 1, 1.07, 1, 1, 1.174, 1, 1.239, 1, 1, 1, 1, 1.175, 1.613, 1, 1},				
	{1.509, 1, 1.073, 1, 1, 1.166, 1, 1.218, 1, 1, 1, 1, 1.181, 1.622, 1, 1},				
	{1.506, 1, 1.079, 1, 1, 1.174, 1, 1.207, 1, 1, 1, 1, 1.179, 1.629, 1, 1},
	{1.536, 1, 1.148, 1, 1, 1.384, 1, 1.291, 1, 1.167, 1, 1, 1.108, 1.675, 1, 1}	},

{       {1, 4.703, 1.079, 1.896, 2.082, 1, 1.673, 1, 1.239, 1.219, 18.12, 5.115, 1, 1, 1.383, 1.21},
	{1, 4.359, 1, 1.878, 1.977, 1, 1.628, 1, 1.143, 1.13, 18.25, 5.129, 1, 1, 1.345, 1.233},
	{1, 4.15, 1, 1.879, 1.907, 1, 1.594, 1, 1.095, 1.084, 17.9, 5.2, 1, 1, 1.322, 1.255},
	{1, 4.026, 1, 1.864, 1.863, 1, 1.561, 1, 1.069, 1.051, 17.66, 5.305, 1, 1, 1.309, 1.275},
	{1, 3.933, 1, 1.891, 1.823, 1, 1.556, 1, 1.048, 1, 17.7, 5.423, 1, 1, 1.306, 1.305},
	{1, 3.885, 1, 1.895, 1.79, 1, 1.538, 1, 1.035, 1, 17.61, 5.579, 1, 1, 1.291, 1.332},
	{1, 3.826, 1, 1.92, 1.761, 1, 1.531, 1, 1, 1, 17.48, 5.737, 1, 1, 1.284, 1.362},
	{1, 3.776, 1, 1.933, 1.736, 1, 1.528, 1, 1, 1, 17.38, 5.821, 1, 1, 1.281, 1.381},
	{1, 3.642, 1, 1.934, 1.71, 1, 1.519, 1, 1, 1, 16.58, 5.794, 1, 1, 1.276, 1.385},
	{1, 3.529, 1, 1.913, 1.524, 1, 1.364, 1, 1, 1, 16.81, 5.422, 1, 1, 1.24, 1.399}		}
};


double SRPD_weights_327327[2][10][16] = 
{
{       {1.29, 1, 1, 1, 1, 1, 1, 1.404, 1, 1, 1, 1, 1, 1.453, 1, 1},
	{1.372, 1, 1, 1, 1, 1.081, 1, 1.4, 1, 1, 1, 1, 1.078, 1.518, 1, 1},
	{1.424, 1, 1, 1, 1, 1.119, 1, 1.384, 1, 1, 1, 1, 1.11, 1.554, 1, 1},
	{1.459, 1, 1, 1, 1, 1.148, 1, 1.355, 1, 1, 1, 1, 1.137, 1.585, 1, 1},
	{1.49, 1, 1.047, 1, 1, 1.166, 1, 1.335, 1, 1, 1, 1, 1.159, 1.606, 1, 1},				
	{1.514, 1, 1.052, 1, 1, 1.177, 1, 1.307, 1, 1, 1, 1, 1.172, 1.623, 1, 1},				
	{1.522, 1, 1.058, 1, 1, 1.184, 1, 1.285, 1, 1, 1, 1, 1.181, 1.638, 1, 1},				
	{1.527, 1, 1.071, 1, 1, 1.186, 1, 1.268, 1, 1.043, 1, 1, 1.183, 1.659, 1, 1},				
	{1.533, 1, 1.062, 1, 1, 1.187, 1, 1.243, 1, 1.048, 1, 1, 1.188, 1.666, 1, 1},
	{1.534, 1, 1.071, 1, 1, 1.208, 1, 1.229, 1, 1.051, 1, 1, 1.177, 1.669, 1, 1}	},

{       {1, 4.588, 1.074, 1.878, 2.058, 1, 1.63, 1, 1.224, 1.188, 17.47, 4.889, 1, 1, 1.33, 1.148},
	{1, 4.273, 1, 1.865, 1.943, 1, 1.577, 1, 1.129, 1.106, 17.38, 4.901, 1, 1, 1.295, 1.17},
	{1, 4.09, 1, 1.86, 1.882, 1, 1.54, 1, 1.087, 1.061, 16.99, 4.947, 1, 1, 1.271, 1.186},
	{1, 3.958, 1, 1.873, 1.833, 1, 1.517, 1, 1.058, 1, 16.74, 5.056, 1, 1, 1.255, 1.208},
	{1, 3.902, 1, 1.876, 1.796, 1, 1.49, 1, 1, 1, 16.57, 5.128, 1, 1, 1.239, 1.226},
	{1, 3.843, 1, 1.905, 1.769, 1, 1.485, 1, 1, 1, 16.66, 5.282, 1, 1, 1.236, 1.255},
	{1, 3.836, 1, 1.924, 1.743, 1, 1.478, 1, 1, 1, 16.67, 5.412, 1, 1, 1.226, 1.28},
	{1, 3.76, 1, 1.937, 1.715, 1, 1.465, 1, 1, 1, 16.39, 5.481, 1, 1, 1.218, 1.291},
	{1, 3.609, 1, 1.95, 1.685, 1, 1.475, 1, 1, 1, 15.71, 5.453, 1, 1, 1.212, 1.296},
	{1, 3.469, 1, 1.935, 1.629, 1, 1.459, 1, 1, 1, 15.26, 5.349, 1, 1, 1.201, 1.303}	}
};


double SRPD_weights_327464[2][10][16] = 
{
{       {1.314, 1, 1, 1, 1, 1, 1, 1.52, 1, 1, 1, 1, 1, 1.451, 1, 1},
	{1.382, 1, 1, 1, 1, 1.075, 1, 1.515, 1, 1, 1, 1, 1.079, 1.511, 1, 1},
	{1.438, 1, 1.062, 1, 1, 1.117, 1, 1.501, 1, 1, 1, 1, 1.114, 1.559, 1, 1},
	{1.474, 1, 1.082, 1, 1, 1.142, 1, 1.482, 1, 1, 1, 1, 1.134, 1.582, 1, 1},
	{1.505, 1, 1.102, 1, 1, 1.167, 1, 1.461, 1, 1, 1, 1, 1.156, 1.607, 1, 1},				
	{1.524, 1, 1.11, 1, 1, 1.177, 1, 1.435, 1, 1, 1, 1, 1.17, 1.625, 1, 1},				
	{1.538, 1, 1.129, 1, 1, 1.189, 1, 1.41, 1, 1, 1, 1, 1.182, 1.643, 1, 1},				
	{1.544, 1, 1.133, 1, 1, 1.19, 1, 1.396, 1, 1, 1, 1, 1.186, 1.657, 1, 1},				
	{1.56, 1, 1.146, 1, 1, 1.202, 1, 1.392, 1, 1.051, 1, 1, 1.198, 1.674, 1, 1},
	{1.561, 1, 1.16, 1, 1, 1.219, 1, 1.395, 1.038, 1.06, 1, 1, 1.197, 1.684, 1, 1}	},

{       {1, 4.542, 1, 1.789, 2.038, 1, 1.551, 1, 1.224, 1.198, 16.21, 4.586, 1, 1, 1.261, 1.075},
	{1, 4.253, 1, 1.775, 1.931, 1, 1.49, 1, 1.128, 1.116, 16.25, 4.562, 1, 1, 1.225, 1.093},
	{1, 4.039, 1, 1.761, 1.869, 1, 1.452, 1, 1.084, 1.064, 15.9, 4.594, 1, 1, 1.201, 1.102},
	{1, 3.928, 1, 1.757, 1.832, 1, 1.435, 1, 1.061, 1.035, 15.95, 4.66, 1, 1, 1.188, 1.117},
	{1, 3.849, 1, 1.758, 1.79, 1, 1.414, 1, 1, 1, 15.66, 4.736, 1, 1, 1.178, 1.135},
	{1, 3.816, 1, 1.78, 1.76, 1, 1.405, 1, 1, 1, 15.69, 4.855, 1, 1, 1.17, 1.158},
	{1, 3.757, 1, 1.796, 1.728, 1, 1.384, 1, 1, 1, 15.54, 4.977, 1, 1, 1.155, 1.175},
	{1, 3.668, 1, 1.805, 1.699, 1, 1.381, 1, 1, 1, 15.29, 5.03, 1, 1, 1.15, 1.185},
	{1, 3.477, 1, 1.795, 1.66, 1, 1.37, 1, 1, 1, 14.58, 4.969, 1, 1, 1.141, 1.186},
	{1, 3.369, 1, 1.789, 1.602, 1, 1.354, 1, 1, 1, 14.09, 4.862, 1, 1, 1.134, 1.192}	}
};


double SRPD_weights_327524[2][10][16] = 
{
{       {1.325, 1, 1, 1, 1, 1, 1, 1.573, 1, 1, 1, 1, 1, 1.47, 1, 1},
	{1.407, 1, 1, 1, 1, 1.089, 1, 1.576, 1, 1, 1, 1, 1.081, 1.533, 1, 1},
	{1.456, 1, 1.077, 1, 1, 1.133, 1, 1.564, 1, 1, 1, 1, 1.117, 1.574, 1, 1},
	{1.49, 1, 1.101, 1, 1, 1.157, 1, 1.55, 1, 1, 1, 1, 1.139, 1.602, 1, 1},
	{1.517, 1, 1.122, 1, 1, 1.174, 1, 1.533, 1, 1, 1, 1, 1.163, 1.629, 1, 1},				
	{1.543, 1, 1.136, 1, 1, 1.192, 1, 1.512, 1, 1, 1, 1, 1.174, 1.645, 1, 1},				
	{1.546, 1, 1.144, 1, 1, 1.191, 1, 1.488, 1, 1, 1, 1, 1.176, 1.651, 1, 1},				
	{1.569, 1, 1.155, 1, 1, 1.202, 1, 1.47, 1, 1.044, 1, 1, 1.19, 1.677, 1, 1},				
	{1.564, 1, 1.159, 1, 1, 1.206, 1, 1.463, 1, 1.054, 1, 1, 1.194, 1.678, 1, 1},
	{1.565, 1, 1.184, 1, 1, 1.238, 1, 1.492, 1.041, 1.076, 1, 1, 1.198, 1.692, 1, 1}   },

{       {1, 4.476, 1, 1.747, 2.017, 1, 1.504, 1, 1.212, 1.185, 15.6, 4.382, 1, 1, 1.226, 1},
	{1, 4.166, 1, 1.718, 1.908, 1, 1.45, 1, 1.124, 1.102, 15.62, 4.374, 1, 1, 1.191, 1.047},
	{1, 3.97, 1, 1.71, 1.846, 1, 1.419, 1, 1.078, 1.055, 15.25, 4.397, 1, 1, 1.171, 1.055},
	{1, 3.866, 1, 1.708, 1.809, 1, 1.395, 1, 1.051, 1, 15.19, 4.439, 1, 1, 1.157, 1.069},
	{1, 3.811, 1, 1.704, 1.764, 1, 1.377, 1, 1, 1, 15.1, 4.501, 1, 1, 1.142, 1.08},
	{1, 3.745, 1, 1.721, 1.738, 1, 1.365, 1, 1, 1, 14.99, 4.582, 1, 1, 1.135, 1.095},
	{1, 3.725, 1, 1.741, 1.729, 1, 1.36, 1, 1, 1, 15.05, 4.7, 1, 1, 1.132, 1.114},
	{1, 3.608, 1, 1.75, 1.686, 1, 1.353, 1, 1, 1, 14.7, 4.757, 1, 1, 1.12, 1.123},
	{1, 3.475, 1, 1.744, 1.653, 1, 1.349, 1, 1, 1, 14.09, 4.68, 1, 1, 1.114, 1.124},
	{1, 3.326, 1, 1.719, 1.6, 1, 1.317, 1, 1, 1, 13.51, 4.55, 1, 1, 1.105, 1.117}		}
};


double SRPD_weights_327560[2][10][16] = 
{
{       {1.337, 1, 1, 1, 1, 1, 1, 1.596, 1, 1, 1, 1, 1, 1.486, 1, 1},
	{1.412, 1, 1.051, 1, 1, 1.095, 1, 1.612, 1, 1, 1, 1, 1.085, 1.544, 1, 1},
	{1.459, 1, 1.09, 1, 1, 1.137, 1, 1.6, 1, 1, 1, 1, 1.119, 1.586, 1, 1},
	{1.5, 1, 1.114, 1, 1, 1.167, 1, 1.578, 1, 1, 1, 1, 1.143, 1.617, 1, 1},
	{1.522, 1, 1.131, 1, 1, 1.187, 1, 1.561, 1, 1, 1, 1, 1.16, 1.645, 1, 1},				
	{1.549, 1, 1.152, 1, 1, 1.199, 1, 1.543, 1, 1, 1, 1, 1.177, 1.661, 1, 1},				
	{1.567, 1, 1.163, 1, 1, 1.206, 1, 1.522, 1, 1.047, 1, 1, 1.194, 1.684, 1, 1},				
	{1.558, 1, 1.172, 1, 1, 1.213, 1, 1.499, 1, 1.059, 1, 1, 1.192, 1.7, 1, 1},				
	{1.569, 1, 1.179, 1, 1, 1.212, 1, 1.485, 1, 1.058, 1, 1, 1.193, 1.697, 1, 1},
	{1.535, 1, 1.179, 1, 1, 1.228, 1, 1.5, 1, 1.062, 1, 1, 1.183, 1.69, 1, 1}    },

{       {1, 4.449, 1, 1.729, 2.006, 1, 1.481, 1, 1.207, 1.172, 15.2, 4.318, 1, 1, 1.207, 1},
	{1, 4.141, 1, 1.69, 1.902, 1, 1.426, 1, 1.122, 1.094, 15.3, 4.275, 1, 1, 1.171, 1},
	{1, 3.963, 1, 1.681, 1.842, 1, 1.39, 1, 1.073, 1.048, 14.97, 4.277, 1, 1, 1.146, 1},
	{1, 3.847, 1, 1.683, 1.795, 1, 1.368, 1, 1.047, 1, 14.81, 4.345, 1, 1, 1.131, 1.041},
	{1, 3.761, 1, 1.69, 1.761, 1, 1.352, 1, 1, 1, 14.72, 4.402, 1, 1, 1.119, 1.056},
	{1, 3.727, 1, 1.696, 1.727, 1, 1.338, 1, 1, 1, 14.66, 4.49, 1, 1, 1.11, 1.066},
	{1, 3.694, 1, 1.713, 1.699, 1, 1.321, 1, 1, 1, 14.5, 4.594, 1, 1, 1.097, 1.079},
	{1, 3.585, 1, 1.732, 1.685, 1, 1.318, 1, 1, 1, 14.18, 4.626, 1, 1, 1.09, 1.088},
	{1, 3.43, 1, 1.732, 1.644, 1, 1.312, 1, 1, 1, 13.6, 4.61, 1, 1, 1.081, 1.092},
	{1, 3.354, 1, 1.689, 1.616, 1, 1.308, 1, 1, 1, 13.33, 4.469, 1, 1, 1.094, 1.091}	}
};

double SRPD_weights_Default[2][10][16] = {0};

void Returns_SRPD_Calibrations(int RunNumber, double (&ReturnCalib)[2][10][16]){
	switch (RunNumber) {
		case 326776 :
			memcpy(ReturnCalib, SRPD_weights_326776, sizeof(ReturnCalib));
			break;
		case 326822 :
			memcpy(ReturnCalib, SRPD_weights_326822, sizeof(ReturnCalib));
			break;
		case 326883 :
			memcpy(ReturnCalib, SRPD_weights_326883, sizeof(ReturnCalib));
			break;
		case 326943 :
			memcpy(ReturnCalib, SRPD_weights_326943, sizeof(ReturnCalib));
			break;
		case 327078 :
			memcpy(ReturnCalib, SRPD_weights_327078, sizeof(ReturnCalib));
			break;
		case 327211 :
			memcpy(ReturnCalib, SRPD_weights_327211, sizeof(ReturnCalib));
			break;
		case 327327 :
			memcpy(ReturnCalib, SRPD_weights_327327, sizeof(ReturnCalib));
			break;
		case 327464 :
			memcpy(ReturnCalib, SRPD_weights_327464, sizeof(ReturnCalib));
			break;
		case 327524 :
			memcpy(ReturnCalib, SRPD_weights_327524, sizeof(ReturnCalib));
			break;
		case 327560 :
			memcpy(ReturnCalib, SRPD_weights_327560, sizeof(ReturnCalib));
			break;
		default:
			memcpy(ReturnCalib, SRPD_weights_Default, sizeof(ReturnCalib));

	}

}
#endif


#ifndef Modern_Data_Organizer
#define Modern_Data_Organizer

using namespace std;

void Modern_RPD_Data_Organizer_and_Cleaner(double RawDataRPD[2][16][10], double RPD_Cuts[2][16], double (&OC_RPD_Data)[2][16], int RunNumber){

	//👁 constants for operation of softrware

	int LookUpRPD_Pos[16] = {11, 15, 2, 5,
						  	  8, 12, 1, 6,
						      9, 13, 0, 4,
						     10, 14, 3, 7};

	//ch 5 and 6 switched due to inverted cable
	//NEG SIDE COLUMNS ARE INVERETED THIS IS CORRECTED FOR BY THESE ARRAYS

	int LookUpRPD_Neg[16] = { 6, 2, 15, 11,
							  5, 1, 12, 8,
							  4, 0, 13, 9,
							  7, 3, 14, 10};


	double O_DataRPD[2][16][10] = {0}; // array for organized RPD data



	for (int c = 0; c < 16; c++){
		//do check on all valuies if pas fill if any value bad set all values to -3000 and return
		for (int ts = 0; ts < 10; ts++){	
			O_DataRPD[0][c][ts] = RawDataRPD[0][LookUpRPD_Neg[c]][ts];
			O_DataRPD[1][c][ts] = RawDataRPD[1][LookUpRPD_Pos[c]][ts];
		}

	}


	for (int c = 0; c < 16; c++){
		if (((O_DataRPD[0][c][4] + O_DataRPD[0][c][5])/O_DataRPD[0][c][6]) > RPD_Cuts[0][c]){ //checks to se if TS distribution is good if yes it fills the output rpd array with the sum of those TS
			OC_RPD_Data[0][c] = (O_DataRPD[0][c][4] + O_DataRPD[0][c][5] + O_DataRPD[0][c][6]);
		}
		else{
			//for (int b = 0; b < 16; b ++){
				OC_RPD_Data[0][c] = 0;
			//}
			//break; //if a bad channel is detected in the rpd the rpd is tossed
		}
	}

	if (RunNumber == 326776 or RunNumber == 326822 or RunNumber == 326883 or RunNumber == 326943){
		for (int c = 0; c < 16; c++){
			if (((O_DataRPD[1][c][4] + O_DataRPD[1][c][5])/O_DataRPD[1][c][6]) > RPD_Cuts[1][c]){ //used to be 345
				OC_RPD_Data[1][c] = (O_DataRPD[1][c][4] + O_DataRPD[1][c][5] + O_DataRPD[1][c][6]);
			}
			else{
				//for (int b = 0; b < 16; b ++){
					OC_RPD_Data[1][c] = 0;
				//}
				//break; // if a bad channel is detected entire rpd is tossed
			}
		}
	}
	if (RunNumber == 327078 or RunNumber == 327211 or RunNumber == 327327 or RunNumber == 327464 or RunNumber == 327524 or RunNumber == 327560){
		for (int c = 0; c < 16; c++){
			if (((O_DataRPD[1][c][4] + O_DataRPD[1][c][5])/O_DataRPD[1][c][6]) > RPD_Cuts[1][c]){
				OC_RPD_Data[1][c] = (O_DataRPD[1][c][4] + O_DataRPD[1][c][5] + O_DataRPD[1][c][6]);
			}
			else{
				//for (int b = 0; b < 16; b ++){
					OC_RPD_Data[1][c] = 0;
				//}
				//break; // if a bad channel is detected entire rpd is tossed
			}
		}
	}

	return;
} //end function

#endif

/* RPD Beam Position Code
This header file is used to insert a function which will measure a weighted average
of the beam position using the 16 channels of the RPD. The channels are aggregated by groups of 4 into rows and columns this is how the weighted average is calcualted to return a position.

These are defined in the code(s) designed to use this header file.

Adjust this as necessary

Eric Adams
*/
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstring> //Liz added
#include <string> // Eric Added

#include <stdexcept>

// I am a chemist not an englishist I cant spell

#ifndef X_Y_P_N_RPD_Mean_Position_Calculator
#define X_Y_P_N_RPD_Mean_Position_Calculator

void Returns_X_Y_P_N_RPD_Mean_Position(double OCCS_RPD_Data[2][16],  const std::string& OnorOff, double EricWeighter[16], double &PosX, double &PosY, double &NegX, double &NegY){
//EM_BEAM_POSITION[ NEG, POS]


	if (OnorOff != "On" && OnorOff != "Off") {
			throw std::runtime_error("ERROR: OnorOFF must be set to On or Off, input variable 3, X_Y_P_N_RPD_Beam_Position_Calculator.h");
	}

	if (OnorOff != "On"){
		for (int i = 0; i < 16; i++){
			EricWeighter[i] = 1;
		}
	}
	
	const int NRPD = 16;
	const int NRPDColnRow = 4;
	double AxisRPD[NRPDColnRow] = { -3.0, -1.0, 1.0, 3.0 };
	double RPD_COL_P[NRPD] = {0};
	double RPD_ROW_P[NRPD] = {0};
	double RPD_COL_N[NRPD] = {0};
	double RPD_ROW_N[NRPD] = {0};

	double sumWeightRPD_P_X = 0;
	double sumRPD_P_X = 0;
	double sumWeightRPD_P_Y = 0;
	double sumRPD_P_Y = 0;
	double sumWeightRPD_N_X = 0;
	double sumRPD_N_X = 0;
	double sumWeightRPD_N_Y = 0;
	double sumRPD_N_Y = 0;

	
	for (int s = 0; s < 2; s++){ //s=side
		if (s == 0) { //Neg columns and rows
			for (int c = 0; c < 16; c++){
				//Y
				if (c == 0 || c == 1 || c == 2 || c == 3) { RPD_ROW_N[0]   += OCCS_RPD_Data[s][c];}
				if (c == 4  || c == 5 || c == 6 || c == 7) { RPD_ROW_N[1]   += OCCS_RPD_Data[s][c];}
				if (c == 8  || c == 9 || c == 10 || c == 11) { RPD_ROW_N[2]   += OCCS_RPD_Data[s][c];}
				if (c == 12 || c == 13 || c == 14 || c == 15) { RPD_ROW_N[3]   += OCCS_RPD_Data[s][c];}											//R0
					///////////////////////////////////////////////// IS THE ORDERING OF THE OUTPUT JEFF ARRAYS MATH DATA??																									 										//R1
																																			//R1
				// X
				if (c == 0 || c == 4  || c == 8  || c == 12) { RPD_COL_N[0] += OCCS_RPD_Data[s][c] * EricWeighter[c];}			//R2             
				if (c == 1 || c == 5 || c == 9 || c == 13) { RPD_COL_N[1] += OCCS_RPD_Data[s][c] * EricWeighter[c];}			//R3							
				if (c == 2  || c == 6  || c == 10  || c == 14)  { RPD_COL_N[2] += OCCS_RPD_Data[s][c] * EricWeighter[c];}			  //C3 C2 C1 C0					
				if (c == 3  || c == 7  || c == 11  || c == 15)  { RPD_COL_N[3] += OCCS_RPD_Data[s][c] * EricWeighter[c];}
			}
		}                                                                                                                                 
		else if (s == 1) { //Pos columns and rows
			for (int c = 0; c < 16; c++){
		
				// Y
				if (c == 0 || c == 1 || c == 2 || c == 3) { RPD_ROW_P[0]   += OCCS_RPD_Data[s][c];}
				if (c == 4  || c == 5 || c == 6 || c == 7) { RPD_ROW_P[1]   += OCCS_RPD_Data[s][c];}
				if (c == 8  || c == 9 || c == 10 || c == 11) { RPD_ROW_P[2]   += OCCS_RPD_Data[s][c];}
				if (c == 12 || c == 13 || c == 14 || c == 15) { RPD_ROW_P[3]   += OCCS_RPD_Data[s][c];}											//R0
					///////////////////////////////////////////////// IS THE ORDERING OF THE OUTPUT JEFF ARRAYS MATH DATA??																									 										//R1
																																			//R1
				// X
				if (c == 0 || c == 4  || c == 8  || c == 12) { RPD_COL_P[0] += OCCS_RPD_Data[s][c] * EricWeighter[c];}			//R2             
				if (c == 1 || c == 5 || c == 9 || c == 13) { RPD_COL_P[1] += OCCS_RPD_Data[s][c] * EricWeighter[c];}			//R3							
				if (c == 2  || c == 6  || c == 10  || c == 14)  { RPD_COL_P[2] += OCCS_RPD_Data[s][c] * EricWeighter[c];}			  //C3 C2 C1 C0					
				if (c == 3  || c == 7  || c == 11  || c == 15)  { RPD_COL_P[3] += OCCS_RPD_Data[s][c] * EricWeighter[c];}
				
			}
		}
	}
				//getting mean values that are the ouputs
	
	if ( RPD_COL_P[0] == 0.0 || RPD_COL_P[1] == 0.0 || RPD_COL_P[2] == 0.0 || RPD_COL_P[3] == 0.0){
		PosX = -343;
	}
	else{
	
		sumWeightRPD_P_X = (RPD_COL_P[0] * AxisRPD[0]) + (RPD_COL_P[1] * AxisRPD[1]) + (RPD_COL_P[2] * AxisRPD[2]) + (RPD_COL_P[3] * AxisRPD[3]);
		sumRPD_P_X = (RPD_COL_P[0]) + (RPD_COL_P[1]) + (RPD_COL_P[2]) + (RPD_COL_P[3]);

		PosX = (sumWeightRPD_P_X / sumRPD_P_X);
	}

	if (RPD_ROW_P[0] == 0.0 || RPD_ROW_P[1] == 0.0 || RPD_ROW_P[2] == 0.0 || RPD_ROW_P[3] == 0.0){ //values of zero screw with the weighting sytem, ideally it will never be 0.0 unless there is an issue
		PosY = -343;
	}
	else{

		sumWeightRPD_P_Y = (RPD_ROW_P[0] * AxisRPD[3]) + (RPD_ROW_P[1] * AxisRPD[2]) + (RPD_ROW_P[2] * AxisRPD[1]) + (RPD_ROW_P[3] * AxisRPD[0]); //weird here
		sumRPD_P_Y = (RPD_ROW_P[0]) + (RPD_ROW_P[1]) + (RPD_ROW_P[2]) + (RPD_ROW_P[3]);
		
		PosY = (sumWeightRPD_P_Y / sumRPD_P_Y);
	}
	
/////////////
	if ( RPD_COL_N[0] == 0.0 || RPD_COL_N[1] == 0.0 || RPD_COL_N[2] == 0.0 || RPD_COL_N[3] == 0.0){
		NegX = -343;
	}
	else{
		sumWeightRPD_N_X = (RPD_COL_N[0] * AxisRPD[0]) + (RPD_COL_N[1] * AxisRPD[1]) + (RPD_COL_N[2] * AxisRPD[2]) + (RPD_COL_N[3] * AxisRPD[3]);
		sumRPD_N_X = (RPD_COL_N[0]) + (RPD_COL_N[1]) + (RPD_COL_N[2]) + (RPD_COL_N[3]);

		NegX = (sumWeightRPD_N_X / sumRPD_N_X); 

	}

	if(RPD_ROW_N[0] == 0.0 || RPD_ROW_N[1] == 0.0 || RPD_ROW_N[2] == 0.0 || RPD_ROW_N[3] == 0.0){ // values of zero screw with the weighting system, ideally it will never be 0.0 unless there is an issue
		NegY = -343;
	}
	else{
	
		sumWeightRPD_N_Y = (RPD_ROW_N[0] * AxisRPD[3]) + (RPD_ROW_N[1] * AxisRPD[2]) + (RPD_ROW_N[2] * AxisRPD[1]) + (RPD_ROW_N[3] * AxisRPD[0]);
		sumRPD_N_Y = (RPD_ROW_N[0]) + (RPD_ROW_N[1]) + (RPD_ROW_N[2]) + (RPD_ROW_N[3]);
		
		NegY = (sumWeightRPD_N_Y / sumRPD_N_Y);  
	
	}
}

#endif

#include <cmath>

#ifndef Raw_Reaction_Plane
#define Raw_Reaction_Plane



void Raw_RXN_Plane_Calculator(int Order, std::string CMSorALICE, double OCCS_RPD_Data[2][16], double &Raw_RXN_Plane_NEG, double &Raw_RXN_Plane_POS, double (&t_X_q_Vector)[2], double (&t_Y_q_Vector)[2]){


	//note input must have fiber effects removed be calibrated or centered at zero organized from top left reading left to right and ending in bottom right

	//NOTE CHANGED THE POLAR COORDINATES TO MATCH UP WITH THE APPROPRIATE NEW 0 POINT SINCE REMOVING A COLUMN
	double RPD_Block_In_Phi[16] ={0, 1.89255, 1.5707, 1.24905, 0, 2.35619, 1.5707, 0.785398, 0, -2.35619, -1.5707, -0.785398, 0, -1.89255, -1.5707, -1.24905};//delete{0, 2.35619, 1.89255, 1.24905, 0, 2.81984, 2.35619, 0.785398, 0, -2.81984, -2.35619, -0.785398, 0, -2.35619, -1.89255, -1.24905};// original{2.35619, 1.89255, 1.24905, 0.785398, 2.81984, 2.35619, 0.785398, 0.321751, -2.81984, -2.35619, -0.785398, -0.321751, -2.35619, -1.89255, -1.24905, -0.785398};
	double fourxfourRPD_In_Phi[4] = {2.35619, 0.785398, -2.35619, -0.785398};
	double B1[4] = {0, 2, 8, 10};
	double B2[4] = {1, 3, 9, 11};
	double B3[4] = {4, 6, 12, 14};
	double B4[4] = {5, 7, 13, 15};
	/*  positions in cm from 0,0 being the center of the rpd -x is left -y is down*/
	/*int X_Array[16] = {-3, -1, 1, 3, -3, -1, 1, 3, -3, -1, 1, 3, -3, -1, 1, 3};
	int Y_Array[16] = {3, 3, 3, 3, 1, 1, 1, 1, -1, -1, -1, -1, -3, -3, -3, -3};
*/
	t_X_q_Vector[0] = {0};
	t_Y_q_Vector[0] = {0};
	t_X_q_Vector[1] = {0};
	t_Y_q_Vector[1] = {0};

	if (CMSorALICE == "CMS"){
		for (int s = 0; s < 2; s++){
			if (OCCS_RPD_Data[s][0] == -343){
				t_X_q_Vector[s] = -343;
				t_Y_q_Vector[s] = -343;
				if (s == 0){
					Raw_RXN_Plane_NEG = -343;
				}
				else{
					Raw_RXN_Plane_POS = -343;
				}
				continue;
			}
			for (int c = 0; c < 16; c++){
				if (c == 0 or c == 4 or c == 8 or c == 12){ /////////////////////////////////////////////////////////////////////////////////////////////////////comment this out this is to test alices idea of removing the far left column
					continue;
				}
				t_X_q_Vector[s] += (OCCS_RPD_Data[s][c] * cos(Order*RPD_Block_In_Phi[c]));
				t_Y_q_Vector[s] += (OCCS_RPD_Data[s][c] * sin(Order*RPD_Block_In_Phi[c]));
			}
	
			if (s == 0){
				Raw_RXN_Plane_NEG = ((1/Order) * atan2(t_Y_q_Vector[0],t_X_q_Vector[0]));
			}
			else{
				Raw_RXN_Plane_POS = ((1/Order) * atan2(t_Y_q_Vector[1],t_X_q_Vector[1]));
			}
		}
	}
	if (CMSorALICE == "ALICE"){
		for (int s = 0; s < 2; s++){
			if (OCCS_RPD_Data[s][0] == -343){
				t_X_q_Vector[s] = -343;
				t_Y_q_Vector[s] = -343;
				if (s == 0){
					Raw_RXN_Plane_NEG = -343;
				}
				else{
					Raw_RXN_Plane_POS = -343;
				}
				continue;
			}
			for (int c = 0; c < 4; c++){
				int aa = B1[c];
				int bb = B2[c];
				int cc = B3[c];
				int dd = B4[c];
				t_X_q_Vector[s] += ((OCCS_RPD_Data[s][aa] + OCCS_RPD_Data[s][bb] + OCCS_RPD_Data[s][cc] + OCCS_RPD_Data[s][dd]) * cos(Order*fourxfourRPD_In_Phi[c]));
				t_Y_q_Vector[s] += ((OCCS_RPD_Data[s][aa] + OCCS_RPD_Data[s][bb] + OCCS_RPD_Data[s][cc] + OCCS_RPD_Data[s][dd]) * sin(Order*fourxfourRPD_In_Phi[c]));
			}
	
			if (s == 0){
				Raw_RXN_Plane_NEG = ((1/Order) * atan2(t_Y_q_Vector[0],t_X_q_Vector[0]));
			}
			else{
				Raw_RXN_Plane_POS = ((1/Order) * atan2(t_Y_q_Vector[1],t_X_q_Vector[1]));
			}
		}
	}
}




#endif

#ifndef CentralityFunc
#define CentralityFunc

//////////////////////funtion that converts numbers to centrality bins ///////////////
const int nBins = 200; // table of bin edges
	const double binTable[nBins+1] = {0, 10.5072, 11.2099, 11.8364, 12.478, 13.1194, 13.7623, 14.4081, 15.0709, 15.7532, 16.4673, 17.1881, 17.923, 18.673, 19.4865, 20.3033, 21.1536, 22.0086, 22.9046, 23.8196, 24.7924, 25.8082, 26.8714, 27.9481, 29.0828, 30.2757, 31.5043, 32.8044, 34.1572, 35.6142, 37.1211, 38.6798, 40.3116, 42.0398, 43.8572, 45.6977, 47.6312, 49.6899, 51.815, 54.028, 56.3037, 58.7091, 61.2024, 63.8353, 66.5926, 69.3617, 72.2068, 75.2459, 78.3873, 81.5916, 84.9419, 88.498, 92.1789, 95.9582, 99.8431, 103.739, 107.78, 111.97, 116.312, 120.806, 125.46, 130.269, 135.247, 140.389, 145.713, 151.212, 156.871, 162.729, 168.762, 174.998, 181.424, 188.063, 194.907, 201.942, 209.19, 216.683, 224.37, 232.291, 240.43, 248.807, 257.416, 266.256, 275.348, 284.668, 294.216, 304.053, 314.142, 324.488, 335.101, 345.974, 357.116, 368.547, 380.283, 392.29, 404.564, 417.122, 429.968, 443.116, 456.577, 470.357, 484.422, 498.78, 513.473, 528.479, 543.813, 559.445, 575.411, 591.724, 608.352, 625.344, 642.686, 660.361, 678.371, 696.749, 715.485, 734.608, 754.068, 773.846, 794.046, 814.649, 835.608, 856.972, 878.719, 900.887, 923.409, 946.374, 969.674, 993.435, 1017.62, 1042.21, 1067.28, 1092.72, 1118.64, 1144.96, 1171.71, 1198.98, 1226.67, 1254.82, 1283.46, 1312.65, 1342.21, 1372.27, 1402.85, 1433.93, 1465.49, 1497.62, 1530.29, 1563.49, 1597.22, 1631.49, 1666.37, 1701.8, 1737.75, 1774.35, 1811.51, 1849.29, 1887.75, 1926.79, 1966.6, 2006.97, 2047.99, 2089.71, 2132.1, 2175.23, 2219.17, 2263.72, 2309.2, 2355.43, 2402.47, 2450.33, 2499.05, 2548.66, 2599.16, 2650.59, 2703.03, 2756.32, 2810.75, 2866.27, 2922.91, 2980.54, 3039.47, 3099.53, 3160.98, 3223.66, 3287.71, 3353.18, 3420.34, 3489.13, 3559.72, 3632.06, 3706.18, 3782.42, 3860.78, 3941.42, 4024.52, 4110.27, 4199.4, 4292.8, 4394.49, 4519.52, 5199.95};
		
	int getHiBinFromhiHF(double hiHF)
	{
		  int binPos = -1;
		  for(int i = 0; i < nBins; ++i){
		    if(hiHF >= binTable[i] && hiHF < binTable[i+1]){
		      binPos = i;
		      break;
		    }
		  }
		
		  binPos = nBins - 1 - binPos;
		
		  return (int)(200*((double)binPos)/((double)nBins)); 
	}

///////////////////////////////////////////////////////////////////////////////////
#endif

#ifndef CentralityChoose
#define CentralityChoose

double Centrality_Array_Chooser( double centBinvalue){

	double centIter = -1;

	if (centBinvalue < 20){ //0-10%
		centIter = 0;
	}
	else if (centBinvalue < 40){ //10-20%
		centIter = 1;
	}
	else if (centBinvalue < 60){ //20-30%
		centIter = 2;
	}
	else if (centBinvalue < 80){ //30-40%
		centIter = 3;
	}
	else if (centBinvalue < 100){ //40-50%
		centIter = 4;
	}
	else if (centBinvalue < 120){ //50-60%
		centIter = 5;
	}
	else if (centBinvalue < 140){ //60-70%
		centIter = 6;
	}
	else if (centBinvalue < 160){ //70-80%
		centIter = 7;
	}
	else if (centBinvalue < 180){ //80-90%
		centIter = 8;
	}
	else if (centBinvalue <= 200){ //90-100%
		centIter = 9;
	}

	return centIter;
}
#endif

#ifndef RXNPLANE_Constants
#define RXNPLANE_Constants



#endif


//ALICE SPECIFIC

void ALICE_Q_Vector( double OCCS_RPD_Data[2][16], double &ALICE_Q_X_Pos, double &ALICE_Q_Y_Pos, double &ALICE_Q_X_Neg, double &ALICE_Q_Y_Neg){
/*

(-3,3)  (-1,3)  (1,3)  (3,3)
(-3,1)  (-1,1)  (1,1)  (3,1)
(-3,-1) (-1,-1) (1,-1) (3,-1)
(-3,-3) (-1,-3) (1,-3) (3,-3)

*/
int X_Array[16] = {0, -1, 0, 1, 0, -1, 0, 1, 0, -1, 0, 1, 0, -1, 0, 1};//{-3, -1, 1, 3, -3, -1, 1, 3, -3, -1, 1, 3, -3, -1, 1, 3};
int Y_Array[16] = {0, 3, 3, 3, 0, 1, 1, 1, 0, -1, -1, -1, 0, -3, -3 -3};//{3, 3, 3, 3, 1, 1, 1, 1, -1, -1, -1, -1, -3, -3, -3, -3};
double Q_X[2] = {0};
double Q_Y[2] = {0};
double nE_Sum_X = 0;
double E_Sum = 0;
double nE_Sum_Y = 0;

	for (int s = 0; s <2; s++){
		for (int c = 0; c < 16; c++){
			if (c == 0 or c == 4 or c == 8 or c == 12){ /////////////////////////////////////////////////////////////////////////////////////////////////////comment this out this is to test alices idea of removing the far left column
					continue;
			}
			nE_Sum_X += X_Array[c]*OCCS_RPD_Data[s][c];
			nE_Sum_Y += Y_Array[c]*OCCS_RPD_Data[s][c];
			E_Sum += OCCS_RPD_Data[s][c];
		}
		Q_X[s] = (nE_Sum_X/E_Sum);
		Q_Y[s] = (nE_Sum_Y/E_Sum);
	}

	ALICE_Q_X_Neg = Q_X[0];
	ALICE_Q_Y_Neg = Q_Y[0];
	ALICE_Q_X_Pos = Q_X[1];
	ALICE_Q_Y_Pos = Q_Y[1];

}

double ALICE_Centrality_Array_Chooser( double centBinvalue){

	double centIter = -1;

	if (centBinvalue < 10){ //0-5%
		centIter = 0;
	}
	else if (centBinvalue < 20){ //5-10%
		centIter = 1;
	}
	else if (centBinvalue < 30){ //10-15%
		centIter = 2;
	}
	else if (centBinvalue < 40){ //15-20%
		centIter = 3;
	}
	else if (centBinvalue < 50){ //20-25%
		centIter = 4;
	}
	else if (centBinvalue < 60){ //25-30%
		centIter = 5;
	}
	else if (centBinvalue < 70){ //30-35%
		centIter = 6;
	}
	else if (centBinvalue < 80){ //35-40%
		centIter = 7;
	}
	else if (centBinvalue < 90){ //40-45%
		centIter = 8;
	}
	else if (centBinvalue < 100){ //45-50%
		centIter = 9;
	}
	else if (centBinvalue < 110){ //50-55%
		centIter = 10;
	}
	else if (centBinvalue < 120){ //55-60%
		centIter = 11;
	}
	else if (centBinvalue < 130){ //60-65%
		centIter = 12;
	}
	else if (centBinvalue < 140){ //65-70%
		centIter = 13;
	}
	else if (centBinvalue < 150){ //70-75%
		centIter = 14;
	}
	else if (centBinvalue < 160){ //75-80%
		centIter = 15;
	}
	else if (centBinvalue < 170){ //80-85%
		centIter = 16;
	}
	else if (centBinvalue < 180){ //85-90%
		centIter = 17;
	}
	else if (centBinvalue < 190){ //90-95%
		centIter = 18;
	}
	else if (centBinvalue < 200){ //95-100%
		centIter = 19;
	}
	return centIter;
}