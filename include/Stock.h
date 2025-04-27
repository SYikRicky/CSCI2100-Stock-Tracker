#ifndef STOCKTRACKER_H
#define STOCKTRACKER_H

#include <string>
#include <vector>

class Stock
{
private:
    int vol;
    float price;
    int id;
    
public:
    Stock(int id, float price, int vol);
    ~Stock();

    // Getter method
    int getVol() const;
    int getID() const;
    float getPrice() const;
    
    // Set method
    void setPrice(float newPrice);
    void increaseVolume(int vol_in);
    

    // ---FOR DEBUGGING---
    void print_stock(int id);
};


#endif // !STOCKTRACKER_H
