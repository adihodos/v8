#pragma once

#include <vector>
#include "v8/base/misc.h"
#include "v8/base/scoped_pointer.h"
#include "v8/rendering/directx/directx_api_model.h"

namespace v8 { namespace rendering_system {

class renderer;

class uniform_buffer_base;

class directx_shader {
private :
  NO_CC_ASSIGN(directx_shader);

protected :
  struct ubuff_data_t {
    unsigned int  u_slot;
    directx_api_model::buffer_t* u_handle;

    ubuff_data_t(
        unsigned int slot, 
        directx_api_model::buffer_t* buff_handle
        ) : u_slot(slot), u_handle(buff_handle) {}

    bool operator==(unsigned int slot) const {
      return u_slot == slot;
    }

    bool operator==(const directx_api_model::buffer_t* buff) const {
      return u_handle == buff;
    }
  };  

  base::scoped_ptr<
      directx_api_model::shader_reflection_t, base::com_storage
  >                                             shader_reflector_;
  base::scoped_ptr<
      directx_api_model::blob_t, base::com_storage
  >                                             shader_bytecode_;
  std::vector<ubuff_data_t>                     shader_uniforms_;
  const char*                                   shader_name_;

  bool create_shader_reflection();

  bool compile_shader_to_bytecode(
    const char* shader_file,
    const char* entry_point,
    const char* shader_profile,
    unsigned int compile_flags
    );

  directx_shader(const char* name) : shader_name_(name) {}

public :
  virtual ~directx_shader() {}

  virtual bool compile_from_file(
    renderer* r,
    const char* file_name, 
    const char* entry_point, 
    int profile, 
    unsigned debug_flags
    ) = 0;

  virtual void bind_to_pipeline(renderer*) = 0;

  //virtual void set_shader_resource(shader_resource_t*) = 0;

  //virtual shader_resource_t* get_shader_resource(const char*) = 0;

  const char* get_name() const {
    shader_name_;
  }

  directx_api_model::shader_reflection_t* get_reflection_interface() const {
    return base::scoped_pointer_get(shader_reflector_);
  }

  directx_api_model::blob_t* get_bytecode() const {
    return base::scoped_pointer_get(shader_bytecode_);
  }

  //void set_uniform_buffer_data(outer_limits::uniform_buffer_dxbase* ubuff_dx);
};

} // namespace rendering_system
} // namespace v8
