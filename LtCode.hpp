/*Utilクラス*/
#ifndef LTCODE_HEAD
#define LTCODE_HEAD

class LtCode
{
  public:
    int degree;
    int flag;
    list<double> timestamp;
    list<int> edge;

    LtCode()
    { //引数なしコンストラクタ
        defineDegree();
        flag = 0;
        //timestamp = 0;
    }
    
    LtCode(int edge_size)
    { //degree代入・edgeサイズ決定用コンストラクタ
        edge.resize(edge_size);
        degree = edge_size;
    }

    void defineDegree()
    {
        double temp = randuni(mt);
        
        if (temp < 0.18)
        {
            degree = 1;
        }
        else if (temp < 0.52)
        {
            degree = 2;
        }
        else if (temp < 0.79)
        {
            degree = 4;
        }
        else if (temp < 0.91)
        {
            degree = 8;
        }
        else if (temp < 0.92)
        {
            degree = 16;
        }
        else
        {
            degree = 32;
        }        
    }

    void defineEdge(int *shf, int size){
    
        Utile::shuffle(shf, size);
        for (int i = 0; i < degree; i++)
        {
            edge.push_back(shf[i]);      
        }
    }

    ~LtCode() {} //デストラクタ
};

#endif
