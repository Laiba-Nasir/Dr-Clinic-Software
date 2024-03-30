using namespace std;
#include <iostream>
#include "Doctor.h"
#include "Insurance.h"
#include "Clinic.h"


int main(void) {
	//A doctor with two patients
	Doctor d1("Dr. Old", 2, new string[2]{ "p1", "p2" });
	cout << "\n d1 " << d1.toString() << endl;

	//testing the custom copy-constructor
	Doctor d2(d1);
	cout << "\n d2 " << d2.toString() << endl;

	//customizing the clone d2
	d2.setDname("Dr. Young");
	cout << "\n d2 " << d2.toString() << endl;

	//create (independent) Insurance objects
	cout << "\n\tCreating independent insurance objects\n";
	Insurance i1("Aetna", "claims@aetna.com");
	Insurance i2("Cigna", "claims@cigna.com");
	cout << i1.toString() << endl;
	cout << i2.toString() << endl;

	//Add Insurance i1 to doctor d2
	d2.addInsuranceCompany(i1);
	cout << "\n d2 " << d2.toString() << endl;

	//Create a new doctor with patients and insurance providers
	Doctor d3("Dr. Strange", 2, new string[2]{ "x1", "x2" },
		vector<Insurance>({ i1, i2 }));
	cout << "\n d3 " << d3.toString() << endl;

	//doctor d3 accepts another insurance company
	d3.addInsuranceCompany(Insurance("Blue Cross", "bcross@gmail.com"));
	cout << "\n d3 " << d3.toString() << endl;

	//add one more patient to doctor d3
	d3.addPatient("x3");
	cout << "\n d3 " << d3.toString() << endl;

	//Testing operator=
	Doctor d4;
	cout << "\n d4 " << d4.toString() << endl;
	d4 = d3;
	d4.setDname("Dr. Strange-Clone");
	cout << "\n d4 " << d4.toString() << endl;

	//================================================================
		//Create a new clinic with only one doctor (d2)
	cout << "\n\n Testing Clinic class\n\n";
	Clinic c1("Providence", vector<Doctor>({ d2 }));
	cout << " c1 " << c1.toString() << endl;

	//Doctor d4 joins the clinic
	c1.addDoctor(d4);
	cout << " c1 " << c1.toString() << endl;

	cout << "\nAll done\n";
}

//// CLINIC CLASS BELOW 
#pragma once
using namespace std;
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Insurance.h"
#include "Doctor.h"

class Clinic {
private:
	string cname; // cname holds the clinics name
	vector <Doctor> vd; // vd vector holds dr's in that clinic 
public:
	void setCname(string cname) {
		this->cname = cname;
	}
	void setVdoctor(vector <Doctor> d) {
		vd = d;
	}
	string getCname() {
		return cname;
	}
	vector <Doctor> getVdoctor() {
		return vd;
	}
	//Clinic c1("Providence", vector<Doctor>({ d2 }));
	Clinic(string cname = "NA", vector<Doctor>vdVal = {}) {
		setCname(cname);
		setVdoctor(vdVal);
	}
	void addDoctor(Doctor d) {
		vd.push_back(d);
	}
	string toString() {
		stringstream sout;
		sout << "\t\n==>> Address = " << this
			<< ", clinic name = " << cname
			<< "Doctors ";
		for (Doctor d : vd) {
			sout << "\n" << d.toString() << ", \n";
		}
		return sout.str();
	}
};



/// DOCTOR CLASS BELOW 
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Insurance.h"
using namespace std;

class Doctor
{
private:
	string  dName;
	string* pName;
	int		pCount;
	vector<Insurance> v;  //Aggregation

	string  titleCaps(string s) {
		string s2 = "";
		char previous = '*';
		for (int i = 0; i < s.length(); i++)
		{
			if (isalnum(s[i]))
				s2 += s[i];
			if (s[i] == ' ' && previous != ' ')
				s2 += s[i];
			previous = s[i];
		}

		//lowercasing all characters
		for (char& c : s2) if (isalnum(c)) c = tolower(c);

		//removing leading and trailing spaces (if needed)
		if (s2[0] == ' ') s2 = s2.substr(1);
		if (s2[s2.length() - 1] == ' ') s2 = s2.substr(0, s2.length() - 1);

		//uppercasing first character of a word
		previous = ' ';
		for (int i = 0; i < s2.length(); i++)
		{
			if (previous == ' ') s2[i] = toupper(s2[i]);
			previous = s2[i];
		}
		return s2;
	}
public:
	//Mutators
	void setDname(string dNameValue) { dName = titleCaps(dNameValue); }
	void setPname(string* pNameValue) { pName = pNameValue; }
	void setPcount(int pCountValue) { pCount = pCountValue; }
	void setVcomp(vector<Insurance> vValue) { v = vValue; }

