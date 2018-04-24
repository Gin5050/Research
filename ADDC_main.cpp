#include "define.h"
#include "system.h"
#include "channel.h"
//#include "degree_define.h"
//#include "awgn.h"

void Initialaize_nodes(NODE *n_data, int N);
void CSMA_CA(NODE *n_data, int N, int num, double t_count);

int main(){
  int i, j;
  int nowround;
  int N;
  double p_sleep;
  double t_count;
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

void Process_node(NODE *n_data, int N, double t_count, double p_sleep){
  int i;
  double temp;

  for(i = 0; i < N; i++){
    switch (n_data[i].mode){

      case SLEEP:
	if(abs(t_count - n_data[i].activetime) < TCOUNT){
	  if(n_data[i].state == SLEEP_TEMP) //SenderからBe_ACKを受信した場合
	    n_data[i].Intlz_Sleep_to_Rx(t_count);
	  else{
	    temp = randuni(mt);
	    if(temp < p_sleep)
	      n_data[i].next_mode = SLEEP;
	    else
	      n_data[i].next_mode = CSMA;
	  }
	}
	n_data[i].sleep_time+=TCOUNT;
	break;

      case CSMA:
     	CSMA_CA(n_data, N, i, t_count);
	n_data[i].csma_time+=TCOUNT;
    	break;
	
    //   case BEACON:
    // 	n_data[shf[i]].Intlz_Be_to_Re_Be(t_count);
    // 	n_data[shf[i]].tx_time+=TCOUNT;
    // 	break;

    //   case Re_Be_ACK: //ACK待機
    // 	if(abs(n_data[shf[i]].re_be_end - t_count) + SECURE < TCOUNT){
    // 	  n_data[shf[i]].Intlz_Re_Be_to_CSMA(t_count);
    // 	}
    // 	else
    // 	  Re_Be_func(n_data, t_count, shf[i], p_data);
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
void CSMA_CA(NODE *n_data, int N, int num, double t_count){
  int i,j;
  double dis_node = 0;
  double r_level = 0;
  double *p_dist;
  complex<double> channel_coeff;
  
  for(i = 0; i < N; i++){ //他端末の送信をチェック
    if(num != i){
      if(n_data[i].mode == TRANSMIT || n_data[i].mode == BEACON || n_data[i].mode == Be_ACK){
	dis_node = sqrt(pow(n_data[num].x - n_data[i].x, 2) + pow(n_data[num].y - n_data[i].y, 2));
	channel_coeff = rayleigh();
	r_level = Pathloss(dis_node, PATHLOSS_num) + abs(channel_coeff) * abs(channel_coeff);
	if(r_level > CA_dBm){
	  break; //CAレベルを超えていれば
	}
      }
    }
  }
  
  // if(j == N){
  //   if(n_data[node_num].ca_time > 0) n_data[node_num].ca_time = n_data[node_num].ca_time - TCOUNT;
  //   else if(n_data[node_num].ca_time < TCOUNT){
  //     switch (n_data[node_num].state){
  // 	case FALSE:
  // 	  n_data[node_num].next_mode = BEACON;
  // 	  n_data[node_num].be_end = t_count + TCOUNT; //Beacon送信時間は1ms
  // 	  break;	  
  // 	case SENDER:
  // 	  if(n_data[node_num].s_end < t_count){ //CSMAなどによりsender終了時刻を飛び越した場合
  // 	    n_data[node_num].next_mode = TRANSMIT;
  // 	    n_data[node_num].tr_end = t_count + Tf; //送信終了時刻
  // 	    n_data[node_num].tr_cnt = 0; //送信終了時刻
  // 	    Encode(n_data, node_num);
  // 	  }
  // 	  else{
  // 	    n_data[node_num].next_mode = Be_ACK;
  // 	  }
  // 	  break;
  // 	case Re_Be_ACK:
  // 	  n_data[node_num].next_mode = TRANSMIT;
  // 	  //n_data[node_num].tr_end = 1; //送信終了時刻
  // 	  n_data[node_num].tr_end = t_count + Tf; //送信終了時刻
  // 	  n_data[node_num].tr_cnt = 0; //送信終了時刻
  // 	  Encode(n_data, node_num);
  // 	  break;
  //     }
  //     if(n_data[node_num].flag != EMPTY && n_data[node_num].state == SENDER) n_data[node_num].n_trn_num++;
  //   }
  // }
}
