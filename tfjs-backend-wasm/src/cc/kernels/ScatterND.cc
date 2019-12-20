/* Copyright 2019 Google Inc. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ===========================================================================*/

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "src/cc/kernels/ScatterND.h"

#include <vector>

#include "src/cc/backend.h"
#include "src/cc/util.h"

#include "src/cc/scatter_impl.h"

namespace tfjs {
namespace wasm {
extern "C" {
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif

void ScatterND(size_t indices_id, size_t updates_id, size_t slice_rank,
               size_t num_updates, size_t slice_size, size_t* strides_ptr,
               size_t output_size, size_t out_id) {
  auto& indices_info = backend::get_tensor_info(indices_id);
  auto& updates_info = backend::get_tensor_info(updates_id);
  const std::vector<size_t>& strides =
      std::vector<size_t>(strides_ptr, strides_ptr + slice_rank);

  const int* indices_buf = indices_info.i32();
  const float* updates_buf = updates_info.f32();
  auto& out_info = backend::get_tensor_info_out(out_id);
  float* out_buf = out_info.f32_write();
  tfjs::wasm::scatter(indices_buf, updates_buf, slice_rank, num_updates,
                      slice_size, strides, output_size, out_buf);
}
}  // extern "C"
}  // namespace wasm
}  // namespace tfjs