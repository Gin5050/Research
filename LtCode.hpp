/*Utilクラス*/
#ifndef CALCUTIL_HEAD
#define CALCUTIL_HEAD

class CalcUtile
{
  public:
    int degree;
    int flag;
    list<double> timestamp;
    list<int> edge;

    LT_CODES()
    { //引数なしコンストラクタ
        degree = 0;
        flag = 0;
        //timestamp = 0;
    }
    LT_CODES(int edge_size)
    { //degree代入・edgeサイズ決定用コンストラクタ
        edge.resize(edge_size);
        degree = edge_size;
    }
    ~LT_CODES() {} //デストラクタ
};

#endif
