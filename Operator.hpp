//Nodeに関連した処理を行う．
#ifndef OPERATOR_HEAD
#define OPERATOR_HEAD

class Operater{
 public:
  int N;
  int minNode;
  NODE *n_data;
  MovingSink *car;
  double sinr;  
  double p_sleep;

  Operater(int nodeNum, double sleep){
    n_data = new NODE[nodeNum];  
    N = nodeNum;
    car = new MovingSink();     //受信車両オブジェクト生成
    minNode = EMPTY;
    sinr = 0;
    p_sleep = sleep;
  }

  void initialaizeNodes(){
    for(int i = 0; i < N; i++){
      n_data[i].Initialaze(i);
    }
  }

  void processNodes(double t_count, Calculator *calc){
    
    for(int i = 0; i < N; i++){
      switch (n_data[i].mode){
	case SLEEP:
	  n_data[i].sleepProcess(t_count, p_sleep);
	  break;

	case CSMA:
	  n_data[i].csmaCaProcess(t_count, calc->CarrierSense(n_data, i));	  
	  break;

	case BEACON:
	  n_data[i].beaconProcess(t_count);
	  break;

	case Re_Be_ACK:
	  minNode = calc->searchAck(n_data[i].x, n_data[i].y, n_data);
	  sinr = calc->calcSinr(n_data, n_data[i].x, n_data[i].y, minNode);
	  n_data[i].Re_Be_ACK_Process(t_count, n_data, minNode, sinr);
	  break;

	case TRANSMIT:
	  n_data[i].txProcess(t_count);
	  break;	  	  
      }
    }
  }

  void initialazeCar(){
    car->initialization();
  }
  
  void carReceiveProcess(Calculator *calc, double t_count){
    car->x += (t_count * V_m); 
    car->receiveProcess(calc, n_data, t_count);
  }

  void updateNodes(Calculator *calc){
    for(int i = 0; i < N; i++){
      switch (n_data[i].next_mode){
	case BEACON:
	  calc->addBeacon(i);
	  break;
	  
	case TRANSMIT:
	  calc->addTrans(i);
	  break;

	case Re_Be_ACK:
	  calc->addReBeAck(i);
	  break;
      }
      n_data[i].mode = n_data[i].next_mode;
    }
  }

  void printFunc(){
    Print::printRecAndTransPacket(n_data, car, N);
  }

  int getNodeNum(){
    return N;
  }
  
  ~Operater(){};
};

class Print{
 public:
  static void printNodesMode(NODE *n_data, int N, double t_count){
    for(int i = 0; i < N; i++){
      if((n_data[i].mode != SLEEP) && (n_data[i].mode != Re_Be_ACK)){
	cout << t_count << "\t" << i << "\t" << n_data[i].mode << endl;
      }
    }
  }

  static void printNodesActiveTime(NODE *n_data, int N){
    for(int i = 0; i < N; i++){      
      cout  << i << "\t" << n_data[i].activetime << endl;
    }
  }

  static  void printRecAndTransPacket(NODE *n_data, MovingSink *car, int N){
    int cnt = 0;
    cout << "RecPacket\t" << car->recPackets;
    for(int i = 0; i < N; i++ ){
      cnt += n_data[i].transCount;
    }
    cout << "TransPacket\t" << cnt << endl;
  }
};

#endif
