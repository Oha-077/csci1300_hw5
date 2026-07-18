#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>   

using namespace std;

class Item {
private:
    string name;
    int value;
    bool legendary;
    bool sellable;

public:
    Item() {
        name = "Empty";
        value = 0;
        legendary = false;
        sellable = true;
    }
    //Fish
    void setup(string n, int v, bool leg, bool canSell) {
        name = n;
        value = v;
        legendary = leg;
        sellable = canSell;
    }

    string getName() { return name; }
    int getValue() { return value; }
    bool isLegendary() { return legendary; }
    bool isSellable() { return sellable; }
};

// Legendary fish bundle calss
class Bundle {
private:
    string neededFish[5];
    bool donated[5];

public:
    Bundle() {
        neededFish[0] = "The Legend";       
        neededFish[1] = "The Crimsonfish";  
        neededFish[2] = "Angler";           
        neededFish[3] = "Glacierfish";      
        neededFish[4] = "Mutant Carp";      
        
        for (int i = 0; i < 5; i++) {
            donated[i] = false;
        }
    }

    void showBundle() {
        cout << endl << "<--- Community Center Bundle --->" << endl;
        for (int i = 0; i < 5; i++) {
            cout << neededFish[i] << ": ";
            if (donated[i]) {
                cout << "DONE" << endl;
            } else {
                cout << "MISSING" << endl;
            }
        }
    }

    bool isComplete() {
        for (int i = 0; i < 5; i++) {
            if (donated[i] == false) {
                return false;
            }
        }
        return true;
    }

    bool donateItem(string itemName) {
        for (int i = 0; i < 5; i++) {
            if (neededFish[i] == itemName && donated[i] == false) {
                donated[i] = true;
                return true;
            }
        }
        return false;
    }
};

//Player class with the sytems they need to manage
class Player {
private:
    int money;
    int hunger;
    int rodLevel; 
    Item inventory[50];
    int itemCount;
    string currentLocation;

public:
    Player() {
        money = 0;
        hunger = 10;
        rodLevel = 1;
        itemCount = 0;
        currentLocation = "River";
    }

    int getMoney() { return money; }
    void addMoney(int amount) { money = money + amount; }
    void spendMoney(int amount) { money = money - amount; }

    int getHunger() { return hunger; }
    void decreaseHunger() { hunger = hunger - 1; }
    
    int getRodLevel() { return rodLevel; }
    void upgradeRod(int level) { rodLevel = level; }

    string getCurrentLocation() { return currentLocation; }
    void setLocation(string loc) { currentLocation = loc; }

    void addItem(Item item) {
        if (itemCount < 50) {
            inventory[itemCount] = item;
            itemCount = itemCount + 1;
        } else {
            cout << "Inventory is full" << endl;
        }
    }
    //Show the status's of some inventory itmes and some extra possible options
    void showInventory() {
        cout << endl << "<--- Inventory --->" << endl;
        if (itemCount == 0) {
            cout << "Your inventory is empty" << endl;
        } else {
            for (int i = 0; i < itemCount; i++) {
                cout << i + 1 << ". " << inventory[i].getName();
                if (inventory[i].isLegendary()) {
                    cout << " (LEGENDARY)";
                }
                if (inventory[i].isSellable() == false) {
                    cout << " (CAN'T SELL";
                }
                cout << " - Value: " << inventory[i].getValue() << "g" << endl;
            }
        }
    }
    
    bool removeItem(string itemName, Item &removedItem) {
        for (int i = 0; i < itemCount; i++) {
            if (inventory[i].getName() == itemName) {
                removedItem = inventory[i]; 
                
                for (int a = i; a < itemCount - 1; a++) {
                    inventory[a] = inventory[a + 1];
                }
                itemCount = itemCount - 1;
                return true;
            }
        }
        return false;
    }
    
    void eatFish(string itemName) {
        Item food;
        bool found = removeItem(itemName, food);
        
        if (found) {
            if (food.getName() == "Algae") {
                hunger = hunger + 1;
                cout << "You choked down some slimy Algae. Hunger +1." << endl;
            } else {
                hunger = hunger + 3;
                cout << "You ate a " << food.getName() << "! Hunger +3." << endl;
            }
        } else {
            cout << "You don't have that item in your inventory!" << endl;
        }
    }
    
