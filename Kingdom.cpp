#include "stronghold.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


// Constructor
Kingdom::Kingdom(const string& kingdomName) : name(kingdomName), stability(50), year(1), socialClassCount(0)
{
    // Initialize population
    population = new Population(1000);

    // Initialize social classes
    socialClasses[socialClassCount++] = new Peasant(500, 40, 20, 50, 30);
    socialClasses[socialClassCount++] = new Merchant(300, 50, 30, 1000);
    socialClasses[socialClassCount++] = new Noble(200, 50, 50, 50);

    // Initialize military
    military = new Infantry(100, 80, 50, 70, 50, 100);

    // Initialize leader
    leader = new Monarch("King Hanry", 50, 10, "Benevolent", "Prince Charles", 10);

    // Initialize economy
    economy = new Economy(5000);
}

// Destructor
Kingdom::~Kingdom() {
    delete population;
    delete military;
    delete leader;
    delete economy;
    for (int i = 0; i < socialClassCount; ++i) {
        delete socialClasses[i];
    }
}

// Simulate a turn (advance the year and update systems)
void Kingdom::simulateTurn() {
    ++year;

    // Update population
    population->calculateGrowth(80, 70); // Example food and housing quality
    population->updateHealth(90, 80);    // Example sanitation and medical care

    // Update social classes
    for (int i = 0; i < socialClassCount; ++i) {
        socialClasses[i]->updateHappiness(1); // Example happiness modifier
    }

    // Update military morale
    military->updateMorale(2); // Example morale boost

    // Update economy
    economy->collectTaxes(socialClasses, socialClassCount);
    economy->calculateInflation();
    economy->processLoans();

    // Adjust stability
    recalculateStability();
}

// Display the current status of the kingdom
void Kingdom::displayStatus() const {
    cout << "\************************************************************************************************************\n";
    cout << "\n                                         Kingdom Status                                                    \n";
    cout << "\n************************************************************************************************************\n";
    cout << "Name: " << name << "\n";
    cout << "Year: " << year << "\n";
    cout << "Stability: " << stability << "\n";
    cout << "Population: " << population->getTotalCount() << "\n";
    cout << "Treasury: " << economy->getTreasury() << " gold\n";
    cout << "Military Strength: " << military->calculateStrength() << "\n";
    cout << "Leader: " << leader->getName() << " (" << leader->getLeadershipStyle() << ")\n";
    cout << "\n============================================================================================================\n";
}

// Save the game state to a file
void Kingdom::saveGameState(const string& filename) const
{
    ofstream outFile(filename);
    if (!outFile)
    {
        throw runtime_error("Failed to open file for saving.");
    }

    outFile << name << "\n";
    outFile << stability << "\n";
    outFile << year << "\n";
    outFile << population->getTotalCount() << "\n";

    for (int i = 0; i < socialClassCount; ++i)
    {
        outFile << socialClasses[i]->getClassName() << " "
            << socialClasses[i]->getPopulation() << " "
            << socialClasses[i]->getHappiness() << " "
            << socialClasses[i]->getInfluence() << "\n";
    }

    outFile << military->getSoldierCount() << " "
        << military->getMorale() << " "
        << military->getTrainingLevel() << " "
        << military->getEquipmentQuality() << "\n";

    outFile << leader->getName() << " "
        << leader->getPopularity() << " "
        << leader->getCorruptionLevel() << " "
        << leader->getLeadershipStyle() << "\n";

    outFile << economy->getTreasury() << " "
        << economy->getTaxRate() << " "
        << economy->getInflation() << " "
        << economy->getGDP() << " "
        << economy->getCorruptionLevel() << "\n";

    outFile.close();
}

// Load the game state from a file
void Kingdom::loadGameState(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        throw runtime_error("Failed to open file for loading.");
    }

    getline(inFile, name);
    inFile >> stability >> year;

    int populationCount;
    inFile >> populationCount;
    delete population;
    population = new Population(populationCount);

    for (int i = 0; i < socialClassCount; ++i) {
        delete socialClasses[i];
    }
    socialClassCount = 0;

    for (int i = 0; i < MAX_SOCIAL_CLASSES; ++i) {
        string className;
        int pop, happy, infl;
        inFile >> className >> pop >> happy >> infl;

        if (className == "Peasant")
        {
            socialClasses[socialClassCount++] = new Peasant(pop, happy, infl, 50, 30);
        }
        else if (className == "Merchant")
        {
            socialClasses[socialClassCount++] = new Merchant(pop, happy, infl, 1000);
        }
        else if (className == "Noble")
        {
            socialClasses[socialClassCount++] = new Noble(pop, happy, infl, 50);
        }
    }

    int soldiers, morale, training, equipment;
    inFile >> soldiers >> morale >> training >> equipment;
    delete military;
    military = new Infantry(soldiers, morale, training, equipment, 50, 100);

    string leaderName, style;
    int popularity, corruption;
    inFile >> leaderName >> popularity >> corruption >> style;
    delete leader;
    leader = new Monarch(leaderName, popularity, corruption, style, "Heir", 10);

    int treasury, taxRate, inflation, gdp, corruptionLevel;
    inFile >> treasury >> taxRate >> inflation >> gdp >> corruptionLevel;
    delete economy;
    economy = new Economy(treasury);
    economy->adjustTaxRate(taxRate);

    inFile.close();
}

// recalculate stability based on various factors
void Kingdom::recalculateStability() {
    int happinessSum = 0;
    for (int i = 0; i < socialClassCount; ++i) {
        happinessSum += socialClasses[i]->getHappiness();
    }
    int averageHappiness = happinessSum / socialClassCount;

    // stability is influenced by happiness, military morale, and leader popularity
    stability = (averageHappiness * 4 / 10) +
        (military->getMorale() * 3 / 10) +
        (leader->getPopularity() * 3 / 10);

    // clamp stability between 0 and 100
    if (stability > 100) stability = 100;
    if (stability < 0) stability = 0;
}

// adjust stability by a given modifier
void Kingdom::adjustStability(int modifier) {
    stability += modifier;
    if (stability > 100) stability = 100; // clamp stability to a maximum of 100
    if (stability < 0) stability = 0;     // clamp stability to a minimum of 0
}

// Trigger a random event
void Kingdom::triggerRandomEvent()
{
    int eventType = rand() % 3;
    if (eventType == 0)
    {
        cout << "A famine has struck the kingdom!\n";
        economy->getFood()->consume(100);
        adjustStability(-10);
    }
    else if (eventType == 1)
    {
        cout << "A plague has spread across the kingdom!\n";
        population->migrate(-50);
        adjustStability(-20);
    }
    else if (eventType == 2)
    {
        cout << "An invasion threatens the kingdom!\n";
        military->recruit(-20);
        adjustStability(-15);
    }
}

// Resolve active events
void Kingdom::resolveActiveEvents() {
    cout << "Resolving all active events...\n";
    adjustStability(10); // Example: Increase stability
    economy->getFood()->produce(50);
}

// Display active events
void Kingdom::displayActiveEvents() {
    cout << "Active Events:\n";
    cout << "1. Famine: Food resources are low.\n";
    cout << "2. Plague: Population health is declining.\n";
    cout << "3. Invasion: Military strength is under threat.\n";
}