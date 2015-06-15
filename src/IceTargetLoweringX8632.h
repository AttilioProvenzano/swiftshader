//===- subzero/src/IceTargetLoweringX8632.h - x86-32 lowering ---*- C++ -*-===//
//
//                        The Subzero Code Generator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the TargetLoweringX8632 class, which
// implements the TargetLowering interface for the x86-32
// architecture.
//
//===----------------------------------------------------------------------===//

#ifndef SUBZERO_SRC_ICETARGETLOWERINGX8632_H
#define SUBZERO_SRC_ICETARGETLOWERINGX8632_H

#include <unordered_map>

#include "IceAssemblerX8632.h"
#include "IceDefs.h"
#include "IceInst.h"
#include "IceInstX8632.h"
#include "IceRegistersX8632.h"
#include "IceTargetLowering.h"

namespace Ice {

class BoolFoldingEntry {
  BoolFoldingEntry(const BoolFoldingEntry &) = delete;

public:
  BoolFoldingEntry()
      : Instr(nullptr), IsComplex(false), IsLiveOut(true), NumUses(0) {}
  explicit BoolFoldingEntry(Inst *I);
  BoolFoldingEntry &operator=(const BoolFoldingEntry &) = default;
  // Instr is the instruction producing the i1-type variable of interest.
  Inst *Instr;
  // IsComplex is the cached result of BoolFolding::hasComplexLowering(Instr).
  bool IsComplex;
  // IsLiveOut is initialized conservatively to true, and is set to false when
  // we encounter an instruction that ends Var's live range.  We disable the
  // folding optimization when Var is live beyond this basic block.  Note that
  // if liveness analysis is not performed (e.g. in Om1 mode), IsLiveOut will
  // always be true and the folding optimization will never be performed.
  bool IsLiveOut;
  // NumUses counts the number of times Var is used as a source operand in the
  // basic block.  If IsComplex is true and there is more than one use of Var,
  // then the folding optimization is disabled for Var.
  uint32_t NumUses;
};

class BoolFolding {
public:
  enum BoolFoldingProducerKind {
    PK_None,
    PK_Icmp32,
    PK_Icmp64,
    PK_Fcmp,
    PK_Trunc
  };

  // Currently the actual enum values are not used (other than CK_None), but we
  // go
  // ahead and produce them anyway for symmetry with the
  // BoolFoldingProducerKind.
  enum BoolFoldingConsumerKind { CK_None, CK_Br, CK_Select, CK_Sext, CK_Zext };

private:
  BoolFolding(const BoolFolding &) = delete;
  BoolFolding &operator=(const BoolFolding &) = delete;

public:
  BoolFolding() {}
  static BoolFoldingProducerKind getProducerKind(const Inst *Instr);
  static BoolFoldingConsumerKind getConsumerKind(const Inst *Instr);
  static bool hasComplexLowering(const Inst *Instr);
  void init(CfgNode *Node);
  const Inst *getProducerFor(const Operand *Opnd) const;
  void dump(const Cfg *Func) const;

private:
  // Returns true if Producers contains a valid entry for the given VarNum.
  bool containsValid(SizeT VarNum) const {
    auto Element = Producers.find(VarNum);
    return Element != Producers.end() && Element->second.Instr != nullptr;
  }
  void setInvalid(SizeT VarNum) { Producers[VarNum].Instr = nullptr; }
  // Producers maps Variable::Number to a BoolFoldingEntry.
  std::unordered_map<SizeT, BoolFoldingEntry> Producers;
};

class TargetX8632 : public TargetLowering {
  TargetX8632() = delete;
  TargetX8632(const TargetX8632 &) = delete;
  TargetX8632 &operator=(const TargetX8632 &) = delete;

public:
  static TargetX8632 *create(Cfg *Func) { return new TargetX8632(Func); }

  void translateOm1() override;
  void translateO2() override;
  void doLoadOpt();
  bool doBranchOpt(Inst *I, const CfgNode *NextNode) override;

