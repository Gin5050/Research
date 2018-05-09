#include "define.h"
#include "system.h"
#include "channel.h"
//#include "degree_define.h"
#include "awgn.h"

void Initialaize_nodes(NODE *n_data, int N);
void Process_node(NODE *n_data, int N, double t_count, double p_sleep, list<int> &Beacon_node, list<int> &ACK_node, list<int> &Trans_node, list<int> &All_trans);
void CSMA_CA(NODE *n_data, int N, int num, double t_count, list<int> &Beacon_node, list<int> &All_trans);
void Re_Be_func(NODE *n_data, double t_count, int num, int N, list<int> &Beacon_node, list<int> &All_trans);
int MinBack(NODE *n_data, int num, int mode, int N, list<int> &Beacon_node);
double SINR(NODE *n_data, double t_count, int num, int min_node, int mode, list<int> &All_trans);
void DBPSK(NODE *n_data, double sinr, int num, int temp);

int main(){
  int i, j;
  int nowround;
  double PPP_CDF[TEMP_NUM] = {0};
  double temp;

  
  NODE *n_data;
  
  PPP(PPP_CDF);
  for(p_sleep = 0.5; p_sleep < 0.6; p_sleep += 0.1){

    for(nowround = 0; nowround < REPEATNUM; nowround++){
      temp = randuni(mt);
      for(i = 0; i < TEMP_NUM; i++)
	if(temp < PPP_CDF[i]) break;
      N = i;
      n_data = new NODE[N];
      cout << N << endl;

      Initialaize_nodes(n_data, N);
      t_count = 0;
      while(t_count < OBSERVE){
	// for(i = 0; i < N; i++){
	//   n_data[i].mode = n_data[i].next_mode;
	//   n_data[i].state = n_data[i].next_state;
	// }
	//if(int(t_count) % 5 == 0 && t_count > 1) cout << t_count << endl;
	Process_node(n_data, N, t_count, p_sleep, Beacon_node, ACK_node, Trans_node, All_trans);
	//if(int(t_count) % 10 == 0 && t_count > 1) cout << t_count << endl;
	t_count += TCOUNT;
      }
    }
  }
  return 0;
}

void Initialaize_nodes(NODE *n_data, int N){
  int i,j;
  
  for(i = 0; i < N; i++){
    n_data[i].Initialaze(i);
  }
}

void Process_node(NODE *n_data, int N, double t_count, double p_sleep, list<int> &Beacon_node, list<int> &ACK_node, list<int> &Trans_node, list<int> &All_trans){
  int i;
  double temp;

  for(i = 0; i < N; i++){
    switch (n_data[i].mode){
      
      case SLEEP:
      	if(abs(t_count - n_data[i].activetime) < TCOUNT){
      	  n_data[i].Sleep_Change_Mode(t_count, p_sleep);	
      	}
      	n_data[i].sleep_time+=TCOUNT;
      	break;

      // case CSMA:
      // 	CSMA_CA(n_data, N, i, t_count, Beacon_node);
      // 	n_data[i].csma_time+=TCOUNT;
      // 	break;
	
      // case BEACON:
      // 	n_data[i].Beacon_mode(t_count);
      // 	n_data[i].tx_time+=TCOUNT;
      // 	break;

      // case Re_Be_ACK: //ACK
      // 	if(abs(n_data[i].re_be_end - t_count) < TCOUNT){
      // 	  //n_data[i].Re_Be_to_ACK(t_count);
      // 	}
      // 	else
      // 	  Re_Be_func(n_data, t_count, i, N, Beacon_node);
      // 	//n_data[shf[i]].rx_time+=TCOUNT;
      // 	break;
	
    //   case TRANSMIT:
    // 	n_data[shf[i]].tr_cnt++;
    // 	if(abs(n_data[shf[i]].tr_end - t_count) + SECURE < TCOUNT){
    // 	  if(n_data[shf[i]].state == SENDER){
    // 	    //cout << t_count << "\tTRANSMIT = " << shf[i] << "\t" << n_data[shf[i]].rx_nodeNum << endl;
    // 	  }	 
    // 	  n_data[shf[i]].Intlz_Tx_to_Sleep(t_count);
    // 	  n_data[shf[i]].trn_num++;	
    // 	}
    // 	n_data[shf[i]].tx_time+=TCOUNT;
    // 	//n_data[shf[i]].tr_cnt++;
    // 	break;

    //   case SENDER:	
    // 	if(abs(n_data[shf[i]].s_end - t_count) + SECURE < TCOUNT){
    // 	  if(n_data[shf[i]].rx_nodeNum >= RX_NODE){
    // 	    n_data[shf[i]].Intlz_Sender_to_CSMA(t_count);
    // 	    communNode.push_back(n_data[shf[i]].rx_nodeNum);
    // 	    //cout << n_data[shf[i]].rx_nodeNum << endl;
    // 	  }
    // 	  else
    // 	    n_data[shf[i]].Intlz_Sender_to_Sleep(t_count);
    // 	  n_data[shf[i]].sender_time += TCOUNT;
    // 	}
    // 	else Sender(n_data, shf[i], t_count, p_sleep, p_data);
    // 	break;

    //   case Be_ACK:
    // 	if(abs(n_data[shf[i]].s_end - t_count) + SECURE < TCOUNT){
    // 	  n_data[shf[i]].s_end+=TCOUNT;
    // 	}
    // 	n_data[shf[i]].next_mode = SENDER;
    // 	n_data[shf[i]].tx_time+=TCOUNT;
    // 	break;

    //   case RECEIVE:
    // 	n_data[shf[i]].rx_time+=TCOUNT;
    // 	if(abs(n_data[shf[i]].rx_end - t_count) + SECURE < TCOUNT){
    // 	  n_data[shf[i]].Intlz_Rx_to_Sleep(t_count);
    // 	  //n_data[shf[i]].check = FALSE;
    // 	}
    // 	else Receive(n_data, shf[i], t_count, p_data);
    // 	break;

    //   case COLLISION_MG:
    // 	n_data[shf[i]].tx_time++;
    // 	n_data[shf[i]].Intlz_to_Sleep(t_count);
    // 	break;
    }
  }
}

