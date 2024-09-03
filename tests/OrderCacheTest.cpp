// tests/OrderCacheTest.cpp

#include "../include/OrderCache.h"
#include <gtest/gtest.h>

TEST(OrderCacheTest, AddOrder) {
    OrderCache cache;
    cache.addOrder(Order("order1", "sec1", "Buy", 100, "user1", "companyA"));
    cache.addOrder(Order("order2", "sec1", "Sell", 200, "user2", "companyB"));

    auto allOrders = cache.getAllOrders();
    ASSERT_EQ(allOrders.size(), 2);
    EXPECT_EQ(allOrders[0].orderId(), "order1");
    EXPECT_EQ(allOrders[1].orderId(), "order2");
}

TEST(OrderCacheTest, CancelOrder) {
    OrderCache cache;
    cache.addOrder(Order("order1", "sec1", "Buy", 100, "user1", "companyA"));

    cache.cancelOrder("order1");
    auto allOrders = cache.getAllOrders();
    EXPECT_TRUE(allOrders.empty());
}

TEST(OrderCacheTest, CancelOrdersForUser) {
    OrderCache cache;
    cache.addOrder(Order("order1", "sec1", "Buy", 100, "user1", "companyA"));
    cache.addOrder(Order("order2", "sec2", "Sell", 200, "user1", "companyA"));
    cache.addOrder(Order("order3", "sec1", "Buy", 150, "user2", "companyB"));

    cache.cancelOrdersForUser("user1");
    auto allOrders = cache.getAllOrders();
    ASSERT_EQ(allOrders.size(), 1);
    EXPECT_EQ(allOrders[0].orderId(), "order3");
}

TEST(OrderCacheTest, CancelOrdersForSecIdWithMinimumQty) {
    OrderCache cache;
    cache.addOrder(Order("order1", "sec1", "Buy", 100, "user1", "companyA"));
    cache.addOrder(Order("order2", "sec1", "Sell", 50, "user2", "companyB"));
    cache.addOrder(Order("order3", "sec1", "Buy", 150, "user3", "companyC"));

    cache.cancelOrdersForSecIdWithMinimumQty("sec1", 100);
    auto allOrders = cache.getAllOrders();
    ASSERT_EQ(allOrders.size(), 1);
    EXPECT_EQ(allOrders[0].orderId(), "order2");
}

TEST(OrderCacheTest, GetMatchingSizeForSecurity) {
    OrderCache cache;
    cache.addOrder(Order("order1", "sec1", "Buy", 100, "user1", "companyA")); // Buy 100
    cache.addOrder(Order("order2", "sec1", "Sell", 50, "user2", "companyB")); // Sell 50
    cache.addOrder(Order("order3", "sec1", "Sell", 60, "user3", "companyC")); // Sell 60

    unsigned int matchingSize = cache.getMatchingSizeForSecurity("sec1");
    EXPECT_EQ(matchingSize, 100);  // Expected 50 from order2 + 50 from order3, not exceeding buy quantity
}

TEST(OrderCacheTest, GetMatchingSizeForSecurity_WithNewRules) {
    OrderCache cache;
    
    // Example setup: One large Buy order and two smaller Sell orders
    cache.addOrder(Order("order1", "ABCD", "Buy", 10000, "user1", "companyA"));  // Buy 10,000
    cache.addOrder(Order("order2", "ABCD", "Sell", 2000, "user2", "companyB")); // Sell 2,000
    cache.addOrder(Order("order3", "ABCD", "Sell", 1000, "user3", "companyC")); // Sell 1,000

    // Calculate the matching size
    unsigned int matchingSize = cache.getMatchingSizeForSecurity("ABCD");

    // Expect a match of 3,000 (Buy 10,000 matched with two Sells: 2,000 + 1,000)
    EXPECT_EQ(matchingSize, 3000);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
