# order-cache_cpp_task


 ## Technologies
  <ul>
    <li>C++ 17; g++</li>
    <li>Debian 12</li>
    <li>Tests: cmake, Google test</li>
    <li>Multi-Threaded tests</li>
  </ul>

 ## 01. Order cache

### 01.01. Task is to implement an in-memory cache of order objects


### 01.02. Provide an implementation for the OrderCacheInterface class in OrderCache.h

 - [x] Due to the simple definitions of the 'Order' and 'OrderCacheInterface' classes at this stage,   
    it is not necessary to separate the header files of these classes.   

### 01.03. There are more comments in OrderCache.h to provide additional guidance


### 01.04. Submit all files as email attachments or as .zip file

 - [x] the solution to the task is placed on the repo, for obvious reasons

### 01.05. I highly recommend you run various tests yourself for verification

 - [x] 01. cmake tests   

    git branch: 01-05-order-cache-tests   

    for Debian 12:   
    $ sudo apt update   
    $ sudo apt install -y g++ make cmake libcatch2-dev   

    OrderCache/   
    ├── include/   
    │   └── OrderCache.h   
    ├── src/   
    │   └── OrderCache.cpp   
    └── tests/   
        └── OrderCacheTest.cpp   

    $ g++ -std=c++17 -I./include -o OrderCacheTest tests/OrderCacheTest.cpp src/OrderCache.cpp   
    $ ./OrderCacheTest   

    output:   
        Retrieved orders:   
        Order ID: order1   
        Order ID: order2   
        Test Add Order: Passed   
        Test Cancel Order: Passed   
        Cancelling Order ID: order1 for user: user1   
        Cancelling Order ID: order2 for user: user1   
        Test Cancel Orders For User: Passed   
        Test Cancel Orders For SecId With Minimum Qty: Passed   
        Matching Size Calculated: 100   
        Test Get Matching Size For Security: Passed   
        All tests passed!   

 - [x] 02. google tests   

    for Debian 12:   
    $ sudo apt-get update   
    $ sudo apt-get install libgtest-dev   
    $ sudo apt-get install cmake   
    $ sudo apt-get install g++   

    $ cd /usr/src/gtest   
    $ sudo cmake .   
    $ sudo make   
    $ sudo cp *.a /usr/lib   

    $ mkdir build   
    $ cd build   
    $ cmake ..   
    $ make   
    $ ctest   

    output :   
            Start 1: OrderCacheTest   
        1/1 Test #1: OrderCacheTest ...................   Passed    0.00 sec   

        100% tests passed, 0 tests failed out of 1   

        Total Test time (real) =   0.01 sec   

    or: 

    $ ./OrderCacheTest   
    output :   
[==========] Running 5 tests from 1 test suite.   
[----------] Global test environment set-up.   
[----------] 5 tests from OrderCacheTest   
[ RUN      ] OrderCacheTest.AddOrder   
[       OK ] OrderCacheTest.AddOrder (0 ms)   
[ RUN      ] OrderCacheTest.CancelOrder   
[       OK ] OrderCacheTest.CancelOrder (0 ms)   
[ RUN      ] OrderCacheTest.CancelOrdersForUser   
Cancelling Order ID: order1 for user: user1   
Cancelling Order ID: order2 for user: user1   
[       OK ] OrderCacheTest.CancelOrdersForUser (0 ms)   
[ RUN      ] OrderCacheTest.CancelOrdersForSecIdWithMinimumQty   
[       OK ] OrderCacheTest.CancelOrdersForSecIdWithMinimumQty (0 ms)   
[ RUN      ] OrderCacheTest.GetMatchingSizeForSecurity   
[       OK ] OrderCacheTest.GetMatchingSizeForSecurity (0 ms)   
[----------] 5 tests from OrderCacheTest (0 ms total)   

[----------] Global test environment tear-down   
[==========] 5 tests from 1 test suite ran. (0 ms total)   
[  PASSED  ] 5 tests.   


### 01.06. Use up to C++17. Your code must compile. Code should be platform agnostic

 - [x] 100% tests passed, for Platform Agnostic Google Test Setup   

### 01.07. Adding thread safety


### 01.08. Order matching rules for getMatchingSizeForSecurity()


### 01.09. Order matching example and explanation


### 01.10. More Examples - Example 1


### 01.11. Example 2
