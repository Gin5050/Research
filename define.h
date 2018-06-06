/*2018　4/15作成　*/
/*SensorパラメータRev 1.0 10/14Copyright © 2014 by Silicon LaboratoriesSi4468/7Si4468/7HIGH-PERFORMANCE, LOW-CURRENT TRANSCEIVER*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <complex>
#include <random>
#include <fstream>
#include <list>
#include <cfloat>

using namespace std;;

#define OBSERVE 1 //second
#define REPEATNUM 1			//繰り返し回数
#define TCOUNT 0.000001
#define PACKETSIZE 500				//パケットの構成ビット
#define OVERHEAD (PACKETSIZE * 0.5)
#define SHORTSIZE 250				//short packet size
#define BEACON_SIZE 82
#define RE_BEACON_SIZE 64
#define TEMP_NUM 2000
#define MEMORY 200
#define Beta 1
#define PoisonSIZE 2000
#define X_RANGE 1200 //range of x axis (m)
#define Y_RANGE 10 //range of y acis (m)
#define DENSITY 0.1 //1m^2あたりの平均の端末数（密度）
#define Lambda (X_RANGE * Y_RANGE * DENSITY)
#define SN 100					//Receiverスタート地点ノード番号
#define EN (X_RANGE - SN) 			//Receiver受信終了地点
#define WAVENUM 15
#define Sint (3.0)				//スリープ間隔（s）
#define PATHLOSS_num 2.5
#define V_km 36					//車速36km
#define V_m ((double)(V_km * 1000 / 3600))	//車両速度 m/s
#define POWER 1.0				//送信電力10dBm
#define CA_dBm -75				//キャリアセンス閾値 IEEE 802.15.4
#define CW 15					//コンテイションウィンド

#define BEACON_TIME 0.00144
#define RE_BEACON_TIME 0.00136

#define FALSE 0
#define TRUE 1
#define EMPTY -1
#define SLEEP 2
#define CSMA 3
#define TRANSMIT 4
#define RECEIVE 5
#define BEACON 6
#define SENDER 7
#define Re_ACK 8						//ack待ち
#define SENDER_ACK 9							//beaconに対するack
#define IDLE 10
#define Re_Be_ACK 13
#define Be_ACK 14
#define SLEEP_TEMP 18
#define COLLISION_MG 19	
#define TRANSSMITING 15
#define COLLISION 16
#define RECEIVING 17

#define ARRAY_LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

const double FREQUENCY = (920 * pow(10,6));				//利用周波数
const double DOPPLER = ((V_m * FREQUENCY)/ (299792458.0));		//ドップラー周波数
const double waveLength = 299792458 / (double)FREQUENCY;		//波長
const double LOG_WAVE = (20 * log10(waveLength /(4 * M_PI)));
const double POWER_dBm =  (10 * log10(POWER));
const double NOISE = (pow(10, (-110.0 / 10.0)));			//ノイズ電力
const double NOISEFLORE_dBm = 10 * log10(NOISE);
const double TIMESLOT = (20 * pow(10, -6));				//CSMA/CA
const double SIFS = (10 * pow(10, -6));					//CSMA/CA
const double DATARATE = (250 * pow(10,3));				//データレート250kbps

random_device rnd;     // 非決定的な乱数生成器を生成
mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
uniform_real_distribution<> randuni(0.0, 1.0);   // [0, 1] 範囲の一様乱数

class NODE;
class Calculator;
class Operater;
class SurportCalc;
class CalcUtile;
class Channel;
class MovingSink;

class Channel{
 public:

  static double pathLoss(double dis, double val){
    return (POWER_dBm + LOG_WAVE - 10 * val *log10(dis));
  }

  static double awgnQ(double sn){
    double nvar;
    double x,y;
    double noise;
  
    nvar= pow(10.0,((-1.0/10.0)*sn)) * 0.5; //分散の計算  
    x=0.0;
    while(x==0.0){
      x=randuni(mt);
    }
    y=randuni(mt);
    noise=sqrt(-2.0*nvar*log(x)) * cos(2.0*M_PI*y); //AWGN生成
   
    return noise;
  }

  // Box-Muller法に基づいて，各SNRに対するAWGNを作成する関数
  static double awgnI(double sn){
    double nvar;
    double x,y;
    double noise;
  
    nvar= pow(10.0,(-1.0/10.0)*sn) * 0.5; //分散の計算
    x=0.0;
    while(x==0.0){
      x=randuni(mt);
    }
    y=randuni(mt);
    noise=sqrt(-2.0*nvar*log(x)) * sin(2.0*M_PI*y); //AWGN生成
  
    return noise;
  }

  static complex<double> rayleigh(){
    double real1;
    double imag1;
    double nvar = 1.0 / 2.0;
    double x = 0;
    double y = randuni(mt);;
    double w = 0;
    double z = randuni(mt);

    while(x == 0.0) x = randuni(mt);
    real1 = sqrt(-2.0 * nvar * log(x)) * sin(2.0 * M_PI * y);
  
    while(w == 0.0) w = randuni(mt);
    imag1 = sqrt(-2.0 * nvar * log(w)) * cos(2.0 * M_PI * z);
    complex<double> fading(real1, imag1); //あとで確認
    
    return fading;
  }
  
};

class NODE{
 public:
  int id;
  int state;
  int mode;
  int next_mode;
  int next_state;
  int rec_time;
  int connect;
  int rec_cnt;
  int memory[MEMORY];
  double x;
  double y;
  double activetime;
  double sleep_time;
  double csma_time;
  double ca_time;
  double tx_time;
  double rx_time;
  double be_end;
  double re_be_end;
  double txEnd;
  double phase0[WAVENUM];
  double arrival_angle[WAVENUM];
  complex<double> fading;
  complex<double> m_prev;
  complex<double> chanel_num;
  complex<double> I;

  NODE(){
    id = -1;
    state = FALSE;
    next_state = FALSE;
    mode = SLEEP;
    next_mode = FALSE;
    rec_time = 0;
    connect = EMPTY;
    rec_cnt = 0;
    x = 0;
    y = 0;
    activetime = 0;
    sleep_time = 0;
    ca_time = 0;
    tx_time = 0;
    rx_time = 0;
    be_end = 0;
    re_be_end = 0;
    fading = complex<double> (0,0);
    m_prev = complex<double> (0,0);
    chanel_num = complex<double> (0,0);
    I = complex< double > (0,1);
  }
  ~NODE(){}

  void Initialaze(int num){
    int i;
    uniform_real_distribution<> randX(0, X_RANGE);
    uniform_real_distribution<> randY(0, Y_RANGE);
    uniform_real_distribution<> randSint(0, Sint);
    uniform_real_distribution<> PHASE(0, 2*M_PI);
  
    id = num;
    next_mode = SLEEP;
    x = randX(mt);
    y = randY(mt);
    activetime = randSint(mt);

    for(i = 0; i < WAVENUM; i++){
      phase0[i] = PHASE(mt);
      arrival_angle[i] = PHASE(mt) + 2 * M_PI * ((double)i / (double)WAVENUM);
    }
  }

  /*---------------------各モード処理-------------------------*/
  void sleepProcess(double t_count, double p_sleep){
    double temp;
    uniform_int_distribution<> randCW(0, CW-1);

    sleep_time+=TCOUNT;
    if(abs(t_count - activetime) < TCOUNT){    
      if(state == SLEEP_TEMP){
	next_mode = RECEIVE;
	return;
      }
      temp = randuni(mt);
      if(temp < p_sleep){
	next_mode = SLEEP;
	next_state = SLEEP;
	activetime = t_count + Sint;
	return;
      }      
      next_mode = CSMA;
      next_state = SLEEP;
      ca_time = SIFS + TIMESLOT * randCW(mt);	                      
    }
  }

  void beaconProcess(double t_count){
    if(abs(be_end - t_count) < TCOUNT){
      next_mode = Re_Be_ACK;
      state = Re_Be_ACK;
      re_be_end = t_count + BEACON_TIME + CW * TIMESLOT + SIFS;
    }
    tx_time+=TCOUNT;
  }

  void csmaCaProcess(double t_count, int flag){
    csma_time += TCOUNT; 
    if(flag == FALSE){
      return;
    }
    if(ca_time > 0){
      ca_time -= TCOUNT;
      return;
    }
    switch (state){
      case SLEEP:
	CsmaCaToBeacon(t_count);
	break;

      case Re_Be_ACK:
	CsmaCaToTransmit(t_count);
	break;
    }         
  }

  void txProcess(double t_count){
    if(abs(txEnd - t_count) < TCOUNT){
      next_mode = SLEEP;
      next_state = SLEEP;
      activetime = Sint + t_count;
    }
    tx_time+=TCOUNT;
  }

  void Re_Be_ACK_Process(double t_count, NODE *n_data, int minNode, double sinr){
    uniform_int_distribution<> randCW(0, CW-1);

    rx_time += TCOUNT;
    if(abs(re_be_end - t_count) < TCOUNT){
      next_mode = CSMA;
      next_state = Re_Be_ACK;
      ca_time = SIFS + TIMESLOT * randCW(mt);
    }
    /*受信処理*/
    if(minNode == EMPTY){
      return;
    }
    DBPSK(sinr, minNode);
    if(rec_cnt == PACKETSIZE){
      /*Senderからの信号受信処理*/
    }    
  }
  
  /*----------------------------------------------------------*/
  /*------------------Mode移行処理-----------------------------*/
  void CsmaCaToTransmit(double t_count){
    next_mode = TRANSMIT;
    next_state = TRANSMIT;
    txEnd = t_count + (double)(PACKETSIZE / DATARATE);
  }

  void CsmaCaToBeacon(double t_count){
    next_mode = BEACON;
    next_state = BEACON;
    be_end = t_count + BEACON_TIME; //Beacon送信時間は1ms   
  }
  /*----------------------------------------------------------*/
   
  void DBPSK(double sinr, int txId){
    int i;
    double p_d = 0;
    complex<double> x;  
    complex<double> m_temp;
  
    if(rec_time == 0){
      x = Channel::rayleigh();
      fading = x;
      connect = txId;
    
      //1シンボル遅延
      m_temp = complex<double>(1.0,0);
      m_temp = m_temp * x;
      m_temp += complex<double>(Channel::awgnQ(sinr), Channel::awgnI(sinr));
      m_prev = m_temp;
    }
    rec_time++;
  
    //DBPSK 1us分の受信
    for(i = 0; i < DATARATE * TCOUNT; i++){
      m_temp = complex<double>(1.0,0);
      m_temp = m_temp * fading;
      m_temp += complex<double>(Channel::awgnQ(sinr), Channel::awgnI(sinr));     
      p_d = abs(arg(m_prev) - arg(m_temp));
      m_prev = m_temp;
      if(cos(p_d) < 0){ //1bitでも誤った場合は終了
	break;
      }
    }
    rec_cnt += i;
  }
  void jakes(double t_count){
    int i;
    chanel_num = 0;

    for(i = 0; i < WAVENUM; i++){      
      chanel_num += exp(I * (phase0[i] + 2 * M_PI * DOPPLER * cos(arrival_angle[i]) * t_count));
    }   
    chanel_num = chanel_num / (sqrt(WAVENUM));
  }

};

