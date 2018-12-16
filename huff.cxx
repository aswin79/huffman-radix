
/*** 
// Huffman Codes
//
// Variable length codes, with shorter codes for more popular characters.
// 
// Example:
// i = 001
// q = 000110
// 
// The code is represented as a binary tree.  Here is an intermediate step 
// during the encoding algorithm.  Nodes '3' and '4' have been combined into a
// a new node '7'.  Then nodes '7' and '9' were combined into a new node '16'.
// What happens next?
//
//   +-----+    +-----+    +-----+
//   | 'a' |    | 'b' |    |     |        <----- Forest of Trees
//   | 10  |    | 12  |    | 16  |
//   +-----+    +-----+    +-----+
//                         0 / \ 1 
//                          /   \
//                   +-----+     +-----+
//                   | 'c' |     |     |
//                   |  9  |     |  7  |
//                   +-----|     +-----|
//                code: c="0"    0 / \ 1
//                                /   \
//                         +-----+     +-----+
//                         | 'd' |     | 'e' |
//                         |  3  |     |  4  |
//                         +-----+     +-----+
//                     code: d="10"   code: e="11"
***/

#include <string>
#include <string.h>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>
using namespace std;

vector<set<char>> vec;

map <char,int> nodeMap;  

class node {
    
   public:
     
     string id=".";
     int weight;
     node* zeroPtr=0;
     node* onePtr=0;
     node* parentPtr=0;
}  ;


map <char,node*> nodePtrMap;
vector<set<node*>> vecPtr;
set <int> validEntries;
node* head=0;
node* nxt=0;

node* nxt1=0;

node* nxt2=0;
node* nxt3=0;
void create_code(string &msg) { 
     
     int sz=msg.size();
     int maxOccr=0;
     for (int i=0;i<sz;i++) {
       char p=msg[i];
       auto it = nodeMap.find(p); 
       if(it==nodeMap.end()) {
          nodeMap[p]=0;
          nodePtrMap[p] = new node();
          nodePtrMap[p]->id=p;
       }
       nodeMap[p]+=1;
       nodePtrMap[p]->weight = nodeMap[p];
       maxOccr=max(maxOccr, nodeMap[p]);  
      }
        
     // size this based on height of final treezx 
     vec.resize(sz+1);
     vecPtr.resize(sz+1);
/*  
     std::transform( nodeMap.begin(), nodeMap.end(),
                   std::back_inserter(vec),
                   boost::bind(&nodeMap::value_type::second,_1) );
*/
  
  // bin entries in terms of freq.
  // the vec index denotes freq
    for( auto & it: nodeMap) {
        cout<<it.first<<" "<<it.second<< endl;
        set<char> val = vec[it.second];
        val.insert (it.first);
        vec[it.second] = val;
        set<node*> valPtr = vecPtr[it.second];
        valPtr.insert (nodePtrMap[it.first]);
        vecPtr[it.second] = valPtr;
        validEntries.insert(it.second);
        
    } 
  /*
    for( auto it1: validEntries) {
       cout<<it1 <<" ";
    }
    cout<<endl;
    */
    int cmbCnt=0, nodesLeft=sz;
    node* toCombine =0;
    // dont need to go over all  entries...just go over valid entries
    for( auto &it1: validEntries) {
      // set of letters with count =it1 
      set<node*> valPtr = vecPtr[it1];
      
      // for each letter in bin
      for (auto &it2: valPtr) {
        int minCnt=2*sz+1;
        // dont need to combine leaves
        if(it2->parentPtr!=0)
          continue;
        // compare with all other letters (forest) and current created nodes to find node of min weight to combine with
        for( auto &it3: validEntries) {
            // if it3 > minCnt?..since this is sorted already can we break
              set<node*> valPtr2 = vecPtr[it3];
              for (auto& it4: valPtr2) {
                if(it4->parentPtr!=0)
                   continue;
                if ( it4==it2) continue;
         //       cout<<" comparing :"<<it4->id<<":"<<it4->weight<<" with "<<minCnt<<endl;
                if ( it4->weight<minCnt){
                    minCnt = it4->weight;
                    toCombine = it4;
                } 
              }      
         } 
         if (!toCombine)
          continue;
        // combine it2 with toCombine. neither nodes can be combined after this and the new combined node
        // should replace it2 with it4 being removed from set of nodes left to be combined;
        // the new combined node should be pushed back into vecPtr with its combined weight to optimize the min weight look up loop
        // this could/will create a new entry in validEntries...which we are currently traversing over....is it 
        // safe to update on the fly 
        cmbCnt++;
        node* combPtr = new node();
        combPtr->weight= toCombine->weight + it2->weight;
        nodesLeft-=2;
        combPtr->zeroPtr  = toCombine;
        combPtr->onePtr = it2;
        it2->parentPtr=combPtr;
        toCombine->parentPtr=combPtr;
        nodesLeft+=1;
        cout<<" 1:"<<combPtr->id<<" "<<combPtr->weight<<" :"<<it2->id<<" "<<it2->weight<<" + "<<" :"<<toCombine->id<<" "<<toCombine->weight<<"   "<<cmbCnt<<" "<<nodesLeft<<endl;
    //    cout<<" "<<combPtr<<" "<<toCombine<<" "<<it2<<" "<<(it2==toCombine)<<endl;
        
        
        head=combPtr;
        validEntries.insert(combPtr->weight);
        vecPtr[it2->weight].erase(it2);
        vecPtr[toCombine->weight].erase(toCombine);
        vecPtr[combPtr->weight].insert(combPtr);
        toCombine = 0;
      } 
    }  
  
  
    // bfs dump to test
    queue<node*> tQ;
    tQ.push(head);
    while (!tQ.empty()) {
     unsigned int qSz=tQ.size();
     for(unsigned int i=0;i<qSz;i++) { 
        node* temp=tQ.front();
        cout<< temp->id<<":"<<temp->weight<<" ";
        if(temp->onePtr)
          tQ.push(temp->onePtr);
       if(temp->zeroPtr)
          tQ.push(temp->zeroPtr);
       tQ.pop();
     }  
     cout<<endl; 
      
    }  
  
    /*
    int headVal=0;
    int nextVal=0;
    for ( auto it: vec) {
          if(it->second.size()==0)
              continue;
           set<char> val = it->second; 
           for ( auto it1 :val) {
               nxt = new node();
               nxt->id = it1;
               
               if ( head==0) {
                      head= nxt;
                      headVal=nxt->id;
             } else {
                 // need to decide on how to combine. 
                 if(nxt1!=0) {
                     nxt3= nxt1->val< head->val ? nxt1 : headVal ;
                     nxt2 = new node();
                     nxt2->val=nxt3->val+nxt->val;
                     nxt2->zeroPt = nxt1;
                     nxt2->onePt  = nxt ;
                     nxt3= nxt1->val< head->val ? nxt1 : headVal ;
                     
                 } else
                    nxt1=nxt;
             }
      
      
    }  
  */
  
}

