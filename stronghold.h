#pragma once
#pragma once
#ifndef STRONGHOLD_H
#define STRONGHOLD_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
using namespace std;

// Forward declarations
class SocialClass;
class Population;
class ArmyUnit;
class Leader;
class Resource;
class Loan;
class Event;
class Economy;
class Bank;

// Constants
const int MAX_SOCIAL_CLASSES = 10;
const int MAX_RESOURCES = 10;

// Kingdom class
class Kingdom {
private:
    string name;
    int stability; // Changed from float to int
    int year;
    Population* population; // Changed from unique_ptr to raw pointer
    SocialClass* socialClasses[MAX_SOCIAL_CLASSES];
    int socialClassCount;
    ArmyUnit* military; // Changed from unique_ptr to raw pointer
    Leader* leader; // Changed from unique_ptr to raw pointer
    Economy* economy; // Changed from unique_ptr to raw pointer

public:
    Kingdom(const string& kingdomName);
    ~Kingdom();

    void simulateTurn();
    void recalculateStability();
    void adjustStability(int modifier); // Changed parameter type from float to int
    void displayStatus() const;
    void saveGameState(const string& filename) const;
    void loadGameState(const string& filename);
    void triggerRandomEvent();
    void resolveActiveEvents();
    void displayActiveEvents();

    // Getters
    string getName() const { return name; }
    int getStability() const { return stability; } // Changed return type from float to int
    int getYear() const { return year; }
    Population* getPopulation() const { return population; }
    ArmyUnit* getMilitary() const { return military; }
    Leader* getLeader() const { return leader; }
    Economy* getEconomy() const { return economy; }
    SocialClass** getSocialClasses() { return socialClasses; }
    int getSocialClassCount() const { return socialClassCount; }
};

// Abstract SocialClass
class SocialClass {
protected:
    string className;
    int population;
    int happiness; // Changed from float to int
    int influence; // Changed from float to int

public:
    SocialClass(const string& name, int pop, int happy, int infl);
    virtual ~SocialClass() = default;

    virtual int calculateTaxContribution() const = 0; // Changed return type from float to int
    void updateHappiness(int modifier); // Changed parameter type from float to int
    virtual void demandChange() const;

    // Getters
    string getClassName() const { return className; }
    int getPopulation() const { return population; }
    int getHappiness() const { return happiness; } // Changed return type from float to int
    int getInfluence() const { return influence; } // Changed return type from float to int

    // Setters
    void setPopulation(int pop) { population = pop; }
};

// Derived Social Classes
class Peasant : public SocialClass {
private:
    int labor; // Changed from float to int
    int skill; // Changed from float to int

public:
    Peasant(int pop, int happy, int infl, int lab, int skl);
    int calculateTaxContribution() const override; // Changed return type from float to int
    void demandChange() const override;
};

class Merchant : public SocialClass {
private:
    int wealth; // Changed from float to int

public:
    Merchant(int pop, int happy, int infl, int wlth);
    int calculateTaxContribution() const override; // Changed return type from float to int
    void demandChange() const override;
};

class Noble : public SocialClass {
private:
    int land; // Changed from float to int

public:
    Noble(int pop, int happy, int infl, int lnd);
    int calculateTaxContribution() const override; // Changed return type from float to int
    void demandChange() const override;
};

// Population Dynamics
class Population {
private:
    int totalCount;
    int growthRate; // Changed from float to int
    int health; // Changed from float to int
    int employmentRate; // Changed from float to int

public:
    Population(int initialCount);

    void calculateGrowth(int foodAvailability, int housingQuality); // Changed parameter types from float to int
    void updateHealth(int sanitation, int medicalCare); // Changed parameter types from float to int
    int getRevoltRisk(int happinessAvg) const; // Changed return type and parameter type from float to int
    void migrate(int amount);

    // Getters
    int getTotalCount() const { return totalCount; }
    int getGrowthRate() const { return growthRate; } // Changed return type from float to int
    int getHealth() const { return health; } // Changed return type from float to int
    int getEmploymentRate() const { return employmentRate; } // Changed return type from float to int
};

