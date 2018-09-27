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
    ~LtCode() {} //デストラクタ
};

#endif
