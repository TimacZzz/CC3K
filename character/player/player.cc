#include "player.h"
#include "../../util.h"
#include "../enemy/merchant.h"
#include "../../item/potion.h"
#include "../../item/item.h"
#include "../../item/gold.h"
#include "../../floor/floor.h"
#include <cmath>
#include <iostream>
using namespace std;

// Constructor
Player::Player(int maxHP, int curHP, int initATK, int curATK, int initDEF, int curDEF, int goldPossessed, string race, Tile tile)
	: Character{maxHP, curHP, initATK, curATK, initDEF, curDEF, goldPossessed, race, tile}, score{0}
{
}

void Player::attack(Character *enemy, string &action_message)
{
	Util util;

	// check if halfling (50% chance to miss)
	if (enemy->getRace() == "Halfling")
	{
		int result = util.getRandomNumber(0, 1);
		if (result == 0)
		{
			action_message += "PC has missed his attack due to Halfling's ability. ";
			return;
		}
	}

	// deals damage
	int damage = ceil((100.0 / (100.0 + enemy->getDef())) * getAtk());
	enemy->mutateHp(-1 * damage);

	// Action Message
	action_message += "PC deals " + to_string(damage) + " to " + std::string(1, static_cast<Enemy *>(enemy)->getSymbol()) + "(" + to_string(enemy->getHp()) + " HP). ";

	// check if player is vampire
	if (this->getRace() == "Vampire")
	{
		if (enemy->getRace() == "Dwarf")
		{
			mutateHp(-5);
			action_message += "PC didn't like that taste of Dwarf's blood and loses 5 HP. ";
		}
		else
		{
			this->mutateHp(5);
			action_message += "PC liked that taste of blood and gains 5 HP. ";
		}
	}

	// make merchant hostile on attack
	if (enemy->getRace() == "Merchant" && Merchant::is_hostile == false)
	{
		Merchant::is_hostile = true;
		action_message += "PC has just attacked a neutral Merchant. All Merchants are now hostile to PC. ";
	}
}

// Attach the potion to the player
void Player::attach(Potion *p)
{
	observers.emplace_back(p);
	p->triggerPermanentEffect(this);
}

// Detach every potion, called when exiting the level
void Player::detach()
{
	for (Potion *p : observers)
	{
		delete p;
		p = nullptr;
	}
	observers.clear();
}

void Player::takeTurn(Floor &curFloor, string &action_message, string command1, string command2)
{
	Util util;
	if (command1 == "u")
	{
		// player picks up potion
		Tile requested_tile = Tile::getTileInDirection(this->getTile(), command2);
		Potion *potion = curFloor.getPotion(requested_tile);

		if (potion == nullptr)
		{
			action_message += "PC consumed some dust. (There's no potion in that direction) ";
			return;
		}
		else
		{
			this->consumePotion(potion, action_message);
			potion->learnPotion(potion->getName());
			// remove potion
			curFloor.removePotion(requested_tile);
		}
	}
	else if (command1 == "a")
	{
		// player attacks
		Tile requested_tile = Tile::getTileInDirection(this->getTile(), command2);

		Enemy *enemy = curFloor.getEnemy(requested_tile);
		if (enemy == nullptr)
		{
			action_message += "PC punched the air really hard. (There's no enemy in that direction) ";
			return;
		}
		else
		{
			this->attack(enemy, action_message);
		}
	}
	else if (command1 == "s")
	{
		// store the potion in the direction into inventory
		Tile requested_tile = Tile::getTileInDirection(this->getTile(), command2);
		Potion *potion = curFloor.getPotion(requested_tile);

		if (potion == nullptr)
		{
			action_message += "PC picked up some dust. (There's no potion in that direction) ";
			return;
		}
		else
		{
			// instantiate a new potion  // TODO, add action messages
			this->storePotion(potion);

			if (potion->isLearned()){
				action_message += "PC picked up a " + potion->getName() + " potion and stored it in his inventory.";
			}
			else{
				action_message += "PC picked up an Unknown Potion and stored it in his inventory. ";
			}

			// remove potion from the floor
			curFloor.removePotion(requested_tile);
		}
	}
	else if (command1 == "d")
	{
		// drinks the potion in inventory
		int index = std::stoi(command2);
		action_message += "PC drank a " + inventory[index]->getName() + " Potion in his inventory. ";
		usePotionInInventory(index);
	}
	else if (command1 == "t")
	{
		// trade with merchant
		Tile requested_tile = Tile::getTileInDirection(this->getTile(), command2);
		Enemy *enemy = curFloor.getEnemy(requested_tile);
		if (enemy == nullptr || enemy->getRace() != "Merchant")
		{
			action_message += "There is no merchant in that direction. ";
			return;
		}
		else
		{
			if (Merchant::is_hostile)
			{
				action_message += "PC has attacked a Merchant before, Merchants do not trade with enemies! ";
				return;
			}
			else
			{
				Merchant *merchant = dynamic_cast<Merchant *>(enemy);
				action_message += "Here is the storage of the Merchant: \n";
				merchant->printStorage(action_message);
			}
		}
	}
	else
	{
		// player moves
		this->move(curFloor, action_message, command1);
	}
}

