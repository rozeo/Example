#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <math.h>
#define ull unsigned long long

template<typename T> std::string toString(T val);       //convert any to std::string
                     ull         ull_sqrt(ull);         //calc unsigned long long int Number sqrt.
                     int         tt      (int,int);     //
                     int         toInt   (std::string); //covert std::string to int

int main(){
	ull n;

	//Input
	std::cout << "Input (n) --> ";
	std::cin >> n;
	std::cout << std::endl;

	//calc sqrt
	ull sq_res = ull_sqrt(n);

	//write ','
	std::string n_str = toString(n);
	for(int i = n_str.length() - 1,j = 0;i >= 0;i--,j++){
		std::cout << n_str.at(j);
		if(i % 3 == 0 && i != 0) std::cout << ',';
	}

	std::cout <<"(digits : " << n_str.length() << ") is ";

	//judge
	for(ull i = 2;i < sq_res;i++){
		if(n % i == 0){
			std::cout << "not Prime.\ncan div " << i << "(" << n << " / " << i << " = " << n / i << ")."<< std::endl;
			return 1;
		}
	}
	std::cout << "Prime Number." << std::endl;
	return 0;
}

//use stringstream
template <typename T>
std::string toString(T val){
    std::stringstream stream;
    stream << val;
    return stream.str();
}

int toInt(std::string str){
	return std::atoi(str.c_str());
}

ull ull_sqrt(ull n){
	std::string n_s = toString(n);
	int cnt = 1;

	ull res = 0;

	std::string num;

	if(n_s.length() < 2) num += n_s.at(0);
	else                 num += n_s.at(0),num += n_s.at(1);

	ull ans = (int)sqrt(toInt(num));
	num = toString(toInt(num) - ans * ans);
	res += ans;
	ans *= 2;

	while(cnt * 2 < n_s.length()){
		num += n_s.at(cnt * 2);
		if(cnt * 2 + 1 < n_s.length())
			num += n_s.at(cnt * 2 + 1);

		ull s_ans = tt(ans,toInt(num));
		res = res * 10 + s_ans;

		num  = toString(toInt(num) - (s_ans * (ans * 10 + s_ans)));
		ans = ans * 10 + s_ans * 2;
		cnt++;
	}
	return res;
}

//calc i * (ans * 10 + i) smaller num
int tt(int n_ans,int t){
	for(int i = 1;i < 11;i++){
		if(i * (n_ans * 10 + i) > t)
			return i - 1;
	}
	return 0;
}