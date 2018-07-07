/*2018　4/15作成　*/
/*SensorパラメータRev 1.0 10/14Copyright © 2014 by Silicon LaboratoriesSi4468/7Si4468/7HIGH-PERFORMANCE, LOW-CURRENT TRANSCEIVER*/

#define OBSERVE 5 //second
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
#define Sint (2.0)				//スリープ間隔（s）
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
#define ACK 8						//ack待ち
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

