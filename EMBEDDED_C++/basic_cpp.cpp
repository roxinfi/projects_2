#include<iostream>
#include<cmath>
#include<math.h>
#include<iomanip>
#include <cstdint>

class cat
{
    private:
        unsigned age;
        unsigned weight;
        unsigned height;
    public:
        void set_age(unsigned a);
        void set_weight(unsigned w);
        void set_height(unsigned h);
};

void cat::set_age(unsigned a)
{
    age = a;
}

void cat::set_weight(unsigned w)
{
    weight = w;
}

void cat::set_height(unsigned h)
{
    height = h;
}

class square
{
    private:
        unsigned int length;
        unsigned int area;
        unsigned int perimeter;
        float diagonal;
    public:
        square(unsigned int length);
        void cal_area();
        void cal_perimeter();
        void cal_diagonal();
        void change_length(unsigned int new_length);
        unsigned int get_area() { return area; }
        unsigned int get_perimeter() { return perimeter; }
        float get_diagonal() { return diagonal; } // 8 decimal places
};

void square::cal_area()
{
    area = length * length;
}

void square::cal_perimeter()
{
    perimeter = 4 * length;
}

void square::cal_diagonal()
{
    diagonal = length * sqrt(2);
}

void square::change_length(unsigned int new_length)
{
    length = new_length;
    cal_area();
    cal_perimeter();
    cal_diagonal();
}

square::square(unsigned int length)
{
    change_length(length);
}


square square_1(5);

int main()
{
    uint32_t x = 0xAABBCCDD;
    uint8_t *p8 = (uint8_t*)&x;
    uint16_t *p16 = (uint16_t*)&x;
    uint32_t *p32 = &x;
    std::cout << "Area: " << square_1.get_area() << std::endl;
    std::cout << "Perimeter: " << square_1.get_perimeter() << std::endl;
    std::cout << "Diagonal: " << std::fixed << std::setprecision(8) << square_1.get_diagonal() << std::endl;
    std::cout << "p8: " << std::hex << (unsigned int)(*p8) << std::endl;
    std::cout << "p16: " << std::hex << (unsigned int)(*p16) << std::endl;
    std::cout << "p32: " << std::hex << (unsigned int)(*p32) << std::endl;
    return 0;
}