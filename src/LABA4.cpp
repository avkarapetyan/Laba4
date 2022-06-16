#include "pricebase.h"


int main() {
	Order order;
	std::cout << order.empty() << std::endl<< std::endl;

	PriceBase& pb = PriceBase::Instance();
	std::string a = "Apple";
	std::string p = "Pencil";
	WeightProduct a_product1 = pb.get_product_weight_price(a);
	WeightPosition a_pos1(a_product1, 0.5);
	order.add_position(&a_pos1);
	AmountProduct p_product = pb.get_product_amount_price(p);
	AmountPosition p_pos(p_product, 2);
	order.add_position(&p_pos);
	order.get_info();
	std::cout << std::endl;

	WeightPosition a_pos2(a_product1, 1.5);
	order.add_position(&a_pos2);
	order.get_info();
	std::cout << std::endl;

	Client Ivan;
	Ivan.pay_and_receive_order(order);
	std::cout << std::endl;

	Ivan.earn_money(500);
	Ivan.pay_and_receive_order(order);
	std::cout << std::endl;

	return 0;
}
