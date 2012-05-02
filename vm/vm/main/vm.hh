// Copyright © 2011, Université catholique de Louvain
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __VM_H
#define __VM_H

#include "mozartcore.hh"

namespace mozart {

////////////////////
// VirtualMachine //
////////////////////

VirtualMachine::VirtualMachine(PreemptionTest preemptionTest,
                               void* preemptionTestData) :
  _preemptionTest(preemptionTest), _preemptionTestData(preemptionTestData),
  gc(this), sc(this) {

  memoryManager.init();

  _topLevelSpace = new (this) Space(this);
  _currentSpace = _topLevelSpace;

  initialize();
}

bool VirtualMachine::testPreemption() {
  return _preemptionTest(_preemptionTestData) || gc.isGCRequired();
}

void VirtualMachine::setCurrentSpace(Space* space) {
  _currentSpace = space;
  _isOnTopLevel = space->isTopLevel();
}

Space* VirtualMachine::cloneSpace(Space* space) {
  return sc.doCloneSpace(space);
}

void VirtualMachine::initialize() {
  coreatoms.initialize(this, atomTable);
}

}

// new operators must be declared outside of any namespace

void* operator new (size_t size, mozart::VM vm) {
  return vm->getMemory(size);
}

void* operator new[] (size_t size, mozart::VM vm) {
  return vm->getMemory(size);
}

#endif // __VM_H
