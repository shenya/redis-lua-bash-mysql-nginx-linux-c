#include <iostream>

using namespace std;


class Student
{
public:
	char name[32];
	int age;

	int GetAge();
	void SetAge(int m);
        void ShowAge();

};

int Student::GetAge()
{
    return age;
}

void Student::SetAge(int m)
{
    age = m;
}

void Student::ShowAge()
{
    cout<<"age:"<<this->age<<endl;
}

int main()
{
	Student student;

	student.SetAge(15);
        cout<<"hello: age:"<<student.GetAge()<<endl;
	student.ShowAge();

	return 0;
}
