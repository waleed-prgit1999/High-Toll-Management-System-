#include <iostream>
#include <fstream>
#include <string.h>


using namespace std;
class date
{
public:
	int day, mon, year;
	date(){}
	date(int d, int m, int y) { day = d; mon = m; year = y; }
	void get_date()
	{
		cout << "\t\tEnter the Day  Month  Year  "; cin >> day >> mon >> year;
		while ((day>31) | (day<1) | (mon<1) | (mon>12) | (year<1))
		{
			cout << "Wrong date!!!! Retry" << endl;
			cout << "\t\tEnter the Day  Month  Year  "; cin >> day >> mon >> year;
		}
	}
	void disp_date()
	{
		cout << day << "/" << mon << "/" << year;
	}
};

class vehicle                                 ///Vehicle class is made abstract here
{
	date d;

protected:
	char number[10];
	double toll = 0;
public:
	virtual void get_data()
	{
		cout << "\t\tEnter Number :"; cin >> number;
		cout << "\t\tEnter Date of Arrival :" << endl;
		d.get_date();
	}
	virtual void put_data()
	{
		cout << "\n";
		d.disp_date(); cout << "\t" << number << "\t\tRs." << toll;
	}
	virtual void write_in_file() = 0;             ///pure virtual function
	virtual void calculate_toll() = 0;            ///pure virtual function
	virtual ~vehicle(){}
};

class car : public vehicle
{
	char model[15], company[15], colour[15];
	int cc;
public:
	void get_data()
	{
		vehicle::get_data();
		cout << "\t\tEnter Car Company :"; cin >> company; cout << "\t\tEnter Car Model :"; cin >> model;
		cout << "\t\tEnter Car Colour :"; cin >> colour;   cout << "\t\tEnter Car Engine CC :"; cin >> cc;
	}

	void calculate_toll()
	{
		if (cc <= 1000)
			toll = 25.0;

		else if (cc <= 1600 && cc>1000)
			toll = 40.0;

		else if (cc>1600)
			toll = 60.0;

		cout << "Toll of Car : Rs." << toll << endl;
	}

	void put_data()
	{
		vehicle::put_data();
		cout << "\t\t\t" << company << "\t\t" << model << "\t\t" << colour << "\t\t" << cc;
	}
	void write_in_file()
	{
		fstream datafile;

		datafile.open("Toll_Records_Cars.dat", ios::app | ios::binary);    ///opening file in binary and append mode
		datafile.write((char *) this, sizeof(car));                         ///writing an object of car type in file
		datafile.close();                                               ///closing file
	}
	void load_from_file()
	{
		fstream datafile;
		car c1;

		datafile.open("Toll_Records_Cars.dat", ios::in | ios::binary);
		cout << "\nDate\t\tNumber\t\tToll Collected\t\tCompany\t\tModel\t\tColor\t\tEngine cc"; ///printing the saved record of cars
		if (!datafile.eof())
		{
			datafile.read((char *)&c1, sizeof(c1));
			while (!datafile.eof())
			{
				c1.put_data();
				datafile.read((char *)&c1, sizeof(c1));   ///reading the record from file in a loop then printing it
			}
			datafile.close();
		}
	}

	void delete_from_file()
	{
		char test_num[15];
		ifstream infile;
		infile.open("Toll_Records_Cars.dat", ios::in | ios::binary);
		ofstream ofile;
		ofile.open("Temp.dat", ios::out | ios::binary);
		cout << "Enter the number of Car to deleted  "; cin >> test_num;
		int flag = 0;

		while (!infile.eof())
		{
			infile.read((char *)this, sizeof(car));
			if (infile)
			{
				if (strcmp(number, test_num) == 0)
				{
					flag = 1;
					put_data();
					cout << "\nRecord Deleted!!" << endl;
				}
				else
				{
					ofile.write((char *)this, sizeof(car));
				}
			}
		}
		if (flag == 0)
			cout << "\n Record Not Found!!!\n";

		infile.close();
		ofile.close();
		remove("Toll_Records_Cars.dat");
		rename("Temp.dat", "Toll_Records_Cars.dat");

	}

	car operator + (car c)
	{
		car temp; temp.toll = toll + c.toll;
		return temp;
	}
	friend double Total_toll_cars();

};
double Total_toll_cars()                             ///A friend function can access the data members and member function of class
///although it is defined out of the class
{
	fstream datafile;
	car c1, c2;
	datafile.open("Toll_Records_Cars.dat", ios::in | ios::binary);
	datafile.read((char *)&c1, sizeof(c1));
	while (!datafile.eof())
	{
		c2 = c2 + c1;                                 ///using overloaded operator +
		datafile.read((char *)&c1, sizeof(c1));
	}
	datafile.close();
	return c2.toll;
}