class CalcUtile{
 public:

  CalcUtile(){}

  static double NodesDistance(int x1, int y1, int x2, int y2){
    return (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
  }

  static int MinNode(double x, double y, list<int> &nodes, NODE *n_data){
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
    }
    return nodeId;
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

class Calculator{
 public:
  list<int> Beacon_node;
  list<int> Trans_node;
  list<int> ACK_node;
  list<int> All_trans;

  Calculator(){     
  }

  int CarrierSense(NODE *n_data,  int id){
    list<int> ::iterator it;
    double distance = 0;
    double CSLevel;
    complex<double> channel_coeff;
      
    /*他端末からの送信信号の受信電力を測定*/
    it = All_trans.begin();
    while(it != All_trans.end()){
      if(id != *it){
	distance = CalcUtile::NodesDistance(n_data[id].x, n_data[id].y, n_data[*it].x, n_data[*it].y);
	channel_coeff = Channel::rayleigh();
	CSLevel = Channel::pathLoss(distance, PATHLOSS_num) + abs(channel_coeff) * abs(channel_coeff);
	if(CSLevel > CA_dBm){
	  break; //CAレベルを超えていれば
	}
      }
      ++it;
    }

    if(it != All_trans.end()){      
      return FALSE;
    }
    return TRUE;
  }

  int searchBeacon(double x, double y, NODE *n_data){
    int nodeId = CalcUtile::MinNode(x, y, Beacon_node, n_data);

a     if(nodeId == EMPTY){
      return EMPTY;
    }
    return nodeId;
  }

  int searchAck(double x, double y, NODE *n_data){
    int nodeId = CalcUtile::MinNode(x, y, ACK_node, n_data);

    if(nodeId == EMPTY){
      return EMPTY;
    }
    return nodeId;
  }

  int searchTx(double x, double y, NODE *n_data){
    int nodeId = CalcUtile::MinNode(x, y, Trans_node, n_data);

    if(nodeId == EMPTY){
      return EMPTY;
    }
    return nodeId;
  }

  double calcSinr(NODE *n_data, double x, double y, int minNode){
    list<int> ::iterator it = All_trans.begin();
    complex<double> chnlCoeff;
    double distance = 0;
    double loss = 0;
    double fading = 0;
    double interference = 0;    
    //int min = CalcUtile::MinNode(n_data, id, All_trans);
    
    while(it != All_trans.end()){
      if(*it != minNode){
	distance = CalcUtile::NodesDistance(x, y, n_data[*it].x, n_data[*it].y);
	loss = Channel::pathLoss(distance, PATHLOSS_num);
	chnlCoeff = Channel::rayleigh();
	fading = 10 * log10(abs(chnlCoeff) * abs(chnlCoeff));
	interference += (pow(10, (fading + loss) / 10.0));
      }
    }
    distance = CalcUtile::NodesDistance(x, y, n_data[minNode].x, n_data[minNode].y);
    loss = Channel::pathLoss(distance, PATHLOSS_num);
    chnlCoeff = Channel::rayleigh();;
    fading = 10 * log10(abs(chnlCoeff) * abs(chnlCoeff));

    return (loss + fading - (10 * log10(interference + NOISE)));
  }

  void addTrans(int id){
    Trans_node.push_back(id);
    All_trans.push_back(id);
  }

  void addBeacon(int id){
    Beacon_node.push_back(id);
    All_trans.push_back(id);
  }

  void addReBeAck(int id){
    ACK_node.push_back(id);
    All_trans.push_back(id);
  }

  list<int> getTransNodes(){
    return Trans_node;
  }

  list<int> getBeaconNodes(){
    return Beacon_node;
  }
};

class MovingSink{
 public:
  int recPackets;
  double x;
  double y;
  complex<double> fading;
  complex<double> m_prev;
  
  MovingSink(){
    x = 0;
    y = 0;
    recPackets = 0;
    fading = complex<double> (0,0);
    m_prev = complex<double> (0,0);  
  }

  void initialization(){
    x = 0;
    y = Y_RANGE / 2.0;    
  }

  void receiveProcess(Calculator calc, NODE *n_data){
    int minNode = EMPTY;
    double sinr = 0;
    double fadingDb = 0;
    
    if(calc.searchTx(x, y, n_data) == EMPTY){
      return;
    }
    minNode = CalcUtile::MinNode(x, y, calc.getTransNodes());
    sinr = calc.calcSinr(n_data, x, y, minNode);
    fadingDb = 10 * log10(abs(n_data[minNode].jakes) * abs(n_data[minNode].jakes));
  }

  void DBPSK(){

  }

   
  
  ~MovingSink(){}

};

//Nodeに関連した処理を行う．
class Operater{
 public:
  int N;
  int minNode;
  NODE *n_data; 
  //Calculator calc;
  double sinr;  
  double p_sleep;

  Operater(int nodeNum, double sleep){
    n_data = new NODE[nodeNum];  
    N = nodeNum;
    minNode = EMPTY;
    sinr = 0;
    p_sleep = sleep;
  }

  void initialaizeNodes(){
    for(int i = 0; i < N; i++){
      n_data[i].Initialaze(i);
    }
  }

  void processNodes(double t_count, Calculator calc){
    
    for(int i = 0; i < N; i++){
      switch (n_data[i].mode){
	case SLEEP:
	  n_data[i].sleepProcess(t_count, p_sleep);
	  break;

	case CSMA:
	  n_data[i].csmaCaProcess(t_count, calc.CarrierSense(n_data, i));	  
	  break;

	case BEACON:
	  n_data[i].beaconProcess(t_count);
	  break;

	case Re_Be_ACK:
	  minNode = calc.searchAck(n_data[i].x, n_data[i].y, n_data);
	  sinr = calc.calcSinr(n_data, n_data[i].x, n_data[i].y, minNode);
	  n_data[i].Re_Be_ACK_Process(t_count, n_data, minNode, sinr);
	  break;

	case TRANSMIT:
	  n_data[i].txProcess(t_count);
	  break;	  	  
      }
    }
  }

  void updateNodes(Calculator calc){
    for(int i = 0; i < N; i++){
      switch (n_data[i].next_mode){
	case BEACON:
	  calc.addBeacon(i);
	  break;
	  
	case TRANSMIT:
	  calc.addTrans(i);
	  break;

	case Re_Be_ACK:
	  calc.addReBeAck(i);
	  break;
      }
      n_data[i].mode = n_data[i].next_mode;
    }
  }

  void printNodesMode(double t_count){
    for(int i = 0; i < N; i++){
      if((n_data[i].mode != SLEEP) && (n_data[i].mode != Re_Be_ACK)){
	cout << t_count << "\t" << i << "\t" << n_data[i].mode << endl;
      }
    }
  }

  void printNodesActiveTime(){
    for(int i = 0; i < N; i++){      
	cout  << i << "\t" << n_data[i].activetime << endl;
    }
  }
  
  
  ~Operater(){};
};
