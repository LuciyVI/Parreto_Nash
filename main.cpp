
#include <iostream>
#include <Eigen/Dense>
#include <random>
#include <boost/format.hpp>

#include </usr/include/termocolor/termcolor.hpp>


using Eigen::MatrixXd;


int getRandomNumber(int min, int max) {
    std::random_device rd; // Используем устройство случайных чисел в качестве источника энтропии
    std::mt19937 gen(rd()); // Используем генератор случайных чисел Mersenne Twister
    std::uniform_int_distribution<int> dis(min, max); // Создаем распределение для целых чисел в заданном диапазоне
    
    return dis(gen); // Возвращаем случайное число
}

int Nash(Eigen::MatrixXd first, Eigen::MatrixXd second )
{   

    std::cout<<"\t{   Nash    }"<<std::endl;
    Eigen::VectorXi maxIndices2(first.cols());
    Eigen::VectorXi maxIndices1(first.cols());
    for(int i=0;i<first.cols();i++){


        int maxVal1 = first.col(i).maxCoeff();
        
        int maxVal2 = second.row(i).maxCoeff();
        
        for(int j=0;j<second.cols();j++)
        { 
        
        if (first(j, i) == maxVal1 && second(j,i) == maxVal2) {
                
                std::cout<<"\nNash : i j =  "<<i<<"  "<<j<<"  "<<first(j,i)<<"   "<<second(j,i)<<std::endl;
            
                
            }
        
        }
            
    }
    
    return 0;
}

void Parreto(Eigen::MatrixXd first, Eigen::MatrixXd second){
 
    std::cout<<"\n \t{   Parreto   }\n"<<std::endl;

    
    for(int i = 0; i < first.cols() - 1; i++){
        
        int maxVal1 = first.col(i).maxCoeff(); 

        
        int maxVal2 = second.row(i).maxCoeff();

        
        for(int j = 0; j < second.cols() - 1; j++){
            
            if (first(j, i) == maxVal1 && second(i, j) == maxVal2) {
                std::cout<<"i : "<<i<<" j :"<<j<<" First: "<<first(i,j)<<"  Second:"<<second(i,j)<<"\n"<<std::endl; 
            }
        }   
    }
}


class Strategy {

    public:

        Eigen::MatrixXd M;

        Strategy(int N) : M(N,N) {

        
        
        for (int i = 0; i < M.rows(); ++i) {
            for (int j = 0; j < M.cols(); ++j) {
                M(i, j) = getRandomNumber(1,100); // Заполняем матрицу значениями
            }
        }
        
      
 
        }
    
    friend std::ostream& operator<<(std::ostream& os, const Strategy& obj) {
        os << obj.M;
        
        return os;
    }
};

void biMatrix_random(){
    
    Strategy  biMatrix1(2);
    Strategy  biMatrix2(2);

    std::cout<< "Игрок 1 :  \n" << biMatrix1 <<"\n"<<std::endl;

    std::cout<< "Игрок 2 :  \n" << biMatrix2 <<"\n"<<std::endl;

}
void biMatrix(){
    
    Eigen::MatrixXd One(2,2);
    Eigen::MatrixXd Two(2,2);

    One << 5,8,7,6;
    Two << 0,4,6,3;

    std::cout<<termcolor::red<<"\n-----{First static Matrix}----- \n"<<std::endl;
    std::cout<<One<<std::endl; 
    std::cout<<termcolor::blue<<"\n-----{Second static Matrix}-----\n"<<std::endl;
    std::cout<<Two<<std::endl;
    std::cout<<termcolor::white<<std::endl; 

    std::cout<<termcolor::red<<"\n-----{Nash for First Matrix}----- \n"<<termcolor::white<<std::endl;
    
    for(int i=0;i<2;i++)
    {  
        
        std::cout<<termcolor::red<<One.col(i).maxCoeff()<<termcolor::white<<std::endl;
        

    }
    std::cout<<termcolor::blue<<"\n-----{Nash for Second Matrix}-----\n"<<std::endl;
    
    for(int i=0;i<2;i++)
    {  
        
        std::cout<<termcolor::blue<<Two.row(i).maxCoeff()<<termcolor::white<<std::endl;
        

    }

    Eigen::Vector2i U(2);
    U<<1,1;
    
    Eigen::MatrixXd inverse_One = One.inverse();
    Eigen::MatrixXd inverse_Two = Two.inverse();
    
    double v1,v2,x,y=0;
    
    for(int i=0;i<inverse_One.cols();i++)
    { 
        for(int j=0;j<inverse_One.cols();j++){
    
        v1 += 1 / (U(i)*inverse_One(i,j)*U(i));
        y += v1*U(i)*inverse_One(i,j); 
        }
    }
    for(int i=0;i<inverse_Two.cols();i++)
    { 
        for(int j=0;j<inverse_Two.cols();j++){
        if(inverse_Two(i,j)!=0){  

            v2 += 1 / (U(i)*inverse_Two(i,j)*U(i));
            x += v2*U(i)*inverse_Two(i,j); 
        
        }else{
            continue;
        }
        }
    }

    std::cout<<"v1 : "<<v1<<std::endl;
    std::cout<<"v2 : "<<v2<<std::endl;
    std::cout<<"x : "<<x<<std::endl;
    std::cout<<"y : "<<y<<std::endl;
    


}

int main() {
   
    Strategy player1(10);
    Strategy player2(10);

    std::cout<< "Игрок 1 :  \n" << player1 <<"\n"<<std::endl;

    std::cout<< "Игрок 2 :  \n" << player2 <<"\n"<<std::endl;
    

    std::cout<<"\n(𝐴, 𝐵)\n"<<std::endl;

    for(int i=0 ;i<player1.M.cols();i++){
    

        for(int j = 0;j<player1.M.rows();j++){

    
                std::cout << boost::format("%-4d%-4d%-4d%-4d%-4d") %"|"%(player1.M(i,j))%"/"%(player2.M(i,j))%"|";

        }
        std::cout<<"\n"<<std::endl;
    }

    Nash(player1.M,player2.M);

    Parreto(player1.M,player2.M);
 
    biMatrix();    
    
    return 0;

}
