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

#define OBSERVE 100 //second
#define REPEATNUM 1			//繰り返し回数
#define TCOUNT 0.000001
#define PACKETSIZE 500				//パケットの構成ビット
#define SHORTSIZE 250				//short packet size
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
#define Sint (10.0)				//スリープ間隔（s）
#define PATHLOSS_num 2.5
#define V_km 36					//車速36km
#define V_m ((double)(V_km * 1000 / 3600))	//車両速度 m/s
#define POWER 1.0				//送信電力10dBm
#define CA_dBm -75				//キャリアセンス閾値 IEEE 802.15.4
#define CW 15					//コンテイションウィンド

#define BEACON_TIME 0.00144
#define RE_BEACOM_TIME 0.00136

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
#define ACK 9							//beaconに対するack
#define IDLE 10
#define Re_Be_ACK 13
#define Be_ACK 14
#define SLEEP_TEMP 18
#define COLLISION_MG 19	
#define TRANSSMITING 15
#define COLLISION 16
#define RECEIVING 17

#define ARRAY_LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

double FREQUENCY = (920 * pow(10,6));				//利用周波数
double DOPPLER = ((V_m * FREQUENCY)/ (299792458.0));		//ドップラー周波数
double waveLength = 299792458 / (double)FREQUENCY;		//波長
double LOG_WAVE = (20 * log10(waveLength /(4 * M_PI)));
double POWER_dBm =  (10 * log10(POWER));
double NOISE = (pow(10, (-110.0 / 10.0)));			//ノイズ電力
double NOISEFLORE_dBm = 10 * log10(NOISE);
double TIMESLOT = (20 * pow(10, -6));				//CSMA/CA
double SIFS = (10 * pow(10, -6));					//CSMA/CA

random_device rnd;     // 非決定的な乱数生成器を生成
mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
uniform_real_distribution<> randuni(0.0, 1.0);   // [0, 1] 範囲の一様乱数

class NODE{
 public:
  int id;
  int state;
  int mode;
  int next_mode;
  int next_state;
  int memory[MEMORY];
  double x;
  double y;
  double activetime;
  double sleep_time;
  double csma_time;
  double ca_time;
  double tx_time;
  double be_end;
  double re_be_end;
  complex<double> fading;
  complex<double> m_prev;
  complex<double> chanel_num;
  complex<double> I;
  double phase0[WAVENUM];
  double arrival_angle[WAVENUM];

  NODE(){
    id = -1;
    state = FALSE;
    next_state = FALSE;
    mode = SLEEP;
    next_mode = FALSE;
    x = 0;
    y = 0;
    activetime = 0;
    sleep_time = 0;
    ca_time = 0;
    tx_time = 0;
    be_end = 0;
    re_be_end = 0;
    fading = complex<double> (0,0);
    m_prev = complex<double> (0,0);
    I = complex< double > (0,1);
    chanel_num = complex<double> (0,0);
    
  }
  ~NODE(){}

  void Initialaze(int num){
    int i;
    uniform_real_distribution<> randX(-X_RANGE/2.0, X_RANGE/2.0);
    uniform_real_distribution<> randY(-Y_RANGE/2.0, Y_RANGE/2.0);
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

  void Sleep_Change_Mode(double t_count, double p_sleep){
    double temp;
    uniform_int_distribution<> randCW(0, CW-1);
    
    if(state == SLEEP_TEMP){
      next_mode = RECEIVE;
    }
    else{
      temp = randuni(mt);
      if(temp < p_sleep)
	next_mode = SLEEP;
      else{
	next_mode = CSMA;
	ca_time = SIFS + TIMESLOT * randCW(mt);
      }
    }
  }

  void Beacon_mode(double t_count){
    if(abs(be_end - t_count) < TCOUNT){
      next_mode = Re_Be_ACK;
      state = Re_Be_ACK;
      re_be_end = t_count + BEACON_TIME + CW * TIMESLOT + SIFS;
    }
  }

  void Re_Be_ACK_mode(double t_count){
    if(abs(re_be_end - t_count) < TCOUNT){
      
    }
    else{


    }
  }
    
  void Intlz_Sleep_to_Rx(double t_count){
    next_mode = RECEIVE;
  }
};