    void sellFish(string itemName) {
        Item fishToSell;
        bool found = removeItem(itemName, fishToSell);
        
        if (found) {
            if (fishToSell.isSellable()) {
                money = money + fishToSell.getValue();
                cout << "You sold the " << fishToSell.getName() << " for " << fishToSell.getValue() << "g!" << endl;
            } else {
                cout << "You cannot sell " << fishToSell.getName() << "!" << endl;
                addItem(fishToSell);
            }
        } else {
            cout << "You don't have that item in your inventory!" << endl;
        }
    }
};

// Game class
class Game {
private:
    Player player;
    Bundle bundle;
    int currentDay;
    int maxDays;
    bool isPlaying;
    
    string possibleRequests[5];
    string currentLinusRequest;

public:
    Game() {
        currentDay = 1;
        maxDays = 28; 
        isPlaying = true;
        srand(time(0)); 
        
        // Here are Linus's possible requests
        possibleRequests[0] = "Carp";
        possibleRequests[1] = "Sunfish";
        possibleRequests[2] = "Perch";
        possibleRequests[3] = "Anchovy";
        possibleRequests[4] = "Chub";
        // Using rand to make it be either 5
        currentLinusRequest = possibleRequests[rand() % 5];
    }
    // Stops the menu infinitly looping on itself
    void pauseGame() {
        cout << endl << "(Type any letter and press enter to continue) ";
        string pausepause;
        cin >> pausepause;
    }

    void displayMap() {
        cout << endl << "Map:" << endl;
        cout << "      <Mountain Lake>" << endl;
        cout << "            |" << endl;
        cout << "<Pond> -- <River> -- <Community Center> -- <Ocean>" << endl;
        cout << "            |" << endl;
        cout << "         <Sewers>" << endl;
        cout << endl << "You are currently at: " << player.getCurrentLocation() << endl;
    }

    string getRodName() {
        int r = player.getRodLevel();
        if (r == 1) return "Flimsy Rod";
        if (r == 2) return "Silver Rod";
        if (r == 3) return "Gold Rod";
        if (r == 4) return "Iridium Rod";
        if (r == 5) return "Joja Brand Rod";
        return "Unknown Rod";
    }

    void showStats() {
        cout << endl << "<==================================================>" << endl;
        cout << "Day: " << currentDay << " / " << maxDays << "    Hunger: " << player.getHunger();
        cout << "    Money: " << player.getMoney() << "g" << endl;
        cout << "Rod: " << getRodName() << endl;
        cout << "<==================================================>" << endl;
        displayMap();
    }
    //Move from place to place
    void moving() {
        cout << endl << "Where would you like to go?" << endl;
        cout << "1. Mountain Lake" << endl;
        cout << "2. Pond" << endl;
        cout << "3. River" << endl;
        cout << "4. Ocean" << endl;
        cout << "5. Sewers" << endl;
        cout << "6. Community Center" << endl;
        cout << "Choice (Enter Number): ";
        
        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            string garbage;
            getline(cin, garbage);
            cout << "Invalid choice. Please enter a number" << endl;
            pauseGame();
            return;
        }

