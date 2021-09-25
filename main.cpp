#include <bits/stdc++.h>
using namespace std;

/*
THIS IS THE PARKING LOT CLASS, CONNTAINS PRIVATE VARIABLES FOR SLOTS AND VARIOUS DATA STRUCTURES FOR THE QUERIES ASKED IN THE PROBLEM STATEMENT:
1)slots: An integer which holds 
2)availableSlots: Stores the empty slots which are free and can be used to park cars in ascending order.
3)ageToPlates: Contains age as key and corresponding registration plates for that age as vector.
4)slotToPlateAndAge: Contains plate number of car and age of driver parked corresponding to a slot.
5)ageToSlots: Contains age as key and corresponding slots for that age as vector.
6) plateToSlot: Contains slot corresponding to a registration plates.
ParkingLot(constructor): Called when instantiating the parking lot object to fill availableSlots in ascending order.
*/
class ParkingLot
{
    int slots;
    priority_queue<int, vector<int>, greater<int>> availableSlots;
    unordered_map<int, vector<string>> ageToPlates;
    unordered_map<int, pair<string, int>> slotToPlateAndAge;    
    unordered_map<int, vector<int>> ageToSlots;
    unordered_map<string, int> plateToSlot;

public:
    ParkingLot(int n)
    {
        slots = n;
        for (int i = 1; i <= slots; i++)
        {
            availableSlots.push(i);
        }
    }
/*
This function parks the vehicle to the slot which is nearest to the gate and adds the data to corresponding Data structure. Returns a string with the slot in which user is parked.
*/
    string parkVehicle(string plateNumber, int age)
    {
        if (availableSlots.size() <= 0)
            return "Parking lot full";
        int slot = availableSlots.top();
        availableSlots.pop();
        plateToSlot[plateNumber] = slot;
        ageToPlates[age].push_back(plateNumber);
        ageToSlots[age].push_back(slot);
        slotToPlateAndAge[slot] = make_pair(plateNumber, age);
        return "Car with vehicle registration number '" + plateNumber + "' has been parked at slot number " + to_string(slot);
    }

//This function iterate the vector contained by ageToSlots and delete the value of particular slot
    void deleteSlotFromAgeToSlots(int slot, vector<int> &removeElementFromVector)
    {
        for (auto it = removeElementFromVector.begin(); it != removeElementFromVector.end(); ++it)
        {
            if ((*it) == slot)
            {
                removeElementFromVector.erase(it);
                return;
            }
        }
    }


//This function iterate the vector contained by ageToPlates and delete the value of particular plate.
    void deletePlateFromAgeToPlates(string plate, vector<string> &removeElementFromHere)
    {
        for (auto it = removeElementFromHere.begin(); it != removeElementFromHere.end(); ++it)
        {
            if ((*it) == plate)
            {
                removeElementFromHere.erase(it);
                return;
            }
        }
    }

//This function takes a slot as an input and clears the entire data of that slot i.e driver age, plate number, slot and their relations from all the data structures.  
    string emptyTheSpot(int slot)
    {
        if (slotToPlateAndAge.find(slot) == slotToPlateAndAge.end())
        {
            return "No vehicle is parked in this slot.";
        }
        availableSlots.push(slot);
        string registrationPlate = slotToPlateAndAge[slot].first;
        int driverAge = slotToPlateAndAge[slot].second;
        slotToPlateAndAge.erase(slot);
        plateToSlot.erase(registrationPlate);
        deleteSlotFromAgeToSlots(slot, ageToSlots[driverAge]);
        deletePlateFromAgeToPlates(registrationPlate, ageToPlates[driverAge]);
        return "Slot number " + to_string(slot) + " vacated, the car with vehicle registration number '" + registrationPlate + "' left the space, the driver of the car was of age " + to_string(driverAge);
    }

//This function returns vector of plates for a given age. In case their is no plate for corresponding age, it returns empty vector.
    vector<string> getPlatesForGivenAge(int age)
    {
        vector<string> output;
        if (ageToPlates.find(age) == ageToPlates.end())
            return output;
        return ageToPlates[age];
    }

//This function returns all the slots corresponding to given age if none present returns empty vector.
    vector<int> getAllSlotsForGivenAge(int age)
    {
        vector<int> output;
        if (ageToSlots.find(age) == ageToSlots.end())
            return output;
        return ageToSlots[age];
    }

//This function returns slot corresponding to a plate. Since slot number are starting from 1 we return 0 if slot is not occupied which is handled gracefully.
    int getSlotFromPlate(string plateNumber)
    {
        if(plateToSlot.find(plateNumber)==plateToSlot.end())
            return 0;
        return plateToSlot[plateNumber];
    }
};

