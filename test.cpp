#include "bigInt.h"
#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

class bigIntTest: public ::testing::Test {
	protected:
		bigInt b1,b2,b3;
		void ctorTest();
		void absTest();
		void addTest();
		void subtractTest();
		void multiTest();
		void compareTest();

};

void bigIntTest::ctorTest(){
	for(int i=0;i<5000;++i){	//test constructor with int
		int num=rand();
		b1=num;
		if(num>=0)
			EXPECT_TRUE(b1.getFlag())<<num<<endl;
		else
			EXPECT_FALSE(b1.getFlag())<<num<<endl;
	}
}
void bigIntTest::absTest(){
	for(int i=0;i<5000;++i){
		int num=rand();
		b1=num;
		b2=abs(num);
		EXPECT_EQ(0,b2.compare(b1.bigAbs()));
	}
}
void bigIntTest::addTest(){
	for(int i=0;i<5000;++i){
		int num1=rand();
		int num2=rand();
		b1=num1;
		b2=num2;
		b3=(num1+num2);
		EXPECT_TRUE(b3==(b1+b2))<<num1<<"\t"<<num2 <<endl;
		num1+=num2;
		b3=num1;
		b1+=b2;
		EXPECT_TRUE(b3==b1)<<num1<<endl;
	}
}
void bigIntTest::subtractTest(){
	for(int i=0;i<5000;++i){
		int num1=rand();
		int num2=rand();
		b1=num1;
		b2=num2;
		b3=(num1-num2);
		EXPECT_TRUE(b3==(b1-b2))<<num1<<"\t"<<num2 <<endl;
		num1-=num2;
		b3=num1;
		b1-=b2;
		EXPECT_TRUE(b3==b1)<<num1<<endl;
	}
}
void bigIntTest::multiTest(){
	for(int i=0;i<5000;++i){
		int num1=rand();
		int num2=rand();
		b1=num1;
		b2=num2;
		b3=num1*num2;
		EXPECT_TRUE(b3==(b1*b2))<<num1<<"\t"<<num2<<endl;
		num1*=num2;
		b3=num1;
		b1*=b2;
		EXPECT_TRUE(b3==b1)<<num1<<endl;
	}
}
void bigIntTest::compareTest(){
	for(int i=0;i<5000;++i){
		int num1=rand();
		int num2=rand();
		b1=num1;
		b2=num2;
		if(num1>num2){
			EXPECT_TRUE(b1!=b2);
			EXPECT_TRUE(b1>b2);
		}
		else if(num1<num2){
			EXPECT_TRUE(b1!=b2);
			EXPECT_TRUE(b1<b2);
		}
		else 
			EXPECT_TRUE(b1==b2);
	}
}

TEST_F(bigIntTest,setUP){
	ASSERT_EQ(BASE,b1.getBase());
	ASSERT_TRUE(b1.getFlag());
	for(int i=0;i<1000;++i){	//test operator '==' in bigInt first , cause we'll use it in later tests
		int num=rand();
		b1=num;
		b2=num;
		ASSERT_TRUE(b1==b2);
	}
}
TEST_F(bigIntTest,ctorTest){
	ctorTest();
}
TEST_F(bigIntTest,absTest){
	absTest();
}
TEST_F(bigIntTest,addTest){
	addTest();
}
TEST_F(bigIntTest,subtractTest){
	subtractTest();
}
TEST_F(bigIntTest,multiTest){
	multiTest();
}
TEST_F(bigIntTest,compareTest){
	compareTest();
}

int main(int argc,char **argv){
	::testing::InitGoogleTest(&argc,argv);
	RUN_ALL_TESTS();

	system("pause");
	return 0;
}