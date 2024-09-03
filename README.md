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

 - [ ] 02. google tests
