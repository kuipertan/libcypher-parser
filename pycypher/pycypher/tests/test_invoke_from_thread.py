# Copyright 2016, Chris Leishman (http://github.com/cleishm)
# Copyright 2017, Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import unittest
import pycypher

import threading


class TestInvokeFromThread(unittest.TestCase):
    def test_invoke_pycypher_from_thread(self):
        """This was sometimes segfaulting in the past."""
        def func():
            import pycypher
            pycypher.parse_query('MATCH (a)')
        t = threading.Thread(target=func, args=())
        t.start()
        t.join()