class bus : public vehicle
{
	int cc, seats;
public:
	void get_data()
	{
		vehicle::get_data();
		cout << "\t\tEnter the number of Seats: "; cin >> seats;
		cout << "\t\tEnter the Engine CC: "; cin >> cc;
	}
	void calculate_toll()
	{
		if (cc <= 5000)
			toll = 160.0;
		else if (cc>5000 && cc <= 8000)
			toll = 250.0;
		else
			toll = 320.0;

		cout << "Toll of Buss: Rs." << toll << endl;
	}
	void put_data()
	{
		vehicle::put_data();
		cout << "\t\t\t" << seats << "\t\t\t" << cc;
	}
	void write_in_file()
	{
		fstream datafile3;
		datafile3.open("Toll_Records_Buses.dat", ios::app | ios::binary);
		datafile3.write((char *) this, sizeof(bus));
		datafile3.close();
	}
	void load_from_file()
	{
		fstream datafile3;
		bus c1;

		datafile3.open("Toll_Records_Buses.dat", ios::in | ios::binary);
		cout << "\nDate\t\tNumber\t\tToll Collected\t\tSeating Capacity\tEngine CC";
		if (datafile3)
		{
			datafile3.read((char *)&c1, sizeof(c1));
			while (!datafile3.eof())
			{
				c1.put_data();
				datafile3.read((char *)&c1, sizeof(c1));
			}
			datafile3.close();
		}
	}

	void delete_from_file()
	{
		char test_num[15];

		ifstream infile;
		infile.open("Toll_Records_Buses.dat", ios::in | ios::binary);
		ofstream ofile;
		ofile.open("Temp2.dat", ios::out | ios::binary);
		cout << "Enter the number of Bus to deleted  "; cin >> test_num;
		int f = 0;
		while (!infile.eof())
		{
			infile.read((char *)this, sizeof(bus));
			if (infile)
			{
				if (strcmp(number, test_num) == 0)
				{
					f = 1;
					put_data();
					cout << "\nRecord Deleted!!" << endl;
				}
				else
				{
					ofile.write((char *)this, sizeof(bus));
				}
			}
		}
		if (f == 0)
		{
			cout << "\nRecord Not Found!!!" << endl;
		}

		infile.close();
		ofile.close();
		remove("Toll_Records_Buses.dat");
		rename("Temp2.dat", "Toll_Records_Buses.dat");
	}

	bus operator + (bus c)
	{
		bus temp; temp.toll = toll + c.toll;
		return temp;
	}
	friend double Total_toll_buses();
};

double Total_toll_buses()                    ///A friend function can access the data members and member function of class
{                                        ///although it is defined out of the class
	fstream datafile3;
	bus c1, c2;
	datafile3.open("Toll_Records_Buses.dat", ios::in | ios::binary);
	datafile3.read((char *)&c1, sizeof(c1));
	while (!datafile3.eof())
	{
		c2 = c2 + c1;                                 ///using overloaded operator +
		datafile3.read((char *)&c1, sizeof(c1));
	}
	datafile3.close();
	return c2.toll;
}

class truck : public vehicle
{
	double load_weight;
	int axels;
public:
	void get_data()
	{
		vehicle::get_data();
		cout << "\t\tEnter Axles of the Truck "; cin >> axels;
		while (axels != 2 && axels != 4)
		{
			cout << "Only 2 and 4 axle trucks are allowed" << endl << "Enter Axles of the Truck again ";
			cin >> axels;
		}
		cout << "\t\tEnter Total Load Weight in Tons "; cin >> load_weight;
	}
	void calculate_toll()                            ///criteria of toll calculation in trucks
	{
		if (load_weight / axels <= 12)                 ///weight per axle should be under 12 tons
		{
			if (axels == 2)
				toll = 150.0;
			if (axels == 4)
				toll = 250.0;

			cout << "Toll of Truck : Rs." << toll << endl;
		}
		else
			cout << "Truck is Overloaded !!! " << endl;
	}
	void put_data()
	{
		vehicle::put_data();
		cout << "\t\t\t" << load_weight << "\t\t\t" << axels;
	}
	void write_in_file()
	{
		fstream datafile2;
		datafile2.open("Toll_Records_Trucks.dat", ios::app | ios::binary);
		datafile2.write((char *) this, sizeof(truck));
		datafile2.close();
	}
	void load_from_file()
	{
		fstream datafile2;
		truck c1;

		datafile2.open("Toll_Records_Trucks.dat", ios::in | ios::binary);
		cout << "\nDate\t\tNumber\t\tToll Collected\t\tLoad Weight\t\tNo. of Axles";
		if (datafile2)
		{
			datafile2.read((char *)&c1, sizeof(c1));
			while (!datafile2.eof())
			{
				c1.put_data();
				datafile2.read((char *)&c1, sizeof(c1));
			}
			datafile2.close();
		}
	}

