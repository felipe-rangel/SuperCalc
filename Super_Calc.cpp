#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include <sys/resource.h>
using namespace std;
class BigInt{
protected:
  vector <int> v;
	int carry=0;
public:
	BigInt(){}
	BigInt(string s){
		for(int i=s.length()-1; i>=0;i--){
			v.push_back((int)s[i]-48);
		}
	}~BigInt(){
		v.clear();
		carry=0;
	}void addDigit(int n){
		if(n>9){
			v.push_back(n%10);
			carry=n/10;
		}else{
			v.push_back(n);
			carry=0;
		}
	}int size(){
		return v.size();
	}int & checkCarry(){
		return carry;
	}int & getN(int pos){
		return v[pos];
	}void decN(int pos){
		v[pos]--;
	}void changeNine(int pos){
		v[pos]=9;
	}void eraseZeros(){
		int c=0;
		bool zeros=false;
		for(int i=v.size()-1;i>=0;i--){
			if(v[i]==0){
				zeros=true;
				c++;
			}else{
				break;
			}
		}if(c==v.size()){
			c--;
		}while(c>0){
			v.pop_back();
			c--;
		}
	}bool isZero(){
		if(v.size()==1&&v[0]==0){
			return true;
		}
		return false;
	}void vanish(){
		v.clear();
		carry=0;	
	}void print(){
		for(int i=v.size()-1;i>=0;i--){
			cout<<v[i];
		}
		cout<<endl;
		cout<<"Number of Digits: "<<v.size()<<endl;
	}
};
BigInt bigAdd(BigInt A, BigInt B){
	BigInt C;
	if(A.size()>=B.size()){
		for(int i=0;i<B.size();i++){
			int sum=A.getN(i)+B.getN(i)+C.checkCarry();
			C.addDigit(sum);
		}
		if(A.size()>B.size()){
			for(int i=B.size();i<A.size();i++){
				C.addDigit(A.getN(i)+C.checkCarry());
			}
		}else{
			if(C.checkCarry()!=0){
				C.addDigit(C.checkCarry());
			}
		}
	}else{
		for(int i=0;i<A.size();i++){
			int sum=A.getN(i)+B.getN(i)+C.checkCarry();
			C.addDigit(sum);
		}
		for(int i=A.size();i<B.size();i++){
			C.addDigit(B.getN(i)+C.checkCarry());
		}
	}
	return C;
}
BigInt bigSub(BigInt A, BigInt B){
	BigInt C;
	if(A.size()<B.size()){
		cout<<"Error Result is Negative"<<endl;
		return C;
	}else{
		for(int i=0; i<B.size();i++){
			int diff=A.getN(i)-B.getN(i)+C.checkCarry();
			if(diff<0){
				if(i==A.size()-1){
					cout<<"Error Answer is Negative. Cannot Compute."<<endl;
					C.vanish();
					return C;
				}
				for(int j=i+1;j<A.size();j++){
					if(A.getN(j)>0){
						A.decN(j);
						j--;
						while(j>i){
							A.changeNine(j);
							j--;
						}
						diff+=10;
						break;
					}
				}
				C.addDigit(diff);
			}else{
				C.addDigit(diff);
			}
		}
		for(int i=B.size();i<A.size();i++){
			C.addDigit(A.getN(i)+C.checkCarry());
		}
	}
	C.eraseZeros();
	return C;
}
BigInt bigMulti(BigInt A, BigInt B){
	int total_size=A.size()+B.size();
	BigInt C;
	for(int i=0;i<total_size;i++){
		C.addDigit(0);
	}
	int posA=0,posB=0;
	for(int i=0;i<A.size();i++){
		C.checkCarry()=0;
		int currA=A.getN(i);
		posB=0;
		for(int j=0;j<B.size();j++){
			int currB=B.getN(j);
			int sum=currA*currB+C.getN(posA+posB)+C.checkCarry();
			C.checkCarry()=sum/10;
			C.getN(posA+posB)=sum%10;
			posB++;
		}
		if(C.checkCarry()>0){
			C.getN(posA+posB)+=C.checkCarry();
		}
		posA++;
	}
	C.eraseZeros();
	return C;
}
BigInt bigFactorial(BigInt num){
	BigInt fact("1");
	BigInt one("1");
	while(!num.isZero()){
		fact=bigMulti(fact,num);
		num=bigSub(num,one);
	}
	return fact;		
}
double get_cpu_time(){
        struct rusage ruse;
        getrusage(RUSAGE_SELF, &ruse);
        return ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec/1000000.0+
        ruse.ru_stime.tv_sec+ruse.ru_stime.tv_usec/1000000.0;
}
int main(){
	string n1,n2,opp;
	double startTime,endTime;
	cout<<"Welcome to the Super Calculator!"<<endl;
	cout<<"Enter + for addition, - for subtraction, * for multiplication, and ! for factorial"<<endl;
	cin>>opp;
	if(opp=="+"||opp=="-"||opp=="*"){
		cout<<"Input First Number"<<endl;
		cin>>n1;
		cout<<"Input Second Number"<<endl;
		cin>>n2;
		BigInt A(n1);
		A.eraseZeros();
		BigInt B(n2);
		B.eraseZeros();
		if(opp=="+"){
	        	startTime=get_cpu_time();
			BigInt C=bigAdd(A,B);
        		endTime=get_cpu_time();
			cout<<"Final Answer Below:"<<endl;
			C.print();
			cout<<"Total Time To Add: "<<endTime-startTime<<endl;	
		}else if(opp=="-"){
	        	startTime=get_cpu_time();
			BigInt C=bigSub(A,B);
        		endTime=get_cpu_time();
			cout<<"Final Answer Below:"<<endl;
			C.print();
			cout<<"Total Time To Subtract: "<<endTime-startTime<<endl;		
		}else if(opp=="*"){
	        	startTime=get_cpu_time();
			BigInt C=bigMulti(A,B);
        		endTime=get_cpu_time();
			cout<<"Final Answer Below:"<<endl;
			C.print();
			cout<<"Total Time To Multiply: "<<endTime-startTime<<endl;		
		}		
	}else if(opp=="!"){
		cout<<"Input Number"<<endl;
		cin>>n1;
		BigInt num(n1);
        	startTime=get_cpu_time();
		BigInt factorial=bigFactorial(num);
        	endTime=get_cpu_time();
		cout<<"Final Answer Below:"<<endl;
		factorial.print();
		cout<<"Total Time To Calculate Factorial: "<<endTime-startTime<<endl;
	}
}
