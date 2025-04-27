#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <unordered_map> 
#include "Stock.h"

struct Node {
    Stock* stock;
    int height;
    Node* left;
    Node* right;
    Node(Stock* stock);
};

class Market
{
private:
    // --- Hash Table --- //
    std::unordered_map<int, Stock*> stockMap;
    Stock* stockSearch(int id);

    // --- Vector to store IDs --- //
    std::vector<int> priceRangeIDs;

    // AVL tree roots
//    Node* root_ID;
    Node* root_Vol;
    Node* root_Price;

    int height(Node* node);
    void updateHeight(Node* node);
    int balanceFactor(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* balance(Node* node);
    Node* minValueNode(Node* node);


    // --- Priced-based AVL Tree --- //
    Node* insertPrice(Node* node, Stock* stock);
    Node* deletePrice(Node* node, float price, int id);
    void collectPriceRange(Node* node, float lowerBound, float upperBound);
    
    // --- Volume-based AVL Tree --- //
    Node* insertVolume(Node* node, Stock* stock);
    Node* deleteVolume(Node* node, int volume, int id);

    // --- DEBUGGING --- //
    void printInorder(Node* node);
    Node* searchPrice(Node* node, float price, int id);
    
public:
    Market();
    void insertNewStock(int id, float price);
    void updatePrice(int id, float newPrice);
    void increaseVolume(int id, int v_inc);
    void priceRange(float lowerBound, float upperBound);
    void lookUpByID(int id);
    void maxVol();
    
    // --- DEBUGGING --- //
    void printPriceTree();
    void printVolumeTree();
};


#endif // AVL_H