//CSMA/CA
void CSMA_CA(NODE *n_data, int N, int num, double t_count, list<int> &Beacon_node, list<int> &All_trans){
  int i,j;
  double dis_node = 0;
  double r_level = 0;
  double *p_dist;
  complex<double> channel_coeff;
  list<int> ::iterator it;  

  it = All_trans.begin();
  while(it != All_trans.end()){
    if(num != *it){
      dis_node = sqrt(pow(n_data[num].x - n_data[i].x, 2) + pow(n_data[num].y - n_data[i].y, 2));
      channel_coeff = rayleigh();
      r_level = Pathloss(dis_node, PATHLOSS_num) + abs(channel_coeff) * abs(channel_coeff);
      if(r_level > CA_dBm){
	break; //CAレベルを超えていれば
      }
    }
    ++it;
  }
  
  if(it != All_trans.end()){
    if(n_data[num].ca_time > 0) n_data[num].ca_time = n_data[num].ca_time - TCOUNT;
    else if(n_data[num].ca_time < TCOUNT){
       switch (n_data[num].state){
  	case FALSE:
  	  n_data[num].next_mode = BEACON;
  	  n_data[num].be_end = t_count + BEACON_TIME; //Beacon送信時間は1ms
	  Beacon_node.push_back(num);
	  All_trans.push_back(num);
  	  break;	  
  	// case SENDER:
  	//   if(n_data[num].s_end < t_count){ //CSMAなどによりsender終了時刻を飛び越した場合
  	//     n_data[num].next_mode = TRANSMIT;
  	//     n_data[num].tr_end = t_count + Tf; //送信終了時刻
  	//     n_data[num].tr_cnt = 0; //送信終了時刻
  	//     //Encode(n_data, node_num);
  	//   }
  	//   else{
  	//     n_data[num].next_mode = Be_ACK;
  	//   }
  	//   break;
  	case Re_Be_ACK:
  	  n_data[num].next_mode = TRANSMIT;
  	  //n_data[node_num].tr_end = 1; //送信終了時刻
  	  //n_data[num].tr_end = t_count + Tf; //送信終了時刻
  	  //n_data[num].tr_cnt = 0; //送信終了時刻
  	  //Encode(n_data, num);
  	  break;
      }
    }
  }     
  //if(n_data[node_num].flag != EMPTY && n_data[node_num].state == SENDER) n_data[node_num].n_trn_num++;
}

