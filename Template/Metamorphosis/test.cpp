#include <iostream>
#include "Metamorphosis.h"
using namespace std;

class Root { 
	private:
		virtual int cal(int a, int b) {
			return a + b;
		}
		virtual void  priva() {
			cout << "Root priva";
		}
};


typedef void(*Fun)(void);
typedef int(*Fun_int)(int, int);

void main() {
	Metamorphosis<Root, Fun> derive;
	(*derive.CallPrivateOfBase(1))();	

	Metamorphosis<Root, Fun_int> derive2;
	printf("%d", (*derive2.CallPrivateOfBase(0))(123, 321));

	getchar();
}

