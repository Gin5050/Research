/*Nodeの各モードを記憶するクラス*/
#ifndef MODE_MEMORY_HEAD
#define MODE_MEMORY_HEAD

class ModeMemory{
 public:
  list<int> Beacon_node;
  list<int> Trans_node;
  list<int> ACK_node;
  list<int> All_trans;

  ModeMemory(){}

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

  void deleteBeacon(int id){
    list<int>::iterator it = Beacon_node.begin();
    while(it != Beacon_node.end()){
      if(*it == id){
	Beacon_node.erase(it);
	break;
      }
    }
  }

  void deleteTrans(int id){
    list<int>::iterator it = Trans_node.begin();
    while(it != Trans_node.end()){
      if(*it == id){
	Trans_node.erase(it);
	break;
      }
    }
  }

  void deleteACKnodes(int id){
    list<int>::iterator it = ACK_node.begin();
    while(it != ACK_node.end()){
      if(*it == id){
	ACK_node.erase(it);
	break;
      }
    }
  }

  list<int> getTransNodes(){
    return Trans_node;
  }

  list<int> getBeaconNodes(){
    return Beacon_node;
  }
};
#endif
