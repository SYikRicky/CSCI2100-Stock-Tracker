#include "Market.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>


Node::Node(Stock* stock):stock(stock), height(1), left(nullptr), right(nullptr){}

Market::Market():root_Vol(nullptr), root_Price(nullptr), stockMap({}){}

// --- Hash Table --- //
Stock* Market::stockSearch(int id)
{
    const auto& target = stockMap.find(id);
    if (target != stockMap.end())
    {
        return target->second;
    }
    else
    {
        return nullptr;
    }
}

void Market::lookUpByID(int id)
{
    const auto& target = stockMap.find(id);
    if (target != stockMap.end())
    {
        std::cout<< id <<"'s ";
        std::cout<< "Price: " << target->second->getPrice();
        std::cout<< " and Volume: " << target->second->getVol() << "\n";
    }
    else 
    {
        std::cout << "The stock does not exist.\n";
    }
}

// --- AVLTree's Function --- // 
int Market::height(Node* node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

void Market::updateHeight(Node* node)
{
    if (node != nullptr)
    {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }
}

int Market::balanceFactor(Node* node)
{
    if (node == nullptr) 
    {
        return 0;
    }

    return height(node->left) - height(node->right); // {-1,0,1}
}

Node* Market::rightRotate(Node* y)
{
    Node* x = y->left;
    Node* T2 = x->right;
    // Rotation
    x->right = y;
    y->left = T2;
    updateHeight(x);
    updateHeight(y);
    return x;
}

Node* Market::leftRotate(Node* x)
{
    Node* y = x->right;
    Node* T2 = y->left;
    // Roatation
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

Node* Market::balance(Node* node)
{
    updateHeight(node);
    int bf = balanceFactor(node);
    // left is heavier
    if (bf > 1)
    {
        if (balanceFactor(node->left) < 0) // check if left-right imbalance
        {
            node->left = leftRotate(node->left);
        }
        return rightRotate(node);
    }
    // right is heavier
    if (bf < -1)
    {
        if (balanceFactor(node->right) > 0) // check if right-left imbalance
        {
            node->right = rightRotate(node->right);
        }
        return leftRotate(node);
    }
    return node;
}

Node* Market::minValueNode(Node* node)
{
    Node* current = node;
    while (current->left != nullptr)
    {
        current = current->left;
    }
    return current;
}

/* node -> root of the subtree
   stock -> new stock to be added */

// --- Priced-based AVL Tree --- //
Node* Market::insertPrice(Node* node, Stock* stock)
{
    if (node == nullptr)
    {
        return new Node(stock);
    } 
    
    if (stock->getPrice() > node->stock->getPrice())
    {
        node->right = insertPrice(node->right, stock);
    }
    else if (stock->getPrice() < node->stock->getPrice())
    {
        node->left = insertPrice(node->left, stock);
    }
    else if (stock->getPrice() == node->stock->getPrice())
    {
        if (stock->getID() > node->stock->getID())
        {
            node->right = insertPrice(node->right, stock);
        }
        else if (stock->getID() < node->stock->getID())
        {
            node->left = insertPrice(node->left, stock);
        }
    }
    return balance(node);
}

Node* Market::deletePrice(Node* node, float price, int id)
{
    if (node == nullptr)
    {
//        std::cout << "[deletePrice] Node is nullptr for price=" << price << ", id=" << id << "\n";
        return nullptr;
    }

//    std::cout << "[deletePrice] Visiting node: ID=" << node->stock->getID()
//              << ", Price=" << node->stock->getPrice() << "\n";

    if (price > node->stock->getPrice())
    {
//        std::cout << "[deletePrice] Going right for price=" << price << ", id=" << id << "\n";
        node->right = deletePrice(node->right, price, id);
    }
    else if (price < node->stock->getPrice())
    {
//        std::cout << "[deletePrice] Going left for price=" << price << ", id=" << id << "\n";
        node->left = deletePrice(node->left, price, id);
    }
    else if (price == node->stock->getPrice())
    {
        if (id > node->stock->getID())
        {
//            std::cout << "[deletePrice] Same price, going right for id=" << id << "\n";
            node->right = deletePrice(node->right, price, id);
        }
        else if (id < node->stock->getID())
        {
//            std::cout << "[deletePrice] Same price, going left for id=" << id << "\n";
            node->left = deletePrice(node->left, price, id);
        }
        else
        {
//            std::cout << "[deletePrice] Found node to delete: ID=" << id << ", Price=" << price << "\n";
            // Case 1: Leaf node
            if (node->left == nullptr && node->right == nullptr)
            {
//                std::cout << "[deletePrice] Deleting leaf node\n";
                delete node;
                return nullptr;
            }
            // Case 2: Only have one child node
            else if (node->left == nullptr)
            {
//                std::cout << "[deletePrice] Deleting node with only right child\n";
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
//                std::cout << "[deletePrice] Deleting node with only left child\n";
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Case 3: Two child nodes
            else
            {
//                std::cout << "[deletePrice] Deleting node with two children\n";
                Node* successor = minValueNode(node->right);
//                std::cout << "[deletePrice] Successor: ID=" << successor->stock->getID()
//                          << ", Price=" << successor->stock->getPrice() << "\n";
                node->stock = successor->stock;
                node->right = deletePrice(node->right, successor->stock->getPrice(), successor->stock->getID());
            }
        }
    }
//    std::cout << "[deletePrice] Balancing node: ID=" << node->stock->getID() << "\n";
    return balance(node);
}

void Market::collectPriceRange(Node* node, float lowerBound, float upperBound)
{
    if (node == nullptr)
    {
        return;
    }
    
    float nodePrice = node->stock->getPrice();

    if (nodePrice <= upperBound)
    {
        collectPriceRange(node->right, lowerBound, upperBound);
    }

    if (nodePrice >= lowerBound)
    {
        collectPriceRange(node->left, lowerBound, upperBound);
    }

    if (nodePrice >= lowerBound && nodePrice <= upperBound)
    {
        priceRangeIDs.push_back(node->stock->getID());
    }
}

// --- Volume-based AVL Tree --- //
Node* Market::insertVolume(Node* node, Stock* stock)
{
    if (node == nullptr)
    {
        return new Node(stock);
    } 
    
    if (stock->getVol() > node->stock->getVol())
    {
        node->right = insertVolume(node->right, stock);
    }
    else if (stock->getVol() < node->stock->getVol())
    {
        node->left = insertVolume(node->left, stock);
    }
    else if (stock->getVol() == node->stock->getVol())
    {
        if (stock->getID() > node->stock->getID())
        {
            node->right = insertVolume(node->right, stock);
        }
        else if (stock->getID() < node->stock->getID())
        {
            node->left = insertVolume(node->left, stock);
        }
    }
    return balance(node);
}

Node* Market::deleteVolume(Node* node, int volume, int id)
{
    if (node == nullptr)
    {
        std::cout << "[deleteVolume] Node is nullptr for volume=" << volume << ", id=" << id << "\n";
        return nullptr;
    }

//    std::cout << "[deleteVolume] Visiting node: ID=" << node->stock->getID()
//              << ", Volume=" << node->stock->getVol() << "\n";

    if (volume > node->stock->getVol())
    {
//        std::cout << "[deleteVolume] Going right for volume=" << volume << ", id=" << id << "\n";
        node->right = deleteVolume(node->right, volume, id);
    }
    else if (volume < node->stock->getVol())
    {
//        std::cout << "[deleteVolume] Going left for volume=" << volume << ", id=" << id << "\n";
        node->left = deleteVolume(node->left, volume, id);
    }
    else if (volume == node->stock->getVol())
    {
        if (id > node->stock->getID())
        {
//            std::cout << "[deleteVolume] Same volume, going right for id=" << id << "\n";
            node->right = deleteVolume(node->right, volume, id);
        }
        else if (id < node->stock->getID())
        {
//            std::cout << "[deleteVolume] Same volume, going left for id=" << id << "\n";
            node->left = deleteVolume(node->left, volume, id);
        }
        else
        {
//            std::cout << "[deleteVolume] Found node to delete: ID=" << id << ", Volume=" << volume << "\n";
            // Case 1: Leaf node
            if (node->left == nullptr && node->right == nullptr)
            {
//                std::cout << "[deleteVolume] Deleting leaf node\n";
                delete node;
                return nullptr;
            }
            // Case 2: Only have one child node
            else if (node->left == nullptr)
            {
//                std::cout << "[deleteVolume] Deleting node with only right child\n";
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
//                std::cout << "[deleteVolume] Deleting node with only left child\n";
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Case 3: Two child nodes
            else
            {
//                std::cout << "[deleteVolume] Deleting node with two children\n";
                Node* successor = minValueNode(node->right);
//                std::cout << "[deleteVolume] Successor: ID=" << successor->stock->getID()
//                          << ", Volume=" << successor->stock->getVol() << "\n";
                node->stock = successor->stock;
                node->right = deleteVolume(node->right, successor->stock->getVol(), successor->stock->getID());
            }
        }
    }
//    std::cout << "[deleteVolume] Balancing node: ID=" << node->stock->getID() << "\n";
    return balance(node);
}



// --- The Tracker Function --- //
void Market::insertNewStock(int id, float price) 
{
    if (stockSearch(id) == nullptr)
    {
        Stock* stock = new Stock(id, price, 0);
        stockMap[id] = stock;
        root_Price = insertPrice(root_Price, stock);
        root_Vol = insertVolume(root_Vol, stock);
        std::cout << "New Stock: " << id << " with price " << price << " is added.\n";
    }
}

void Market::updatePrice(int id, float newPrice)
{
    Stock* stock = stockSearch(id);
    if (stock != nullptr)
    {
        float oldPrice = stock->getPrice();
        root_Price = deletePrice(root_Price, oldPrice, id);
        stock->setPrice(newPrice);
        root_Price = insertPrice(root_Price, stock);
        std::cout << "The price of " << id << " has changed from " << oldPrice << " to " << newPrice << "\n";
    }
    else
    {
        std::cout << "[updatePrice] Stock ID=" << id << " not found!\n";
    }
}

void Market::increaseVolume(int id, int v_inc)
{
    Stock* stock = stockSearch(id);
    if (stock != nullptr)
    {
        int oldVolume = stock->getVol();
        root_Vol = deleteVolume(root_Vol, oldVolume, id);
        stock->increaseVolume(v_inc);
        root_Vol = insertVolume(root_Vol, stock);
        std::cout << "The volume of " << id << " has changed from " << oldVolume << " to " << oldVolume+v_inc << "\n";
    }
    else
    {
        std::cout << "[increaseVolume] Stock ID=" << id << " not found!\n";
    }
}

void Market::priceRange(float lowerBound, float upperBound)
{
    priceRangeIDs.clear();

    collectPriceRange(root_Price, lowerBound, upperBound);

    if (priceRangeIDs.empty())
    {
        std::cout << "No stocks is foound in range (priceRange) \n";
    }
    else
    {
        std::cout << "Stocks in [" << lowerBound << ", " << upperBound << "]:\n";
        for (const auto& id : priceRangeIDs)
        {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
}

void Market::maxVol()
{
    Node* maxVol = root_Vol;
    while(maxVol->right != nullptr)
    {
        maxVol = maxVol->right;
    }
    std::cout << "Highest-volume stock's ID: " << maxVol->stock->getID() << "\n";
}


// --- DEBUGGING --- //
// void Market::printInorder(Node* node)
// {
//     if (node == nullptr)
//     {
//         return;
//     }
//     printInorder(node->left);
//     std::cout << node->stock->getID() << " " << node->stock->getPrice() << " " << node->stock->getVol() << ", ";
//     printInorder(node->right);
// }

// void Market::printPriceTree()
// {
//     std::cout << "PriceTree:\n";
//     printInorder(root_Price);
//     std::cout << "\n";
// }

// void Market::printVolumeTree()
// {
//     std::cout << "VolumeTree:\n";
//     printInorder(root_Vol);
//     std::cout << "\n";
// }

// Node* Market::searchPrice(Node* node, float price, int id)
// {
//     if (node == nullptr)
//         return nullptr;
//     if (price == node->stock->getPrice() && id == node->stock->getID())
//         return node;
//     if (price > node->stock->getPrice() || (price == node->stock->getPrice() && id > node->stock->getID()))
//         return searchPrice(node->right, price, id);
//     return searchPrice(node->left, price, id);
// }
