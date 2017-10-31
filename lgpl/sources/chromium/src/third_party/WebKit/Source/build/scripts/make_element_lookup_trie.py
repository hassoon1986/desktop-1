#!/usr/bin/env python
# Copyright (C) 2013 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import sys

import json5_generator
import trie_builder
import template_expander


class ElementLookupTrieWriter(json5_generator.Writer):
    # FIXME: Inherit all these from somewhere.
    default_parameters = {
        'JSInterfaceName': {},
        'constructorNeedsCreatedByParser': {},
        'interfaceName': {},
        'noConstructor': {},
        'runtimeEnabled': {},
    }
    default_metadata = {
        'attrsNullNamespace': None,
        'export': '',
        'fallbackInterfaceName': '',
        'fallbackJSInterfaceName': '',
        'namespace': '',
        'namespacePrefix': '',
        'namespaceURI': '',
    }

    def __init__(self, json5_file_paths):
        super(ElementLookupTrieWriter, self).__init__(json5_file_paths)
        self._tags = {}
        for entry in self.json5_file.name_dictionaries:
            self._tags[entry['name']] = entry['name']
        self._namespace = self.json5_file.metadata['namespace'].strip('"')
        self._outputs = {
            (self._namespace + 'ElementLookupTrie.h'): self.generate_header,
            (self._namespace + 'ElementLookupTrie.cpp'): self.generate_implementation,
        }

    @template_expander.use_jinja('templates/ElementLookupTrie.h.tmpl')
    def generate_header(self):
        return {
            'input_files': self._input_files,
            'namespace': self._namespace,
        }

    @template_expander.use_jinja('templates/ElementLookupTrie.cpp.tmpl')
    def generate_implementation(self):
        return {
            'input_files': self._input_files,
            'namespace': self._namespace,
            'length_tries': trie_builder.trie_list_by_str_length(self._tags)
        }


if __name__ == '__main__':
    json5_generator.Maker(ElementLookupTrieWriter).main()
