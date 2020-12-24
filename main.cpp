#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "defines.h"
#include "triss.h"
std::ifstream infile("newDB.csv");
std::ofstream outfile("fitness.csv");

int currGen = 0;
int bestIndIndex = 0;
std::vector<float> bestFitness;
std::vector<Patient> patients;
std::vector<dbInfo> dbA;
std::vector<dbInfo> db10;
std::vector<dbInfo> db20;
std::vector<dbInfo> db30;
std::vector<dbInfo> db50;

std::vector<float> randVec;

std::vector<std::vector<float>> population;

std::vector<std::string> split(std::string str, std::string token);
void loadDatabase();
void fitnessToFile();
void bestIndToFile();
void createPopulation();
void evolve();

void generateDatabase();

int main()
{
	loadDatabase();
	//generateDatabase();
	createPopulation();

	for(int i=0;i<20000;i++)
		randVec.push_back(rand()%1000/1000.0);

	float sumSurvived = 0;
	float sumDead = 0;
	int countDead = 0;
	int countSurvived = 0;

	int p=0;
	for(auto patient : patients)
	{
		float agtriss = calculateTriss(patient);

		if(dbA[p++].survived)
		{
			sumSurvived+=agtriss;
			countSurvived++;
		}
		else
		{
			sumDead+=agtriss;
			countDead++;
		}

	}
	sumSurvived/=countSurvived;
	sumDead/=countDead;
	std::cout << "RESULT TRISS:" << sumSurvived-sumDead 
		<< "\tdead: " << sumDead
		<< "\tsurvived: " << sumSurvived
		<< std::endl; 

	evolve();

	return 0;
}


void generateDatabase()
{
	std::ofstream outdbfile("newDB.csv", std::ofstream::out | std::ofstream::trunc);
	if(outdbfile.is_open())
	{
		for(auto patient : patients)
		{
			outdbfile
				<< patient.fr << ","
				<< patient.pas << ","	
				<< patient.gcs << ","
				<< patient.iss << ","
				<< patient.age << ","
				<< calculateTriss(patient) << ","
				<< 0 << ",";

			for(int db=0;db<5;db++)
			{
				float triss = calculateTriss(patient);

				float sum=0;

				for(int j=0; j<100; j++)
					if(!(rand()%1000/1000.0>triss))
						sum+=0.01;

				if(sum>=0.5)
					outdbfile << 1;
				else
					outdbfile << 0;

				if(db<4)	
					outdbfile << ",";
				else
					outdbfile << std::endl;

			}
		}
	}
	outdbfile.close();
}

void evolve()
{
	while(true)
	{
		std::vector<std::pair<float, int>>	fitness;
		for(int i=0; i<(int)population.size(); i++)
		{
			int qtyPatients = patients.size();
			// Method 1
			float difference = 0;
			// Method 2
			//2) exp -> Sum(Triss_Ag)_1 / Count_1   -  Sum(Triss_Ag)_0 / count_0
			float sumSurvived = 0;
			float sumDead = 0;
			int countDead = 0;
			int countSurvived = 0;
			// Method 3
			std::vector<int> fits;

			int p=0;
			for(int k=0; k<10; k++)
			{
				int fit=0;
				for(auto patient : patients)
				{
					//float triss = calculateTriss(patient);
					//difference += std::fabs(triss-agtriss);
					float agtriss = calculateTriss(patient, population[i]);

					//if(dbA[p].survived == !(dbA[p].randNum>agtriss))
					if(dbA[p].survived == !(rand()%1000/1000.0>agtriss))
						fit++;

					//if(dbA[p++].survived)
					//{
					//	sumSurvived+=agtriss;
					//	countSurvived++;
					//}
					//else
					//{
					//	sumDead+=agtriss;
					//	countDead++;
					//}

					p++;
				}
				p=0;
				fits.push_back(fit);
			}
			std::sort(fits.begin(), fits.end());
			float fitmean = 0;
			for(int k=1; k<(int)(fits.size()-1);k++)
				fitmean+=fits[k]/8.0;

			//sumSurvived/=countSurvived;
			//sumDead/=countDead;
			//difference/=qtyPatients;
			//fitness.push_back(std::make_pair(sumSurvived-sumDead, i));
			fitness.push_back(std::make_pair(fitmean, i));
		}

		//std::sort(fitness.begin(), fitness.end());
		int melhorIndex = 0;
		float melhorFitness = fitness[0].first;
		int piorIndex = 0;
		float piorFitness = fitness[0].first;
		for(int i=1; i<(int)population.size(); i++)
		{
			if(fitness[i].first>melhorFitness)
			{
				melhorFitness = fitness[i].first;
				melhorIndex = i;
			}

			if(fitness[i].first<piorFitness)
			{
				piorFitness = fitness[i].first;
				piorIndex = i;
			}
		}

		for(int i=0; i<(int)population.size(); i++)
		{
			if(i==melhorIndex)
				continue;

			for(int j=0;j<TAM_CRO;j++)
			{
				population[i][j] = (population[i][j]+population[melhorIndex][j])/2.0;
			}

			int num = rand()%100;
			int qtyMutate = 1;

			int cro = rand()%TAM_CRO;
			float mut = rand()%2000/1000.0f*TAX_MUT-TAX_MUT;
			population[i][cro] = population[i][cro]+mut;

			if(num<25)
			{
				cro = rand()%TAM_CRO;
				mut = rand()%2000/1000.0f*TAX_MUT-TAX_MUT;
				population[i][cro] = population[i][cro]+mut;
			}

			if(num<10)
			{
				cro = rand()%TAM_CRO;
				mut = rand()%2000/1000.0f*TAX_MUT-TAX_MUT;
				population[i][cro] = population[i][cro]+mut;
			}
		}

		float sumBestSurvived = 0;
		float sumBestDead = 0;
		{
			int countDead = 0;
			int countSurvived = 0;

			int p=0;
			for(auto patient : patients)
			{
				//float triss = calculateTriss(patient);
				//difference += std::fabs(triss-agtriss);
				float agtriss = calculateTriss(patient, population[melhorIndex]);

				if(dbA[p++].survived)
				{
					sumBestSurvived+=agtriss;
					countSurvived++;
				}
				else
				{
					sumBestDead+=agtriss;
					countDead++;
				}

			}
			sumBestSurvived/=countSurvived;
			sumBestDead/=countDead;
		}

		std::cout << "(" << currGen++ << ") "<<melhorFitness <<
			" triss: "
			<< std::to_string(calculateTriss(patients[3])) 
			<< " ag:"
			<< std::to_string(calculateTriss(patients[3], population[melhorIndex])) 
			<< " best_s: " << sumBestSurvived 
			<< " best_d: " << sumBestDead 
			<< " cro: "
			<< population[melhorIndex][0] << " "
			<< population[melhorIndex][1] << " "
			<< population[melhorIndex][2] << " "
			<< population[melhorIndex][3]
			<< std::endl;
		bestFitness.push_back(melhorFitness);

		bestIndIndex = melhorIndex;

		if(currGen%10==0)
			fitnessToFile();
		if(currGen%5==0)
			bestIndToFile();
	}
}