  SizeT getNumRegisters() const override { return RegX8632::Reg_NUM; }
  Variable *getPhysicalRegister(SizeT RegNum, Type Ty = IceType_void) override;
  IceString getRegName(SizeT RegNum, Type Ty) const override;
  llvm::SmallBitVector getRegisterSet(RegSetMask Include,
                                      RegSetMask Exclude) const override;
  const llvm::SmallBitVector &getRegisterSetForType(Type Ty) const override {
    return TypeToRegisterSet[Ty];
  }
  bool hasFramePointer() const override { return IsEbpBasedFrame; }
  SizeT getFrameOrStackReg() const override {
    return IsEbpBasedFrame ? RegX8632::Reg_ebp : RegX8632::Reg_esp;
  }
  size_t typeWidthInBytesOnStack(Type Ty) const override {
    // Round up to the next multiple of 4 bytes.  In particular, i1,
    // i8, and i16 are rounded up to 4 bytes.
    return (typeWidthInBytes(Ty) + 3) & ~3;
  }

  void emitVariable(const Variable *Var) const override;

  const char *getConstantPrefix() const final { return "$"; }
  void emit(const ConstantUndef *C) const final;
  void emit(const ConstantInteger32 *C) const final;
  void emit(const ConstantInteger64 *C) const final;
  void emit(const ConstantFloat *C) const final;
  void emit(const ConstantDouble *C) const final;

  void lowerArguments() override;
  void initNodeForLowering(CfgNode *Node) override;
  void addProlog(CfgNode *Node) override;
  void addEpilog(CfgNode *Node) override;
  // Ensure that a 64-bit Variable has been split into 2 32-bit
  // Variables, creating them if necessary.  This is needed for all
  // I64 operations, and it is needed for pushing F64 arguments for
  // function calls using the 32-bit push instruction (though the
  // latter could be done by directly writing to the stack).
  void split64(Variable *Var);
  Operand *loOperand(Operand *Operand);
  Operand *hiOperand(Operand *Operand);
  void finishArgumentLowering(Variable *Arg, Variable *FramePtr,
                              size_t BasicFrameOffset, size_t &InArgsSizeBytes);
  X8632::Address stackVarToAsmOperand(const Variable *Var) const;

  enum X86InstructionSet {
    Begin,
    // SSE2 is the PNaCl baseline instruction set.
    SSE2 = Begin,
    SSE4_1,
    End
  };

  X86InstructionSet getInstructionSet() const { return InstructionSet; }

protected:
  explicit TargetX8632(Cfg *Func);

  void postLower() override;

  void lowerAlloca(const InstAlloca *Inst) override;
  void lowerArithmetic(const InstArithmetic *Inst) override;
  void lowerAssign(const InstAssign *Inst) override;
  void lowerBr(const InstBr *Inst) override;
  void lowerCall(const InstCall *Inst) override;
  void lowerCast(const InstCast *Inst) override;
  void lowerExtractElement(const InstExtractElement *Inst) override;
  void lowerFcmp(const InstFcmp *Inst) override;
  void lowerIcmp(const InstIcmp *Inst) override;
  void lowerIntrinsicCall(const InstIntrinsicCall *Inst) override;
  void lowerInsertElement(const InstInsertElement *Inst) override;
  void lowerLoad(const InstLoad *Inst) override;
  void lowerPhi(const InstPhi *Inst) override;
  void lowerRet(const InstRet *Inst) override;
  void lowerSelect(const InstSelect *Inst) override;
  void lowerStore(const InstStore *Inst) override;
  void lowerSwitch(const InstSwitch *Inst) override;
  void lowerUnreachable(const InstUnreachable *Inst) override;
  void prelowerPhis() override;
  void lowerPhiAssignments(CfgNode *Node,
                           const AssignList &Assignments) override;
  void doAddressOptLoad() override;
  void doAddressOptStore() override;
  void randomlyInsertNop(float Probability) override;

  // Naive lowering of cmpxchg.
  void lowerAtomicCmpxchg(Variable *DestPrev, Operand *Ptr, Operand *Expected,
                          Operand *Desired);
  // Attempt a more optimized lowering of cmpxchg. Returns true if optimized.
  bool tryOptimizedCmpxchgCmpBr(Variable *DestPrev, Operand *Ptr,
                                Operand *Expected, Operand *Desired);
  void lowerAtomicRMW(Variable *Dest, uint32_t Operation, Operand *Ptr,
                      Operand *Val);
  void lowerCountZeros(bool Cttz, Type Ty, Variable *Dest, Operand *FirstVal,
                       Operand *SecondVal);

