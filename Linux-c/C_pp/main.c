#include <iostream>
#include <string.h>

using namespace std;


class Student
{
public:
        Student(int Age);
	int GetAge();
	void SetAge(int m);
        void ShowAge();
        void SetName(const char *in_name);

	friend void ShowName(Student & myStudent);
	friend class SuperStudent;
	Student operator+(Student b)
	{
	    return Student(age + b.age);
	}
private:
	char name[32];
	int age;
void ShowInfo()
{
    cout<<"name:"<<name<<", age:"<<age<<endl;
}
};

Student::Student(int in_age)
{
    age = in_age;
}

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

void Student::SetName(const char *in_name)
{
    strcpy(name, in_name);
}

void ShowName(Student & myStudent)
{
    cout<<"name:"<<myStudent.name<<endl;
}



int main()
{
	Student a(200);
        Student b(10);

        Student c(0);
	c = a + b;
	cout<<"c-age:"<<endl;
	c.ShowAge();


	return 0;
}
