#include <iostream>
#include <string.h>

using namespace std;


class Student
{
public:

	int GetAge();
	void SetAge(int m);
        void ShowAge();
        void SetName(const char *in_name);

	friend void ShowName(Student & myStudent);
	friend class SuperStudent;
private:
	char name[32];
	int age;
void ShowInfo()
{
    cout<<"name:"<<name<<", age:"<<age<<endl;
}
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

class SuperStudent
{
private:
    Student student;
public:
    void OutPutInfo();
};

void SuperStudent::OutPutInfo()
{
    student.SetAge(20);
    student.ShowInfo();
}


int main()
{
	Student student;

	student.SetAge(15);
	student.SetName("zhangsan");
        cout<<"hello: age:"<<student.GetAge()<<endl;
	student.ShowAge();
	ShowName(student);

	SuperStudent s_student;
	s_student.OutPutInfo();

	return 0;
}
