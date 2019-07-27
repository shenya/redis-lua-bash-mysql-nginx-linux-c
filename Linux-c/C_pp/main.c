#include <iostream>
#include <string.h>

using namespace std;


class Student
{
public:
	char name[32];
	int age;

	int GetAge();
	void SetAge(int m);
        void ShowAge();
        void SetName(const char *in_name);

	friend void ShowName(Student & myStudent);
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
	Student student;

	student.SetAge(15);
	student.SetName("zhangsan");
        cout<<"hello: age:"<<student.GetAge()<<endl;
	student.ShowAge();
	ShowName(student);

	return 0;
}
