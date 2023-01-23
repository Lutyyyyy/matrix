#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>


namespace input_data 
{

void handle_input_error() {
    std::cout << "Incorrect input" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if ((std::cin >> std::ws).eof()) {
        throw std::runtime_error("Input: EOF reached");
    }
}

template<typename T> 
T get_val() {
    
    T value;

    std::cin >> value;

    if (std::cin.eof()) {
        throw std::runtime_error("Input: EOF reached");
    }

    while((!std::cin) || (!std::isspace(std::cin.peek()))) {
        handle_input_error();
        std::cin >> value;
    }

    return value;
}

template<typename T> 
std::pair<size_t, std::vector<T>> get_data() {

    if ((std::cin >> std::ws).eof()) {
        throw std::runtime_error("empty file");
    }

    size_t sz = get_val<size_t> ();
    
    while (sz <= 0) {
        std::cout << "size must be > 0" << std::endl;
        handle_input_error();
        std::cin >> sz;
    }

    std::vector<T> data (sz * sz);

    for (auto& i : data) {
        i = get_val<T>();
    }

    return {sz, data};
}

} // input_data