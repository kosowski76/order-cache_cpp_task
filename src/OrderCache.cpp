#include "../include/OrderCache.h"
#include <algorithm>

void OrderCache::addOrder(Order order) {
    std::lock_guard<std::mutex> lock(cacheMutex);  // Lock mutex for thread safety
    orders[order.orderId()] = order;
    updateMappingsOnAdd(order);
}

void OrderCache::cancelOrder(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(cacheMutex);  // Lock mutex for thread safety
    auto orderIter = orders.find(orderId);
    if (orderIter != orders.end()) {
        updateMappingsOnCancel(orderIter->second);
        orders.erase(orderIter);
    }
}

void OrderCache::cancelOrdersForUser(const std::string& user) {
    std::lock_guard<std::mutex> lock(cacheMutex);  // Lock mutex for thread safety
    
    // Find the user orders in the map
    auto userOrdersIter = userOrders.find(user);
    if (userOrdersIter != userOrders.end()) {
        // Iterate over all order IDs for this user
        for (const auto& orderId : userOrdersIter->second) {
            // Find and remove the order from the orders map
            auto orderIter = orders.find(orderId);
            if (orderIter != orders.end()) {
                // Remove the order from the securityOrders map
                auto& secOrdersList = securityOrders[orderIter->second.securityId()];
                secOrdersList.erase(std::remove(secOrdersList.begin(), secOrdersList.end(), orderId), secOrdersList.end());
                if (secOrdersList.empty()) {
                    securityOrders.erase(orderIter->second.securityId());
                }
                // Remove the order from the orders map
                orders.erase(orderIter);
            }
        }
        // Finally, remove the user from the userOrders map
        userOrders.erase(userOrdersIter);
    }
}


void OrderCache::cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) {
    std::lock_guard<std::mutex> lock(cacheMutex);  // Lock mutex for thread safety
    auto securityOrdersIter = securityOrders.find(securityId);
    if (securityOrdersIter != securityOrders.end()) {
        for (const auto& orderId : securityOrdersIter->second) {
            auto orderIter = orders.find(orderId);
            if (orderIter != orders.end() && orderIter->second.qty() >= minQty) {
                updateMappingsOnCancel(orderIter->second);
                orders.erase(orderIter);
            }
        }
    }
}

unsigned int OrderCache::getMatchingSizeForSecurity(const std::string& securityId) {
    std::lock_guard<std::mutex> lock(cacheMutex);  // Lock mutex for thread safety
    unsigned int totalMatchedQty = 0;
    std::vector<Order*> buyOrders, sellOrders;

    auto securityOrdersIter = securityOrders.find(securityId);
    if (securityOrdersIter != securityOrders.end()) {
        for (const auto& orderId : securityOrdersIter->second) {
            auto& order = orders[orderId];
            if (order.side() == "Buy") {
                buyOrders.push_back(&order);
            } else if (order.side() == "Sell") {
                sellOrders.push_back(&order);
            }
        }

        std::sort(buyOrders.begin(), buyOrders.end(), [](const Order* a, const Order* b) {
            return a->orderId() < b->orderId();
        });
        std::sort(sellOrders.begin(), sellOrders.end(), [](const Order* a, const Order* b) {
            return a->orderId() < b->orderId();
        });

        for (auto& buyOrder : buyOrders) {
            for (auto& sellOrder : sellOrders) {
                if (buyOrder->company() != sellOrder->company()) {
                    unsigned int matchedQty = std::min(buyOrder->qty(), sellOrder->qty());
                    totalMatchedQty += matchedQty;

                    buyOrder->setQty(buyOrder->qty() - matchedQty);
                    sellOrder->setQty(sellOrder->qty() - matchedQty);

                    if (buyOrder->qty() == 0) break;
                }
            }
        }
    }

    return totalMatchedQty;
}

std::vector<Order> OrderCache::getAllOrders() const {
    std::lock_guard<std::mutex> lock(cacheMutex);  // Lock mutex for thread safety
    std::vector<Order> allOrders;
    for (const auto& [orderId, order] : orders) {
        allOrders.push_back(order);
    }
    std::sort(allOrders.begin(), allOrders.end(), [](const Order& a, const Order& b) {
        return a.orderId() < b.orderId();
    });
    return allOrders;
}

// Helper function to update mappings when an order is added
void OrderCache::updateMappingsOnAdd(const Order& order) {
    userOrders[order.user()].push_back(order.orderId());
    securityOrders[order.securityId()].push_back(order.orderId());
}

// Helper function to update mappings when an order is canceled
void OrderCache::updateMappingsOnCancel(const Order& order) {
    auto& userOrderIds = userOrders[order.user()];
    userOrderIds.erase(std::remove(userOrderIds.begin(), userOrderIds.end(), order.orderId()), userOrderIds.end());
    if (userOrderIds.empty()) {
        userOrders.erase(order.user());
    }

    auto& securityOrderIds = securityOrders[order.securityId()];
    securityOrderIds.erase(std::remove(securityOrderIds.begin(), securityOrderIds.end(), order.orderId()), securityOrderIds.end());
    if (securityOrderIds.empty()) {
        securityOrders.erase(order.securityId());
    }
}
