#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>
#include "idx_metadata.h"

using namespace idx_metadata;

int main(int argc, char** argv){

  if(argc < 2){
    fprintf(stderr, "Usage: read file_path [debug]\n");

    return 1;
  }

  clock_t start, finish;
  start = clock();

  IDX_Metadata meta(argv[1]); // default: use simple layout

  int ret = meta.load();

  finish = clock();

  printf("Time taken %fms\n",(double(finish)-double(start))/CLOCKS_PER_SEC);

  if (argc < 3) // if no args just load, otherwise debug
    return 0;

  // Debug and verify
  for(int t=0; t < meta.get_n_timesteps(); t++){
    std::shared_ptr<TimeStep> ts = meta.get_timestep(t);

    std::shared_ptr<Level> level = ts->get_level(0);

    for(int g=0; g < level->get_n_datagrids(); g++){
      std::shared_ptr<DataGrid> datagrid = level->get_datagrid(g);

      for(auto att : datagrid->get_attributes()){
        printf("Attribute %s cent %s type %s num_type %s prec %s dim %s endian %s\n",
          att.name.c_str(), ToString(att.centerType), ToString(att.attributeType),
          ToString(att.data.numberType), att.data.precision.c_str(), att.data.dimensions.c_str(),
          ToString(att.data.endianType));
      }

      Grid& grid = datagrid->get_grid();
      DataItem& item_o = grid.geometry.item[0];
      DataItem& item_d = grid.geometry.item[1];
      printf("Geometry type %s dim %s O %s D %s topo type %s dim %s\n", 
          ToString(grid.geometry.geometryType), item_o.dimensions.c_str(), item_o.text.c_str(), item_d.text.c_str(), ToString(grid.topology.topologyType),
          grid.topology.dimensions.c_str());
    }
  }

  // add a timestep to the current metadata
  std::shared_ptr<TimeStep> ts(new TimeStep());
  std::shared_ptr<Level> level = meta.get_timestep(0)->get_level(0);

  ret = ts->add_level(level);

  ts->set_timestep(777, float(7777));
  meta.add_timestep(ts);

  // write the metadata to compare with original
  meta.set_path("load_test.xmf");
  meta.save();

  return ret;
}