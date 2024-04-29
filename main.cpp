
#include <iostream>
#include <Eigen/Dense>
#include <random>
#include <boost/format.hpp>


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

        Strategy() : M(10,10) {

        
        
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



int main() {
   
    Strategy player1;
    Strategy player2;

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
 
    return 0;

}