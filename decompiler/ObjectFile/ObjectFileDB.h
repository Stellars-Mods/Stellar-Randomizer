#pragma once

/*!
 * @file ObjectFileDB.h
 * A "database" of object files found in DGO files.
 * Eliminates duplicate object files, and also assigns unique names to all object files
 * (there may be different object files with the same name sometimes)
 */

#include "common/util/assert.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "LinkedObjectFile.h"
#include "decompiler/util/DecompilerTypeSystem.h"
#include "common/common_types.h"

namespace decompiler {
/*!
 * A "record" which can be used to identify an object file.
 */
struct ObjectFileRecord {
  std::string name;  // including -ag, not including dgo suffix
  int version = -1;
  uint32_t hash = 0;
};

/*!
 * All of the data for a single object file
 */
struct ObjectFileData {
  std::vector<uint8_t> data;     // raw bytes
  LinkedObjectFile linked_data;  // data including linking annotations
  ObjectFileRecord record;       // name
  std::vector<std::string> dgo_names;
  int obj_version = -1;
  bool has_multiple_versions = false;
  std::string name_in_dgo;
  std::string name_from_map;
  std::string to_unique_name() const;
  uint32_t reference_count = 0;  // number of times its used.
};

class ObjectFileDB {
 public:
  ObjectFileDB(const std::vector<std::string>& _dgos,
               const std::string& obj_file_name_map_file,
               const std::vector<std::string>& object_files,
               const std::vector<std::string>& str_files,
               const Config& config);
  std::string generate_dgo_listing();
  std::string generate_obj_listing();
  void process_link_data(const Config& config);
  void process_labels();
  void find_code(const Config& config);
  void find_and_write_scripts(const std::string& output_dir);
  void dump_raw_objects(const std::string& output_dir);

  void write_object_file_words(const std::string& output_dir, bool dump_data, bool dump_code);
  void write_disassembly(const std::string& output_dir,
                         bool disassemble_data,
                         bool disassemble_code,
                         bool print_hex);

  void analyze_functions_ir1(const Config& config);
  void analyze_functions_ir2(const std::string& output_dir,
                             const Config& config,
                             bool skip_debug_output = false);
  void ir2_top_level_pass(const Config& config);
  void ir2_stack_spill_slot_pass(int seg);
  void ir2_basic_block_pass(int seg, const Config& config);
  void ir2_atomic_op_pass(int seg, const Config& config);
  void ir2_type_analysis_pass(int seg, const Config& config);
  void ir2_register_usage_pass(int seg);
  void ir2_variable_pass(int seg);
  void ir2_cfg_build_pass(int seg);
  void ir2_store_current_forms(int seg);
  void ir2_build_expressions(int seg, const Config& config);
  void ir2_insert_lets(int seg);
  void ir2_rewrite_inline_asm_instructions(int seg);
  void ir2_insert_anonymous_functions(int seg);
  void ir2_symbol_definition_map(const std::string& output_dir);
  void ir2_write_results(const std::string& output_dir, const Config& config);
  void ir2_do_common_segment_analysis(int seg, const Config& config);
  std::string ir2_to_file(ObjectFileData& data, const Config& config);
  std::string ir2_function_to_string(ObjectFileData& data, Function& function, int seg);
  std::string ir2_final_out(ObjectFileData& data,
                            const std::unordered_set<std::string>& skip_functions = {});

  std::string process_tpages();
  std::string process_game_count_file();
  std::string process_game_text_files();

  ObjectFileData& lookup_record(const ObjectFileRecord& rec);
  DecompilerTypeSystem dts;
  std::string all_type_defs;

  bool lookup_function_type(const FunctionName& name,
                            const std::string& obj_name,
                            const Config& config,
                            TypeSpec* result);

 public:
  void load_map_file(const std::string& map_data);
  void get_objs_from_dgo(const std::string& filename, const Config& config);
  void add_obj_from_dgo(const std::string& obj_name,
                        const std::string& name_in_dgo,
                        const uint8_t* obj_data,
                        uint32_t obj_size,
                        const std::string& dgo_name,
                        const Config& config);

  /*!
   * Apply f to all ObjectFileData's. Does it in the right order.
   */
  template <typename Func>
  void for_each_obj(Func f) {
    assert(obj_files_by_name.size() == obj_file_order.size());
    for (const auto& name : obj_file_order) {
      for (auto& obj : obj_files_by_name.at(name)) {
        f(obj);
      }
    }
  }

  /*!
   * Apply f to all functions
   * takes (Function, segment, linked_data)
   * Does it in the right order.
   */
  template <typename Func>
  void for_each_function(Func f) {
    for_each_obj([&](ObjectFileData& data) {
      for (int i = 0; i < int(data.linked_data.segments); i++) {
        int fn = 0;
        for (auto& goal_func : data.linked_data.functions_by_seg.at(i)) {
          f(goal_func, i, data);
          fn++;
        }
      }
    });
  }

  template <typename Func>
  void for_each_function_def_order(Func f) {
    for_each_obj([&](ObjectFileData& data) {
      for (int i = 0; i < int(data.linked_data.segments); i++) {
        int fn = 0;
        for (size_t j = data.linked_data.functions_by_seg.at(i).size(); j-- > 0;) {
          f(data.linked_data.functions_by_seg.at(i).at(j), i, data);
          fn++;
        }
      }
    });
  }

  template <typename Func>
  void for_each_function_in_seg(int seg, Func f) {
    for_each_obj([&](ObjectFileData& data) {
      int fn = 0;
      if (data.linked_data.segments == 3) {
        for (size_t j = data.linked_data.functions_by_seg.at(seg).size(); j-- > 0;) {
          f(data.linked_data.functions_by_seg.at(seg).at(j), data);
          fn++;
        }
      }
    });
  }

  // Danger: after adding all object files, we assume that the vector never reallocates.
  std::unordered_map<std::string, std::vector<ObjectFileData>> obj_files_by_name;
  std::unordered_map<std::string, std::vector<ObjectFileRecord>> obj_files_by_dgo;

  std::vector<std::string> obj_file_order;
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> dgo_obj_name_map;

  struct {
    uint32_t total_dgo_bytes = 0;
    uint32_t total_obj_files = 0;
    uint32_t unique_obj_files = 0;
    uint32_t unique_obj_bytes = 0;
  } stats;
};
}  // namespace decompiler
