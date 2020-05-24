#include<iostream>

void func1();

using namespace std;
class date
{

public:

date(int year,int month,int day)
{
_year=year;
_month=month;
_day=day;
}

void print()
{
cout<<_year<<" "<<_month<<" "<<_day<<" "<<endl;
}

private:
int _year;
int _month;
int _day;

};