void Player::move(Floor &curFloor, string &action_message, string command)
{
	Util util;
	// player is moving
	Tile destination = Tile::getTileInDirection(this->getTile(), command);

	// check if destination is valid
	if (!destination.isInBounds(curFloor) || destination.isWall(curFloor) || destination.isPotion(curFloor) || destination.isEnemy(curFloor) || destination.getValue(curFloor) == ' ')
	{
		action_message += "PC can't go that way! ";
		return;
	}

	bool picked = false;

	// check if destination is gold
	Gold *gold = curFloor.getGold(destination);
	if (gold != nullptr)
	{
		if (gold->getCanPick())
		{
			action_message += "PC moves " + command + ". ";
			picked = true;
			this->pickGold(gold->getValue());
			action_message += "PC picked up " + gold->getName() + " and gains " + to_string(gold->getValue()) + " gold. ";

			// remove gold
			curFloor.removeGold(destination);
			delete gold;
			gold = nullptr;
		}
		else
		{
			action_message += "PC can't go that way! ";
			return;
		}
	}
	// perform movement
	currentTile.setValue(curFloor, prevchar);
	prevchar = destination.getValue(curFloor);
	currentTile = destination;
	currentTile.setValue(curFloor, '@');

	if (picked == false){
		action_message += "PC moves " + command + ". ";
	}
	

	// check potions around PC
	vector<Tile> tiles = currentTile.getNeighbors(curFloor);
	for (int i = 0; i < tiles.size(); i++)
	{
		Potion *potion = curFloor.getPotion(tiles[i]);
		if (potion != nullptr)
		{
			if (potion->getName() == "RH" && Potion::rh_learned)
			{
				action_message += "PC sees a RH potion on the ground. ";
			}
			else if (potion->getName() == "PH" && Potion::ph_learned)
			{
				action_message += "PC sees a PH potion on the ground. ";
			}
			else if (potion->getName() == "WA" && Potion::wa_learned)
			{
				action_message += "PC sees a WA potion on the ground. ";
			}
			else if (potion->getName() == "WD" && Potion::wd_learned)
			{
				action_message += "PC sees a WD potion on the ground. ";
			}
			else if (potion->getName() == "BA" && Potion::ba_learned)
			{
				action_message += "PC sees a BA potion on the ground. ";
			}
			else if (potion->getName() == "BD" && Potion::bd_learned)
			{
				action_message += "PC sees a BD potion on the ground. ";
			}
			else
			{
				action_message += "PC sees an unknown potion on the ground. ";
			}
		}
	}
}

// Player consumes the player
void Player::consumePotion(Potion *potion, string &action_message)
{
	attach(potion);
	if (potion->isLearned() == true)
	{
		action_message += "PC drank the " + potion->getName() + " potion. ";
	}
	else
	{
		action_message += "PC drank the " + potion->getName() + " potion. ";
	}
	potion->learnPotion(potion->getName());
}

// Player picks up the gold
void Player::pickGold(int value)
{
	goldPossessed += value;
	score += value;
}

// Player uses the potion at given position of his inventory
void Player::usePotionInInventory(int position)
{
	inventory[position]->learnPotion(inventory[position]->getName());
	attach(inventory[position]);
	inventory.erase(inventory.begin() + position);
}

// Player stores the potion into his inventory
void Player::storePotion(Potion *p)
{
	inventory.push_back(p);
}

// Display the Player's inventory to output stream
void Player::printInventory(string &action_message)
{
	if (inventory.size() == 0)
	{
		action_message += "PC has nothing in his inventory. ";
		return;
	}
	action_message += "PC's Inventory: \n";
	for (int i = 0; i < inventory.size(); i++){
		if (i == inventory.size() - 1){
			if (inventory[i]->isLearned()){
				action_message += "Index " + to_string(i) + ": " + inventory[i]->getName() + " Potion ";
			}
			else{
				action_message += "Index " + to_string(i) + ": " + "Unknown Potion ";
			}
			break;
		}
		if (inventory[i]->isLearned())
		{
			action_message += "Index " + to_string(i) + ": " + inventory[i]->getName() + " Potion \n";
		}
		else
		{
			action_message += "Index " + to_string(i) + ": " + "Unknown Potion \n";
		}
	}
}

// Player buys an item at given position from the merchant
void Player::buyItem(Merchant *merchant, int position, string &action_message)
{
	if (position >= merchant->storage.size() || position < 0)
	{
		action_message += "Merchant does not have a Potion at this index in his storage. Please enter a valid number for index! ";
		return;
	}
	int price = merchant->storage[position]->getPrice();
	if (goldPossessed >= price)
	{
		goldPossessed -= price;
		action_message += "PC purchases a " + merchant->storage[position]->getName() + " Potion from the Merchant. ";
		consumePotion(merchant->storage[position], action_message);
		merchant->sellItem(position);
	}
	else
	{
		action_message += "Please come back with more gold! ";
	}
}

// Return the score of the Player
int Player::getScore()
{
	return score;
}

// Return the previous char the Player has walked on
char Player::getPrevChar()
{
	return prevchar;
}

// Set the Player's field "prevchar"
void Player::setPrevChar(char c)
{
	prevchar = c;
}

int Player::getInventorySize()
{
	return inventory.size();
}

// Destructor
Player::~Player()
{
	for (Potion *p : inventory)
	{
		delete p;
		p = nullptr;
	}

	for (Potion *p : observers)
	{
		delete p;
		p = nullptr;
	}
}
