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


TEST(OrderCacheTest, GetMatchingSizeForSecurity_SecId1_NoMatchDueToSameCompany) {
    OrderCache cache;
    
    // Add orders for SecId1 with the same company, so they should not match
    cache.addOrder(Order("OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA"));
    cache.addOrder(Order("OrdId3", "SecId1", "Sell", 500, "User3", "CompanyA"));

    // Since both orders are from the same company, there should be no matches
    unsigned int matchingSize = cache.getMatchingSizeForSecurity("SecId1");

    // Expect no matching quantity
    EXPECT_EQ(matchingSize, 0) << "Expected no matches due to same company for SecId1.";
}

TEST(OrderCacheTest, GetMatchingSizeForSecurity_SecId2_MultipleMatches) {
    OrderCache cache;
    
    // Add orders for SecId2
    cache.addOrder(Order("OrdId2", "SecId2", "Sell", 3000, "User2", "CompanyB")); // Sell 3000
    cache.addOrder(Order("OrdId4", "SecId2", "Buy", 600, "User4", "CompanyC"));  // Buy 600
    cache.addOrder(Order("OrdId5", "SecId2", "Buy", 100, "User5", "CompanyB"));  // Buy 100
    cache.addOrder(Order("OrdId7", "SecId2", "Buy", 2000, "User7", "CompanyE")); // Buy 2000
    cache.addOrder(Order("OrdId8", "SecId2", "Sell", 5000, "User8", "CompanyE")); // Sell 5000

    // Calculate the total matched quantity for SecId2
    unsigned int matchingSize = cache.getMatchingSizeForSecurity("SecId2");

    // Expected matches:
    // - OrdId2 (Sell 3000) matches 600 with OrdId4 and 2000 with OrdId7: Total 2600
    // - OrdId8 (Sell 5000) matches 100 with OrdId5: Total 100
    // Total expected: 2600 + 100 = 2700
    EXPECT_EQ(matchingSize, 2700) << "Expected matching quantity of 2700 for SecId2.";
}

TEST(OrderCacheTest, GetMatchingSizeForSecurity_SecId3_NoSellOrders) {
    OrderCache cache;
    
    // Add a single Buy order for SecId3
    cache.addOrder(Order("OrdId6", "SecId3", "Buy", 1000, "User6", "CompanyD"));

    // No Sell orders available for SecId3, so there should be no matches
    unsigned int matchingSize = cache.getMatchingSizeForSecurity("SecId3");

    // Expect no matching quantity
    EXPECT_EQ(matchingSize, 0) << "Expected no matches due to lack of Sell orders for SecId3.";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