// Military System
class ArmyUnit {
protected:
    int soldierCount;
    int morale; // Changed from float to int
    int trainingLevel; // Changed from float to int
    int equipmentQuality; // Changed from float to int

public:
    ArmyUnit(int soldiers, int mrle, int training, int equipment);
    virtual ~ArmyUnit() = default;

    virtual void recruit(int amount);
    virtual void train(int intensity); // Changed parameter type from float to int
    virtual int calculateStrength() const; // Changed return type from float to int
    void paySalaries(int amount); // Changed parameter type from float to int
    void updateMorale(int modifier); // Changed parameter type from float to int

    // Getters
    int getSoldierCount() const { return soldierCount; }
    int getMorale() const { return morale; } // Changed return type from float to int
    int getTrainingLevel() const { return trainingLevel; } // Changed return type from float to int
    int getEquipmentQuality() const { return equipmentQuality; } // Changed return type from float to int
};

// Derived Army Units
class Infantry : public ArmyUnit {
private:
    int armor;
    int weapons;

public:
    Infantry(int soldiers, int mrle, int training, int equipment, int armr, int wpns);
    int calculateStrength() const override; // Changed return type from float to int
    void train(int intensity) override; // Changed parameter type from float to int
};

class Cavalry : public ArmyUnit {
private:
    int horses;

public:
    Cavalry(int soldiers, int mrle, int training, int equipment, int hrs);
    int calculateStrength() const override; // Changed return type from float to int
    void recruit(int amount) override;
};

// Leadership Mechanics
class Leader {
protected:
    string name;
    int popularity; // Changed from float to int
    int corruptionLevel; // Changed from float to int
    string leadershipStyle;

public:
    Leader(const string& nm, int pop, int corrupt, const string& style);
    virtual ~Leader() = default;

    virtual void makeDecision(int option) = 0;
    virtual void addressCrisis();
    virtual bool holdElection() const;

    // Getters
    string getName() const { return name; }
    int getPopularity() const { return popularity; } // Changed return type from float to int
    int getCorruptionLevel() const { return corruptionLevel; } // Changed return type from float to int
    string getLeadershipStyle() const { return leadershipStyle; }
};

// Derived Leader Classes
class Monarch : public Leader {
private:
    string heir;
    int reignYears;

public:
    Monarch(const string& nm, int pop, int corrupt, const string& style,
        const string& hr, int years);
    void makeDecision(int option) override;
    bool holdElection() const override;
};

class ElectedLeader : public Leader {
private:
    int termLength;
    int currentTerm;

public:
    ElectedLeader(const string& nm, int pop, int corrupt, const string& style,
        int term, int current);
    void makeDecision(int option) override;
    bool holdElection() const override;
};

// Resource Management
class Resource {
protected:
    string name;
    int quantity; // Changed from float to int
    int productionRate; // Changed from float to int

public:
    Resource(const string& resName, int initialQuantity, int prodRate);
    virtual ~Resource() = default;

    virtual void produce(int efficiency); // Changed parameter type from float to int
    virtual bool consume(int amount); // Changed parameter type from float to int
    virtual int calculateDepletion() const; // Changed return type from float to int

    // Getters
    string getName() const { return name; }
    int getQuantity() const { return quantity; } // Changed return type from float to int
};

// Derived Resources
class Food : public Resource {
private:
    int farms;

public:
    Food(int initialQuantity, int farmCount);
    void produce(int efficiency) override; // Changed parameter type from float to int
    bool consume(int amount) override; // Changed parameter type from float to int
};

class Wood : public Resource {
private:
    int forests;

public:
    Wood(int initialQuantity, int forestCount);
    void produce(int efficiency) override; // Changed parameter type from float to int
};

class Iron : public Resource {
private:
    int mines;

public:
    Iron(int initialQuantity, int mineCount);
    void produce(int efficiency) override; // Changed parameter type from float to int
};


