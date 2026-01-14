#include <iostream>
#include <cmath>

typedef int square_t;

class square
{
    private:
    square_t side;
    long area;
    int perimeter;
    float diagonal;

    public:
    square(int newside);
    void calculateArea();
    void calculatePerimeter();
    void calculateDiagonal();
    void changeSize(int newSize);
};


void square::calculateArea() // Calculate the area of the square
{
    area = side * side;
}

void square::calculatePerimeter() // Calculate the perimeter of the square
{
    perimeter = 4 * side;
}

void square::calculateDiagonal() // Calculate the diagonal of the square
{
    diagonal = side * (sqrt(2));
}

void square::changeSize(int newSize)
{
    side = newSize;
    calculateArea();
    calculatePerimeter();
    calculateDiagonal();
}

square::square(int newside)
{
    changeSize(newside);
}


int main()
{
    square sq1(5);
    return 0;
}