void createPopulation()
{
	for(int i=0; i<QTY_IND; i++)
	{
		population.push_back({});
		for(int j=0; j<TAM_CRO; j++)
			population.back().push_back(
					rand()%100000/10000.0-5.0);
	}
	population[0] = {-11.3354, 8.42973, -0.680873, -16.5565};
	//population[0] = {-9.78965, 7.57236, -0.615578, -15.8916};
	//population[1] = {-15, 10, -0.615578, -25};
	//population[2] = {-11.5763, 7.57775, -0.587015, -15.1926};
	//population[0] = {-5.27352, 4.44677, -0.374437, -8.52701};
	//population[1] = {-4.77462, 4.21944, -0.350737, -8.48897};
	//population[0] = {-1.2470, 0.9544, -0.0768, -1.9052};
	//population[1] = {-3.21441, 3.0372, -0.266569, -5.51494};
}

void fitnessToFile()
{
	if(outfile.is_open())
	{
		for(int i=0; i<(int)bestFitness.size(); i++)
		{
 			outfile << currGen-10+i <<"," << int(bestFitness[i]) << std::endl;
		}
	}
	bestFitness.clear();
	//outfile.close()
}

void bestIndToFile()
{
	std::ofstream outindfile("res-ind.csv", std::ofstream::out | std::ofstream::trunc);
	if(outindfile.is_open())
	{
		int p=0;
		for(auto patient : patients)
		{
			outindfile << calculateTriss(patient) <<"," << calculateTriss(patient, population[bestIndIndex]) << ","<< dbA[p].survived<<std::endl;

			p++;
		}
	}
	outindfile.close();
}

void loadDatabase()
{
	if(infile.is_open())
	{
		std::string line;
		infile >> line;
		int i = 0;
		while (infile >> line)
		{
			//if(i>=200)
			//	break;

			std::vector<std::string> values = split(line, ",");
			Patient patient = {
				.fr = std::stoi(values[0]),
				.pas = std::stoi(values[1]),
				.gcs = std::stoi(values[2]),
				.iss = std::stoi(values[3]),
				.age = std::stoi(values[4])
			};
			patients.push_back(patient);

			dbInfo info = {
				.triss = std::stof(values[5]),
				.randNum = std::stof(values[6])
			};

			info.survived = values[7]=="1";
			dbA.push_back(info);
			info.survived = values[8]=="1";
			db10.push_back(info);
			info.survived = values[9]=="1";
			db20.push_back(info);
			info.survived = values[10]=="1";
			db30.push_back(info);
			info.survived = values[11]=="1";
			db50.push_back(info);

			float ourTriss = calculateTriss(patient);

			//std::cout << patient.toString() << 
			//	"\t-> db:" + values[5] +
			//	"\tour:" + std::to_string(ourTriss)
			//	<< std::endl;
			i++;
		}
    }
    infile.close();
}

std::vector<std::string> split(std::string str, std::string token)
{
	// https://stackoverflow.com/questions/5607589/right-way-to-split-an-stdstring-into-a-vectorstring
	std::vector<std::string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=std::string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