        if (choice == 1) player.setLocation("Mountain Lake");
        else if (choice == 2) player.setLocation("Pond");
        else if (choice == 3) player.setLocation("River");
        else if (choice == 4) player.setLocation("Ocean");
        else if (choice == 5) player.setLocation("Sewers");
        else if (choice == 6) player.setLocation("Community Center");
        else {
            cout << "Invalid choice." << endl;
            pauseGame();
        }
    }
    //Hunger system preventing you from fishing when too hungry
    void startFishing() {
        if (player.getHunger() <= 0) {
            cout << "You are too hungry to fish! Eat something first." << endl;
            pauseGame();
            return;
        }

        if (player.getCurrentLocation() == "Community Center") {
            cout << "You can't fish inside the Community Center!" << endl;
            pauseGame();
            return;
        }

        player.decreaseHunger();
        //Chacnes for fish
        Item caughtFish;
        string loc = player.getCurrentLocation();
        int rod = player.getRodLevel();

        int legRoll = rand() % 100;
        int legChance = 1; 
        //Better rod = better fish chances
        if (rod == 2) legChance = 2;
        if (rod == 3) legChance = 5;
        if (rod == 4) legChance = 10;
        if (rod == 5) legChance = 75; 

        bool caughtLegendary = false;
        if (legRoll < legChance) {
            caughtLegendary = true;
        }
        //better rod = better normal fish 
        int fishRoll = rand() % 100;
        if (rod == 2) fishRoll = fishRoll + 15;
        if (rod == 3) fishRoll = fishRoll + 30;
        if (rod >= 4) fishRoll = fishRoll + 45;
        if (fishRoll > 99) fishRoll = 99; 

        if (loc == "Ocean") {
            if (caughtLegendary) {
                // Fish name, sell worth, is legendary, is sellable
                caughtFish.setup("The Crimsonfish", 150, true, true);
            } else {
                if (fishRoll < 30) caughtFish.setup("Anchovy", 5, false, true);
                else if (fishRoll < 55) caughtFish.setup("Herring", 8, false, true);
                else if (fishRoll < 75) caughtFish.setup("Sardine", 10, false, true);
                else if (fishRoll < 90) caughtFish.setup("Tuna", 20, false, true);
                else caughtFish.setup("Eel", 30, false, true);
            }
        } 
        else if (loc == "River") {
            if (caughtLegendary) {
                caughtFish.setup("Angler", 150, true, true);
            } else {
                if (fishRoll < 30) caughtFish.setup("Sunfish", 8, false, true);
                else if (fishRoll < 55) caughtFish.setup("Bream", 10, false, true);
                else if (fishRoll < 75) caughtFish.setup("Rainbow Trout", 15, false, true);
                else if (fishRoll < 90) caughtFish.setup("Walleye", 18, false, true);
                else caughtFish.setup("Catfish", 25, false, true);
            }
        } 
        else if (loc == "Mountain Lake") {
            if (caughtLegendary) {
                caughtFish.setup("The Legend", 200, true, true);
            } else {
                if (fishRoll < 30) caughtFish.setup("Carp", 5, false, true);
                else if (fishRoll < 55) caughtFish.setup("Chub", 10, false, true);
                else if (fishRoll < 75) caughtFish.setup("Bullhead", 12, false, true);
                else if (fishRoll < 90) caughtFish.setup("Largemouth Bass", 20, false, true);
                else caughtFish.setup("Sturgeon", 30, false, true);
            }
        } 
        else if (loc == "Pond") {
            if (caughtLegendary) {
                caughtFish.setup("Glacierfish", 150, true, true);
            } else {
                if (fishRoll < 30) caughtFish.setup("Perch", 10, false, true);
                else if (fishRoll < 60) caughtFish.setup("Smallmouth Bass", 12, false, true);
                else if (fishRoll < 85) caughtFish.setup("Pike", 18, false, true);
                else caughtFish.setup("Midnight Carp", 25, false, true);
            }
        } 
        else if (loc == "Sewers") {
            if (caughtLegendary) {
                caughtFish.setup("Mutant Carp", 150, true, true);
            } else {
                if (fishRoll < 50) caughtFish.setup("Algae", 0, false, false); 
                else caughtFish.setup("Carp", 5, false, true);
            }
        }
        //Fishing text
        cout << endl << "You cast your line" << endl;
        cout << "You caught a " << caughtFish.getName() << "!" << endl;
        player.addItem(caughtFish);
        
        pauseGame(); 
    }

    void startTalk() {
        string loc = player.getCurrentLocation();
        
        if (loc == "Ocean") {
            cout << endl << "Willy is here, tending to his shop by the sea." << endl;
            int currentRod = player.getRodLevel();
            
            if (currentRod == 1) {
                cout << "Willy: I can sell you a Silver Rod for 15g. It will catch better fish." << endl;
                cout << "1. Buy Silver Rod (15g)" << endl;
                cout << "2. No" << endl;
            } else if (currentRod == 2) {
                cout << "Willy: Want the Gold Rod? It's 30g." << endl;
                cout << "1. Buy Gold Rod (30g)" << endl;
                cout << "2. No" << endl;
            } else if (currentRod == 3) {
                cout << "Willy: 'I have an Iridium Rod for 50g!'" << endl;
                cout << "1. Buy Iridium Rod (50g)" << endl;
                cout << "2. No" << endl;
            } else {
                cout << "Willy: Your rod is already top tier... Go catch what I couldn't." << endl;
                pauseGame();
                return;
            }
            
            cout << "Choice (Enter Number): ";
            int c; 
            cin >> c;

            if (cin.fail()) {
                cin.clear();
                string garbage;
                getline(cin, garbage);
                cout << "Invalid choice." << endl;
                pauseGame();
                return;
            }

            if (c == 1) {
                if (currentRod == 1 && player.getMoney() >= 15) {
                    player.spendMoney(15);
                    player.upgradeRod(2);
                    cout << "You upgraded to the Silver Rod!" << endl;
                } else if (currentRod == 2 && player.getMoney() >= 30) {
                    player.spendMoney(30);
                    player.upgradeRod(3);
                    cout << "You upgraded to the Gold Rod!" << endl;
                } else if (currentRod == 3 && player.getMoney() >= 50) {
                    player.spendMoney(50);
                    player.upgradeRod(4);
                    cout << "You upgraded to the Iridium Rod!" << endl;
                } else {
                    cout << "Not enough money" << endl;
                }
            }
        } 
        //Joja ending dialouage
        else if (loc == "Sewers") {
            cout << endl << "A shady figure from JojaMart imerges from the shadows." << endl;
            cout << "Morris: Want the ultimate fishing rod? It's only 2g. It's only for free." << endl;
            cout << "1. Accept the Joja deal (0g)" << endl;
            cout << "2. Walk away" << endl;
            cout << "Choice (Enter Number): ";
            
            int c; 
            cin >> c;

            if (cin.fail()) {
                cin.clear();
                string bad;
                getline(cin, bad);
                cout << "Invalid choice." << endl;
                pauseGame();
                return;
            }

            if (c == 1) {
                if (player.getMoney() >= 0) {
                    player.spendMoney(0);
                    player.upgradeRod(5); 
                    cout << "You skim through and signed the contract presented before you and took the Joja Rod." << endl;
                } else {
                    cout << "Morris: I'm not sure how you have negative money, but congradulations on getting in debt I guess." << endl;
                }
            }
        }
        //Quest npc
        else if (loc == "Mountain Lake") {
            cout << endl << "Linus is outside his tent right down from the lake." << endl;
            cout << "Linus: I'm so hungry. I want a " << currentLinusRequest << ". Do you have one? I will pay you with very real money that I very totally have a lot of." << endl;
            cout << "1. Give " << currentLinusRequest << endl;
            cout << "2. I've got bigger fish to fry." << endl;
            cout << "Choice (Enter Number): ";
            
            int c; 
            cin >> c;

            if (cin.fail()) {
                cin.clear();
                string garbage;
                getline(cin, garbage);
                cout << "Invalid choice." << endl;
                pauseGame();
                return;
            }

            if (c == 1) {
                Item givenFish;
                if (player.removeItem(currentLinusRequest, givenFish)) {
                    int reward = givenFish.getValue() * 3;
                    player.addMoney(reward);
                    cout << "Linus: Thank you, kind friend. Here is " << reward << "g made out of real gold and are not fake gold." << endl;
                    
                    // Picks a new request for his next quest
                    currentLinusRequest = possibleRequests[rand() % 5];
                } else {
                    cout << "Linus: 'You don't have that fish in your inventory.'" << endl;
                }
            }
        }
        // bundle donation npc
        else if (loc == "Community Center") {
            cout << endl << "Mayor Lewis is standing in front of the community center bundle." << endl;
            cout << "Lewis: The last bundle... the one no one could complete. Are you here to finish the legendary fish bundle?" << endl;
            cout << "1. Yes, I'd like to donate" << endl;
            cout << "2. I just like walking into abandoned buildings" << endl;
            cout << "Choice (Enter Number): ";
            
            int c; 
            cin >> c;

            if (cin.fail()) {
                cin.clear();
                string garbage;
                getline(cin, garbage);
                cout << "Invalid choice" << endl;
                pauseGame();
                return;
            }

            if (c == 1) {
                communityCenter();
            }
        } 
        else {
            cout << endl << "There is no one here to talk to" << endl;
        }
        pauseGame();
    }
    //Iventory navigation
    void inventoryMenu() {
        bool inInventory = true;
        while (inInventory) {
            player.showInventory();
            cout << endl << "<--- Inventory Options --->" << endl;
            cout << "1. Eat a fish" << endl;
            cout << "2. Sell a fish" << endl;
            cout << "3. Exit Inventory" << endl;
            cout << "Choice (Enter Number): ";
            
            int choice;
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                string garbage;
                getline(cin, garbage);
                cout << "Invalid choice." << endl;
                pauseGame();
                continue;
            }

            if (choice == 1) {
                cout << "Enter the exact name of the fish you want to eat, or type 'Cancel': ";
                string eatName;
                cin >> ws;
                getline(cin, eatName);
                if (eatName != "Cancel") {
                    player.eatFish(eatName);
                    pauseGame();
                }
            } else if (choice == 2) {
                cout << "Enter the exact name of the fish you want to sell, or type 'Cancel': ";
                string sellName;
                cin >> ws;
                getline(cin, sellName);
                if (sellName != "Cancel") {
                    player.sellFish(sellName);
                    pauseGame();
                }
            } else if (choice == 3) {
                inInventory = false; 
            } else {
                cout << "Invalid choice." << endl;
                pauseGame();
            }
        }
    }
    //Donation menu
    void communityCenter() {
        bundle.showBundle();
        cout << endl << "Enter the exact name of the fish you want to donate, or type 'Leave': ";
        
        string attemptName;
        cin >> ws;
        getline(cin, attemptName);
        
        if (attemptName == "Leave") {
            return;
        }

        Item tempItem;
        if (player.removeItem(attemptName, tempItem)) {
            bool success = bundle.donateItem(attemptName);
            if (success) {
                cout << "Lewis: You actually did it? I must inform the whole valley!!!" << endl;
            } else {
                cout << "Lewis: Hrm, that fish isn't needed right now or was already donated." << endl;
                player.addItem(tempItem); 
            }
        } else {
            cout << "You don't have that fish in your inventory" << endl;
        }
    }
    //Calculating ending
    void evaluateEnding() {
        cout << endl << "<==================================================>" << endl;
        cout << "                  YEAR COMPLETED                       " << endl;
        cout << "<==================================================>" << endl;

        bool completedCenter = bundle.isComplete();
        bool rich = (player.getMoney() >= 500);
        bool joja = (player.getRodLevel() == 5); 

        if (joja) {
            cout << "ENDING 5/5: Worse ending" << endl;
            cout << "Due to your faliure to read the fine print on the contract you signed, JojaMart repossessed everything and every penny you caught with their rod." << endl;
            cout << "You are super broke and very homeless." << endl;
        } else if (completedCenter && rich) {
            cout << "ENDING 1/5: The best ending!" << endl;
            cout << "The valley loves you for what you did for their community. You also have amassed such a fortune that you bought a mansion for you and your fish to live in." << endl;
            cout << "Live could not be going better for you." << endl;
        } else if (completedCenter && !rich) {
            cout << "ENDING 2/5: Fame ending!" << endl;
            cout << "You restored the center, but you are completely broke. People are happy for what you did, but you still only live in a run-down shack and live off of the food you catch yourself." << endl;
        } else if (!completedCenter && rich) {
            cout << "ENDING 3/5: Money ending!" << endl;
            cout << "You didn't finish the bundle, but at least you are rich! People may not like you, but I'm sure that's something money can fix." << endl;
        } else {
            cout << "ENDING 4/5: Bad ending" << endl;
            cout << "You both failed the bundle and you are poor. A year of your life, and nothing to show for it." << endl;
        }
    }
    //starting dialouge and the base menu
    void startGame() {
        cout << "Welcome to Stardew Valley! A year is only " << maxDays << "." << endl;
        cout << "Catch fish, manage your hunger, upgrade your rod, and amass a fortune!" << endl;
        pauseGame();
        
        while (isPlaying) {
            showStats();
            
            cout << endl << "What would you like to do?" << endl;
            cout << "1. Move to another location" << endl;
            cout << "2. Fish here" << endl;
            cout << "3. Talk to someone here" << endl;
            cout << "4. Open Inventory Menu" << endl;
            cout << "5. End Day" << endl;
            cout << "Choice (Enter Number): ";
            
            int action;
            cin >> action;

            if (cin.fail()) {
                cin.clear(); 
                string bad;
                getline(cin, bad);
                cout << endl << "Please type the number (1-5), not the word" << endl;
                pauseGame();
                continue; 
            }

            if (action == 1) {
                moving();
            } else if (action == 2) {
                startFishing();
            } else if (action == 3) {
                startTalk();
            } else if (action == 4) {
                inventoryMenu();
            } else if (action == 5) {
                currentDay = currentDay + 1;
                cout << "You go to sleep. Day ended." << endl;
                pauseGame();
                if (currentDay > maxDays) {
                    isPlaying = false;
                }
            } else {
                cout << "Invalid choice" << endl;
                pauseGame();
            }
        }
        evaluateEnding();
    }
};

int main() {
    Game FishGameIsDone;
    FishGameIsDone.startGame();
    return 0;
}