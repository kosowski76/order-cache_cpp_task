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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
