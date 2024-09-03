// OrderCacheTest.cpp
#include "../include/OrderCache.h"
#include <cassert>
#include <iostream>

void testAddOrder() {
    OrderCache cache;
    cache.addOrder(Order("order1", "sec1", "Buy", 100, "user1", "companyA"));
    cache.addOrder(Order("order2", "sec1", "Sell", 200, "user2", "companyB"));

    auto allOrders = cache.getAllOrders();

    // Debug output to see the contents
    std::cout << "Retrieved orders:\n";
    for (const auto& order : allOrders) {
        std::cout << "Order ID: " << order.orderId() << "\n";
    }

    assert(allOrders.size() == 2);
    assert(allOrders[0].orderId() == "order1");
    assert(allOrders[1].orderId() == "order2");

    std::cout << "Test Add Order: Passed\n";
}

void testCancelOrder() {
    OrderCache cache;
    Order order1("order1", "sec1", "Buy", 100, "user1", "companyA");
    cache.addOrder(order1);

    cache.cancelOrder("order1");
    auto allOrders = cache.getAllOrders();
    assert(allOrders.empty());

    std::cout << "Test Cancel Order: Passed\n";
}

void testCancelOrdersForUser() {
    OrderCache cache;
    Order order1("order1", "sec1", "Buy", 100, "user1", "companyA");
    Order order2("order2", "sec2", "Sell", 200, "user1", "companyA");
    Order order3("order3", "sec1", "Buy", 150, "user2", "companyB");
    cache.addOrder(order1);
    cache.addOrder(order2);
    cache.addOrder(order3);

    cache.cancelOrdersForUser("user1");
    auto allOrders = cache.getAllOrders();
    assert(allOrders.size() == 1);
    assert(allOrders[0].orderId() == "order3");

    std::cout << "Test Cancel Orders For User: Passed\n";
}

void testCancelOrdersForSecIdWithMinimumQty() {
    OrderCache cache;
    Order order1("order1", "sec1", "Buy", 100, "user1", "companyA");
    Order order2("order2", "sec1", "Sell", 50, "user2", "companyB");
    Order order3("order3", "sec1", "Buy", 150, "user3", "companyC");
    cache.addOrder(order1);
    cache.addOrder(order2);
    cache.addOrder(order3);

    cache.cancelOrdersForSecIdWithMinimumQty("sec1", 100);
    auto allOrders = cache.getAllOrders();
    assert(allOrders.size() == 1);
    assert(allOrders[0].orderId() == "order2");

    std::cout << "Test Cancel Orders For SecId With Minimum Qty: Passed\n";
}

void testGetMatchingSizeForSecurity() {
    OrderCache cache;
    cache.addOrder(Order("order1", "sec1", "Buy", 100, "user1", "companyA")); // Buy 100
    cache.addOrder(Order("order2", "sec1", "Sell", 50, "user2", "companyB")); // Sell 50
    cache.addOrder(Order("order3", "sec1", "Sell", 60, "user3", "companyC")); // Sell 60

    unsigned int matchingSize = cache.getMatchingSizeForSecurity("sec1");

    // Debug Output
    std::cout << "Matching Size Calculated: " << matchingSize << "\n";
    assert(matchingSize == 100);  // Expected 50 from order2 + 50 from order3, not exceeding buy quantity

    std::cout << "Test Get Matching Size For Security: Passed\n";
}

int main() {
    testAddOrder();
    testCancelOrder();
    testCancelOrdersForUser();
    testCancelOrdersForSecIdWithMinimumQty();
    testGetMatchingSizeForSecurity();

    std::cout << "All tests passed!\n";
    return 0;
}
