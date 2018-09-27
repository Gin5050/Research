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
        degree = 0;
        flag = 0;
        //timestamp = 0;
    }
    LtCode(int edge_size)
    { //degree代入・edgeサイズ決定用コンストラクタ
        edge.resize(edge_size);
        degree = edge_size;
    }

    int defineDegree()
    {
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

    ~LtCode() {} //デストラクタ
};

#endif
