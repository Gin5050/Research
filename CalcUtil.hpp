/*Utilクラス*/
#ifndef CALCUTIL_HEAD
#define CALCUTIL_HEAD

class CalcUtile{
 public:

  CalcUtile(){}

  static double NodesDistance(int x1, int y1, int x2, int y2){
    return (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
  }

  static int MinNode(double x, double y, list<int> nodes, NODE *n_data){
    list<int> ::iterator it;
    double distance;
    int min = 10000000;
    int nodeId = EMPTY;

    /*他端末からの送信信号の受信電力を測定*/
    it = nodes.begin();
    while(it != nodes.end()){
      distance = NodesDistance(x, n_data[*it].x, y, n_data[*it].y);
      if(distance < min){
	min = distance;
	nodeId = *it;
      }
      it++;
    }
    return nodeId;
  }

  static complex<double> getConsteration(NODE *n_data, double sinr, int txId, double t_count){
    complex<double> x;  
    complex<double> m_temp;
    
    x = n_data[txId].jakes(t_count);
    
    //1シンボル遅延
    m_temp = complex<double>(1.0,0);
    m_temp = m_temp * x;
    m_temp += complex<double>(Channel::awgnQ(sinr), Channel::awgnI(sinr));
    return m_temp;
  }
  
  static int PPP(vector<double> &PPP_CDF, double randomValue){
    int i, j, k;
    int split = 0;
    double factorial = 0;
    double temp_term = 0;

    /*--------------------Poison分布生成---------------------------------*/
    if(exp(-Lambda) < DBL_MIN){ //Underflowの場合
      split = 2;
      while(0){
	if(exp(Lambda / split) > DBL_MIN)
	  break;
	else
	  split++;
      }
    }
  
    for(i = 0; i < TEMP_NUM; i++){
      temp_term = 1;      
      if(i != 0){
	for(j = 1; j <= i; j++){	
	  temp_term = (Lambda / (double)j) * temp_term * exp(-Lambda / (double)i);
	}
	PPP_CDF[i] = temp_term + PPP_CDF[i - 1];
      }
      else{
	for(k = 0; k < split; k++){
	  temp_term = temp_term * exp(-Lambda / (double)split);
	}
	PPP_CDF[i] = temp_term;
      }
    
      if(PPP_CDF[i] > 1)
	break;    
    }

    for(i = 0; PPP_CDF.size(); i++){
      if(randomValue < PPP_CDF[i])
	return i;
    }
    return EMPTY;
  }
  
  ~CalcUtile(){}
};

#endif