  typedef void (TargetX8632::*LowerBinOp)(Variable *, Operand *);
  void expandAtomicRMWAsCmpxchg(LowerBinOp op_lo, LowerBinOp op_hi,
                                Variable *Dest, Operand *Ptr, Operand *Val);

  void eliminateNextVectorSextInstruction(Variable *SignExtendedResult);

  void scalarizeArithmetic(InstArithmetic::OpKind K, Variable *Dest,
                           Operand *Src0, Operand *Src1);

  // Operand legalization helpers.  To deal with address mode
  // constraints, the helpers will create a new Operand and emit
  // instructions that guarantee that the Operand kind is one of those
  // indicated by the LegalMask (a bitmask of allowed kinds).  If the
  // input Operand is known to already meet the constraints, it may be
  // simply returned as the result, without creating any new
  // instructions or operands.
  enum OperandLegalization {
    Legal_None = 0,
    Legal_Reg = 1 << 0, // physical register, not stack location
    Legal_Imm = 1 << 1,
    Legal_Mem = 1 << 2, // includes [eax+4*ecx] as well as [esp+12]
    Legal_All = ~Legal_None
  };
  typedef uint32_t LegalMask;
  Operand *legalize(Operand *From, LegalMask Allowed = Legal_All,
                    int32_t RegNum = Variable::NoRegister);
  Variable *legalizeToVar(Operand *From, int32_t RegNum = Variable::NoRegister);
  // Legalize the first source operand for use in the cmp instruction.
  Operand *legalizeSrc0ForCmp(Operand *Src0, Operand *Src1);
  // Turn a pointer operand into a memory operand that can be
  // used by a real load/store operation. Legalizes the operand as well.
  // This is a nop if the operand is already a legal memory operand.
  OperandX8632Mem *formMemoryOperand(Operand *Ptr, Type Ty,
                                     bool DoLegalize = true);

  Variable *makeReg(Type Ty, int32_t RegNum = Variable::NoRegister);
  static Type stackSlotType();

  Variable *copyToReg(Operand *Src, int32_t RegNum = Variable::NoRegister);

  // Returns a vector in a register with the given constant entries.
  Variable *makeVectorOfZeros(Type Ty, int32_t RegNum = Variable::NoRegister);
  Variable *makeVectorOfOnes(Type Ty, int32_t RegNum = Variable::NoRegister);
  Variable *makeVectorOfMinusOnes(Type Ty,
                                  int32_t RegNum = Variable::NoRegister);
  Variable *makeVectorOfHighOrderBits(Type Ty,
                                      int32_t RegNum = Variable::NoRegister);
  Variable *makeVectorOfFabsMask(Type Ty,
                                 int32_t RegNum = Variable::NoRegister);

  // Return a memory operand corresponding to a stack allocated Variable.
  OperandX8632Mem *getMemoryOperandForStackSlot(Type Ty, Variable *Slot,
                                                uint32_t Offset = 0);

  void makeRandomRegisterPermutation(
      llvm::SmallVectorImpl<int32_t> &Permutation,
      const llvm::SmallBitVector &ExcludeRegisters) const override;

