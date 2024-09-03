#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "OrderCacheInterface.cpp" // Make sure this header includes the OrderCacheInterface definition.

class OrderCache : public OrderCacheInterface {

    public:
        // Constructor and Destructor
        OrderCache() = default;
        ~OrderCache() = default;

        // Adds a new order to the cache.
        // Updates internal mappings for quick access based on user and security identifiers.
        void addOrder(Order order) override;

        // Cancels (removes) an order from the cache by its unique order ID.
        // Also updates internal mappings to keep data consistent.
        void cancelOrder(const std::string& orderId) override;

        // Cancels all orders associated with a given user.
        // This method iterates through all orders for the specified user and removes them from the cache.
        void cancelOrdersForUser(const std::string& user) override;

        // Cancels all orders associated with a specific security ID that have a quantity greater than or equal to the specified minimum.
        // This helps in bulk removal of orders that match the criteria.
        void cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) override;

        // Returns the total matched quantity for orders with the specified security ID.
        // Orders match if they have the same security ID, different sides (buy/sell), and belong to different companies.
        unsigned int getMatchingSizeForSecurity(const std::string& securityId) override;

        // Retrieves all orders currently in the cache.
        // Returns a vector containing copies of all Order objects stored.
        std::vector<Order> getAllOrders() const override;

    private:
        // Maps each order's unique ID to its Order object for quick retrieval and management.
        std::unordered_map<std::string, Order> orders;

        // Maps each user's name to a list of order IDs they have placed.
        // This allows for efficient access and management of orders by user.
        std::unordered_map<std::string, std::vector<std::string>> userOrders;

        // Maps each security ID to a list of order IDs associated with that security.
        // Facilitates fast lookups and operations on orders for a particular security.
        std::unordered_map<std::string, std::vector<std::string>> securityOrders;

        // Updates internal mappings (userOrders and securityOrders) when a new order is added.
        // Ensures that orders can be efficiently accessed by both user and security ID.
        void updateMappingsOnAdd(const Order& order);

        // Updates internal mappings when an order is removed.
        // Removes the order's associations from userOrders and securityOrders to keep the data consistent.
        void updateMappingsOnCancel(const Order& order);
};
