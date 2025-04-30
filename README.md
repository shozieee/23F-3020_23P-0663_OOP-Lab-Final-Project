# 23F-3020_23P-0663_OOP-Lab-Final-Project
Strong hold game
:Overview: 
Stronghold is a medieval kingdom simulation engine that models the complex systems of a feudal society. The project demonstrates object-oriented programming principles with a focus on class hierarchies, resource management, and event-driven gameplay.

:Key Features: 
Core Systems Kingdom Management: Rule over a medieval kingdom with various social classe
Economic Simulation: Manage resources (food, wood, iron), taxes, and banking
Military System: Recruit and train different army units (infantry, cavalry)
Event System: Handle random events like famines, plagues, and invasions
Leadership Mechanics: Different leader types (Monarchs, Elected Leaders) with unique behaviors

Technical Implementation :
C++ coded
Object-oriented design with inheritance and polymorphism
Resource management with raw pointers (manual memory management)
File I/O for game state saving/loading
Input validation system
How It Works Game Flow Create or load a kingdom
Manage various aspects through menu options:
Advance years to simulate time
Interact with social classes
Control population growth and health
Command military forces
Make leadership decisions
Manage economy and resources
Handle bank loans and audits
Respond to random events
Save progress to continue later


Key Classes Kingdom:
 Main container class that holds all other systems
SocialClass: Base class for Peasants, Merchants, and Nobles
Population: Tracks demographics and health
ArmyUnit: Base for military forces (Infantry, Cavalry)
Leader: Base for different leadership types (Monarch, ElectedLeader)
Resource: Base for kingdom resources (Food, Wood, Iron)
Bank: Handles loans and gold reserves
Event: Base for random events (Famine, Plague, Invasion)

:OOP Concepts implimentation: 
Classes and Objects Encapsulation Inheritance Polymorphism(Method Overriding,Virtual Functions) Abstraction Composition & Aggregation Association Dynamic Memory Management Method Overloading

:Project Division: 
Resource.cpp,ArmyUnit.cpp,Bank.cpp,Event.cpp,Economy.cpp;main.cpp are mainly implimented by komal(23p-0663).

Kingdom.cpp,Loan.cpp,SocialClass.cpp,stronghold.h,Leader.cpp,Population.cpp are mainly implimented by shozab mehdi(23f-3020).

stronghold.h & main.cpp are implimented mutually.

 
 
 
 
