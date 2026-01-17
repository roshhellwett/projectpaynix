cmake_minimum_required(VERSION 4.0)
project(PayNix)

set(CMAKE_CXX_STANDARD 26)

add_executable(PayNix main.cpp
        menu.cpp
        menu.h
        storeItems.cpp
        storeItems.h
        showItems.cpp
        showItems.h
        billing.cpp
        billing.h
        items.h
        updateItems.cpp
        updateItems.h
        deleteItems.cpp
        deleteItems.h
        DateTime.cpp
        DateTime.h
        viewCustomers.cpp
        viewCustomers.h
        phoneValidation.cpp
        phoneValidation.h
        resumeOrder.cpp
        resumeOrder.h
        ui.cpp
        ui.h
        credits.cpp
        credits.h
        employee.cpp
        employee.h
        admin.cpp
        admin.h
)
