#include <iostream>
using namespace std;
#include "bertinivector.cpp"

int main()
{
	//bertinivector<int> vec,vec3,vec4,vec5,vec6,ve8;
	bertinivector<double> vec,vec3,vec4,vec5,vec6,vec8;
	//bertinivector<char> vec,vec3,vec4,vec5,vec6,vec8;
	
	bertinivector<bertinivector<double> > vec7;//matrix
	
	//int s,sp,tmp;
	double s,sp,tmp;
	//char s,sp,tmp;
	int size;
	
	std::cout <<"Enter size of vector:";
	std::cin >> size;
	for(int k=0;k<size;k++)
	{
		std::cin>>tmp;
		vec.push_back(tmp);
	}
	
	std::cout <<"\nYou have entered "<< vec.size() << " elements. These are:\n";
	for (int i=0; i<vec.size();i++)
		std::cout<<vec[i]<< " ";
	std::cout << "\n";
	
	
	//bertinivector<int> vec2(vec);
	bertinivector<double> vec2(vec);
	vec+=99;
	//vec+='f';
	//bertinivector<char> vec2(vec);
	for (int i=0; i<vec.size();i++)
		std::cout<<vec[i]<< " ";
	std::cout << "\n";
	
	vec3=vec;
	vec3+=vec;
	vec4=9.9*vec3;
	vec5=vec4.dot_product(vec3);
	vec6=vec*2.0;

	for (int i=0; i<vec2.size();i++)
		std::cout<<vec2[i]<< " ";
	std::cout << "\n";
	for (int i=0; i<vec3.size();i++)
		std::cout<<vec3[i]<< " ";
	std::cout << "\n";
	for (int i=0; i<vec4.size();i++)
		std::cout<<vec4[i]<< " ";
	std::cout << "\n";
	for (int i=0; i<vec5.size();i++)
		std::cout<<vec5[i]<< " ";
	std::cout << "\n";
	for (int i=0; i<vec6.size();i++)
		std::cout<<vec6[i]<< " ";
	std::cout << "\n";
	vec6*=2.0;
	for (int i=0; i<vec6.size();i++)
		std::cout<<vec6[i]<< " ";
	std::cout << "\n";
	s=vec3.sumv();
	sp=vec6.dist(vec6,2);
	std::cout <<s<<"\n";
	std::cout <<sp<<"\n";
	bool ans=vec6.isnear(vec6,0.1,2);
	std::cout <<ans<<"\n";
	vec8=vec;
	vec8=2;
	for (int i=0; i<vec8.size();i++)
		std::cout<<vec8[i]<< " ";
	std::cout << "\n\n";
	
	
	std::cout << "here\n" << std::endl;
	
	/*vec7=outer_product(vec6,vec6);
	std::cout << vec7.size() << std::endl;
	for (int i=0; i<vec7.size();i++){
		for(int j=0; i<vec7[i].size();j++)
			std::cout<<vec7[i][j]<< " ";
	std::cout << "\n";}*/
	return 0;
}
