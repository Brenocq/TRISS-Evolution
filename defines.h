#ifndef DEFINES_H
#define DEFINES_H

#define QTY_IND 20
#define TAM_CRO 4
#define TAX_MUT 0.2

struct Patient
{
	int fr;
	int pas;	
	int gcs;
	int iss;
	int age;

	std::string toString()
	{
		return std::string("Patient(")+
				"fr:"+std::to_string(fr)+", "
				"pas:"+std::to_string(pas)+", "
				"gcs:"+std::to_string(gcs)+", "
				"iss:"+std::to_string(iss)+", "
				"age:"+std::to_string(age)+
				+")";
	}
};

struct dbInfo
{
	double triss;
	double randNum;
	bool survived;
};

#endif// DEFINES_H
