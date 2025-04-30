#include "stronghold.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <limits> // For input validation
using namespace std;


void displayMainMenu();
void playGame(Kingdom& kingdom);
void displayKingdomOptions(Kingdom& kingdom);
void handleSocialClassInteraction(Kingdom& kingdom);
void handlePopulationActions(Kingdom& kingdom);
void handleMilitaryActions(Kingdom& kingdom);
void handleLeadershipActions(Kingdom& kingdom);
void handleEconomyActions(Kingdom& kingdom);
void handleBankActions(Kingdom& kingdom);
void handleResourceManagement(Kingdom& kingdom);
void handleEventSystem(Kingdom& kingdom);
void logGameEvent(const string& event, const Kingdom& kingdom);
void displayLogs();
int getValidatedInput(int min, int max);


int main() {
    cout << "\n============================================================================================================\n";
    cout << "\n                                       STRONGHOLD: CORE KINGDOM ENGINE                                      \n";
    cout << "\n============================================================================================================\n";
    Kingdom* currentKingdom = nullptr;
    bool gameRunning = true;

    while (gameRunning) {
        displayMainMenu();
        int choice = getValidatedInput(1, 4); // Validate input for main menu

        try {
            switch (choice) {
            case 1: { // New Kingdom
                string kingdomName;
                cout << "Enter your kingdom's name: ";
                cin.ignore();
                getline(cin, kingdomName);

                currentKingdom = new Kingdom(kingdomName);
                logGameEvent("Kingdom " + kingdomName + " was founded", *currentKingdom);
                playGame(*currentKingdom);
                break;
            }
            case 2: { // Load Kingdom
                string filename;
                cout << "Enter save file name: ";
                cin >> filename;

                currentKingdom = new Kingdom("Temp"); // Temporary name
                currentKingdom->loadGameState(filename);
                logGameEvent("Kingdom loaded from " + filename, *currentKingdom);
                playGame(*currentKingdom);
                break;
            }
            case 3: // View Logs
                displayLogs();
                break;
            case 4: // Exit
                gameRunning = false;
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << "\n";
            if (currentKingdom) {
                delete currentKingdom;
                currentKingdom = nullptr;
            }
        }
    }

    if (currentKingdom) {
        delete currentKingdom;
    }

    return 0;
}


int getValidatedInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;

        if (cin.fail() || input < min || input > max) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra input
            return input;
        }
    }
}

