#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include <vector>
#include <iomanip>
#include<windows.h>
using namespace std;

class Bill{
    private:
        string itemName;
        int rate, quantity;

    public:
        Bill() : itemName(""), rate(0),quantity(0){}
        Bill(string name, int r, int q) : itemName(name), rate(r), quantity(q) {}

        string getName() const{ return itemName; }
        
        int getRate() const{ return rate; }

        int getQuantity() const{ return quantity;}

        void setQuantity(int q){ quantity = q; }
        int getAmount() const{
            return rate *  quantity;
        }
        string toString() const {
            stringstream ss;
            ss << itemName <<":"<<rate<<":"<<quantity;
            return ss.str();
        }
        static Bill fromString(const string& line){
            stringstream ss(line);
            string name;
            int r,q;
            getline(ss,name,':');
            ss >> r;
            ss.ignore();
            ss>>q;
            return Bill(name, r, q);
        }
};

class Inventory{
    private:
        vector<Bill> items;
        string filename;
    public:
        Inventory(const string& file): filename(file){
            loadFromFile();
        }

        void loadFromFile(){
            items.clear();
            ifstream in(filename);
            string line;
            while(getline(in, line)){
                if(!line.empty()){
                    items.push_back(Bill::fromString(line));
                }
            }
            in.close();
        }
        void saveToFile(){
            ofstream out(filename);
            for(const auto& item : items){
                out << item.toString() << endl;
            }
            out.close();
        }
        void addItem(){
            string name;
            int rate,quantity;
            cout<<"Enter item Name: ";
            cin>>name;

            cout<<"Enter item rate: ";
            cin>>rate;

            cout<<"\tEnter item quantity: ";
            cin>>quantity;

            items.emplace_back(name, rate, quantity);
            saveToFile();
            cout<<"Item added successfully!"<<endl;
            Sleep(2000);
        }

        bool findAndSellItem(const string& name, int sellQuantity, Bill& soldItem){
            for(auto& item: items){
                if(item.getName() == name){
                    if(item.getQuantity() >= sellQuantity){
                        soldItem = Bill(name, item.getRate(),sellQuantity);
                        item.setQuantity(item.getQuantity()-sellQuantity);
                        saveToFile();
                        return true;
                    }else{
                        cout<<"Not enough stock!"<<endl;
                        return false;
                    }
                    
                }
            }
            cout<<"Item not found!"<<endl;
            return false;
        }
        void displayInventory() const{
            cout<<"\nAvailable Stock:\n";
            cout<<left<<setw(15) <<"Item" <<setw(10)<<"Rate"<< setw(10)<<"Qty"<<endl;
            cout<<"--------------------------------------\n";
            for(const auto& item: items){
                cout<<left<<setw(15)<<item.getName()
                    <<setw(10)<<item.getRate()<<setw(10)<<item.getQuantity()<<endl;
            }
            cout<<"----------------------------------\n";
        }
};

class BillingSystem {
    private:
        Inventory inventory;
        vector<Bill> cart;
    
    public:
        BillingSystem(const string& stockFile) : inventory(stockFile) {}
    
        void addStock() {
            inventory.addItem();
        }
    
        void createBill() {
            string itemName;
            int quantity;
            bool done = false;
    
            while (!done) {
                system("cls");
                inventory.displayInventory();
    
                cout << "\n1. Add item to cart\n2. Finish and print bill\nEnter choice: ";
                int choice;
                cin >> choice;
    
                if (choice == 1) {
                    cout << "Enter item name: ";
                    cin >> itemName;
                    cout << "Enter quantity: ";
                    cin >> quantity;
    
                    Bill soldItem;
                    if (inventory.findAndSellItem(itemName, quantity, soldItem)) {
                        cart.push_back(soldItem);
                        cout << "Item added to cart.\n";
                    }
                    Sleep(1500);
                } else if (choice == 2) {
                    done = true;
                    printFinalBill();
                }
            }
        }

void printFinalBill() {
    system("cls");
    ofstream out("bill.txt");
    int total = 0;

    cout << "\nFinal Bill:\n";
    cout << left << setw(15) << "Item" << setw(10) << "Rate" << setw(10) << "Qty" << setw(10) << "Amount" << endl;
    cout << "------------------------------------------------\n";

    for (const auto& item : cart) {
        int amt = item.getAmount();
        cout << left << setw(15) << item.getName()
             << setw(10) << item.getRate()
             << setw(10) << item.getQuantity()
             << setw(10) << amt << endl;
        total += amt;
        out << item.toString() << " | Amount: " << amt << endl;
    }

    cout << "------------------------------------------------\n";
    cout << "Total Amount: Rs. " << total << endl;
    out << "Total: " << total << endl;
    out.close();

    cout << "\nThank you for shopping!\n";
    Sleep(5000);
}
};



int main() {
    BillingSystem billingSystem("items.txt");
    bool exit = false;

    while (!exit) {
        system("cls");
        cout << "\nSupermarket Billing System (OOP Version)\n";
        cout << "*****************************************\n";
        cout << "1. Add Item to Stock\n";
        cout << "2. Create Bill\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: billingSystem.addStock(); break;
            case 2: billingSystem.createBill(); break;
            case 3: exit = true; break;
            default: cout << "Invalid choice!\n"; Sleep(1000);
        }
    }
    return 0;
}