/*
This function reads 'input.txt' file and process it line by line.
There are different conditions based on different commands according the which the output is logged and is inserted in a file to be printed seperately.
Here, we are creating a refrence to class ParkingLot which initializes out availble slot heap.
We call the functions of this class to perform our tasks accordingly.
It returns vector of string which is later converted to output.txt for user to download.
*/
vector<string> executeIndividualTasks()
{
    vector<string> outputFile;
    fstream readInputFile;
    readInputFile.open("input.txt", ios::in);
    string task;
    ParkingLot *parkingLot = NULL;
    while (getline(readInputFile, task))
    {
        string individualTask;
        stringstream iss(task);
        while (iss >> individualTask)
        {
            if (individualTask == "Create_parking_lot")
            {
                iss >> individualTask;
                ParkingLot *temp = new ParkingLot(stoi(individualTask));
                parkingLot = temp;
                cout<<"Created parking of " + individualTask + " slots"<<endl;
                outputFile.push_back("Created parking of " + individualTask + " slots");
            }
            else if (individualTask == "Park")
            {
                iss >> individualTask;
                string registrationPlate = individualTask;
                iss >> individualTask;
                iss >> individualTask;
                int ageOfDriver = stoi(individualTask);
                string output = parkingLot->parkVehicle(registrationPlate, ageOfDriver);
                cout<<output<<endl;
                outputFile.push_back(output);
            }
            else if (individualTask == "Slot_numbers_for_driver_of_age")
            {
                iss >> individualTask;
                vector<int> slots = parkingLot->getAllSlotsForGivenAge(stoi(individualTask));
                int slotSize = slots.size();
                if (!slotSize)
                {
                    cout<<"No such vehicle in the lot."<<endl;
                    outputFile.push_back("No such vehicle in the lot.");
                    continue;
                }
                string outStr = "";
                for (int i = 0; i < slotSize; i++)
                {
                    outStr += to_string(slots[i]);
                    if (i != slotSize - 1)
                        outStr += ",";
                }
                cout<<outStr<<endl;
                outputFile.push_back(outStr);
            }
            else if (individualTask == "Slot_number_for_car_with_number")
            {
                iss >> individualTask;
                int slot = parkingLot->getSlotFromPlate(individualTask);
                if (!slot)
                {
                    cout<<"No such vehicle in the parking lot."<<endl;
                    outputFile.push_back("No such vehicle in the parking lot.");
                }
                else
                {
                    cout<<to_string(slot)<<endl;
                    outputFile.push_back(to_string(slot));
                }
            }
            else if (individualTask == "Leave")
            {
                iss >> individualTask;
                int spotToBeEmptied = stoi(individualTask);
                string output = parkingLot->emptyTheSpot(spotToBeEmptied);
                cout<<output<<endl;
                outputFile.push_back(output);
            }
            else if (individualTask == "Vehicle_registration_number_for_driver_of_age")
            {
                iss >> individualTask;
                vector<string> registrationNumbers = parkingLot->getPlatesForGivenAge(stoi(individualTask));
                int RNVectorSize = registrationNumbers.size();
                if (!RNVectorSize)
                {
                    cout<<"No such vehicle in the lot."<<endl;
                    outputFile.push_back("No such vehicle in the lot.");
                    continue;
                }
                string outStr = "";
                for (int i = 0; i < RNVectorSize; i++)
                {
                    outStr += registrationNumbers[i];
                    if (i != RNVectorSize - 1)
                        outStr += ",";
                }
                cout<<outStr<<endl;
                outputFile.push_back(outStr);
            }
            else
                cout<<"Exceptional task."<<endl;
                outputFile.push_back("Exceptional task.");
        }
    }
    readInputFile.close();
    return outputFile;
}

// This function iterate the given vector of string and convert it to output.txt by leaving line corresponding to each line of output.
void setOutputFile(vector<string> output)
{
    std::ofstream outputFile("output.txt");
    std::ostream_iterator<std::string> outputIterator(outputFile, "\n");
    std::copy(output.begin(), output.end(), outputIterator);
    return;
}

//Here we call executeIndividualTasks function to read and exectue tasks.
int main()
{
    cout << "######################## PARKING LOT ########################" << endl;
    cout << "Note: input.txt(case sensetive) should be in same folder." << endl<<endl;
    vector<string> outputFile;
    outputFile = executeIndividualTasks();
    setOutputFile(outputFile);
    cout <<endl<<"P.s: An output file has also been made for download.";
    return 0;
}
