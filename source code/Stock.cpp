#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "Stock.h"

using namespace std;


Stock::Stock(int id, float price, int vol) : id(id), price(price), vol(vol){}

Stock::~Stock()
{

}

float Stock::getPrice() const 
{
    return price;
}

int Stock::getVol() const
{
    return vol;
}

int Stock::getID() const
{
    return id;
}

void Stock::setPrice(float newPrice) 
{
    price = newPrice;
}

void Stock::increaseVolume(int vol_in) 
{
    // std::cout << "(Stock::increaseVolume) increaseVolume: Old vol=" << vol << ", Adding=" << vol_in << "\n";
    vol += vol_in;
    // std::cout << "(Stock::increaseVolume) increaseVolume: New vol=" << vol << "\n";
}


