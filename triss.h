#include "defines.h"
#include <math.h>

float RTS(int FR, int PAS, int GCS);
float TRISSprecalc(float RTS,int ISS,
		int idade, std::vector<float> b, int limiteIdade = 55);

//-1.24593 0.952941 -0.0720031 -1.9063
//-1.24728 0.956701 -0.0774125 -1.90627
float calculateTriss(Patient patient, std::vector<float> b = {-1.2470, 0.9544, -0.0768, -1.9052})
{
	float rts = RTS(patient.fr, patient.pas, patient.gcs);
	return TRISSprecalc(rts, patient.iss, patient.age, b);
}

float RTS(int FR, int PAS, int GCS)
{
	std::vector<float> pesos = {0.2908, 0.7326, 0.9368};
	int scoreFR = 0;
	int scorePAS = 0;
	int scoreGCS = 0;

	if (FR >= 29) {
		scoreFR = 3;
	} else if (FR >= 10) {
		scoreFR = 4;
	} else if (FR >= 6) {
		scoreFR = 2;
	} else if (FR >= 1) {
		scoreFR = 1;
	}

	if (PAS >= 89) {
		scorePAS = 4;
	} else if (PAS >= 76) {
		scorePAS = 3;
	} else if (PAS >= 50) {
		scorePAS = 2;
	} else if (PAS >= 1) {
		scorePAS = 1;
	}

	if (GCS >= 13) {
		scoreGCS = 4;
	} else if (GCS >= 9) {
		scoreGCS = 3;
	} else if (GCS >= 6) {
		scoreGCS = 2;
	} else if (GCS >= 4) {
		scoreGCS = 1;
	}    

	return (scoreFR * pesos[0] + scorePAS * pesos[1] + scoreGCS * pesos[2]);
}

float TRISSprecalc(float RTS,int ISS,int idade, std::vector<float> b, int limiteIdade)
{
	int age;
	if (idade >= limiteIdade) {
		age = 1;
	} else {
		age = 0;
	}

	//int i=0;
	//if (blunt) {
	//	i = 0;
	//} else {
	//	i = 1;
	//}

	float B = b[0] + b[1] * RTS + b[2] * ISS + b[3] * age;

	return (1.0 / (1.0 + exp(-B)));
}