  // The following are helpers that insert lowered x86 instructions
  // with minimal syntactic overhead, so that the lowering code can
  // look as close to assembly as practical.
  void _adc(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Adc::create(Func, Dest, Src0));
  }
  void _add(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Add::create(Func, Dest, Src0));
  }
  void _adjust_stack(int32_t Amount) {
    Context.insert(InstX8632AdjustStack::create(
        Func, Amount, getPhysicalRegister(RegX8632::Reg_esp)));
  }
  void _addps(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Addps::create(Func, Dest, Src0));
  }
  void _addss(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Addss::create(Func, Dest, Src0));
  }
  void _and(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632And::create(Func, Dest, Src0));
  }
  void _blendvps(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Blendvps::create(Func, Dest, Src0, Src1));
  }
  void _br(CondX86::BrCond Condition, CfgNode *TargetTrue,
           CfgNode *TargetFalse) {
    Context.insert(
        InstX8632Br::create(Func, TargetTrue, TargetFalse, Condition));
  }
  void _br(CfgNode *Target) {
    Context.insert(InstX8632Br::create(Func, Target));
  }
  void _br(CondX86::BrCond Condition, CfgNode *Target) {
    Context.insert(InstX8632Br::create(Func, Target, Condition));
  }
  void _br(CondX86::BrCond Condition, InstX8632Label *Label) {
    Context.insert(InstX8632Br::create(Func, Label, Condition));
  }
  void _bsf(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Bsf::create(Func, Dest, Src0));
  }
  void _bsr(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Bsr::create(Func, Dest, Src0));
  }
  void _bswap(Variable *SrcDest) {
    Context.insert(InstX8632Bswap::create(Func, SrcDest));
  }
  void _cbwdq(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Cbwdq::create(Func, Dest, Src0));
  }
  void _cmov(Variable *Dest, Operand *Src0, CondX86::BrCond Condition) {
    Context.insert(InstX8632Cmov::create(Func, Dest, Src0, Condition));
  }
  void _cmp(Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Icmp::create(Func, Src0, Src1));
  }
  void _cmpps(Variable *Dest, Operand *Src0, CondX86::CmppsCond Condition) {
    Context.insert(InstX8632Cmpps::create(Func, Dest, Src0, Condition));
  }
  void _cmpxchg(Operand *DestOrAddr, Variable *Eax, Variable *Desired,
                bool Locked) {
    Context.insert(
        InstX8632Cmpxchg::create(Func, DestOrAddr, Eax, Desired, Locked));
    // Mark eax as possibly modified by cmpxchg.
    Context.insert(
        InstFakeDef::create(Func, Eax, llvm::dyn_cast<Variable>(DestOrAddr)));
    _set_dest_nonkillable();
    Context.insert(InstFakeUse::create(Func, Eax));
  }
  void _cmpxchg8b(OperandX8632Mem *Addr, Variable *Edx, Variable *Eax,
                  Variable *Ecx, Variable *Ebx, bool Locked) {
    Context.insert(
        InstX8632Cmpxchg8b::create(Func, Addr, Edx, Eax, Ecx, Ebx, Locked));
    // Mark edx, and eax as possibly modified by cmpxchg8b.
    Context.insert(InstFakeDef::create(Func, Edx));
    _set_dest_nonkillable();
    Context.insert(InstFakeUse::create(Func, Edx));
    Context.insert(InstFakeDef::create(Func, Eax));
    _set_dest_nonkillable();
    Context.insert(InstFakeUse::create(Func, Eax));
  }
  void _cvt(Variable *Dest, Operand *Src0, InstX8632Cvt::CvtVariant Variant) {
    Context.insert(InstX8632Cvt::create(Func, Dest, Src0, Variant));
  }
  void _div(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Div::create(Func, Dest, Src0, Src1));
  }
  void _divps(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Divps::create(Func, Dest, Src0));
  }
  void _divss(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Divss::create(Func, Dest, Src0));
  }
  void _fld(Operand *Src0) { Context.insert(InstX8632Fld::create(Func, Src0)); }
  void _fstp(Variable *Dest) {
    Context.insert(InstX8632Fstp::create(Func, Dest));
  }
  void _idiv(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Idiv::create(Func, Dest, Src0, Src1));
  }
  void _imul(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Imul::create(Func, Dest, Src0));
  }
  void _insertps(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Insertps::create(Func, Dest, Src0, Src1));
  }
  void _jmp(Operand *Target) {
    Context.insert(InstX8632Jmp::create(Func, Target));
  }
  void _lea(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Lea::create(Func, Dest, Src0));
  }
  void _mfence() { Context.insert(InstX8632Mfence::create(Func)); }
  // If Dest=nullptr is passed in, then a new variable is created,
  // marked as infinite register allocation weight, and returned
  // through the in/out Dest argument.
  void _mov(Variable *&Dest, Operand *Src0,
            int32_t RegNum = Variable::NoRegister) {
    if (Dest == nullptr)
      Dest = makeReg(Src0->getType(), RegNum);
    Context.insert(InstX8632Mov::create(Func, Dest, Src0));
  }
  void _mov_nonkillable(Variable *Dest, Operand *Src0) {
    Inst *NewInst = InstX8632Mov::create(Func, Dest, Src0);
    NewInst->setDestNonKillable();
    Context.insert(NewInst);
  }
  void _movd(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Movd::create(Func, Dest, Src0));
  }
  void _movp(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Movp::create(Func, Dest, Src0));
  }
  void _movq(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Movq::create(Func, Dest, Src0));
  }
  void _movss(Variable *Dest, Variable *Src0) {
    Context.insert(InstX8632MovssRegs::create(Func, Dest, Src0));
  }
  void _movsx(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Movsx::create(Func, Dest, Src0));
  }
  void _movzx(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Movzx::create(Func, Dest, Src0));
  }
  void _mul(Variable *Dest, Variable *Src0, Operand *Src1) {
    Context.insert(InstX8632Mul::create(Func, Dest, Src0, Src1));
  }
  void _mulps(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Mulps::create(Func, Dest, Src0));
  }
  void _mulss(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Mulss::create(Func, Dest, Src0));
  }
  void _neg(Variable *SrcDest) {
    Context.insert(InstX8632Neg::create(Func, SrcDest));
  }
  void _nop(SizeT Variant) {
    Context.insert(InstX8632Nop::create(Func, Variant));
  }
  void _or(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Or::create(Func, Dest, Src0));
  }
  void _padd(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Padd::create(Func, Dest, Src0));
  }
  void _pand(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Pand::create(Func, Dest, Src0));
  }
  void _pandn(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Pandn::create(Func, Dest, Src0));
  }
  void _pblendvb(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Pblendvb::create(Func, Dest, Src0, Src1));
  }
  void _pcmpeq(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Pcmpeq::create(Func, Dest, Src0));
  }
  void _pcmpgt(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Pcmpgt::create(Func, Dest, Src0));
  }
  void _pextr(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Pextr::create(Func, Dest, Src0, Src1));
  }
  void _pinsr(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Pinsr::create(Func, Dest, Src0, Src1));
  }
  void _pmull(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Pmull::create(Func, Dest, Src0));
  }
  void _pmuludq(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Pmuludq::create(Func, Dest, Src0));
  }
  void _pop(Variable *Dest) {
    Context.insert(InstX8632Pop::create(Func, Dest));
  }
  void _por(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Por::create(Func, Dest, Src0));
  }
  void _pshufd(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Pshufd::create(Func, Dest, Src0, Src1));
  }
  void _psll(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Psll::create(Func, Dest, Src0));
  }
  void _psra(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Psra::create(Func, Dest, Src0));
  }
  void _psrl(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Psrl::create(Func, Dest, Src0));
  }
  void _psub(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Psub::create(Func, Dest, Src0));
  }
  void _push(Variable *Src0) {
    Context.insert(InstX8632Push::create(Func, Src0));
  }
  void _pxor(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Pxor::create(Func, Dest, Src0));
  }
  void _ret(Variable *Src0 = nullptr) {
    Context.insert(InstX8632Ret::create(Func, Src0));
  }
  void _rol(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Rol::create(Func, Dest, Src0));
  }
  void _sar(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Sar::create(Func, Dest, Src0));
  }
  void _sbb(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Sbb::create(Func, Dest, Src0));
  }
  void _setcc(Variable *Dest, CondX86::BrCond Condition) {
    Context.insert(InstX8632Setcc::create(Func, Dest, Condition));
  }
  void _shl(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Shl::create(Func, Dest, Src0));
  }
  void _shld(Variable *Dest, Variable *Src0, Variable *Src1) {
    Context.insert(InstX8632Shld::create(Func, Dest, Src0, Src1));
  }
  void _shr(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Shr::create(Func, Dest, Src0));
  }
  void _shrd(Variable *Dest, Variable *Src0, Variable *Src1) {
    Context.insert(InstX8632Shrd::create(Func, Dest, Src0, Src1));
  }
  void _shufps(Variable *Dest, Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Shufps::create(Func, Dest, Src0, Src1));
  }
  void _sqrtss(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Sqrtss::create(Func, Dest, Src0));
  }
  void _store(Operand *Value, OperandX8632 *Mem) {
    Context.insert(InstX8632Store::create(Func, Value, Mem));
  }
  void _storep(Variable *Value, OperandX8632Mem *Mem) {
    Context.insert(InstX8632StoreP::create(Func, Value, Mem));
  }
  void _storeq(Variable *Value, OperandX8632Mem *Mem) {
    Context.insert(InstX8632StoreQ::create(Func, Value, Mem));
  }
  void _sub(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Sub::create(Func, Dest, Src0));
  }
  void _subps(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Subps::create(Func, Dest, Src0));
  }
  void _subss(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Subss::create(Func, Dest, Src0));
  }
  void _test(Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Test::create(Func, Src0, Src1));
  }
  void _ucomiss(Operand *Src0, Operand *Src1) {
    Context.insert(InstX8632Ucomiss::create(Func, Src0, Src1));
  }
  void _ud2() { Context.insert(InstX8632UD2::create(Func)); }
  void _xadd(Operand *Dest, Variable *Src, bool Locked) {
    Context.insert(InstX8632Xadd::create(Func, Dest, Src, Locked));
    // The xadd exchanges Dest and Src (modifying Src).
    // Model that update with a FakeDef followed by a FakeUse.
    Context.insert(
        InstFakeDef::create(Func, Src, llvm::dyn_cast<Variable>(Dest)));
    _set_dest_nonkillable();
    Context.insert(InstFakeUse::create(Func, Src));
  }
  void _xchg(Operand *Dest, Variable *Src) {
    Context.insert(InstX8632Xchg::create(Func, Dest, Src));
    // The xchg modifies Dest and Src -- model that update with a
    // FakeDef/FakeUse.
    Context.insert(
        InstFakeDef::create(Func, Src, llvm::dyn_cast<Variable>(Dest)));
    _set_dest_nonkillable();
    Context.insert(InstFakeUse::create(Func, Src));
  }
  void _xor(Variable *Dest, Operand *Src0) {
    Context.insert(InstX8632Xor::create(Func, Dest, Src0));
  }
  void _set_dest_nonkillable() {
    Context.getLastInserted()->setDestNonKillable();
  }

  bool optimizeScalarMul(Variable *Dest, Operand *Src0, int32_t Src1);

  const X86InstructionSet InstructionSet;
  bool IsEbpBasedFrame;
  bool NeedsStackAlignment;
  size_t SpillAreaSizeBytes;
  llvm::SmallBitVector TypeToRegisterSet[IceType_NUM];
  llvm::SmallBitVector ScratchRegs;
  llvm::SmallBitVector RegsUsed;
  VarList PhysicalRegisters[IceType_NUM];
  static IceString RegNames[];