class Loan {
private:
    int amount;
    int interestRate;
    int dueDate;
    string lender;
    bool isPaid;

public:
    Loan(int amt, int rate, int due, const string& lndr);

    int calculateInterest() const;
    bool isOverdue(int currentYear) const;
    void makePayment(int payment);

    // Getters
    int getAmount() const { return amount; }
    int getInterestRate() const { return interestRate; }
    bool getIsPaid() const { return isPaid; }
    int getDueDate() const { return dueDate; } // Added this getter
    string getLender() const { return lender; } // Added this getter
};

// Bank System
class Bank {
private:
    int goldReserves; // Changed from float to int
    int loanInterestRate; // Changed from float to int
    int corruptionCases;
    Loan* activeLoans[MAX_RESOURCES];
    int loanCount;
    int year;

public:
    Bank(int initialReserves);

    Loan* issueLoan(int amount, int termYears, const string& lender); // Changed parameter types from float to int
    void collectDebt();
    void audit(int corruptionThreshold); // Changed parameter type from float to int
    bool detectFraud(int auditSeverity); // Changed parameter type from float to int

    // Getters
    int getGoldReserves() const { return goldReserves; } // Changed return type from float to int
    Loan** getActiveLoans() { return activeLoans; }
    int getLoanCount() const { return loanCount; }
};

// Event System
class Event {
protected:
    string name;
    int severity; // Changed from float to int
    int duration;
    int currentDuration;

public:
    Event(const string& evtName, int sev, int dur);
    virtual ~Event() = default;

    virtual void trigger(Kingdom& kingdom);
    virtual void resolve(Kingdom& kingdom);
    virtual bool isActive() const { return currentDuration < duration; }
    virtual int getImpact() const; // Changed return type from float to int

    // Getters
    string getName() const { return name; }
};

// Derived Events
class Famine : public Event {
private:
    int foodLossRate; // Changed from float to int

public:
    Famine(int severity);
    void trigger(Kingdom& kingdom) override;
    void resolve(Kingdom& kingdom) override;
    int getImpact() const override; // Added declaration
};

class Plague : public Event {
private:
    int mortalityRate; // Changed from float to int

public:
    Plague(int severity);
    void trigger(Kingdom& kingdom) override;
    void resolve(Kingdom& kingdom) override;
    int getImpact() const override; // Added declaration
};

class Invasion : public Event {
private:
    string enemy;
    int troopCount;

public:
    Invasion(int severity, const string& invadingEnemy);
    void trigger(Kingdom& kingdom) override;
    void resolve(Kingdom& kingdom) override;
    int getImpact() const override; // Added declaration
};

// Economy class
class Economy {
private:
    int treasury; // Changed from float to int
    int taxRate; // Changed from float to int
    int inflation; // Changed from float to int
    int gdp; // Changed from float to int
    int corruptionLevel; // Changed from float to int
    Bank* bank;

    // Resource management
    Food food;
    Wood wood;
    Iron iron;

public:
    Economy(int initialTreasury);

    void collectTaxes(SocialClass** socialClasses, int classCount);
    void calculateInflation();
    void adjustTaxRate(int newRate); // Changed parameter type from float to int
    void fundService(int amount); // Changed parameter type from float to int
    void processLoans();
    void addToTreasury(int amount); // Changed parameter type from float to int

    // Resource management methods
    Food* getFood() { return &food; }
    Wood* getWood() { return &wood; }
    Iron* getIron() { return &iron; }

    // Getters
    int getTreasury() const { return treasury; } // Changed return type from float to int
    int getTaxRate() const { return taxRate; } // Changed return type from float to int
    int getInflation() const { return inflation; } // Changed return type from float to int
    int getGDP() const { return gdp; } // Changed return type from float to int
    int getCorruptionLevel() const { return corruptionLevel; } // Changed return type from float to int
    Bank* getBank() const { return bank; }
};

#endif // STRONGHOLD_H