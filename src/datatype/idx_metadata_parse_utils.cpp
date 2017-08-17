
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "idx_metadata_parse_utils.h"
#include "idx_metadata_attribute.h"

#define MY_ENCODING "ISO-8859-1"

namespace idx_metadata{


int parse_level(xmlNode *space_grid, std::shared_ptr<Level> lvl){
  for (xmlNode* cur_node = space_grid; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if(is_node_name(cur_node, "Grid")){
        std::shared_ptr<DataGrid> dgrid(new DataGrid());
        Grid grid;

        grid.name = getProp(cur_node, "Name");

        for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
          if (inner_node->type == XML_ELEMENT_NODE) {
            
            if(is_node_name(inner_node, "Topology")){
              grid.topology.XMLToObj(inner_node);
            }
            else if(is_node_name(inner_node, "Geometry")){
              grid.geometry.XMLToObj(inner_node);
            }

          }
        }

        dgrid->set_grid(grid);
        lvl->add_datagrid(dgrid);
      }
    }
  }

  for (xmlNode* cur_node = space_grid; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      
      if(is_node_name(cur_node, "Attribute")){
        Attribute att;

        att.XMLToObj(cur_node);

        // TODO do not assume every grid has same attributes, resolve xpointers
        for(int i=0; i < lvl->get_n_datagrids(); i++){
          lvl->get_datagrid(i)->add_attribute(att);
        }
      }

    }
  }

  return 0;
}


}