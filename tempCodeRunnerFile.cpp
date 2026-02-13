#include<iostream>

class point{
public:
int x,y;
point(int a, int b):x(a), y(b){
}
};

int main(){
    point p(2,3);
    std::cout<<"X = "<<a<<"  and  Y = "<<p.y;
    return 0;
}