	//Accessors
	string  getDname() { return dName; }
	string* getPname() { return pName; }
	int     getPcount() { return pCount; }
	vector<Insurance> getVcomp() { return v; }

	//Constructors
	~Doctor()
	{
		//cout << " " << this << " Destructor called (removing patients)\n";
		delete[] pName;
	}

	Doctor(string dNameValue = "n.a.", int pCountValue = 0, string* pNameValue = nullptr,
		vector<Insurance> vValue = {})
	{
		//Basic data includes doctor's name, count of patients, and pointer to array
		setDname(dNameValue);
		setPcount(pCountValue);
		pName = nullptr;

		if (pCount == 0) return;

		//If one or more patients, create an array-box to store them
		pName = new string[pCount];

		for (int i = 0; i < pCount; i++)
		{
			*(pName + i) = *(pNameValue + i);
			pName[i] = pNameValue[i];
		}

		//If Insurance data is supplied, copy each onject into our local vector
		if (vValue.size() > 0)
		{
			for (Insurance i : vValue)
			{
				v.push_back(i);
			}
		}
	}

	//Custom Copy-Constructor
	Doctor(const Doctor& otherDoctor)
	{
		//The Doctor class has a pointer to data on the HEAP 
		//therefor we MUST create a custom copy-constructor
		this->dName = otherDoctor.dName;
		this->pCount = otherDoctor.pCount;

		//If there are patients copy their data into a NEW array-box
		if (this->pCount == 0)
			pName = nullptr;
		else
		{
			pName = new string[pCount];
			for (int i = 0; i < pCount; i++)
			{
				pName[i] = otherDoctor.pName[i];
			}
		}
		//copy insurance companies accepted by the doctor
		this->setVcomp(otherDoctor.v);

	}

	//User-defined Methods
	void operator=(const Doctor& otherDoctor)
	{
		this->setDname(otherDoctor.dName);
		this->setPcount(otherDoctor.pCount);
		this->setVcomp(otherDoctor.v);
		if (pCount == 0) return; //no patients

		//copy existing patients into new array-box
		pName = new string[pCount];
		for (int i = 0; i < pCount; i++)
		{
			pName[i] = otherDoctor.pName[i];
		}

		//copy insurance companies
		this->setVcomp(otherDoctor.v);
	}


	string toString()
	{
		//show Doctor's name, count of patients, pointer to array
		ostringstream sout;
		sout << this << " Doctor [ Name: " << getDname()
			<< ", P. Count: " << getPcount()
			<< ", pName: " << getPname()
			<< ", \n\t\tPatients: ";

		//now list patient's names (if any!)
		if (pCount == 0)
			sout << "None";
		else
		{
			sout << "\n";
			for (int i = 0; i < pCount; i++)
			{
				sout << "\t\t" << (pName + i) << " : " << *(pName + i) << ", " << endl;
			}
		}

		//show insurance companies accepted by the Doctor object
		sout << "\t\tInsurance: ";
		if (v.size() == 0)
			sout << "None";
		else
		{
			for (Insurance c : v) sout << "\n\t" << c.toString() << ", ";
		}

		sout << " ]\n";
		return sout.str();
	}

	//Doctor accepts another insurance company c
	void addInsuranceCompany(Insurance c)
	{
		v.push_back(c);
	}


	void addPatient(string p)
	{
		//create a new array-box with room for one more patient
		string* newlist = new string[pCount + 1];

		//copy old patients into new array-box
		for (int i = 0; i < pCount; i++)
		{
			newlist[i] = pName[i];
		}

		//add new patient to bigger array-box
		newlist[pCount] = p;

		//get rid of old array-box
		cout << "\t\tDoctor: (addPatient) Detroying old list of Patients " << pName << endl;
		delete[] pName;

		//update Doctor's pointer to new array-box
		pName = newlist;

		//one more patient
		pCount++;
	}

};

// INSURANCE CLASS BELOW 
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Doctor.h"
using namespace std;

class Insurance {
private:
	string name;
	string email;
public:
	// setters
	void setName(string name) { this->name = name; }
	void setEmail(string email) { this->email = email; }
	//getters
	string getName() { return name; }
	string getEmail() { return email; }
	// constructor 
	Insurance(string name = "NA", string email = "NA") {
		setName(name);
		setEmail(email);
	}
	string toString() {
		stringstream sout;
		sout << "\t Address = " << this << ", Insurance [Name = " << getName()
			<< ", Email = " << getEmail() << "]\n";
		return sout.str();
	}
};




