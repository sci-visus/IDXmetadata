#ifndef XIDX_LIST_DOMAIN_H_
#define XIDX_LIST_DOMAIN_H_

#include <sstream>
#include "xidx/xidx.h"

namespace xidx{

template<typename T>
class ListDomain : public Domain{

public:
  std::vector<T> values_vector;
  
  ListDomain(std::string _name) : Domain(_name) {
    type = LIST_DOMAIN_TYPE;
    data_items.push_back(DataItem(name, this));
  };
  
  ListDomain(std::string _name, DataItem& item) : Domain(_name) {
    type = LIST_DOMAIN_TYPE;
    data_items.push_back(item);
  };
  
  ListDomain(const ListDomain& c) : Domain(c){
    type = LIST_DOMAIN_TYPE;
    name = c.name;
    data_items = c.data_items;
    values_vector = c.values_vector;
  };
  
  ListDomain(const ListDomain* d) : Domain(d->name){
    type = LIST_DOMAIN_TYPE;
    data_items = d->data_items;
    values_vector = d->values_vector;
  }
  
  int AddDomainItem(T phy){
    values_vector.push_back(phy);
  
    return 0;
  }
  
  virtual const IndexSpace<PHY_TYPE>& GetLinearizedIndexSpace() override{
    return values_vector;
  };
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    assert(data_items.size() >= 1);
    DataItem& physical = data_items[0];
    
    if(!std::is_same<T, DataSource>::value){
      for(auto phy: values_vector)
        physical.text+=std::to_string(phy)+" ";
      
      physical.dimensions.push_back(values_vector.size());
    }
    xmlNodePtr domain_node = Domain::Serialize(parent, text);
      
    return domain_node;
  }
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    Domain::Deserialize(node, _parent);
    
    SetParent(_parent);
  
    assert(GetParent()!=nullptr);
      
    int count = data_items.size();
  
    if(count == 1){
      auto& item = data_items[0];
      
      assert(item.dimensions.size()>0);
      
      size_t length = item.dimensions[0];

      values_vector.resize(length);
      
      std::stringstream stream_data(item.text);
      
      for(int i=0; i< length; i++){
        stream_data >> values_vector[i];
      }
      
    }
    else{
      assert(false);
      fprintf(stderr, "too many data items in list domain\n");
    }
    
    return 0;
  }
  
  virtual std::string GetClassName() const override { return "ListDomain"; };

private:
  std::vector<INDEX_TYPE> log_indices;
  
};

}
#endif
