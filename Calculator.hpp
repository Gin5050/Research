/*Calculatorクラス*/
#ifndef CALCULATOR_HEAD
#define CALCULATOR_HEAD

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

   if(nodeId == EMPTY){
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
      it++;
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

#endif
