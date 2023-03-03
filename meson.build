#
# Copyright 2023 Collabora, Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

project (
  'c', 'cpp',
  version: '0.0.1',
  default_options: [
    'warning_level=3',
    'c_std=c2x',
    'cpp_std=c++20'
  ],
  meson_version: '>= 0.60',
  license: 'MIT/Expat',
)

qt5 = import('qt5')
subdir('src')