//SenderからのACK待ち
void Re_Be_func(NODE *n_data, double t_count, int num, int N, list<int> &Beacon_node, list<int> &All_trans){
   int temp = -1;
   int flag = 0;
   double sinr = 0;
   uniform_int_distribution<> randCW(0, CW-1);
 
   temp = MinBack(n_data, num, Be_ACK, N, Beacon_node);
   if(temp != -1){
     sinr = SINR(n_data, t_count, num, temp, Be_ACK, All_trans);
     DBPSK(n_data, sinr, num, temp);
     //if(n_data[num].rec_cnt == )
    //n_data[num].rx_time+=TCOUNT;
    // if(n_data[num].rec_cnt == SHORTSIZE && n_data[num].timestamp[flag] < n_data[temp].timestamp[flag]){
    //   n_data[num].next_mode = SLEEP;
    //   n_data[num].activetime = n_data[temp].s_end;
    //   n_data[num].next_state = SLEEP_TEMP;
    //   n_data[temp].rx_nodeNum++;
    // }
    // else {
    //   n_data[num].ca_time = (IFS + randCW(mt) * TIMESLOT);
    //   n_data[num].next_mode = CSMA;
    // }
    //n_data[num].Intlz_DBPSK(t_count);    
  }
  //else n_data[num].idle_time+=TCOUNT;
}

double SINR(NODE *n_data, double t_count, int num, int min_node, int mode, list<int> &All_trans){
  int i;
  int nummode = n_data[num].mode;
  int temp = -1;
  double distance = 0;
  double min = 100000;
  double interference = 0;
  double loss = 0;
  double sinr = 0;
  complex<double> x;
  double fading = 0;
  list<int> ::iterator it;

  //calculate minimum distance
  it = All_trans.begin();
  while(it != All_trans.end()){
    if(num != *it){
      if(n_data[*it].mode == mode){
	distance = sqrt(pow(n_data[num].x - n_data[i].x, 2) + pow(n_data[num].y - n_data[i].y, 2));
	if(min > distance){
	  min = distance;
	  temp = *it;
	}
      }     
    }
    ++it;
  }

  //calculate SINR
  if(temp != -1){
    it = All_trans.begin();
    while(it != All_trans.end()){
      if(num != *it && temp != *it){
	distance = sqrt(pow(n_data[num].x - n_data[i].x, 2) + pow(n_data[num].y - n_data[i].y, 2));	
	loss = Pathloss(distance, PATHLOSS_num);
	x = rayleigh();
	fading = 10 * log10(abs(x) * abs(x));
	interference += (pow(10, (fading + loss) / 10.0));
	//if(nummode != RECEIVE &&  nummode != SENDER)
	  //Collision_check_sensor(n_data, p_data, t_count, num, i);	
      }
    }
    loss = Pathloss(min, PATHLOSS_num);
    x = rayleigh();;
    fading = 10 * log10(abs(x) * abs(x));
    sinr = loss + fading - (10 * log10(interference + NOISE));
    //cout << sinr << "\t" << temp << "\t" << num << "\t" << min << "\t" << interference << "\t" << fading << endl;
  }
  return sinr;
}

void DBPSK(NODE *n_data, double sinr, int num, int temp){

  int i,j;
  complex<double> x;  
  double p_d = 0;
  complex<double> m_temp;
  
  if(n_data[num].rec_time == 0){
    x = rayleigh();
    n_data[num].fading = x;
    n_data[num].sinr = sinr;
    n_data[num].connect = temp;
    
    //1シンボル遅延
    m_temp = complex<double>(1.0,0);
    m_temp = m_temp * x;
    m_temp += complex<double>(awgnQ(sinr), awgnI(sinr));
    n_data[num].m_prev = m_temp;
  }
  n_data[num].rec_time++;
  
  //DBPSK 1us分の受信
  for(j = 0; j < DATARATE * TCOUNT; j++){
    m_temp = complex<double>(1.0,0);
    m_temp = m_temp * n_data[num].fading;
    m_temp += complex<double>(awgnQ(sinr), awgnI(sinr));     
    p_d = abs(arg(n_data[num].m_prev) - arg(m_temp));
    n_data[num].m_prev = m_temp;
    if(cos(p_d) < 0){ //1bitでも誤った場合は終了
      break;
    }
  }
  n_data[num].rec_cnt += j;
}

int MinBack(NODE *n_data, int num, int mode, int N, list<int> &nodes){
  int i;
  int temp = -1;
  double distance = 0;
  double min = 1000000;
  list<int> ::iterator it;

  it = nodes.begin();
  while(it != nodes.end()){
    if(*it != num){
      if(n_data[*it].mode == mode){
	distance = sqrt(pow(n_data[num].x - n_data[*it].x, 2) + pow(n_data[num].y - n_data[*it].y, 2));
	if(min > distance){
	  min = distance;
	  temp = i;
	}
      }
    }
    ++it;
  }    
  return temp;
}
  