void displayMainMenu() {
    cout << "\n============================================================================================================\n";
    cout << "\n                                                    Main Menu:                                              \n";
    cout << "\n============================================================================================================\n";
    cout << "1. New Kingdom\n";
    cout << "2. Load Kingdom\n";
    cout << "3. View Game Logs\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}


void playGame(Kingdom& kingdom) {
    bool inGame = true;

    while (inGame) {
        /* cout << "\n=== Year " << kingdom.getYear() << " ===\n";
         kingdom.displayStatus();*/

        displayKingdomOptions(kingdom);
        int choice = getValidatedInput(1, 11); // Validate input for kingdom options

        switch (choice) {
        case 1: // Advance Year
            kingdom.simulateTurn();
            logGameEvent("Year " + to_string(kingdom.getYear()) + " advanced", kingdom);
            break;
        case 2: // Social Class Interactions
            handleSocialClassInteraction(kingdom);
            break;
        case 3: // Population Actions
            handlePopulationActions(kingdom);
            break;
        case 4: // Military Actions
            handleMilitaryActions(kingdom);
            break;
        case 5: // Leadership Actions
            handleLeadershipActions(kingdom);
            break;
        case 6: // Economy Actions
            handleEconomyActions(kingdom);
            break;
        case 7: // Bank Actions
            handleBankActions(kingdom);
            break;
        case 8: // Resource Management
            handleResourceManagement(kingdom);
            break;
        case 9: // Event System
            handleEventSystem(kingdom);
            break;
        case 10: { // Save Game
            string filename;
            cout << "Enter save file name: ";
            cin >> filename;

            kingdom.saveGameState(filename);
            logGameEvent("Game saved to " + filename, kingdom);
            cout << "Game saved successfully.\n";
            break;
        }
        case 11: // Return to Main Menu
            inGame = false;
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }

        // Display updated status after every action
        kingdom.displayStatus();
    }
}

void displayKingdomOptions(Kingdom& kingdom) {
    cout << "\n============================================================================================================\n";
    cout << "\n                                                 : Kingdom Options :                                        \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Advance to Next Year\n";
    cout << "2. Social Class Interactions\n";
    cout << "3. Population Actions\n";
    cout << "4. Military Actions\n";
    cout << "5. Leadership Actions\n";
    cout << "6. Economy Actions\n";
    cout << "7. Bank Actions\n";
    cout << "8. Resource Management\n";
    cout << "9. Event System\n";
    cout << "10. Save Game\n";
    cout << "11. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void handleSocialClassInteraction(Kingdom& kingdom) {
    SocialClass** socialClasses = kingdom.getSocialClasses();
    int classCount = kingdom.getSocialClassCount();
    cout << "\n============================================================================================================\n";
    cout << "\n                                       :Social Class Interactions:                                          \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Collect Taxes\n";
    cout << "2. Address Class Demands\n";
    cout << "3. View Social Class Details\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1:
    { // Collect Taxes
        int totalTaxes = 0;
        for (int i = 0; i < classCount; ++i)
        {
            int tax = socialClasses[i]->calculateTaxContribution();
            totalTaxes += tax;
        }
        kingdom.getEconomy()->addToTreasury(totalTaxes);
        kingdom.adjustStability(1); // Increase stability for collecting taxes
        logGameEvent("Collected taxes from all social classes", kingdom);
        break;
    }
    case 2: // Address Class Demands
        for (int i = 0; i < classCount; ++i)
        {
            if (socialClasses[i]->getHappiness() < 50)
            {
                socialClasses[i]->demandChange();
            }
        }
        kingdom.adjustStability(1); // Increase stability for addressing demands
        logGameEvent("Addressed demands of social classes", kingdom);
        break;
    case 3: // View Social Class Details
        for (int i = 0; i < classCount; ++i)
        {
            cout << socialClasses[i]->getClassName() << " - Population: " << socialClasses[i]->getPopulation()
                << ", Happiness: " << socialClasses[i]->getHappiness() << "%\n";
        }
        break;
    case 4: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handlePopulationActions(Kingdom& kingdom) {
    Population* population = kingdom.getPopulation();
    cout << "\n============================================================================================================\n";
    cout << "\n                                              :Population Actions:                                          \n";
    cout << "\n============================================================================================================\n";
    cout << "1. View Population Details\n";
    cout << "2. Adjust Growth Rate\n";
    cout << "3. Improve Health\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1: // View Population Details
        cout << "Total Population: " << population->getTotalCount() << "\n";
        cout << "Growth Rate: " << population->getGrowthRate() << "%\n";
        cout << "Health: " << population->getHealth() << "%\n";
        cout << "Employment Rate: " << population->getEmploymentRate() << "%\n";
        break;
    case 2: { // Adjust Growth Rate
        int food, housing;
        cout << "Enter food availability (0-100): ";
        food = getValidatedInput(0, 100);
        cout << "Enter housing quality (0-100): ";
        housing = getValidatedInput(0, 100);
        population->calculateGrowth(food, housing);
        kingdom.adjustStability(2); // Increase stability for improving growth rate
        logGameEvent("Population growth rate adjusted", kingdom);
        break;
    }
    case 3: { // Improve Health
        int sanitation, medicalCare;
        cout << "Enter sanitation level (0-100): ";
        sanitation = getValidatedInput(0, 100);
        cout << "Enter medical care level (0-100): ";
        medicalCare = getValidatedInput(0, 100);
        population->updateHealth(sanitation, medicalCare);
        kingdom.adjustStability(2); // Increase stability for improving health
        logGameEvent("Population health improved", kingdom);
        break;
    }
    case 4: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleMilitaryActions(Kingdom& kingdom) {
    ArmyUnit* military = kingdom.getMilitary();
    cout << "\n============================================================================================================\n";
    cout << "\n                                            :Military Actions:                                              \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Recruit Soldiers\n";
    cout << "2. Train Army\n";
    cout << "3. View Military Strength\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1: { // Recruit Soldiers
        int amount;
        cout << "Enter number of soldiers to recruit: ";
        amount = getValidatedInput(0, 1000); // Example range
        military->recruit(amount);
        kingdom.adjustStability(2); // Increase stability for recruiting soldiers
        logGameEvent("Recruited " + to_string(amount) + " soldiers", kingdom);
        break;
    }
    case 2: { // Train Army
        int intensity;
        cout << "Enter training intensity (0-100): ";
        intensity = getValidatedInput(0, 100);
        military->train(intensity);
        kingdom.adjustStability(2); // Increase stability for training the army
        logGameEvent("Army trained with intensity " + to_string(intensity), kingdom);
        break;
    }
    case 3: // View Military Strength
        cout << "Military Strength: " << military->calculateStrength() << "\n";
        break;
    case 4: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleLeadershipActions(Kingdom& kingdom) {
    Leader* leader = kingdom.getLeader();
    cout << "\n============================================================================================================\n";
    cout << "\n                                              :Leadership Actions:                                          \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Make Decision\n";
    cout << "2. Address Crisis\n";
    cout << "3. Hold Election (if applicable)\n";
    cout << "4. View Leader Details\n";
    cout << "5. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 5);

    switch (choice) {
    case 1: { // Make Decision
        cout << "\n============================================================================================================\n";
        cout << "\n                                               :Decision Options:                                           \n";
        cout << "\n============================================================================================================\n";
        cout << "1. Harsh Decision (order, control)\n";
        cout << "2. Moderate Decision (balance)\n";
        cout << "3. Benevolent Decision (people first)\n";
        cout << "Enter choice: ";
        int decision = getValidatedInput(1, 3);
        leader->makeDecision(decision);
        kingdom.adjustStability(2); // Increase stability for making a decision
        logGameEvent("Leader made decision type " + to_string(decision), kingdom);
        break;
    }
    case 2: // Address Crisis
        leader->addressCrisis();
        kingdom.adjustStability(2); // Increase stability for addressing a crisis
        logGameEvent("Leader addressed a crisis", kingdom);
        break;
    case 3: // Hold Election
        if (leader->holdElection()) {
            cout << "Election held! New leader may take over.\n";
            kingdom.adjustStability(1); // Increase stability for holding an election
            logGameEvent("Election was held", kingdom);
        }
        else {
            cout << "This leader doesn't hold elections.\n";
        }
        break;
    case 4: // View Leader Details
        cout << "\n============================================================================================================\n";
        cout << "\n                                              :Leader Details:                                              \n";
        cout << "\n============================================================================================================\n";
        cout << "Name: " << leader->getName() << "\n";
        cout << "Style: " << leader->getLeadershipStyle() << "\n";
        cout << "Popularity: " << leader->getPopularity() << "%\n";
        cout << "Corruption: " << leader->getCorruptionLevel() << "%\n";
        break;
    case 5: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}


void handleEconomyActions(Kingdom& kingdom) {
    Economy* economy = kingdom.getEconomy();
    cout << "\n============================================================================================================\n";
    cout << "\n                                                   :Economy Actions:                                        \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Adjust Tax Rate\n";
    cout << "2. Fund Public Service\n";
    cout << "3. View Economy Details\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1: { // Adjust Tax Rate
        int newRate;
        cout << "Enter new tax rate (0-100): ";
        newRate = getValidatedInput(0, 100);
        economy->adjustTaxRate(newRate);
        if (newRate < 20) {
            kingdom.adjustStability(1); // Increase stability for lowering taxes
        }
        else {
            kingdom.adjustStability(-1); // Decrease stability for raising taxes
        }
        logGameEvent("Tax rate adjusted to " + to_string(newRate), kingdom);
        break;
    }
    case 2: { // Fund Public Service
        int amount;
        cout << "Enter amount to fund public service: ";
        amount = getValidatedInput(0, economy->getTreasury());
        economy->fundService(amount);
        kingdom.adjustStability(2); // Increase stability for funding public services
        logGameEvent("Funded public service with " + to_string(amount) + " gold", kingdom);
        break;
    }
    case 3: // View Economy Details
        cout << "\n============================================================================================================\n";
        cout << "\n                                               :Economy Details:                                            \n";
        cout << "\n============================================================================================================\n";
        cout << "Treasury: " << economy->getTreasury() << " gold\n";
        cout << "Tax Rate: " << economy->getTaxRate() << "%\n";
        cout << "Inflation: " << economy->getInflation() << "%\n";
        cout << "GDP: " << economy->getGDP() << "\n";
        cout << "Corruption Level: " << economy->getCorruptionLevel() << "%\n";
        break;
    case 4: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}


void handleBankActions(Kingdom& kingdom) {
    Bank* bank = kingdom.getEconomy()->getBank();
    cout << "\n============================================================================================================\n";
    cout << "\n                                                       :Bank Actions:                                       \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Issue Loan\n";
    cout << "2. Collect Debt\n";
    cout << "3. Audit Bank\n";
    cout << "4. View Bank Details\n";
    cout << "5. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 5);

    switch (choice) {
    case 1: { // Issue Loan
        int amount, termYears;
        string lender;
        cout << "Enter loan amount: ";
        amount = getValidatedInput(1, bank->getGoldReserves());
        cout << "Enter loan term in years: ";
        termYears = getValidatedInput(1, 30); // Example max term
        cout << "Enter lender name: ";
        cin.ignore();
        getline(cin, lender);

        Loan* loan = bank->issueLoan(amount, termYears, lender);
        if (loan) {
            cout << "Loan issued successfully.\n";
            logGameEvent("Loan issued to " + lender, kingdom);
        }
        else {
            cout << "Failed to issue loan.\n";
        }
        break;
    }
    case 2: // Collect Debt
        bank->collectDebt();
        logGameEvent("Bank collected debt", kingdom);
        break;
    case 3: { // Audit Bank
        int corruptionThreshold;
        cout << "Enter corruption threshold for audit: ";
        corruptionThreshold = getValidatedInput(0, 100);
        bank->audit(corruptionThreshold);
        logGameEvent("Bank audit conducted", kingdom);
        break;
    }
    case 4: // View Bank Details
        cout << "\nBank Details:\n";
        cout << "Gold Reserves: " << bank->getGoldReserves() << "\n";
        cout << "Active Loans: " << bank->getLoanCount() << "\n";
        break;
    case 5: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleResourceManagement(Kingdom& kingdom) {
    Economy* economy = kingdom.getEconomy();
    cout << "\n============================================================================================================\n";
    cout << "\n                                                  :Resource Management:                                     \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Produce Food\n";
    cout << "2. Harvest Wood\n";
    cout << "3. Mine Iron\n";
    cout << "4. View Resource Details\n";
    cout << "5. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 5);

    switch (choice) {
    case 1: { // Produce Food
        int efficiency;
        cout << "Enter production efficiency (0-100): ";
        efficiency = getValidatedInput(0, 100);
        economy->getFood()->produce(efficiency);
        logGameEvent("Food production increased", kingdom);
        break;
    }
    case 2: { // Harvest Wood
        int efficiency;
        cout << "Enter harvesting efficiency (0-100): ";
        efficiency = getValidatedInput(0, 100);
        economy->getWood()->produce(efficiency);
        logGameEvent("Wood harvesting increased", kingdom);
        break;
    }
    case 3: { // Mine Iron
        int efficiency;
        cout << "Enter mining efficiency (0-100): ";
        efficiency = getValidatedInput(0, 100);
        economy->getIron()->produce(efficiency);
        logGameEvent("Iron mining increased", kingdom);
        break;
    }
    case 4: // View Resource Details
        cout << "\n============================================================================================================\n";
        cout << "\n                                              :Resource Details:                                            \n";
        cout << "\n============================================================================================================\n";
        cout << "Food: " << economy->getFood()->getQuantity() << "\n";
        cout << "Wood: " << economy->getWood()->getQuantity() << "\n";
        cout << "Iron: " << economy->getIron()->getQuantity() << "\n";
        break;
    case 5: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleEventSystem(Kingdom& kingdom) {
    cout << "\n============================================================================================================\n";
    cout << "\n                                                      :Event System:                                        \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Trigger Random Event\n";
    cout << "2. Resolve Active Events\n";
    cout << "3. View Active Events\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1: // Trigger Random Event
        kingdom.triggerRandomEvent();
        logGameEvent("Random event triggered", kingdom);
        break;
    case 2: // Resolve Active Events
        kingdom.resolveActiveEvents();
        logGameEvent("Active events resolved", kingdom);
        break;
    case 3: // View Active Events
        kingdom.displayActiveEvents();
        break;
    case 4: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void logGameEvent(const string& event, const Kingdom& kingdom) {
    ofstream logFile("game_logs.txt", ios::app);
    if (logFile.is_open()) {
        logFile << "Year " << kingdom.getYear() << ": " << event << "\n";
        logFile.close();
    }
    else {
        cerr << "Failed to open log file.\n";
    }
}

void displayLogs() {
    ifstream logFile("game_logs.txt");
    if (logFile.is_open()) {
        cout << "\n=== Game Logs ===\n";
        string line;
        while (getline(logFile, line)) {
            cout << line << "\n";
        }
        logFile.close();
    }
    else {
        cerr << "No logs available.\n";
    }
}