#pragma once
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
class ProductInfo{
public:
    explicit ProductInfo(std::string info): m_info(std::move(info)){};
    std::string get_info() const noexcept {return m_info;}
private:
    std::string m_info;
};

class Product{
public:
    explicit Product(ProductInfo productInfo): m_product_info(std::move(productInfo)){};
    virtual std::string get_info() const noexcept = 0;
    virtual double get_cost() const noexcept = 0;
protected:
    ProductInfo m_product_info;
};

class WeightProduct: public Product{
public:
    WeightProduct(ProductInfo productInfo, double cost): m_cost_per_kg(cost),
                                                        Product(std::move(productInfo)){};
    std::string get_info() const noexcept override {
        std::stringstream ss;
        ss << m_product_info.get_info();
        ss << ": ";
        ss << m_cost_per_kg << " per kg";
        return ss.str();
    }
    double get_cost() const noexcept override{
        return m_cost_per_kg;
    }
private:
    double m_cost_per_kg;
};

class AmountProduct: public Product{
public:
    AmountProduct(ProductInfo productInfo, double cost): m_cost_per_one(cost),
                                                           Product(std::move(productInfo)){};
    std::string get_info() const noexcept override {
        std::stringstream ss;
        ss << m_product_info.get_info();
        ss << ": ";
        ss << m_cost_per_one << " per one";
        return ss.str();
    }
    double get_cost() const noexcept override{
        return m_cost_per_one;
    }
private:
    double m_cost_per_one;
};

class Position{
public:
    explicit Position(Product& product): m_ptr_product(&product){};
    Product* get_ptr_product() const noexcept {return m_ptr_product;}
    virtual double get_cost() const noexcept = 0;
    virtual double get_quantity() const noexcept = 0;
protected:
    Product* m_ptr_product;
};

class AmountPosition: public Position{
public:
    AmountPosition(AmountProduct& product, size_t amount): Position(product), m_amount(amount) {};
    double get_cost() const noexcept override {return m_amount * m_ptr_product->get_cost();}
    double get_quantity() const noexcept override {return m_amount;}
private:
    size_t m_amount;
};

class WeightPosition: public Position{
public:
    WeightPosition(WeightProduct& product, double weight): Position(product), m_weight(weight){};
    double get_cost() const noexcept override {return m_weight * m_ptr_product->get_cost();}
    double get_quantity() const noexcept override {return m_weight;}
private:
    double m_weight;
};

class Client;

class Order{
public:
    friend Client;
    void add_position(Position* ptr_pos){
        int i = 0;
        for(auto ptr_position: m_ptr_positions){
            if(ptr_position->get_ptr_product()->get_info() == ptr_pos->get_ptr_product()->get_info()){
                m_ptr_positions.at(i) = ptr_pos;
                return;
            }
            ++i;
        }
        m_ptr_positions.push_back(ptr_pos);
    }
    double get_cost() {
        double sum = 0;
        for(auto ptr_position: m_ptr_positions){
            sum += ptr_position->get_cost();
        }
        return sum;
    }
    void get_info() {
        for(auto ptr_position: m_ptr_positions){
            std::cout << ptr_position->get_ptr_product()->get_info()
            << "\n\tQuantity: " << ptr_position->get_quantity()
            << "\n\tCost: " <<ptr_position->get_cost() << std::endl;
        }
    }
    bool empty() {return m_ptr_positions.empty();}
private:
    std::vector<Position*> m_ptr_positions;
    void clear(){
        m_ptr_positions.clear();
    }
};

class Balance{
public:
    void add_money(double money) noexcept {m_money += money;}
    bool waste_money(double money) noexcept {
        if (m_money - money < 0){
            return false;
        }
        m_money -= money;
        return true;
    }
private:
    double m_money = 0;
};

class Client{
public:
    void earn_money(double money) noexcept {m_balance.add_money(money);}
    bool pay_and_receive_order(Order& order){
        if (m_balance.waste_money(order.get_cost())){
            order.clear();
            std::cout << "The order is received!" << std::endl;
            return true;
        }
        std::cout << "Not enough money to pay for order!" << std::endl;
        return false;
    };
private:
    Balance m_balance;
};


class PriceBase {
public:
    PriceBase(const PriceBase&) = delete;
    PriceBase& operator = (const PriceBase&) = delete;
    static PriceBase& Instance() {
        static PriceBase instance;
        return instance;
    }
    WeightProduct get_product_weight_price(std::string& Name){
        for(auto& [name, weight_product]: m_products_weight_price){
            if(name == Name){
                return weight_product;
            }
        }
        throw std::runtime_error("Wrong product name");
    }
    AmountProduct get_product_amount_price(std::string& Name){
        for(auto& [name, amount_product]: m_products_amount_price){
            if(name == Name){
                return amount_product;
            }
        }
        throw std::runtime_error("Wrong product name");
    }
protected:
    PriceBase() = default;
private:
    std::map<std::string, ProductInfo> m_products_info = {
            { "Apple", ProductInfo("Green sweet apple") },
            { "Nuts", ProductInfo("Macadamia nut") },
            { "Pencil", ProductInfo("Red pensile with rubber") },
            { "Chair", ProductInfo("Wooden chair with armrests") },
    };
    std::map<std::string, WeightProduct> m_products_weight_price = {
            {"Apple", WeightProduct(m_products_info.at("Apple"), 11.3)},
            {"Nuts", WeightProduct(m_products_info.at("Nuts"), 34.2)}
    };
    std::map<std::string, AmountProduct> m_products_amount_price = {
            { "Pencil", AmountProduct(m_products_info.at("Pencil"), 5.7) },
            { "Chair", AmountProduct(m_products_info.at("Chair"), 75) },
    };
};
