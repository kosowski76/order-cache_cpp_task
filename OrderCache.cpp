#pragma once

#include "OrderCache.h"
#include <algorithm>

// OrderCache method implementations
void OrderCache::addOrder(Order order) {
    orders[order.orderId()] = order;
    updateMappingsOnAdd(order);
}

void OrderCache::cancelOrder(const std::string& orderId) {
    auto orderIter = orders.find(orderId);
    if (orderIter != orders.end()) {
        updateMappingsOnCancel(orderIter->second);
        orders.erase(orderIter);
    }
}

void OrderCache::cancelOrdersForUser(const std::string& user) {
    auto userOrdersIter = userOrders.find(user);
    if (userOrdersIter != userOrders.end()) {
        for (const auto& orderId : userOrdersIter->second) {
            auto orderIter = orders.find(orderId);
            if (orderIter != orders.end()) {
                updateMappingsOnCancel(orderIter->second);
                orders.erase(orderIter);
            }
        }
        userOrders.erase(userOrdersIter);
    }
}

void OrderCache::cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) {
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
    unsigned int totalMatchedQty = 0;
    std::vector<Order> buyOrders, sellOrders;

    auto securityOrdersIter = securityOrders.find(securityId);
    if (securityOrdersIter != securityOrders.end()) {
        for (const auto& orderId : securityOrdersIter->second) {
            const auto& order = orders.at(orderId); // Using at() for safer access with exception handling
            if (order.side() == "Buy") {
                buyOrders.push_back(order);
            } else if (order.side() == "Sell") {
                sellOrders.push_back(order);
            }
        }

        // Sort buy and sell orders by order ID for consistent matching
        std::sort(buyOrders.begin(), buyOrders.end(), [](const Order& a, const Order& b) {
            return a.orderId() < b.orderId();
        });
        std::sort(sellOrders.begin(), sellOrders.end(), [](const Order& a, const Order& b) {
            return a.orderId() < b.orderId();
        });

        // Attempt to match orders
        for (auto& buyOrder : buyOrders) {
            for (auto& sellOrder : sellOrders) {
                if (buyOrder.company() != sellOrder.company()) {
                    unsigned int matchedQty = std::min(buyOrder.qty(), sellOrder.qty());
                    totalMatchedQty += matchedQty;

                    // Reduce the quantities by the matched amount
                    buyOrder = Order(buyOrder.orderId(), buyOrder.securityId(), buyOrder.side(), buyOrder.qty() - matchedQty, buyOrder.user(), buyOrder.company());
                    sellOrder = Order(sellOrder.orderId(), sellOrder.securityId(), sellOrder.side(), sellOrder.qty() - matchedQty, sellOrder.user(), sellOrder.company());

                    // Stop matching this buy order if fully allocated
                    if (buyOrder.qty() == 0) break;
                }
            }
        }
    }

    return totalMatchedQty;
}

std::vector<Order> OrderCache::getAllOrders() const {
    std::vector<Order> allOrders;
    for (const auto& [orderId, order] : orders) {
        allOrders.push_back(order);
    }
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