private:
  ~TargetX8632() override {}
  BoolFolding FoldingInfo;
};

class TargetDataX8632 final : public TargetDataLowering {
  TargetDataX8632() = delete;
  TargetDataX8632(const TargetDataX8632 &) = delete;
  TargetDataX8632 &operator=(const TargetDataX8632 &) = delete;

public:
  static std::unique_ptr<TargetDataLowering> create(GlobalContext *Ctx) {
    return std::unique_ptr<TargetDataLowering>(new TargetDataX8632(Ctx));
  }

  void lowerGlobals(std::unique_ptr<VariableDeclarationList> Vars) override;
  void lowerConstants() override;

protected:
  explicit TargetDataX8632(GlobalContext *Ctx);

private:
  ~TargetDataX8632() override {}
  template <typename T> static void emitConstantPool(GlobalContext *Ctx);
};

class TargetHeaderX8632 final : public TargetHeaderLowering {
  TargetHeaderX8632() = delete;
  TargetHeaderX8632(const TargetHeaderX8632 &) = delete;
  TargetHeaderX8632 &operator=(const TargetHeaderX8632 &) = delete;

public:
  static std::unique_ptr<TargetHeaderLowering> create(GlobalContext *Ctx) {
    return std::unique_ptr<TargetHeaderLowering>(new TargetHeaderX8632(Ctx));
  }

protected:
  explicit TargetHeaderX8632(GlobalContext *Ctx);

private:
  ~TargetHeaderX8632() = default;
};

} // end of namespace Ice

#endif // SUBZERO_SRC_ICETARGETLOWERINGX8632_H