	void delete_from_file()
	{
		char test_num[15];
		ifstream infile;
		infile.open("Toll_Records_Trucks.dat", ios::in | ios::binary);
		ofstream ofile;
		ofile.open("Temp3.dat", ios::out | ios::binary);
		cout << "Enter the number of Truck to deleted  "; cin >> test_num;
		int flag = 0;
		while (!infile.eof())
		{
			infile.read((char *)this, sizeof(truck));
			if (infile)
			{
				if (strcmp(number, test_num) == 0)
				{
					flag = 1;
					put_data();
					cout << "\nRecord Deleted!!" << endl;
				}
				else
				{
					ofile.write((char *)this, sizeof(truck));
				}
			}
		}
		if (flag == 0)
		{
			cout << "\n Record not Found!!!";
		}
		infile.close();
		ofile.close();
		remove("Toll_Records_Trucks.dat");
		rename("Temp3.dat", "Toll_Records_Trucks.dat");
	}

	truck operator + (truck c)
	{
		truck temp; temp.toll = toll + c.toll;
		return temp;
	}
	friend double Total_toll_trucks();
};

double Total_toll_trucks()            ///A friend function can access the data members and member function of class through an object
{                               ///although it is defined out of the class
	fstream datafile2;
	truck c1, c2;
	datafile2.open("Toll_Records_Trucks.dat", ios::in | ios::binary);
	datafile2.read((char *)&c1, sizeof(c1));
	while (!datafile2.eof())
	{
		c2 = c2 + c1;                                 ///using overloaded operator +
		datafile2.read((char *)&c1, sizeof(c1));
	}
	datafile2.close();
	return c2.toll;
}

int main()
{
	int choice, choice2, choice3, v_choice, choice4;
	while (1)
	{
		cout << "\n\n\t\t Main Menu:" << endl;
		cout << "\t\t 1. Enter New Toll Record " << endl; cout << "\t\t 2. Load Records from File  " << endl;
		cout << "\t\t 3. View Total toll " << endl; cout << "\t\t 4. Delete Records from File " << endl; cout << "\t\t 5. Exit " << endl;
		cout << "Enter Your choice "; cin >> choice;
		switch (choice)
		{
		case 1:
			cout << "\t\t 1. Enter New Toll Record " << endl; cout << "\t\t 2. Back TO Main Menu " << endl;
			cout << "Enter Your Choice "; cin >> choice2;
			switch (choice2)
			{
			case 1:
				cout << endl << "\t\tEnter Vehicle Type" << endl;
				cout << "\t\t1. Car" << endl;
				cout << "\t\t2. Bus" << endl;
				cout << "\t\t3. Truck" << endl;
				cout << "Enter Your choice "; cin >> v_choice;
				vehicle* ptr;                                ///vehicle type pointer
				switch (v_choice)
				{
				case 1:
					ptr = new car;               ///polymorphism
					ptr->get_data();             ///base class pointer calling derived class function duo to virtual
					ptr->calculate_toll();
					ptr->write_in_file();
					delete ptr;
					break;
				case 2:
					ptr = new bus;
					ptr->get_data();
					ptr->calculate_toll();
					ptr->write_in_file();
					delete ptr;
					break;
				case 3:
					ptr = new truck;
					ptr->get_data();
					ptr->calculate_toll();
					ptr->write_in_file();
					delete ptr;
					break;
				}
				break;
			case 2: break;
			}
			break;
		case 2:
			cout << "\t\t 1. Load Car Records " << endl; cout << "\t\t 2. Load Bus Records " << endl;  cout << "\t\t 3. Load Truck Records " << endl;
			cout << "Enter Your Choice "; cin >> choice3;
			switch (choice3)
			{
			case 1:
			{   car c;
			cout << endl << endl << endl; c.load_from_file(); cout << endl << endl << endl;
			break; }
			case 2:
			{   bus b;
			cout << endl << endl << endl; b.load_from_file(); cout << endl << endl << endl;
			break; }
			case 3:
			{   truck t;
			cout << endl << endl << endl; t.load_from_file(); cout << endl << endl << endl;
			break; }
			}
			break;
		case 3:
		{double t = Total_toll_cars() + Total_toll_buses() + Total_toll_trucks();
		cout << "Total Toll of All Vehicles Until Now : Rs." << t << endl << endl;
		break; }
		case 4:
		{
				  cout << "\t\t 1. Delete Car Records " << endl; cout << "\t\t 2. Delete Bus Records " << endl;  cout << "\t\t 3. Delete Truck Records " << endl;
				  cout << "Enter Your Choice "; cin >> choice4;
				  switch (choice4)
				  {
				  case 1:
				  {car *ptr1;
				  ptr1 = new car;
				  ptr1->delete_from_file();
				  delete ptr1; break; }
				  case 2:
				  {bus *ptr1;
				  ptr1 = new bus;
				  ptr1->delete_from_file();
				  delete ptr1; break; }
				  case 3:
				  {truck *ptr1;
				  ptr1 = new truck;
				  ptr1->delete_from_file();
				  delete ptr1; break; }
				  }
				  break; }
		case 5: exit(1); break;

		default:
			cout << "Wrong choice!! Choose Again" << endl;
		}
	}
}