// simple dfs 
bool findCode(node* h, string t, string &c) {

      if(!h) return false;
      if(t==h->id) return true;
      string temp=c; 
      if ( h->zeroPtr) {
         c=temp+"0";
         if(findCode(h->zeroPtr,t,c))
            return true;
      }  
      if ( h->onePtr) {
         c=temp+"1";
        
        if(findCode(h->onePtr,t,c))
           return true;
      }  
      return false;     
  
}

string encode(string &msg) { 
  string res="";
  for(unsigned int i=0;i<msg.size();i++) {
  // cout<<msg[i]<<endl; 
   string c(1,msg[i]), t; 
   if(findCode(head,c,t)) {
     res+=t;
    // cout<<" coded "<<c<<" "<<t<<endl;
   } else {
       cout<<" Unable to encode "<<c<<endl;
   }  
  }  
    
  return res; 
}

string decode(string &code) { 
  string res=""  ;
  node* ptr=head;
  string runCode;
  for(unsigned int i=0;i<code.size();i++) {
    string c(1,code[i]), t; 
    //cout<<c<<" ";
    if(c=="0" && ptr->zeroPtr) {
    //  cout<<" Zero ";
      ptr = ptr->zeroPtr;
        
    } else if(c=="1" && ptr->onePtr) {
      ptr = ptr->onePtr;
     // cout<<" One ";
    } else {
      cout<<" Error! "<<endl;
      return res;
    }  
  //  cout<<ptr->id<<" ";
    if(ptr->id !=".") {//found a letter. save & reset
      res+=ptr->id;
      ptr=head;
    }  
  //  cout<<endl;
  }  
    
  return res; 

}

int main() {
  string reference_msg = "the quick brown fox jumped over the lazy dog";
  string test_msg = "this is a test";
  
  create_code(reference_msg);
  string code = encode(test_msg);
  string msg = decode(code);
  
  cout << "code: " << code << endl;
  cout << endl;
  cout << "msg: " << msg << endl;
  
  return 0;
}
